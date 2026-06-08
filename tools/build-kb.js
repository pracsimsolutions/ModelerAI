// Build the ModelerAI knowledge base from FlexSim's shipped XML docs.
//
// Reads:
//   <flexsim install>/help/FlexScriptAPIReference/**/*.xml   (classes)
//   <flexsim install>/help/CommandReference/Commands.xml     (commands)
//
// Writes:
//   KNOWLEDGE/topics/classes/<Class>.md      (one file per class)
//   KNOWLEDGE/topics/commands/<command>.md   (one file per command)
//   KNOWLEDGE/catalog/classes.md             (always-on symbol table)
//   KNOWLEDGE/catalog/commands.md            (always-on symbol table)
//   KNOWLEDGE/INDEX.json                     (BM25 stats + symbol catalog)
//
// Per the KB-architecture research, the always-on catalog is the
// cached prefix; per-topic markdown is fetched on demand by the
// runtime via search_kb / read_topic tools (DLL wiring lives separately).
//
// Run from the repo root:   node tools/build-kb.js
// Dependency-free (matches embed.js style).
'use strict';

const fs   = require('fs');
const path = require('path');

// -----------------------------------------------------------------------
// Path setup. FlexSim install root is two levels above the modules dir.
// -----------------------------------------------------------------------
const repoRoot   = path.join(__dirname, '..');
// modules/ModelerAI -> modules -> FlexSim install root
const fxRoot     = path.resolve(repoRoot, '..', '..');
const helpDir    = path.join(fxRoot, 'help');
const apiRefDir  = path.join(helpDir, 'FlexScriptAPIReference');
const cmdXmlPath = path.join(helpDir, 'CommandReference', 'Commands.xml');

const kbRoot       = path.join(repoRoot, 'KNOWLEDGE');
const classesDir   = path.join(kbRoot, 'topics', 'classes');
const commandsDir  = path.join(kbRoot, 'topics', 'commands');
const catalogDir   = path.join(kbRoot, 'catalog');
const indexJsonPath = path.join(kbRoot, 'INDEX.json');

if (!fs.existsSync(helpDir)) {
  console.error('Could not find FlexSim help directory at: ' + helpDir);
  process.exit(1);
}

// -----------------------------------------------------------------------
// Minimal XML parser. The help XMLs use a small, well-formed schema —
// we don't need a general DOMParser. Returns a tree of:
//   { tag, attrs, children: [string | node, ...] }
// Whitespace-only text nodes are kept (matters for some <code> blocks).
// -----------------------------------------------------------------------
function parseXml(src) {
  // Strip leading XML decl + doctype.
  src = src.replace(/^\s*<\?xml[\s\S]*?\?>\s*/, '').replace(/^\s*<!DOCTYPE[\s\S]*?>\s*/, '');
  let i = 0;
  const len = src.length;

  function parseNode() {
    if (src[i] !== '<') return null;
    // Comment?
    if (src.startsWith('<!--', i)) {
      const end = src.indexOf('-->', i);
      i = end < 0 ? len : end + 3;
      return null;
    }
    // Closing tag — caller handles.
    if (src[i + 1] === '/') return null;

    const tagStart = i;
    i++; // consume '<'
    let tagEnd = i;
    while (tagEnd < len && !/[\s/>]/.test(src[tagEnd])) tagEnd++;
    const tag = src.slice(i, tagEnd);
    i = tagEnd;

    const attrs = {};
    while (i < len && src[i] !== '>' && src[i] !== '/') {
      // Skip whitespace.
      while (i < len && /\s/.test(src[i])) i++;
      if (src[i] === '>' || src[i] === '/') break;
      // Attribute name.
      const aStart = i;
      while (i < len && src[i] !== '=' && !/\s/.test(src[i]) && src[i] !== '>' && src[i] !== '/') i++;
      const aName = src.slice(aStart, i);
      // = "value" or = 'value' or bareword
      let aValue = '';
      if (src[i] === '=') {
        i++;
        const q = src[i];
        if (q === '"' || q === "'") {
          i++;
          const vStart = i;
          while (i < len && src[i] !== q) i++;
          aValue = src.slice(vStart, i);
          i++; // closing quote
        } else {
          const vStart = i;
          while (i < len && !/[\s/>]/.test(src[i])) i++;
          aValue = src.slice(vStart, i);
        }
      }
      if (aName) attrs[aName] = decodeXmlEntities(aValue);
    }

    // Self-closing tag?
    if (src[i] === '/' && src[i + 1] === '>') {
      i += 2;
      return { tag, attrs, children: [] };
    }
    if (src[i] === '>') i++;

    // Children until matching close tag.
    const children = [];
    while (i < len) {
      if (src[i] === '<') {
        if (src[i + 1] === '/') {
          // Close tag.
          const closeEnd = src.indexOf('>', i);
          // Skip past it.
          i = closeEnd < 0 ? len : closeEnd + 1;
          break;
        }
        const child = parseNode();
        if (child) children.push(child);
      } else {
        // Text run until next '<'.
        const tStart = i;
        while (i < len && src[i] !== '<') i++;
        const txt = src.slice(tStart, i);
        if (txt.length) children.push(decodeXmlEntities(txt));
      }
    }
    return { tag, attrs, children };
  }

  // Skip leading whitespace, then parse the root.
  while (i < len && /\s/.test(src[i])) i++;
  return parseNode();
}

function decodeXmlEntities(s) {
  return String(s)
    .replace(/&amp;/g, '&')
    .replace(/&lt;/g, '<')
    .replace(/&gt;/g, '>')
    .replace(/&quot;/g, '"')
    .replace(/&apos;/g, "'")
    .replace(/&#x([0-9a-fA-F]+);/g, (_, hex) => String.fromCodePoint(parseInt(hex, 16)))
    .replace(/&#(\d+);/g, (_, dec) => String.fromCodePoint(parseInt(dec, 10)));
}

// -----------------------------------------------------------------------
// Tree-walking helpers.
// -----------------------------------------------------------------------
function findChild(node, tag) {
  if (!node || !node.children) return null;
  for (const c of node.children) {
    if (c && typeof c === 'object' && c.tag === tag) return c;
  }
  return null;
}
function findChildren(node, tag) {
  if (!node || !node.children) return [];
  return node.children.filter(c => c && typeof c === 'object' && c.tag === tag);
}
function textOf(node) {
  if (!node) return '';
  if (typeof node === 'string') return node;
  if (!node.children) return '';
  return node.children.map(c => {
    if (typeof c === 'string') return c;
    // Inline references like <ClassRef>Math</ClassRef>, <MethodRef class="Math">cos</MethodRef>:
    // strip the tag, keep the text. <brr/> -> newline.
    if (c.tag === 'brr') return '\n';
    return textOf(c);
  }).join('');
}
function textOfClean(node) {
  return textOf(node).replace(/\s+/g, ' ').trim();
}
function textOfMultiline(node) {
  // Preserve newlines from <brr/> but normalize runs of whitespace within lines.
  return textOf(node)
    .split('\n')
    .map(l => l.replace(/[ \t]+/g, ' ').trim())
    .join('\n')
    .replace(/\n{3,}/g, '\n\n')
    .trim();
}

// -----------------------------------------------------------------------
// Filesystem helpers.
// -----------------------------------------------------------------------
function ensureDir(p) { fs.mkdirSync(p, { recursive: true }); }
function listXmlsRecursive(dir) {
  const out = [];
  function walk(d) {
    for (const ent of fs.readdirSync(d, { withFileTypes: true })) {
      const full = path.join(d, ent.name);
      if (ent.isDirectory()) walk(full);
      else if (ent.isFile() && ent.name.endsWith('.xml')) out.push(full);
    }
  }
  walk(dir);
  return out;
}

// Manual-topic helpers — walk topics/manual/ recursively and parse YAML
// frontmatter from the ingested markdown files.
function walkMarkdownRecursive(dir, out) {
  for (const ent of fs.readdirSync(dir, { withFileTypes: true })) {
    const full = path.join(dir, ent.name);
    if (ent.isDirectory()) walkMarkdownRecursive(full, out);
    else if (ent.isFile() && ent.name.toLowerCase().endsWith('.md')) out.push(full);
  }
}
function splitFrontmatter(text) {
  if (!text.startsWith('---')) return { frontmatter: '', body: text };
  const end = text.indexOf('\n---', 3);
  if (end < 0) return { frontmatter: '', body: text };
  const fm = text.slice(3, end).replace(/^\n/, '');
  const body = text.slice(end + 4).replace(/^\n/, '');
  return { frontmatter: fm, body };
}
function parseFrontmatterSimple(fm) {
  // Lightweight key: value parser. Values that start with [ or " pass through
  // for downstream JSON parsing; everything else is treated as a string.
  const out = {};
  for (const line of fm.split('\n')) {
    const m = line.match(/^([a-zA-Z_][\w-]*):\s*(.*)$/);
    if (!m) continue;
    out[m[1]] = m[2].trim();
  }
  return out;
}
function parseJsonArray(v) {
  if (!v) return [];
  try {
    const parsed = JSON.parse(v);
    return Array.isArray(parsed) ? parsed.map(String) : [];
  } catch { return []; }
}

// FlexSim's XML files name module classes with a dot ("AGV.Trailer"), but
// FlexScript source code uses double colon ("AGV::Trailer"). Normalize once
// at extraction; everything downstream sees the "::" form.
function normalizeClassName(rawName) {
  if (!rawName) return '';
  // Only split on the FIRST '.' — multi-dot names are not expected, but a
  // single split keeps "Module::Inner.Sub" type collisions from happening.
  const i = rawName.indexOf('.');
  if (i < 0) return rawName;
  return rawName.slice(0, i) + '::' + rawName.slice(i + 1);
}

// Module-class to filesystem-safe slug: "Conveyor::StraightConveyor" -> "Conveyor__StraightConveyor".
function classToSlug(className) {
  return className.replace(/::/g, '__');
}

// -----------------------------------------------------------------------
// Class-XML extraction. Each FlexScriptAPIReference/*.xml has:
//   <FlexScriptClass name="Foo" extends="Bar" ...>
//     <Summary>...</Summary>
//     <Description>...</Description>
//     <Method name="x">...</Method>     (many)
//     <Property name="y">...</Property> (many)
//   </FlexScriptClass>
// -----------------------------------------------------------------------
function extractClass(xmlText, sourcePath) {
  const root = parseXml(xmlText);
  if (!root) return null;
  // Some files wrap in different tags (FlexScriptClass, ModuleClass, ...).
  // Heuristic: take the outer node if it has a `name` attr, otherwise look
  // one level deeper.
  let cls = root;
  if (!cls.attrs || !cls.attrs.name) {
    for (const c of cls.children || []) {
      if (c && typeof c === 'object' && c.attrs && c.attrs.name) { cls = c; break; }
    }
  }
  if (!cls.attrs || !cls.attrs.name) return null;

  const className   = normalizeClassName(cls.attrs.name);
  const extendsName = normalizeClassName(cls.attrs.extends || cls.attrs.parent || '');
  const summary     = textOfClean(findChild(cls, 'Summary'));
  const description = textOfMultiline(findChild(cls, 'Description'));

  function extractParam(p) {
    return {
      name: p.attrs && p.attrs.name || '',
      type: p.attrs && p.attrs.type || '',
      defaultValue: p.attrs && p.attrs['default-value'] || ''
    };
  }
  function extractMember(memberNode) {
    const name = memberNode.attrs && memberNode.attrs.name || '';
    const isStatic = (memberNode.attrs && memberNode.attrs.static) === 'true';
    const readonly = (memberNode.attrs && memberNode.attrs.readonly) === 'true';
    const returnType = (memberNode.attrs && memberNode.attrs['return-type']) || '';
    const sig = findChild(memberNode, 'Signature');
    let sigParams = [], sigReturn = returnType;
    if (sig) {
      sigReturn = sigReturn || sig.attrs && sig.attrs['return-type'] || '';
      sigParams = findChildren(sig, 'Param').map(extractParam);
    }
    const summary = textOfClean(findChild(memberNode, 'Summary'));
    // ParamDescription elements live as siblings of Signature.
    const paramDescs = {};
    findChildren(memberNode, 'ParamDescription').forEach(pd => {
      const n = pd.attrs && pd.attrs.name || '';
      if (n) paramDescs[n] = textOfClean(pd);
    });
    const returnsDesc = textOfClean(findChild(memberNode, 'Returns'));
    const description = textOfMultiline(findChild(memberNode, 'Description'));
    return {
      tag: memberNode.tag,   // Method / Property / Constructor
      name, isStatic, readonly,
      sigParams, sigReturn,
      summary, paramDescs, returnsDesc, description
    };
  }

  const methods = findChildren(cls, 'Method').map(extractMember);
  const properties = findChildren(cls, 'Property').map(extractMember);
  const constructors = findChildren(cls, 'Constructor').map(extractMember);

  return {
    sourcePath,
    className,
    extendsName,
    summary,
    description,
    methods,
    properties,
    constructors,
  };
}

// -----------------------------------------------------------------------
// Command-XML extraction. Commands.xml has:
//   <commands>
//     <command><name>foo</name>
//       <parameters>(...)</parameters>
//       <shortdescription>...</shortdescription>
//       <description>...</description>
//       <example>...</example>
//       <returntype>num</returntype>
//       <gr>N</gr>
//     </command>
//     ...
//   </commands>
// -----------------------------------------------------------------------
function extractAllCommands(xmlText) {
  const root = parseXml(xmlText);
  if (!root) return [];

  // Group code -> human-readable name (e.g. "31" -> "Deprecated", "19" -> "Communication").
  const groupNameByCode = {};
  const groupsNode = findChild(root, 'groups');
  if (groupsNode) {
    for (const g of findChildren(groupsNode, 'group')) {
      const code = textOfClean(findChild(g, 'value'));
      const name = textOfClean(findChild(g, 'groupname'));
      if (code) groupNameByCode[code] = name;
    }
  }

  const cmdNodes = findChildren(root, 'command');
  return cmdNodes.map(node => {
    const nameNode = findChild(node, 'name');
    const name = textOfClean(nameNode);
    const parameters = textOfClean(findChild(node, 'parameters'));
    const shortdesc  = textOfClean(findChild(node, 'shortdescription'));
    const description = textOfMultiline(findChild(node, 'description'));
    const example     = textOfMultiline(findChild(node, 'example'));
    const returntype  = textOfClean(findChild(node, 'returntype'));
    const grNode      = findChild(node, 'gr');
    const groupCode   = textOfClean(grNode);
    const groupName   = groupNameByCode[groupCode] || '';

    // Deprecation: authoritative signal is group code 31 (group name "Deprecated").
    // Also catch commands whose shortdescription/description starts with "Deprecated"
    // in case any slip through with a different group.
    const deprecated = groupCode === '31'
      || /^\s*deprecated\b/i.test(shortdesc)
      || /^\s*deprecated\b/i.test(description);

    // Pull a replacement hint when one is mentioned. The description body
    // typically reads "This command is deprecated. Use treenode.copy() instead."
    let replacedBy = '';
    const replaceSrc = description || shortdesc || '';
    const m = replaceSrc.match(/\buse\s+([A-Za-z_][\w.]*(?:\.[A-Za-z_]\w*)*)\s*(?:\(\s*\))?/i);
    if (m) replacedBy = m[1];

    return {
      name, parameters, shortdesc, description, example,
      returntype, groupCode, groupName, deprecated, replacedBy
    };
  });
}

// -----------------------------------------------------------------------
// Frontmatter rendering. Stays byte-stable across runs (sorted keys
// where order isn't semantic) so prompt-cache stays hot.
// -----------------------------------------------------------------------
function yamlList(arr) {
  if (!arr || !arr.length) return '[]';
  return '[' + arr.map(s => JSON.stringify(s)).join(', ') + ']';
}
function yamlStr(s) {
  if (s == null || s === '') return '""';
  // Avoid YAML quote pitfalls: always JSON-string (which is valid YAML).
  return JSON.stringify(String(s));
}

// -----------------------------------------------------------------------
// Class topic rendering.
// -----------------------------------------------------------------------
function renderClassTopic(cls) {
  const moduleName = cls.className.includes('::')
    ? cls.className.split('::')[0]
    : '';
  const id = classToSlug(cls.className).toLowerCase();
  // Tags inferred from module + name. Aliases left empty for the
  // generator; humans can add to the topic file by hand later.
  const tags = ['class'];
  if (moduleName) tags.push(moduleName.toLowerCase() + '-module');
  if (cls.extendsName) tags.push('extends-' + cls.extendsName.toLowerCase());
  const signature = cls.extendsName
    ? `${cls.className} extends ${cls.extendsName}`
    : cls.className;
  const description = cls.summary || `FlexScript class: ${cls.className}.`;

  // See-also: parent class.
  const seeAlso = cls.extendsName ? [classToSlug(cls.extendsName).toLowerCase()] : [];

  let out = '---\n';
  out += `id: ${id}\n`;
  out += `name: ${yamlStr(cls.className)}\n`;
  out += `kind: class\n`;
  out += `module: ${yamlStr(moduleName)}\n`;
  out += `signature: ${yamlStr(signature)}\n`;
  out += `aliases: []\n`;
  out += `tags: ${yamlList(tags)}\n`;
  out += `deprecated: false\n`;
  out += `replacedBy: null\n`;
  out += `description: ${yamlStr(description)}\n`;
  out += `seeAlso: ${yamlList(seeAlso)}\n`;
  out += `source: ${yamlStr(path.relative(fxRoot, cls.sourcePath))}\n`;
  out += '---\n\n';

  out += `# ${cls.className}\n\n`;
  if (cls.summary) out += cls.summary + '\n\n';
  if (cls.extendsName) out += `**Extends:** \`${cls.extendsName}\`\n\n`;
  if (cls.description && cls.description !== cls.summary) {
    out += cls.description + '\n\n';
  }

  // Methods (alphabetical, static first per signature group).
  if (cls.methods.length) {
    out += `## Methods\n\n`;
    const sorted = cls.methods.slice().sort((a, b) => {
      if (a.isStatic !== b.isStatic) return a.isStatic ? -1 : 1;
      return a.name.localeCompare(b.name);
    });
    for (const m of sorted) out += renderMember(m, cls.className);
  }
  if (cls.properties.length) {
    out += `## Properties\n\n`;
    const sorted = cls.properties.slice().sort((a, b) => a.name.localeCompare(b.name));
    for (const p of sorted) out += renderMember(p, cls.className);
  }
  if (cls.constructors.length) {
    out += `## Constructors\n\n`;
    for (const c of cls.constructors) out += renderMember(c, cls.className);
  }
  return out;
}

function renderMember(m, className) {
  // Compose a signature line.
  const params = (m.sigParams || []).map(p => {
    let s = (p.type ? p.type + ' ' : '') + p.name;
    if (p.defaultValue) s += ' = ' + p.defaultValue;
    return s;
  }).join(', ');
  const ret = m.sigReturn ? `${m.sigReturn} ` : '';
  const staticPrefix = m.isStatic ? '(static) ' : '';
  const accessCall = m.tag === 'Property' ? `.${m.name}` : `.${m.name}(${params})`;
  const sigLine = m.tag === 'Property'
    ? `${ret}${className}${accessCall}${m.readonly ? ' (readonly)' : ''}`
    : `${ret}${staticPrefix}${className}${accessCall}`;

  let out = `### ${m.name}\n\n`;
  out += '```flexscript\n' + sigLine + '\n```\n\n';
  if (m.summary) out += m.summary + '\n\n';
  if (m.sigParams && m.sigParams.length) {
    out += '**Parameters:**\n';
    for (const p of m.sigParams) {
      const desc = m.paramDescs && m.paramDescs[p.name] ? ' — ' + m.paramDescs[p.name] : '';
      out += `- \`${p.name}\` *(${p.type || 'any'}${p.defaultValue ? ', default ' + p.defaultValue : ''})*${desc}\n`;
    }
    out += '\n';
  }
  if (m.returnsDesc) out += `**Returns:** ${m.returnsDesc}\n\n`;
  if (m.description && m.description !== m.summary) {
    out += m.description + '\n\n';
  }
  return out;
}

// -----------------------------------------------------------------------
// Command topic rendering.
// -----------------------------------------------------------------------
function commandToSlug(name) {
  // FlexScript command names are simple identifiers — keep as-is, lowercase.
  return name.toLowerCase();
}

function renderCommandTopic(cmd) {
  const id = commandToSlug(cmd.name);
  const tags = ['command'];
  if (cmd.deprecated) tags.push('deprecated');
  if (cmd.groupName) {
    tags.push('group-' + cmd.groupName.toLowerCase().replace(/[^a-z0-9]+/g, '-').replace(/^-|-$/g, ''));
  }

  let signature = cmd.name + (cmd.parameters || '()');
  if (cmd.returntype) signature += ' -> ' + cmd.returntype;

  const description = cmd.shortdesc || `FlexScript command: ${cmd.name}.`;

  let out = '---\n';
  out += `id: ${id}\n`;
  out += `name: ${yamlStr(cmd.name)}\n`;
  out += `kind: command\n`;
  out += `module: ""\n`;
  out += `signature: ${yamlStr(signature)}\n`;
  out += `aliases: []\n`;
  out += `tags: ${yamlList(tags)}\n`;
  out += `deprecated: ${cmd.deprecated ? 'true' : 'false'}\n`;
  out += `replacedBy: ${cmd.replacedBy ? yamlStr(cmd.replacedBy) : 'null'}\n`;
  out += `description: ${yamlStr(description)}\n`;
  out += `seeAlso: []\n`;
  out += `source: ${yamlStr(path.relative(fxRoot, cmdXmlPath))}\n`;
  out += '---\n\n';

  out += `# ${cmd.name}\n\n`;
  if (cmd.deprecated) {
    out += `> ⚠ **DEPRECATED.** ${cmd.replacedBy ? 'Use `' + cmd.replacedBy + '` instead.' : 'See description for replacement.'}\n\n`;
  }
  out += '```flexscript\n' + signature + '\n```\n\n';
  if (cmd.shortdesc) out += cmd.shortdesc + '\n\n';
  if (cmd.description && cmd.description !== cmd.shortdesc) {
    out += cmd.description + '\n\n';
  }
  if (cmd.example) {
    out += `## Example\n\n\`\`\`flexscript\n${cmd.example}\n\`\`\`\n\n`;
  }
  return out;
}

// -----------------------------------------------------------------------
// Catalog rendering — the always-on symbol tables.
// -----------------------------------------------------------------------
function renderClassesCatalog(allClasses) {
  // Group by module, then sort.
  const byModule = new Map();
  for (const cls of allClasses) {
    const mod = cls.className.includes('::') ? cls.className.split('::')[0] : '(core)';
    if (!byModule.has(mod)) byModule.set(mod, []);
    byModule.get(mod).push(cls);
  }
  const modules = Array.from(byModule.keys()).sort();
  let out = '# FlexScript class catalog\n\n';
  out += `Generated by tools/build-kb.js from FlexSim help XMLs. ${allClasses.length} classes.\n`;
  out += 'For full details on any class, ask via search_kb or read_topic.\n\n';
  for (const mod of modules) {
    out += `## ${mod === '(core)' ? 'Core (no module prefix)' : mod + ' module'}\n\n`;
    const list = byModule.get(mod).slice().sort((a, b) => a.className.localeCompare(b.className));
    out += '| Class | Extends | Summary |\n';
    out += '|---|---|---|\n';
    for (const c of list) {
      const summary = (c.summary || '').replace(/\|/g, '\\|').slice(0, 120);
      out += `| \`${c.className}\` | ${c.extendsName ? '`' + c.extendsName + '`' : ''} | ${summary} |\n`;
    }
    out += '\n';
  }
  return out;
}
function renderCommandsCatalog(allCommands, opts) {
  // opts.activeOnly = true => omit the entire Deprecated group (smaller catalog
  //   intended for the always-on system prefix).
  // opts.activeOnly = false => full reference, deprecated section at the bottom.
  const activeOnly = !!(opts && opts.activeOnly);
  const filtered = activeOnly ? allCommands.filter(c => !c.deprecated) : allCommands;

  // Group by the human-readable group name (e.g. "Communication", "Deprecated").
  // Deprecated bucket is rendered LAST so the active surface dominates the
  // top of the catalog — when the LLM scans this list, it sees recommended
  // commands first.
  const byGroup = new Map();
  for (const cmd of filtered) {
    const g = cmd.groupName || '(ungrouped)';
    if (!byGroup.has(g)) byGroup.set(g, []);
    byGroup.get(g).push(cmd);
  }
  const groups = Array.from(byGroup.keys()).sort((a, b) => {
    if (a === 'Deprecated') return  1;
    if (b === 'Deprecated') return -1;
    return String(a).localeCompare(String(b));
  });

  const activeCount = allCommands.filter(c => !c.deprecated).length;
  const depCount    = allCommands.filter(c =>  c.deprecated).length;
  let out = activeOnly
    ? '# FlexScript command catalog — active only\n\n'
    : '# FlexScript command catalog — full\n\n';
  out += `Generated by tools/build-kb.js. ${allCommands.length} commands total (${activeCount} active, ${depCount} deprecated).\n`;
  if (activeOnly) {
    out += 'Deprecated commands are NOT listed here — they live in `catalog/commands.md` (the full reference). Prefer the methods on FlexScript classes over commands where one exists (e.g. `Object.location.x` over `xloc()`).\n\n';
  } else {
    out += 'Deprecated entries are marked with ⚠ and a "use X" hint. The Deprecated section is at the bottom — prefer the active commands above it.\n\n';
  }
  for (const g of groups) {
    const list = byGroup.get(g).slice().sort((a, b) => a.name.localeCompare(b.name));
    out += `## ${g}\n\n`;
    out += '| Command | Returns | Signature | Note |\n';
    out += '|---|---|---|---|\n';
    for (const c of list) {
      const sig = (c.parameters || '()').replace(/\|/g, '\\|');
      const dep = c.deprecated
        ? '⚠ deprecated' + (c.replacedBy ? ' — use `' + c.replacedBy + '`' : '')
        : (c.shortdesc || '').replace(/\|/g, '\\|').slice(0, 120);
      out += `| \`${c.name}\` | ${c.returntype || '—'} | \`${sig}\` | ${dep} |\n`;
    }
    out += '\n';
  }
  return out;
}

// -----------------------------------------------------------------------
// Alias mining. For each topic we generate structural aliases so the
// search index doesn't miss when the user spells a symbol differently
// than its canonical form. Three sources:
//
//   1. Case-style flips:   setItemSpeed <-> set_item_speed
//   2. Verb-prefix strip:  getRank -> rank        (also is/has/can prefixes)
//   3. Module-prefix strip: AGV::Trailer -> Trailer (so "trailer" still hits)
//
// A second pass elsewhere folds in the replacedBy / seeAlso graph so a
// deprecated command's name becomes an alias on the active replacement
// it points at. That second pass needs all topics extracted first.
// -----------------------------------------------------------------------
function camelToSnake(s) {
  // Two-step: insert _ between lower/digit + Upper, then before Acronym + Word.
  return s
    .replace(/([a-z0-9])([A-Z])/g, '$1_$2')
    .replace(/([A-Z]+)([A-Z][a-z])/g, '$1_$2')
    .toLowerCase();
}
function snakeToCamel(s) {
  return s.replace(/_([a-zA-Z])/g, (_, c) => c.toUpperCase());
}
function stripVerbPrefix(s) {
  // Strips set/get/is/has/can prefix. Accepts BOTH:
  //   - camelCase methods: "getRank" -> "rank"          (FlexScript class methods)
  //   - all-lowercase commands: "setrank" -> "rank"     (FlexScript legacy commands)
  // Requires the tail to be at least 2 chars so we don't strip things like
  // "set" (a verb on its own).
  const m = s.match(/^(set|get|is|has|can)([A-Za-z]\w+)$/);
  if (!m) return null;
  // Lowercase first char so "getRank" and "getrank" both produce "rank".
  return m[2].charAt(0).toLowerCase() + m[2].slice(1);
}
function generateStructuralAliases(name, kind) {
  const out = new Set();
  if (!name) return [];

  // Strip module prefix for alias purposes: "AGV::Trailer" -> "Trailer",
  // "AGV.Trailer" -> "Trailer". Keep both forms in the alias bag.
  const bareCandidates = new Set([name]);
  const colonIdx = name.indexOf('::');
  if (colonIdx >= 0) bareCandidates.add(name.slice(colonIdx + 2));
  const dotIdx = name.indexOf('.');
  if (dotIdx >= 0) bareCandidates.add(name.slice(dotIdx + 1));

  for (const bare of bareCandidates) {
    out.add(bare);

    // CamelCase -> snake_case
    const snake = camelToSnake(bare);
    if (snake !== bare.toLowerCase()) out.add(snake);

    // snake_case -> camelCase (only if there's a real underscore)
    if (bare.includes('_')) out.add(snakeToCamel(bare));

    // Verb-prefix strip — "getRank" -> "rank", "setColor" -> "color"
    const stripped = stripVerbPrefix(bare);
    if (stripped) {
      out.add(stripped);
      out.add(camelToSnake(stripped));
    }
  }

  // Drop the canonical name itself — we only want NEW surface forms.
  out.delete(name);
  return Array.from(out);
}

// -----------------------------------------------------------------------
// BM25F-style field-weighted tf. Each topic field gets a weight that
// reflects how strongly a term occurring there should contribute to the
// score. Name/aliases dominate (a query that names a symbol verbatim
// must hit that symbol); description is the soft-fallback field that
// catches "describe the behavior" queries the agent flagged.
//
// Tokenization is alphanumeric-only, lowercased — the C++ side mirrors
// this so the scoring stays consistent.
// -----------------------------------------------------------------------
function tokenize(s) {
  if (!s) return [];
  return String(s).toLowerCase().match(/[a-z0-9_]+/g) || [];
}

const FIELD_WEIGHTS = {
  name:        8.0,
  aliases:     6.0,
  signature:   4.0,
  tags:        2.0,
  kind:        1.0,
  module:      1.0,
  description: 1.0,
};

function buildBm25FStats(topics) {
  const df   = new Map();    // term -> doc-frequency (count of docs containing the term at all)
  const docs = [];           // [{ id, len, tf: { term -> weighted-count } }]
  for (const t of topics) {
    const tf = {};

    // Tokenize each field separately, scale tf by the field weight, sum
    // across fields. The result is a single weighted-tf vector per doc;
    // BM25F's weighted-length is the sum of weighted tokens.
    let weightedLen = 0;
    const accumField = (text, weight) => {
      if (!text) return;
      const toks = tokenize(text);
      if (!toks.length) return;
      for (const w of toks) tf[w] = (tf[w] || 0) + weight;
      weightedLen += toks.length * weight;
    };

    accumField(t.name,        FIELD_WEIGHTS.name);
    accumField((t.aliases || []).join(' '),  FIELD_WEIGHTS.aliases);
    accumField(t.signature,   FIELD_WEIGHTS.signature);
    accumField((t.tags || []).join(' '),     FIELD_WEIGHTS.tags);
    accumField(t.kind,        FIELD_WEIGHTS.kind);
    accumField(t.module,      FIELD_WEIGHTS.module);
    accumField(t.description, FIELD_WEIGHTS.description);

    docs.push({
      id: t.id,
      len: Math.round(weightedLen * 10) / 10, // small precision is fine
      tf,
      deprecated: !!t.deprecated,
      kind: t.kind,
    });
    // doc-frequency counts each term ONCE per doc regardless of field — this
    // is the standard BM25F treatment (IDF reflects rarity across the corpus,
    // not within fields).
    for (const term of Object.keys(tf)) df.set(term, (df.get(term) || 0) + 1);
  }
  const avgdl = docs.reduce((a, d) => a + d.len, 0) / Math.max(1, docs.length);
  return {
    n: docs.length,
    avgdl,
    df: Object.fromEntries(df),
    docs,
    field_weights: FIELD_WEIGHTS,
  };
}

// -----------------------------------------------------------------------
// Compact always-on index — the small file that lives in the cached
// system-prompt prefix in place of the heavyweight classes.md +
// commands-active.md catalogs. Goal is ~4K tokens (down from ~21K).
//
// Contains:
//   - Every class, grouped by module, names only.
//   - Every command GROUP, with the group's command count and a small
//     sample of command names (so the LLM can recognize "this is the
//     group I want" before calling list_commands(group)).
//   - A short usage hint telling the LLM how to fetch deeper details.
// -----------------------------------------------------------------------
function renderCompactIndex(allClasses, allCommands) {
  // Group classes by module
  const classByModule = new Map();
  for (const c of allClasses) {
    const mod = c.className.includes('::') ? c.className.split('::')[0] : '(core)';
    if (!classByModule.has(mod)) classByModule.set(mod, []);
    classByModule.get(mod).push(c.className);
  }
  const modules = Array.from(classByModule.keys()).sort();

  // Group ACTIVE commands by group
  const cmdByGroup = new Map();
  let activeCount = 0;
  let deprecatedCount = 0;
  for (const c of allCommands) {
    if (c.deprecated) { deprecatedCount++; continue; }
    activeCount++;
    const g = c.groupName || '(ungrouped)';
    if (!cmdByGroup.has(g)) cmdByGroup.set(g, []);
    cmdByGroup.get(g).push(c.name);
  }
  const groups = Array.from(cmdByGroup.keys()).sort();

  let out = '# FlexScript API surface — compact index\n\n';
  out += `Auto-extracted from FlexSim's shipped XML docs. ${allClasses.length} classes; ${activeCount} active commands; ${deprecatedCount} deprecated (hidden).\n\n`;
  out += 'This is the only catalog page in the cached system prompt. For full ' +
         'details on any symbol use the tools:\n' +
         '- `search_kb(query, kind?, module?)` — BM25 search over the entire index ' +
         '(classes + commands + aliases).\n' +
         '- `read_topic(id, kind?)` — fetch the full body of a single topic.\n' +
         '- `list_commands(group)` — fetch the full table of active commands in a group.\n' +
         '- `list_classes(module)` — fetch the full table of classes in a module.\n\n' +
         'Prefer FlexScript class methods over the equivalent legacy command (e.g. ' +
         '`Object.location.x` over `xloc()`). Deprecated commands are hidden from this ' +
         'index — to find a deprecated symbol\'s replacement, call ' +
         '`search_kb(query, include_deprecated: true)`.\n\n';

  out += '## Classes (by module)\n\n';
  for (const mod of modules) {
    const list = classByModule.get(mod).slice().sort();
    out += `**${mod === '(core)' ? 'Core' : mod}** (${list.length}): `;
    out += list.map(n => '`' + n + '`').join(', ');
    out += '\n\n';
  }

  out += '## Commands (by group)\n\n';
  out += 'Use `list_commands(group)` to fetch the full table for a group.\n\n';
  out += '| Group | # | Sample command names |\n';
  out += '|---|---|---|\n';
  for (const g of groups) {
    const list = cmdByGroup.get(g).slice().sort();
    const sample = list.slice(0, 6).map(n => '`' + n + '`').join(', ');
    const more = list.length > 6 ? `, … +${list.length - 6} more` : '';
    out += `| ${g} | ${list.length} | ${sample}${more} |\n`;
  }
  out += '\n';
  return out;
}

// -----------------------------------------------------------------------
// Deprecation-aware reverse-alias graph. Given a deprecated command
// whose `replacedBy` names an active class/method/command, we add the
// deprecated command's NAME as an alias on the REPLACEMENT topic. So
// when the agent (or the user, via the agent) searches for the old
// name, it finds the new replacement directly even though the old
// topic is filtered out.
//
// Run AFTER all topics have been collected so we can resolve targets.
// -----------------------------------------------------------------------
function buildReverseDeprecationAliases(allCommands, indexTopicsById) {
  let edges = 0;
  for (const cmd of allCommands) {
    if (!cmd.deprecated || !cmd.replacedBy) continue;
    const target = cmd.replacedBy.trim();
    if (!target) continue;

    // Candidates: full target, class-only (before the dot/colon), method-only (after).
    const cands = new Set([target]);
    const dotIdx = target.indexOf('.');
    if (dotIdx >= 0) {
      cands.add(target.slice(0, dotIdx));            // class
      cands.add(target.slice(dotIdx + 1));            // method
    }
    const colonIdx = target.indexOf('::');
    if (colonIdx >= 0) cands.add(target.slice(colonIdx + 2));

    // Convert each candidate to plausible topic ids (matches our slug rules).
    const tryIds = new Set();
    for (const cand of cands) {
      tryIds.add(cand.toLowerCase());                          // command slug
      tryIds.add(cand.replace(/::/g, '__').toLowerCase());     // class slug
    }
    for (const id of tryIds) {
      const t = indexTopicsById.get(id);
      if (!t) continue;
      if (!t.aliases) t.aliases = [];
      if (!t.aliases.includes(cmd.name)) {
        t.aliases.push(cmd.name);
        edges++;
      }
    }
  }
  return edges;
}

// -----------------------------------------------------------------------
// Main.
// -----------------------------------------------------------------------
function main() {
  console.log('ModelerAI KB generator');
  console.log('FlexSim install: ' + fxRoot);
  console.log('Output: ' + kbRoot);

  // Wipe + recreate generated dirs so removed entries don't linger.
  // Hand-written topics in topics/patterns and topics/concepts are not
  // touched (only topics/classes and topics/commands + catalog/ are
  // regenerated).
  for (const d of [classesDir, commandsDir, catalogDir]) {
    if (fs.existsSync(d)) {
      for (const f of fs.readdirSync(d)) {
        if (f.endsWith('.md')) fs.unlinkSync(path.join(d, f));
      }
    } else {
      ensureDir(d);
    }
  }

  // ---- Classes ----
  const classFiles = listXmlsRecursive(apiRefDir);
  console.log(`Found ${classFiles.length} class XML files.`);
  const allClasses = [];
  for (const xf of classFiles) {
    try {
      const txt = fs.readFileSync(xf, 'utf8');
      const cls = extractClass(txt, xf);
      if (!cls) {
        console.warn(`  skip (no class element): ${path.relative(fxRoot, xf)}`);
        continue;
      }
      const md = renderClassTopic(cls);
      const outPath = path.join(classesDir, classToSlug(cls.className) + '.md');
      fs.writeFileSync(outPath, md, 'utf8');
      allClasses.push(cls);
    } catch (e) {
      console.warn(`  error on ${path.relative(fxRoot, xf)}: ${e.message}`);
    }
  }
  console.log(`Wrote ${allClasses.length} class topics to ${path.relative(repoRoot, classesDir)}`);

  // ---- Commands ----
  let allCommands = [];
  if (fs.existsSync(cmdXmlPath)) {
    const cmdTxt = fs.readFileSync(cmdXmlPath, 'utf8');
    allCommands = extractAllCommands(cmdTxt);
    console.log(`Found ${allCommands.length} commands.`);
    for (const cmd of allCommands) {
      if (!cmd.name) continue;
      const md = renderCommandTopic(cmd);
      const outPath = path.join(commandsDir, commandToSlug(cmd.name) + '.md');
      fs.writeFileSync(outPath, md, 'utf8');
    }
    console.log(`Wrote ${allCommands.length} command topics to ${path.relative(repoRoot, commandsDir)}`);
  } else {
    console.warn('Commands.xml not found, skipping commands.');
  }

  // ---- Catalogs ----
  // - catalog/index.md           : compact always-on prefix (classes + group index).
  // - catalog/classes.md         : full per-module class tables (fetched on demand).
  // - catalog/commands-active.md : full per-group active command tables (on demand).
  // - catalog/commands.md        : full + deprecated (on demand, include_deprecated path).
  ensureDir(catalogDir);
  fs.writeFileSync(path.join(catalogDir, 'index.md'),            renderCompactIndex(allClasses, allCommands), 'utf8');
  fs.writeFileSync(path.join(catalogDir, 'classes.md'),          renderClassesCatalog(allClasses), 'utf8');
  fs.writeFileSync(path.join(catalogDir, 'commands.md'),         renderCommandsCatalog(allCommands, { activeOnly: false }), 'utf8');
  fs.writeFileSync(path.join(catalogDir, 'commands-active.md'),  renderCommandsCatalog(allCommands, { activeOnly: true  }), 'utf8');
  console.log(`Wrote catalog/index.md (always-on) + classes.md + commands.md + commands-active.md (on-demand)`);

  // ---- Topic records — first pass populates the structural aliases. ----
  const indexTopics = [];
  for (const cls of allClasses) {
    const moduleName = cls.className.includes('::') ? cls.className.split('::')[0] : '';
    indexTopics.push({
      id: classToSlug(cls.className).toLowerCase(),
      name: cls.className,
      kind: 'class',
      module: moduleName,
      signature: cls.extendsName ? `${cls.className} extends ${cls.extendsName}` : cls.className,
      tags: ['class'].concat(moduleName ? [moduleName.toLowerCase() + '-module'] : []),
      aliases: generateStructuralAliases(cls.className, 'class'),
      description: cls.summary || '',
      deprecated: false,
    });
  }
  for (const cmd of allCommands) {
    const tags = ['command'];
    if (cmd.deprecated) tags.push('deprecated');
    if (cmd.groupName) tags.push('group-' + cmd.groupName.toLowerCase().replace(/[^a-z0-9]+/g, '-').replace(/^-|-$/g, ''));
    indexTopics.push({
      id: commandToSlug(cmd.name),
      name: cmd.name,
      kind: 'command',
      module: '',
      group: cmd.groupName || '',
      signature: cmd.name + (cmd.parameters || '()') + (cmd.returntype ? ' -> ' + cmd.returntype : ''),
      tags,
      aliases: generateStructuralAliases(cmd.name, 'command'),
      description: cmd.shortdesc || '',
      deprecated: cmd.deprecated,
      replacedBy: cmd.replacedBy || null,
    });
  }

  // ---- Manual topics — scrape generated by tools/ingest-manual.js. ----
  // We walk KNOWLEDGE/topics/manual recursively and add each .md to the
  // BM25 index. Body text is included so BM25 matches against the full
  // content, not just the title — this is what makes search_kb useful
  // for "find me a page that explains X" queries.
  const manualDir = path.join(kbRoot, 'topics', 'manual');
  let manualCount = 0;
  let manualBytes = 0;
  if (fs.existsSync(manualDir)) {
    const manualFiles = [];
    walkMarkdownRecursive(manualDir, manualFiles);
    for (const mf of manualFiles) {
      try {
        const raw = fs.readFileSync(mf, 'utf8');
        const { frontmatter, body } = splitFrontmatter(raw);
        const fm = parseFrontmatterSimple(frontmatter);
        const id = fm.id || path.basename(mf, '.md').toLowerCase();
        const name = (fm.name || '').replace(/^"|"$/g, '');
        const breadcrumb = parseJsonArray(fm.breadcrumb);
        const tags = parseJsonArray(fm.tags);
        // Description is the first ~800 chars of the body (one or two paras
        // — enough for a meaningful snippet without drowning out class +
        // command topics in BM25 scoring). The full body lives separately
        // and feeds the per-search excerpt extraction at query time.
        const description = body.replace(/\s+/g, ' ').trim().slice(0, 800);
        indexTopics.push({
          id,
          name: name || id,
          kind: 'manual',
          module: '',
          signature: breadcrumb.join(' > '),
          tags: ['manual'].concat(tags.filter(t => t !== 'manual')),
          aliases: [],
          description,
          deprecated: false,
        });
        manualCount++;
        manualBytes += Buffer.byteLength(raw, 'utf8');
      } catch (e) {
        console.warn(`  manual topic load failed: ${path.relative(repoRoot, mf)}: ${e.message}`);
      }
    }
    console.log(`Indexed ${manualCount} manual topics (${(manualBytes / 1024).toFixed(1)} KB on disk)`);
  } else {
    console.log('No manual topics found (run tools/ingest-manual.js to scrape).');
  }

  // Second pass — deprecation reverse-alias graph: the deprecated command's
  // name becomes an alias on the active topic it points at via replacedBy.
  const byId = new Map(indexTopics.map(t => [t.id, t]));
  const edges = buildReverseDeprecationAliases(allCommands, byId);
  console.log(`Reverse-deprecation aliases: ${edges} edges added`);

  const bm25 = buildBm25FStats(indexTopics);
  fs.writeFileSync(indexJsonPath, JSON.stringify({
    version: 2,
    built_at: '(omitted for cache stability — use file mtime if you need it)',
    topics: indexTopics.map(t => ({
      id: t.id, name: t.name, kind: t.kind, module: t.module,
      signature: t.signature, deprecated: t.deprecated,
      description: t.description,
      aliases: t.aliases || [],
      tags: t.tags || [],
      replacedBy: t.replacedBy || null,
      group: t.group || '',
    })),
    bm25,
  }, null, 2), 'utf8');
  console.log(`Wrote INDEX.json (${indexTopics.length} topics, avgdl=${bm25.avgdl.toFixed(2)}, version 2 with aliases + BM25F weights)`);

  console.log('\nDone.');
}

main();

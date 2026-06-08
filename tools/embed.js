// Embed webview/index.html into ModelerAI.fsx at the html data node.
// XML-escapes the HTML (function-form replace to avoid $1/$2 interpretation).
//
// Run from the repo root:   node tools/embed.js
'use strict';
const fs = require('fs');
const path = require('path');

const repoRoot = path.join(__dirname, '..');
const fsxPath  = path.join(repoRoot, 'ModelerAI.fsx');
const htmlPath = path.join(repoRoot, 'webview', 'index.html');

const fsx  = fs.readFileSync(fsxPath, 'utf8');
const html = fs.readFileSync(htmlPath, 'utf8');

function xmlEscape(s) {
  return s.replace(/[&<>]/g, function (ch) {
    if (ch === '&') return '&amp;';
    if (ch === '<') return '&lt;';
    if (ch === '>') return '&gt;';
    return ch;
  });
}

const escaped = xmlEscape(html);

// Replace from the opening `<node f="42" dt="2"><name>html</name><data>...` block.
const openTag = '<node f="42" dt="2"><name>html</name><data>';
const closeTag = '</data></node>';

const openIdx = fsx.indexOf(openTag);
if (openIdx < 0) {
  console.error('html node open tag not found');
  process.exit(1);
}
const dataStart = openIdx + openTag.length;
const closeIdx = fsx.indexOf(closeTag, dataStart);
if (closeIdx < 0) {
  console.error('html node close tag not found');
  process.exit(1);
}

const out = fsx.slice(0, dataStart) + escaped + fsx.slice(closeIdx);
fs.writeFileSync(fsxPath, out, 'utf8');
console.log('Embedded', html.length, 'chars (escaped:', escaped.length, ') into ModelerAI.fsx');

---
name: modelerai-quickref
description: Current modelerai_* curated tool surface. Signatures only — no scenario routing, no prescriptive "use template X" claims. Always-on. Bodies are being rebuilt one at a time per docs/learning-sessions.md.
appliesWhen: Always-on. Lists what tools currently exist.
tags: modelerai, modelerai_call, quickref, tools
last_verified: 2026-06-02
---

# modelerai_* — current tool surface

All curated commands run through one bridge tool:

```
modelerai_call({ name: "modelerai_<cmd>", args: ... })
```

`args` accepts an array (positional), an object (single structured arg, JSON-encoded for transport), a string, or nothing.

## Build-time tools

These shape the model before it runs. The tool surface is being intentionally narrowed and rebuilt — many entries below are placeholders pending design. Where a tool is implemented, it's noted. Where it's `[planned]`, calling it returns `not_implemented`; use the modeler-led design conversation to specify it.

### Object lifecycle — args shapes

All three take a JSON object as `args`. Examples shown with `modelerai_call` wrapping.

**`modelerai_create_object`** — `Object.create` + optional setProperty for Vec3 transform fields + parent via `Model.find()`.
```jsonc
modelerai_call({
  name: "modelerai_create_object",
  args: {
    class: "Source",                       // required
    name: "Src1",                          // optional
    location: [5, 5, 0],                   // optional Vec3
    rotation: [0, 0, 90],                  // optional Vec3 degrees
    size:     [1, 1, 0.5],                 // optional Vec3
    parent:   "Plane1"                     // optional, passed to Model.find()
  }
})
```

**`modelerai_delete_object`** — `destroyobject(obj)`. Reports auto-disconnected wires. AGV::AGVNetwork refuses if AGV objects still exist.
```jsonc
modelerai_call({ name: "modelerai_delete_object", args: "Source1" })
// or: args: { name: "Source1" }
```

**`modelerai_set_property`** — Set any property on an object. THE TOOL for "move/resize/rotate/configure existing objects." Replaces ad-hoc `run_script` with `obj.setProperty(...)`. **Schema-gated**: before dispatching, validates the property exists on the object's class AND the value shape matches the expected `value_kind`. Errors come back as `property_not_on_class` (with suggestions) or `value_kind_mismatch` (with expected shape + valid options for enums) instead of generic FlexSim failures.
```jsonc
modelerai_call({
  name: "modelerai_set_property",
  args: {
    object:   "Source1",           // required, Model.find-compatible
    property: "Location",          // required — FlexSim property name
    value:    [5, 10, 0]           // required — shape per value_kind:
                                   //   number          -> setProperty("X", 5)
                                   //   number          -> "duniform(1,50,getstream(current))"  (FlexScript expr)
                                   //   number_with_unit-> setProperty("X", 5)  OR "expr"
                                   //   boolean         -> setProperty("X", 0|1) or true/false  OR "expr"
                                   //   string          -> setProperty("X", "value")
                                   //   enum            -> setProperty("X", 2) OR "Arrival Schedule"
                                   //   vec3            -> setProperty("X", Vec3(x,y,z))
                                   //   color_rgba      -> setProperty("X", Color(r,g,b,a))
                                   //   any             -> any of the above
  }
})
```
Discover what a class accepts via `modelerai_list_properties({class: "Source"})`. For ComboProperty (`value_kind: "enum"`), you can pass the option name as a string and the gate resolves it to the integer value automatically.

**FlexScript expressions** — for `value_kind ∈ {number, number_with_unit, boolean, any, unknown}`, you can pass a STRING containing a FlexScript expression. The tool wraps it with the standard header so `current` (= the object being configured, via `ownerobject(c)`) and `item` (= `param(1)`) are in scope, then prepends `return ` and appends `;`. Example: `value: "duniform(1, 50, getstream(current))"` becomes
```
Object current = ownerobject(c);
Object item = param(1);
return duniform(1, 50, getstream(current));
```
If your string already contains `return ` (e.g. you wrote the full body yourself), the tool passes it through unwrapped.

**`modelerai_get_property`** — Read one property off an instance. Vec3 / Color come back as JSON arrays; ComboProperty also returns `option_name` resolved from the integer value.
```jsonc
modelerai_call({
  name: "modelerai_get_property",
  args: { object: "Source1", property: "ArrivalStyle" }
})
// returns: { ok: true, object, class, property, value: 2, value_kind: "enum", option_name: "Arrival Schedule" }
```

**`modelerai_list_properties`** — Discover what's settable/readable on a class. Compact output: name + value_kind, plus options for static enums. Either pass `class` directly or pass `object` (class resolved via `classobject()`). Optional `category` substring filter.
```jsonc
modelerai_call({
  name: "modelerai_list_properties",
  args: {
    class:    "Source",            // required (unless `object` is given)
    object:   "Source1",           // alt — resolves to class
    category: "FixedResource"      // optional substring filter on schema's category
  }
})
// returns: { ok, class, property_count, properties: [{name, value_kind, category, options?}, ...] }
```

**`modelerai_clone_object`** — `src.copy()` + optional Location/offset, parent, `copy_connections` (runs `recreateObjectConnections`), `count`.
```jsonc
modelerai_call({
  name: "modelerai_clone_object",
  args: {
    source: "Src1",                        // required
    name?: "Src1Copy",                     // optional, used only when count==1
    location?: [x, y, z],                  // optional absolute
    offset?:   [dx, dy, dz],               // optional relative; stacks N times
    parent?:   "Plane1",                   // optional, defaults to source's parent
    copy_connections?: true,               // optional, recreate the wire topology
    count?: 1                              // optional, capped at 100
  }
})
```

### Connections — args shapes

All connection tools auto-append to the relevant port array and return assigned indices. Order matters everywhere (`outObjects[1]`, `centerObjects[1]` are picklist defaults).

**`modelerai_connect_fixed_resources`** — A connection between two FixedResources. Item flow.
```jsonc
modelerai_call({
  name: "modelerai_connect_fixed_resources",
  args: { from: "Source1", to: "Queue1" }
})
```

**`modelerai_connect_task_executer_to_navigator`** — `setProperty("Navigator", nav)` on the TE. GIS requires `point` and additionally A-wires the TE to the GIS::Point.
```jsonc
modelerai_call({
  name: "modelerai_connect_task_executer_to_navigator",
  args: {
    executer:  "Operator1",                // required, TaskExecuter
    navigator: "AGVNetwork1",              // required, instance name of:
                                           //   DefaultNavigator, DefaultNetworkNavigator,
                                           //   AStar::AStarNavigator, AGV::AGVNetwork,
                                           //   or GIS::GISNavigator
    point?:    "GISPoint1"                 // REQUIRED for GIS::GISNavigator, ignored otherwise
  }
})
```

**`modelerai_connect_fixed_resource_to_navigator`** — A connection from a FixedResource to a per-system anchor.
```jsonc
modelerai_call({
  name: "modelerai_connect_fixed_resource_to_navigator",
  args: {
    fr:     "Processor1",                  // required, FixedResource
    anchor: "ControlPoint5"                // required, one of:
                                           //   AGV::ControlPoint, NetworkNode,
                                           //   GIS::Point, AStar::AStarNavigator
  }
})
// returns { ..., system: "AGV"|"Network"|"GIS"|"AStar" }
```

**`modelerai_connect_dispatcher_to_task_executer`** — A connection adding the TE to the dispatcher's pool. Dispatcher must be a PURE Dispatcher (not a TaskExecuter — TEs inherit from Dispatcher).
```jsonc
modelerai_call({
  name: "modelerai_connect_dispatcher_to_task_executer",
  args: { dispatcher: "Dispatcher1", executer: "Operator1" }
})
```

**`modelerai_connect_centerport`** — S connection. Promiscuous (no class validation). Use for Use-Transport links, dispatcher↔FR, general reference, etc.
```jsonc
modelerai_call({
  name: "modelerai_connect_centerport",
  args: { from: "Processor1", to: "Operator1" }
})
// returns { ..., from_center_index, to_center_index }
```

**`modelerai_disconnect`** — kind-dispatched.
```jsonc
modelerai_call({
  name: "modelerai_disconnect",
  args: {
    from: "Source1",
    to:   "Queue1",
    kind: "flow"                           // "flow" | "centerport" | "navigator"
  }
})
// kind=navigator: from must be a TE; to is informational (the navigator being left).
// The TE's Navigator property is reset to DefaultNavigator.
```

**`modelerai_inspect_connections`** — read-only. Returns inObjects/outObjects/centerObjects (each with indices), `navigator` (from getProperty), `groups` (from >Groups walk).
```jsonc
modelerai_call({ name: "modelerai_inspect_connections", args: "Source1" })
// or: args: { object: "Source1" }
```

### Properties + introspection

| Tool | Status | One-line purpose |
|---|---|---|
| `modelerai_set_property` | implemented (schema-gated) | Set any property on an object. Schema-validates property name + value shape against `KNOWLEDGE/schemas/<Class>.json` BEFORE dispatch. `value` is number / boolean / string / `[x,y,z]` (Vec3) / `[r,g,b,a]` (Color). For ComboProperty you can pass the integer value OR the option name string. |
| `modelerai_get_property` | implemented (schema-gated) | Read a property's current value. Vec3/Color come back as arrays; ComboProperty also returns `option_name`. |
| `modelerai_list_properties` | implemented | Compact list of settable/readable properties for a class. Takes `{class}` or `{object}`; optional `category` substring filter. Each entry has `name`, `value_kind`, and for static ComboProperty, the full `options`. |
| `modelerai_extract_class_schema` | implemented | Walk `function_s(class, "enumerateProperties")` for a library class and emit a per-class property catalog. Source of truth for the three tools above. Re-run after a FlexSim version bump. |

**`modelerai_extract_class_schema`** — schema source for `set_property`. Runs on a library *class* (not a model instance). `save_to_file: true` writes `KNOWLEDGE/schemas/<Class>.json` next to the module; the wrap-all-classes pattern is `forobjecttreeunder(maintree().find("project/library")) { ... modelerai_extract_class_schema(name) }`.
```jsonc
modelerai_call({
  name: "modelerai_extract_class_schema",
  args: {
    class_name:    "Source",                 // required — bare class name ("Source") or namespaced ("AGV::Path")
    save_to_file:  true                      // optional — write KNOWLEDGE/schemas/<Class>.json
  }
})
```

### Labels

FlexSim has seven native label kinds: **Number**, **String**, **Pointer** (treenode reference), **Array**, **FlexScript**, **Bundle**, **TrackedVariable**. There is no boolean label — JSON `true`/`false` lands as a Number with value `1`/`0`.

| Tool | Status | One-line purpose |
|---|---|---|
| `modelerai_set_label` | implemented | Assert + set a label on an object. Value shape selects the kind (see below). |
| `modelerai_get_label` | implemented | Read one label. Returns `label_not_found` if missing. FlexScript labels return the raw body (not evaluated); Bundle / TrackedVariable return metadata + current value. |
| `modelerai_list_labels` | implemented | Enumerate every label on an object with name + value_kind + value. |
| `modelerai_remove_label` | implemented | Destroy a label. Returns `label_not_found` if it didn't exist (idempotent). |

**Value shape → label kind**:
```jsonc
modelerai_call({
  name: "modelerai_set_label",
  args: {
    object: "Source1",
    name:   "MyLabel",
    value:  ...                              // pick a shape:

    // Number (booleans land here as 1/0):
    // value: 42

    // String:
    // value: "hello"

    // Pointer (treenode reference) — two acceptable forms:
    // value: { object: "Queue1" }            // Model.find-style
    // value: { node_path: "MAIN:/Tools/Foo" }  // raw tree path

    // Array (mixed elements — each is number / string / pointer object):
    // value: [1, 2, "three", { object: "Queue1" }]

    // FlexScript (auto-wrapped with `Object current = ownerobject(c); Object item = param(1); return <expr>;`,
    // script flag is toggled on so reads evaluate the body):
    // value: { flexscript: "duniform(1, 50, getstream(current))" }

    // Bundle (table-data; fields[].type = number | int | string | node):
    // value: {
    //   bundle: {
    //     fields: [{name:"PartType", type:"string"}, {name:"Count", type:"int"}],
    //     rows:   [["A", 5], ["B", 3]]
    //   }
    // }

    // TrackedVariable (type defaults to time_series, start_value 0):
    // value: { tracked_variable: { type: "time_series", start_value: 0 } }
    // Other types: level_history, discrete_value, discrete_change,
    //              categorical, categorical_combo
  }
})

// list_labels: bare name OR { object } JSON
modelerai_call({ name: "modelerai_list_labels", args: "Source1" })
```

`get_label` shape for special kinds:
- `bundle` → `value: { row_count, field_count }` (use `run_script` + `getbundlevalue` to read cells)
- `flexscript` → `value: "<raw body text>"` (the body, NOT the evaluated result)
- `tracked_variable` → `value: <current numeric value>`

### Triggers

| Tool | Status | One-line purpose |
|---|---|---|
| `modelerai_set_trigger` | implemented (v1) | Write raw FlexScript to a named trigger (OnReset/OnEntry/OnExit/OnMessage/etc.); canonical `codeHeader` is auto-prepended so `current`/`item`/`port` etc. are in scope. Picklist presets deferred to v2. |
| `modelerai_get_trigger` | implemented (v1) | Read current code for one trigger; returns `set` flag, `code_header`, `stored_code`, and `user_code` (header stripped). |
| `modelerai_list_triggers` | implemented (v1) | Index every trigger declared on the object's class. Per-entry: name, code_header, params signature, `set` flag, storage_path. No bodies — pair with `get_trigger`. |
| `modelerai_remove_trigger` | implemented (v1) | Destroy a trigger's per-instance storage. Idempotent; returns `was_set` + `removed`. |

**`modelerai_set_trigger`** — write a trigger body. `code` is the raw body; do NOT redeclare named params from the header.
```jsonc
modelerai_call({
  name: "modelerai_set_trigger",
  args: { object: "Source1", trigger: "OnExit", code: "outputtoport(item, 1);" }
})
```

**`modelerai_get_trigger`** — read current code. Check `set` before assuming `user_code` is meaningful.
```jsonc
modelerai_call({
  name: "modelerai_get_trigger",
  args: { object: "Source1", trigger: "OnExit" }
})
```

**`modelerai_list_triggers`** — discover available triggers + which ones are set. Use before `set_trigger` to confirm the name + param signature.
```jsonc
modelerai_call({
  name: "modelerai_list_triggers",
  args: { object: "Source1" }
})
```

**`modelerai_remove_trigger`** — clear a trigger. Safe to call when unsure if it was set.
```jsonc
modelerai_call({
  name: "modelerai_remove_trigger",
  args: { object: "Source1", trigger: "OnExit" }
})
```

### Tools — Groups

| Tool | Status | One-line purpose |
|---|---|---|
| `modelerai_create_group` | implemented | Assert-or-create a Group by name. Optional `members` array adds them in the same call. Idempotent — returns `existing: true` when the group was already there. |
| `modelerai_group_add_member` | implemented | Add one (`member`) or many (`members: []`). Already-members are skipped, missing names land in `not_found`. |
| `modelerai_group_remove_member` | implemented | Symmetric removal. Returns `removed`, `not_member`, `not_found`. |
| `modelerai_list_groups` | implemented | Every Group in the model + each group's member count. |
| `modelerai_list_group_members` | implemented | One group's members. **Default `recursive: true`** — flattens nested groups via `toFlatArray`. Response includes `has_nested_groups` + `direct_count` so you can tell whether the flat list came from a nested structure. Pass `recursive: false` for the structural view. |

**`modelerai_create_group`** — assert/create. Idempotent on the name.
```jsonc
modelerai_call({
  name: "modelerai_create_group",
  args: {
    name:    "Operators",         // required
    members: ["Op1","Op2","Op3"]  // optional — Model.find()-resolvable names
  }
})
// returns: { ok, name, existing, path, member_count, added?, skipped?, not_found? }
```

**`modelerai_group_add_member`** / **`modelerai_group_remove_member`** — bulk operations.
```jsonc
modelerai_call({
  name: "modelerai_group_add_member",
  args: { group: "Operators", members: ["Op4","Op5"] }   // OR member: "Op4"
})
// returns: { ok, group, added, skipped, not_found: [...], total_after }
```

**`modelerai_list_group_members`** — discover what's in a group before acting on it. Default `recursive: true` flattens nested groups (so "all the operators across every sub-team" lands in one array). Pass `recursive: false` to see the structure (nested groups appear as named entries).
```jsonc
modelerai_call({
  name: "modelerai_list_group_members",
  args: { group: "Operators" }   // recursive defaults to true
})
// returns: {
//   ok, group, recursive,
//   member_count, direct_count, has_nested_groups, nested_group_count,
//   members: ["Op1", "Op2", ...]
// }
```

**Nesting**: groups can contain other groups. `create_group` / `group_add_member` / `group_remove_member` resolve member names via `Model.find(name)` first, then fall back to `Tools.get("Group", name)` — so passing `members: ["Operators", "Forklifts"]` to a parent group nests both. Use `recursive: false` on `list_group_members` to inspect the structure.

### Tools — Lists

| Tool | Status | One-line purpose |
|---|---|---|
| `modelerai_create_list` | [planned] | Create a List with field setup. (Runtime push/pull are FlexScript inside the model — not curated tools.) |

### Tools — Parameter Tables

| Tool | Status | One-line purpose |
|---|---|---|
| `modelerai_add_parameter` | implemented | Add a parameter with optional `on_set`/`on_constrain` bodies. |
| `modelerai_set_parameter` | implemented | Update a parameter's current value. |
| `modelerai_get_parameter` | implemented | Read one parameter's current value + `value_kind`. Global lookup (cross-table). |
| `modelerai_list_parameters` | implemented | Every parameter (or every parameter in one table if `table_name` passed) with name + table + value + value_kind. |
| `modelerai_remove_parameter` | implemented | Destroy a parameter by name (global lookup across all ParameterTables; `table_name` optional disambiguator). |

### Tools — Global Tables

| Tool | Status | One-line purpose |
|---|---|---|
| `modelerai_create_global_table` | implemented | Assert/create a GlobalTable. Optional `rows` / `cols` / `row_headers` / `col_headers` / `cells` (2D array of numbers or strings). Idempotent — returns `existing: true` when already present. |
| `modelerai_resize_global_table` | implemented | `setSize(rows, cols)` on an existing table. Optional `overwrite: true` resets pre-existing cells. |
| `modelerai_set_global_table_cell` | implemented | Write one cell. **1-indexed**. Value is number / boolean (becomes 0/1) / string. Returns `index_out_of_range` if `row`/`col` exceeds current dimensions. |
| `modelerai_get_global_table_cell` | implemented | Read one cell + `value_kind` (number / string / null). 1-indexed; out-of-range errors symmetric with set. |
| `modelerai_list_global_tables` | implemented | Every GlobalTable in the model + each table's `num_rows` / `num_cols`. |

**`modelerai_create_global_table`** — idempotent assert/create.
```jsonc
modelerai_call({
  name: "modelerai_create_global_table",
  args: {
    name: "ProcessRates",        // required
    rows: 4, cols: 3,            // optional dims
    row_headers: ["A","B","C","D"],    // optional
    col_headers: ["Min","Mean","Max"], // optional
    cells: [                     // optional 2D values; 0-indexed in JSON, 1-indexed in FlexScript
      [1, 5, 10],
      [2, 6, 12],
      [0, 0, 0],
      [3, 8, 15]
    ]
  }
})
// returns: { ok, name, existing, path, num_rows, num_cols, cells_written, cells_skipped }
```

**`modelerai_set_global_table_cell`** / **`modelerai_get_global_table_cell`** — 1-indexed.
```jsonc
modelerai_call({ name: "modelerai_set_global_table_cell",
  args: { table: "ProcessRates", row: 2, col: 3, value: 14 }})
// returns: { ok, table, row, col, value_kind }

modelerai_call({ name: "modelerai_get_global_table_cell",
  args: { table: "ProcessRates", row: 2, col: 3 }})
// returns: { ok, table, row, col, value, value_kind }
```

**Bundle vs tree storage** — every GlobalTable is backed by ONE of two storage formats:

- **Tree table** (what `create_global_table` builds today) — each cell is its own tree node. Flexible per-cell types; cheap to mutate; fine for small/medium reference tables (hundreds of rows).
- **Bundle table** — a single `DATA_BUNDLE` node with typed fields (columns are typed up front, not per-cell). Much faster for large datasets and supports `addIndex()` for O(log n) lookups via `Table.getRowByKey` / `getValueByKey`. Used internally by StatisticsCollector outputs.

**Known behavior caveats for bundle-backed tables — unverified by us, surface to the modeler:**
- `Table.cell(r,c)` definitively throws on bundles.
- Whether `Table()[r][c]` bracket-indexing (which is what our `set_global_table_cell` / `get_global_table_cell` tools use under the hood) also throws is UNCONFIRMED in our docs. Some Table accessors route around `cell()`, others don't. **Don't assume our cell tools work on bundle-backed tables — try the call, and if it errors, fall back to the dedicated bundle commands** (`setbundlevalue(datanode, entry, field, val)`, `getbundlevalue(datanode, entry, field)`, both 0-indexed, field by name or number) via `run_script` against the bundle's data node.

Suggest a UI conversion to bundle to the modeler when:
- the table will hold thousands of rows
- the model reads the same key column on every event (an index will dominate the speedup)
- the data is naturally numeric / homogeneous per column

Bundle-creation tools aren't surfaced yet. When triggers / stats_collectors land we'll have concrete usage patterns to design a proper bundle API against; until then, treat bundles as advanced territory the AI passes through to the modeler.

### Tools — Performance Measures

| Tool | Status | One-line purpose |
|---|---|---|
| `modelerai_create_performance_measure` | implemented | Define a KPI with a FlexScript body. |
| `modelerai_get_performance_measures` | implemented | Evaluate all PMs; returns `{ name → value }`. |
| `modelerai_get_performance_measure` | implemented | Evaluate one PM by name. |
| `modelerai_list_performance_measures` | implemented | Metadata only — name/table/desc/units. Does NOT evaluate. |
| `modelerai_delete_performance_measure` | implemented | Destroy a PM. |

### Tools — Statistics Collectors

| Tool | Status | One-line purpose |
|---|---|---|
| `modelerai_create_statistics_collector` | [planned] | Will land after the simpler categories are stable — has 16 picklist sub-families. |

## Discovery tools

For "what's in the model / what does this object look like / what classes are available" — before reaching for `set_property` / `create_object` you don't have to guess at, ask the model.

| Tool | Status | One-line purpose |
|---|---|---|
| `modelerai_list_classes` | implemented | Names + property counts of every class with an on-disk schema. Optional `category` substring filter narrows by property category (e.g. "FixedResource"). |
| `modelerai_get_object_info` | implemented | One-shot characterization of an instance: class, path, location/rotation/size, parent, groups it belongs to, label count + label names, subnode count. Use before mutating an object you don't know much about. |

```jsonc
modelerai_call({ name: "modelerai_list_classes", args: {} })
// returns: { ok, class_count, classes: [{name, property_count, extracted_at}, ...] }

modelerai_call({ name: "modelerai_get_object_info", args: { object: "Processor3" }})
// returns: {
//   ok, name, class, path,
//   location: {x,y,z}, size: {x,y,z}, rotation: {x,y,z},
//   parent: {name, class}?,
//   groups: ["Workcells", ...],
//   label_count, label_names: [...],
//   subnode_count
// }
```

## Run-time tools (model execution + reading state)

These are already built and stable.

| Tool | Purpose |
|---|---|
| `modelerai_reset_model` | Reset the model (clears stats, returns to t=0). |
| `modelerai_run_to_time` | Run until a target sim time. Blocks; returns `{completed, final_sim_time, reason}`. |
| `modelerai_run_to_end` | Run until the event queue drains naturally. No target. Returns same shape; `reason: "events_drained"` is the normal terminal state. |
| `modelerai_run_until` | Run step-by-step until a condition is met. Accepts either `{expression: "..."}` (FlexScript) or `{pm, op, value}` (structured PM check). `step()` ignores user-defined stop times — use `run_to_time` if those matter. Returns `{steps_taken, final_condition_value, reason}`. |
| `modelerai_run` | Async start. Pair with `wait_for_stop`. |
| `modelerai_stop_model` | Clean stop. |
| `modelerai_step_model` | Single event. |
| `modelerai_wait_for_stop` | Block until OnRunStop fires. |
| `modelerai_get_run_state` | Returns `{run_state, sim_time}`. |
| `modelerai_add_stop_time` | Persistent stop time. |
| `modelerai_set_warmup_time` | Single warmup. |
| `modelerai_set_run_speed` | Playback speed. |
| `modelerai_install_run_hooks` / `_uninstall_run_hooks` | Manage the OnRunStart/Stop/Reset trigger hooks the DLL uses to detect run completion. |
| `modelerai_get_object_stats` | Read curated stats for one object. |
| `modelerai_get_model_summary` | Headline rollup: sim time, run state, class counts, flow balance. |
| `modelerai_export_tree_json` | Dump a tree subtree as annotated JSON (kind/class/value/coupling target). Walks BOTH model-tree `subnodes` AND the object's attribute tree (`attributes`). Always set guard flags before exporting heavy nodes (warehouse racks, GlobalTables, library classes) — see flags below. |

**`modelerai_export_tree_json`** — flags you SHOULD set before walking anything you don't have a depth/size feel for. The default ceilings are generous; the AI is expected to clip them down for unknown territory.

```jsonc
modelerai_call({
  name: "modelerai_export_tree_json",
  args: {
    target:                  "MAIN:/...",     // path | treenode | null (uses selection)
    output:                  "inline",        // "file" (default) | "inline"
    max_depth:               4,               // 0 = unlimited (hard cap 64 regardless)
    max_nodes:               2000,            // 0 = unlimited; stops cleanly when hit
    include_subnodes:        true,            // default true
    include_attributes:      true,            // default true; turn OFF when you only want the model-tree side
    include_bundle_data:     false,           // DATA_BUNDLE (GlobalTable rows). Default false = marker only
    include_byteblock_value: true,            // strings/scripts. Default true
    include_float_value:     true,            // numbers. Default true
    include_coupling_target: true,            // coupling target path. Default true
    max_string_length:       4096,            // 0 = unlimited; truncates long byteblocks/scripts
    skip_names:              [">behaviour", ">classes", ">visual"]   // prune by node name at any depth
  }
})
```
When any limit fires, the root payload gains a `"truncated": { reason, node_count, nodes_skipped, depth_clipped }` block so the AI can see what got cut.

## Discovery posture

This quickref lists tool names only. For anything more — how to use a tool, what a class is, what a command does — call `search_kb(query)` or `read_topic(id)`. Auto-generated topics for every FlexScript class and command are indexed and findable; hand-curated coverage is being rebuilt one topic at a time per `docs/learning-sessions.md`. If a needed topic doesn't exist yet, tell the modeler — don't invent one.

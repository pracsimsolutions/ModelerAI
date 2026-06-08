# KNOWLEDGE/schemas — per-class property catalogs

Machine-readable schemas, one JSON file per FlexSim object class.
Drives `modelerai_set_property` / `_get_property` / `_list_properties` so
the AI can't set a property that doesn't exist on a class, and can't pass
an array where a number was expected.

## How these are generated

Auto-extracted by `modelerai_extract_class_schema`, which walks
`function_s(class, "enumerateProperties")` on each library class and emits
per-property: `name`, `type` (NumberProperty / ComboProperty /
Vec3Property / ColorProperty / TableProperty / etc.), `value_kind`
(number / boolean / enum / vec3 / color_rgba / array / table /
connections / number_with_unit / any / unknown), `source` (global vs
class), `localized_name`, `category`, folded composite `components`
(Color.RGBA, Location.XYZ), and ComboProperty `options` (static inline
options as `[{name, value}, ...]`).

`value_kind == "unknown"` means the property's superclass chain stops at
`BaseProperty` with no more specific type marker — typically a read-only
system handle (Class, Container, AllGroups). AI should treat these as
not-settable.

## Regenerate after a FlexSim version bump

In the FlexScript console:

```cpp
treenode lib = maintree().find("project/library");
forobjecttreeunder(lib) {
    if (content(a) == 0) {
        string name = a.name;
        applicationcommand("modelerai_extract_class_schema",
            "{\"class_name\":\"" + name + "\",\"save_to_file\":true}");
    }
}
```

Each call overwrites `<Class>.json` with `std::ios::trunc`, so reruns are
clean. Module classes (AGV::Path, GIS::Point, etc.) are found by
recursive name-search anchored at `project/library`, so namespaced names
land at the bare-class filename (`Path.json`, not `AGV__Path.json` —
this means namespace collisions across modules would overwrite; not an
issue today).

## What's NOT in here

- Per-instance overrides. Schemas describe what the class supports, not
  what a placed instance currently has set. Read instance state with
  `modelerai_get_property` (planned) or by selecting + exporting via
  `modelerai_export_tree_json`.
- Dynamic ComboProperty options that get added at model-build time. Those
  only appear on instances, not on the class definition.
- Trigger / picklist bodies. Triggers are their own subsystem
  (`modelerai_set_trigger`, planned).

---
id: resetuniqueid
name: "resetuniqueid"
kind: command
module: ""
signature: "resetuniqueid(obj flowitem)"
aliases: []
tags: ["command", "group-fixedresources"]
deprecated: false
replacedBy: null
description: "Resets the uniqueid attributes on a flowitem to a value that has not been assigned to another flowitem"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# resetuniqueid

```flexscript
resetuniqueid(obj flowitem)
```

Resets the uniqueid attributes on a flowitem to a value that has not been assigned to another flowitem

This command resets the uniqueid attribute on a flowitem to a value that has not been assigned to another flowitem already. This attribute is required to collect Full History information for the Full Report. This command should be used if a flowitem has not been destroyed but needs to be considered as a new item for data collection purposes. For example, if a flowitem is being recycled by a modeler (not with the built-in recycling functionality), they should call this when the flowitem is returned to circulation. Flowitems typically only have the uniqueid attribute when Full History collection is enabled.

## Example

```flexscript
resetuniqueid(item);
```


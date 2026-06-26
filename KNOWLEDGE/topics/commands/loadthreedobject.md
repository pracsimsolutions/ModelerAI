---
id: loadthreedobject
name: "loadthreedobject"
kind: command
module: ""
signature: "loadthreedobject(str file, str name) -> num"
aliases: []
tags: ["command", "group-media"]
deprecated: false
replacedBy: null
description: "FlexScript command: loadthreedobject."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# loadthreedobject

```flexscript
loadthreedobject(str file, str name) -> num
```

For developer use. Load a 3D model directly. A shape file on disk will be loaded into FlexSim and recorded in the Media List. The media list of FlexSim will keep a list of all loaded shapes. Each shape has a corresponding name and an index. This command will return the index corresponding to the loaded shape.

## Example

```flexscript
loadthreedobject("c:/myshape.wrl","myshape")
```


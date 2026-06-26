---
id: objectsoundcode
name: "objectsoundcode"
kind: command
module: ""
signature: "objectsoundcode(obj object) -> num"
aliases: []
tags: ["command", "group-media"]
deprecated: false
replacedBy: null
description: "Gets the index of object's default sound"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# objectsoundcode

```flexscript
objectsoundcode(obj object) -> num
```

Gets the index of object's default sound

Get the index of the object's default sound. The media list of FlexSim will keep a list of all loaded sounds. Each sound has a corresponding name and an index. This function will return the index corresponding to the objects default sound assuming the object has a sound and soundindex attribute (default objects do not at this time)

## Example

```flexscript
objectsoundcode(current)
```


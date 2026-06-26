---
id: getsoundindex
name: "getsoundindex"
kind: command
module: ""
signature: "getsoundindex(str name) -> num"
aliases: []
tags: ["command", "group-media"]
deprecated: false
replacedBy: null
description: "Gets the index of a loaded sound"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getsoundindex

```flexscript
getsoundindex(str name) -> num
```

Gets the index of a loaded sound

Returns the index of a loaded sound by the name associated with it in the project/media/sounds folder.

## Example

```flexscript
getsoundindex("C:/My Sounds/APPLAUSE.WAV");
```


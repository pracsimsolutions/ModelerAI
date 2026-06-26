---
id: getlastfiletype
name: "getlastfiletype"
kind: command
module: ""
signature: "getlastfiletype() -> num"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "FlexScript command: getlastfiletype."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getlastfiletype

```flexscript
getlastfiletype() -> num
```

For developer use. Returns the type of the last file saved. 1 = Model file 2 = Project file 3 = View file 4 = Session file 0 = Other file

## Example

```flexscript
if (getlastfiletype()) {pt("Known file type"); pr(); }
```


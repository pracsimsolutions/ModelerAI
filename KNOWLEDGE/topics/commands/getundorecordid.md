---
id: getundorecordid
name: "getundorecordid"
kind: command
module: ""
signature: "getundorecordid(obj record) -> num"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: getundorecordid."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getundorecordid

```flexscript
getundorecordid(obj record) -> num
```

Returns the id of the specified undo record.
See Miscellaneous Concepts > Advanced Undo for more information.

## Example

```flexscript
int id = getundorecordid(getcurrentundorecord(view));
```


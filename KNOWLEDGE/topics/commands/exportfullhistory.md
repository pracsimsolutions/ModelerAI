---
id: exportfullhistory
name: "exportfullhistory"
kind: command
module: ""
signature: "exportfullhistory([str filename, num scenario, num replication]) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: null
description: "Deprecated"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# exportfullhistory

> ⚠ **DEPRECATED.** See description for replacement.

```flexscript
exportfullhistory([str filename, num scenario, num replication]) -> num
```

Deprecated

This command is deprecated. Writes to a database (.mdb file) the Full History data that was collected during a run. If a file name is passed to this command, that name willbe used for the database. If no file name is passed in, the user will be prompted for a file to save the data to. If the file (either passed in or selected from a dialog) does not exist, it will be created.

## Example

```flexscript
exportfullhistory("myrundata.mdb");
```


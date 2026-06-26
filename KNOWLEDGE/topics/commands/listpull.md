---
id: listpull
name: "listpull"
kind: command
module: ""
signature: "listpull(str/obj list, str/node query, num requestNum [, num requireNum, str/node/num puller, str/node/num partitionId, num flags]) -> node/str/num/array"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "List.pull"
description: "Deprecated, use List.pull()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# listpull

> ⚠ **DEPRECATED.** Use `List.pull` instead.

```flexscript
listpull(str/obj list, str/node query, num requestNum [, num requireNum, str/node/num puller, str/node/num partitionId, num flags]) -> node/str/num/array
```

Deprecated, use List.pull()

This command is deprecated. Use List.pull() instead.

Pulls one or more entries from a list.


---
id: getstatetableutilization
name: "getstatetableutilization"
kind: command
module: ""
signature: "getstatetableutilization(Table stateTable, TrackedVariable stateProfile) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "FlexScript command: getstatetableutilization."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getstatetableutilization

```flexscript
getstatetableutilization(Table stateTable, TrackedVariable stateProfile) -> num
```

Returns the utilization of a state profile given a state table. See State Table for more information on how to create and modify a state table.

## Example

```flexscript
getstatetableutilization(Table("StateTable1"), Model.find("Processor1").as(Object).stats.state())
```


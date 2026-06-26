---
id: exportdataset
name: "exportdataset"
kind: command
module: ""
signature: "exportdataset(node datatree, str filename, num format[, str wrOption = \"w\"])"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "FlexScript command: exportdataset."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# exportdataset

```flexscript
exportdataset(node datatree, str filename, num format[, str wrOption = "w"])
```

Export tree dataset to .CSV (Comma Separated Value) format. the format parameter can be: 1 = table 2 = graph 3 = graphxy This implements the opposite functionality of importdataset(). Refer to importdataset() for more information.

## Example

```flexscript
exportdataset(so(),"mydataset.csv",1)
```


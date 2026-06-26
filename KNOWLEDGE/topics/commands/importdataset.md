---
id: importdataset
name: "importdataset"
kind: command
module: ""
signature: "importdataset(node datatree, str filename, num format)"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "FlexScript command: importdataset."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# importdataset

```flexscript
importdataset(node datatree, str filename, num format)
```

Import tree dataset from a Comma Separated Value (CSV) file. datatree should be the node to import the data into. There are 3 possible formats to import. If format is 1, then the data is imported as a table, meaning each sub-node of datatree is a row in the table, and each sub-node of each row is a cell in the table. This is the same as using the importtable() command. If format is 2, then the data is imported as a simple data series. The csv file is assumed to have one column, and each row's value is imported into a sub-node of datatree. If format is 3, then the data is imported as a series of x,y pairs. The csv file is assumed to have 2 columns. Row 1 column 1 of the csv file is imported as sub-node 1 of datatree, row 1 column 2 is imported as sub-node 2 of datatree, row 2 column 1 is imported as sub-node 3 of datatree, and so forth.

## Example

```flexscript
importdataset(so(),"C:\\mydataset.csv",1)
```


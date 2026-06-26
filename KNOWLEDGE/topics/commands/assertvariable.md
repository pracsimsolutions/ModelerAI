---
id: assertvariable
name: "assertvariable"
kind: command
module: ""
signature: "assertvariable(obj object, str variablename, num datatype) -> node"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "Gets a reference to variable variablename of object, creates the variable if it does not exist"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# assertvariable

```flexscript
assertvariable(obj object, str variablename, num datatype) -> node
```

Gets a reference to variable variablename of object, creates the variable if it does not exist

This command returns a reference to a variable with the given name. If the variable does not exist, one is created with the defined data type.

## Example

```flexscript
treenode newvar = assertvariable(Model.find("/Processor1"),"MyVariable", DATATYPE_NUMBER);
```


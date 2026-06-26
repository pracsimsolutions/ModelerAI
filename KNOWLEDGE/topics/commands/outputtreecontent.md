---
id: outputtreecontent
name: "outputtreecontent"
kind: command
module: ""
signature: "outputtreecontent(node containernode, str path+filename)"
aliases: []
tags: ["command", "group-output"]
deprecated: false
replacedBy: "menu"
description: "Writes the data of all nodes in the tree under containernode to a text file"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# outputtreecontent

```flexscript
outputtreecontent(node containernode, str path+filename)
```

Writes the data of all nodes in the tree under containernode to a text file

Writes the data of all nodes in the tree under containernode to a text file on disk. If file does not exist, it will be created. For more functionality, use menu option: Stats | Model Documentation.

## Example

```flexscript
outputtreecontent(model(),"C:/MyModelDocumentation.txt")
```


---
id: gettenavigator
name: "gettenavigator"
kind: command
module: ""
signature: "gettenavigator(obj taskexecuter) -> obj"
aliases: []
tags: ["command", "group-networknodes"]
deprecated: false
replacedBy: null
description: "Gets the current navigator that a taskexecuter is connect to"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# gettenavigator

```flexscript
gettenavigator(obj taskexecuter) -> obj
```

Gets the current navigator that a taskexecuter is connect to

Returns the current navigator that a taskexecuter (TE) is connected to, either DefaultNavigator or DefaultNetworkNavigator.

## Example

```flexscript
treenode navigator = gettenavigator(current);
```


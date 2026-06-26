---
id: nodefunctionlisten
name: "nodefunctionlisten"
kind: command
module: ""
signature: "nodefunctionlisten(node theNode, node relayNode[, num flags, par1, par2, ... par10]) -> node"
aliases: []
tags: ["command", "group-object-functions"]
deprecated: false
replacedBy: null
description: "FlexScript command: nodefunctionlisten."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# nodefunctionlisten

```flexscript
nodefunctionlisten(node theNode, node relayNode[, num flags, par1, par2, ... par10]) -> node
```

For developer use. This command will create a listening mechanism where whenever nodefunction() is called on theNode, relayNode will also be executed.
flags is a bitwise mask of one of the following values:
NFL_PRE_LISTEN: The listening function will be called before theNode is called.
NFL_LISTEN: (default) The listening function will be called after theNode is called.
NFL_PERSIST: The listener will persist across model resets.
NFL_PASS_THROUGH_PARAMS: When theNode is called, the parameters passed into that function will also be "passed through"
into the listening node. In this case, par1, par2, etc. are not used.
When the listening node is called, if it returns NFL_STOP_LISTENING, the listener mechanism will be removed and the listening node will no longer be called.
The return value is a reference to a node associated with the listening. If this node is later deleted, the listening mechanism will stop.

## Example

```flexscript
nodefunctionlisten(getvarnode(current, "entrytrigger"), current.entrylistener);
```


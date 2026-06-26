---
id: listenerinfo
name: "listenerinfo"
kind: command
module: ""
signature: "listenerinfo(int info) -> num/node"
aliases: []
tags: ["command", "group-events"]
deprecated: false
replacedBy: "within"
description: "FlexScript command: listenerinfo."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# listenerinfo

```flexscript
listenerinfo(int info) -> num/node
```

For developer use. This command returns information for use within the OnListen and OnPreListen event functions.
The information returned is information that was passed to the event you are listening to.
Info 1 will return the engine event code. Engine event codes have macros such as SM_MESSAGE and SM_DRAW.
Info 2 and 3 return pointers to associated treenodes.

To set up listening, you add a "listeners" attribute to the object you want to listen to (Object A). The listeners attribute should have
subnodes that are couplings to the object(s) that you want to listen from (Object B). Before an event fires on Object A, Object B's OnPreListen
event function will fire. After an event first on Object A, Object B's OnListen event function will fire. You can also filter events
by additing a subnode with number data below Object B's coupling node. The number data should be a bitwise sum of the bitshifted event codes you want to
listen for. If the coupling has no subnode, the object will default to listening to every event.

## Example

```flexscript
treenode current = c;
treenode involved = i;
int code = listenerinfo(1);
treenode associated = tonode(listenerinfo(2));
treenode coupling = tonode(listenerinfo(3));

print("OnListen");
print("current: ", current);
print("involved: ", involved);
print("code: ", code);
print("associated: ", associated);
print("coupling: ", coupling);
```


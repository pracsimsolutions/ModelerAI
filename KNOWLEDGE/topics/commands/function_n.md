---
id: function_n
name: "function_n"
kind: command
module: ""
signature: "function_n(obj object, num index [, num param_a,...])"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: function_n."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# function_n

```flexscript
function_n(obj object, num index [, num param_a,...])
```

For developer use. Executes code associated with a node found in the object's eventfunctions group having the specified event code. The event code is not the rank of the node in the eventfunctions group, but it corresponds to one of the following pre-defined FlexSim event :

OnReset 1
OnMessage 2
OnEvent 3
OnDraw 4
OnInopen 5
OnOutopen 6
OnReceive 7
OnSend 8
OnEntering 9
OnExiting 10
OnCompile 11
OnCreate 12
OnDestroy 13
OnLoad 14
OnCollision 15
OnClick 16
OnRunend 17
OnRunstart 18
OnInterrupted 19
OnCaptured 20
OnListen 21
OnPredraw 22
OnDrawplanar 23
OnPredrawplanar 24
OnPreListen 25

## Example

```flexscript
function_n(current,1,1,2,3)

Executes the OnReset event function defined on the current object.
```


---
id: savestoppedtransportin
name: "savestoppedtransportin"
kind: command
module: ""
signature: "savestoppedtransportin(obj fixedresource, obj taskexecuter)"
aliases: []
tags: ["command", "group-fixedresources"]
deprecated: false
replacedBy: null
description: "FlexScript command: savestoppedtransportin."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# savestoppedtransportin

```flexscript
savestoppedtransportin(obj fixedresource, obj taskexecuter)
```

This should only be executed from the transport in complete trigger of a BasicFR object. If the BasicFR is not ready to receive an item that a taskexecuter is about to unload, it must save off a reference to the taskexecuter, then return the value -1.The taskexecuter will then go into a blocked state and wait until the BasicFR is ready to receive it. When the BasicFR becomes ready to receive the item(s), it should call the resumetransportsin() command. This will look up saved references and notify their corresponding taskexecuters that they can immediately resume their unload operation.

## Example

```flexscript
if(getvarnum(current, "nrofstops") > 0){ savestoppedtransportin(current, transporter); return -1;}
```


---
id: savestoppedtransportout
name: "savestoppedtransportout"
kind: command
module: ""
signature: "savestoppedtransportout(obj fixedresource, obj taskexecuter)"
aliases: []
tags: ["command", "group-fixedresources"]
deprecated: false
replacedBy: null
description: "FlexScript command: savestoppedtransportout."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# savestoppedtransportout

```flexscript
savestoppedtransportout(obj fixedresource, obj taskexecuter)
```

This should only be executed from the transport out complete trigger of a BasicFR object. If the BasicFR is not ready to send an item that a taskexecuter is about to load, it must save off a reference to the taskexecuter, then return the value -1.The taskexecuter will then go into a blocked state and wait until the BasicFR is ready to send the item. When the BasicFR becomes ready to send the item, it should call the resumetransportsout command. This will look up saved references and notify their corresponding taskexecuters that they can immediately resume their load operation.

## Example

```flexscript
if(getvarnum(current, "nrofstops") > 0){ savestoppedtransportout(current, transporter); return -1;}
```


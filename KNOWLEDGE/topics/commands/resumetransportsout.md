---
id: resumetransportsout
name: "resumetransportsout"
kind: command
module: ""
signature: "resumetransportsout(obj fixedresource [, num saverank])"
aliases: []
tags: ["command", "group-fixedresources"]
deprecated: false
replacedBy: null
description: "FlexScript command: resumetransportsout."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# resumetransportsout

```flexscript
resumetransportsout(obj fixedresource [, num saverank])
```

This should only be executed from triggers of a BasicFR object, and usually will only be executed from the resume object field. This command looks up references to taskexecuters that have been saved using the savestoppedtransportout command, and notifies those taskexecuters that they can resume their load operation.The saverank parameter refers to a rank in the tree of a specific saved reference. If the saverank parameter is not specified, all saved references will be resumed. If this parameter is specified, only the taskexecuter associated with the specific rank will be allowed to resume. Saved references are stored as sub-nodes in the BasicFR's nroftransportsout variable. References to specific saved taskexecuters can be referenced by: nroftransportsoutnode.subnodes[saverank].value

## Example

```flexscript
resumetransportsout(current);
```


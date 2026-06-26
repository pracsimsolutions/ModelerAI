---
id: attlink
name: "attlink"
kind: command
module: ""
signature: "attlink(node viewdatanode, str path) -> node"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: attlink."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# attlink

```flexscript
attlink(node viewdatanode, str path) -> node
```

For developer use. Find a node by text path, relative to the view's objectfocus. This command will return the node on the path relative to the object focus of the ownerview of viewdatanode. For example, let viewdatanode be a node inside the structure of a view, such as an OnPress event node. Then, let the objectfocus of the parent view point to an object in the model called "Processor". Now, whatever is passed to the path argument of attlink() will be relative to the Processor object. So, if ">variables" was passed to path in the attlink call, then attlink would return the pointer to "variables" of the Processor. If the node does not exist, attlink will return a null "safenode" which signals a failure to find the requested node. The return value can be tested for validity with the objectexists() command.

## Example

```flexscript
attlink(so(),">variables") // where so() is data node inside view
```


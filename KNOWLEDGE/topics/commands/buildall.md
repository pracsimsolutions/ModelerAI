---
id: buildall
name: "buildall"
kind: command
module: ""
signature: "buildall()"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: buildall."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# buildall

```flexscript
buildall()
```

For developer use. This command builds Flexscript trees for all of the nodes in the tree that are designated as Flexscript nodes. A Flexscript node must have a Flexscript tree made before it can run. This option is available in the main menu under Build -> Build All Flexscript. This must be done before running a model with Flexscript written anywhere in it. It should also be done after making any changes to GUIs.

## Example

```flexscript
buildall();
This builds Flexscript trees for all Flexscript nodes in the tree.
```


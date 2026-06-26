---
id: releaseitem
name: "releaseitem"
kind: command
module: ""
signature: "releaseitem(obj flowitem [, num port ]) -> num"
aliases: []
tags: ["command", "group-fixedresources"]
deprecated: false
replacedBy: null
description: "Releases the flowitem from its current object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# releaseitem

```flexscript
releaseitem(obj flowitem [, num port ]) -> num
```

Releases the flowitem from its current object

Releases the specified flowitem. A flowitem is released when it is free to exit the object currently containing it, at which time the Send to Port is evaluated. This command should only be used if the flowitem is in a BasicFR object and the flowitem has not been released already, or if it is any FixedResource and has been held with holditem(). The second paramter, port, is optional. If passed, the sendto will not be evaluated, but the item will instead be released out the specified port. If the flowitem exits, releaseitem will return 1. Otherwise it will return 0.See also receiveitem() command.

## Example

```flexscript
releaseitem(item)
```


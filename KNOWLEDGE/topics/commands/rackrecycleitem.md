---
id: rackrecycleitem
name: "rackrecycleitem"
kind: command
module: ""
signature: "rackrecycleitem(obj rack, obj item, num binrank)"
aliases: []
tags: ["command", "group-rack-object"]
deprecated: false
replacedBy: null
description: "Recycles the flowitem into the flowitem bin"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# rackrecycleitem

```flexscript
rackrecycleitem(obj rack, obj item, num binrank)
```

Recycles the flowitem into the flowitem bin

Recycles the flowitem into the flowitem bin specified. This command is for advanced users, and should not be used unless you are prepared to do some debugging. Bin rank should be the rank of the flowitem in the flowitem bin (textured colored box = 5, etc.). When this command is called, the flowitem is recycled, but the spot where it is located is designated as taken, and further flowitems will be put into the rack as if this spot is already filled. This can significantly increase the speed of a model. Subsequent commands to rackgetbaycontent(), rackgetcellcontent(), etc. will return values as if the item were still there. However, rackgetitembybaylevel() will return NULL for an item that has been recycled and has yet to be restored. Make sure the item is not released or waiting for a transport before calling this command. Note that max content of the rack will not work properly if this command is used. Also you will need to eventually restore the item using the rackrestoreitem() command.

## Example

```flexscript
rackrecycleitem(current, item, 5);
This recycles the flowitem into the 5th ranked flowitem of the flowitem bin (usually the textured colored box).
```


---
id: transportincomplete
name: "transportincomplete"
kind: command
module: ""
signature: "transportincomplete(obj object, obj item [, num inport, obj transporter])"
aliases: []
tags: ["command", "group-fixedresources"]
deprecated: false
replacedBy: "Transport"
description: "Notifies the FixedResource object that item will now be moved into it"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# transportincomplete

```flexscript
transportincomplete(obj object, obj item [, num inport, obj transporter])
```

Notifies the FixedResource object that item will now be moved into it

This command notifies a downstream FixedResource object that the specified item will now be moved into it from an upstream object. This should only be used if the upstream object's Use Transport field is checked, but you are moving the item explicitly, instead of using an FRLOAD task. This allows the downstream FixedResource to manage data on how many items are slated to enter the object, but haven't arrived yet. Execute this command just before you move the item out with the moveobject command. For more information, refer to the FixedResource documentation. If this command returns 0, then the object has been stopped using the stopobject command, and you must wait until it has been resumed before moving the item into it.

## Example

```flexscript
transportincomplete(current.outObjects[port], item, opipno(current, port));
This example should be executed from the Request Transport From field before a moveobject command if you decide that you don't want to use a transport, but rather want to immediately move the flowitem
```


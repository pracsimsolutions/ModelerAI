---
id: transportoutcomplete
name: "transportoutcomplete"
kind: command
module: ""
signature: "transportoutcomplete(obj object, obj item [, num outport, obj transporter])"
aliases: []
tags: ["command", "group-fixedresources"]
deprecated: false
replacedBy: "Transport"
description: "Notifies the FixedResource object that item will now be moved out of it"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# transportoutcomplete

```flexscript
transportoutcomplete(obj object, obj item [, num outport, obj transporter])
```

Notifies the FixedResource object that item will now be moved out of it

This command notifies the FixedResource object that the specified item will now be moved out of it. This should only be used if the object's Use Transport field is checked, but you are moving the item explicitly, instead of using an FRLOAD task. This allows the FixedResource to manage data on how many items are still in the object but are ready to leave. Execute this command just before you move the item out with the moveobject command. For more information, refer to the FixedResource documentation. If this command returns 0, then the object has been stopped using the stopobject command, and you must wait until it has been resumed before moving the item into it.

## Example

```flexscript
transportoutcomplete(current, item, port);

This example should be executed from the Request Transport From field before a moveobject command if you decide that you don't want to use a transport, but rather want to immediately move the flowitem
```


---
id: createinstance
name: "createinstance"
kind: command
module: ""
signature: "createinstance(obj class, obj location) -> obj"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Object.create"
description: "FlexScript command: createinstance."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# createinstance

> ⚠ **DEPRECATED.** Use `Object.create` instead.

```flexscript
createinstance(obj class, obj location) -> obj
```

This command is deprecated. Use Object.create() instead.
For developer use.
This command creates an instance of the class specified as class and places it in the object specified as location. The instance will be given a name that is the name of the class followed by a number. The instance will have all of the variables and attributes of class and it's superclasses. Nodes in the behavior node will not be copied into the instance, as they apply to all instances of the class. The object passed as class should be in the library. Generally, location is the model. The library and the model can be easily referenced using the library() and model() commands.

## Example

```flexscript
createinstance(library().find("?Source"), model()); This creates an instance of the Source class in the model.
```


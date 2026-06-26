---
id: autoloadsounds
name: "autoloadsounds"
kind: command
module: ""
signature: "autoloadsounds()"
aliases: []
tags: ["command", "group-media"]
deprecated: false
replacedBy: null
description: "FlexScript command: autoloadsounds."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# autoloadsounds

```flexscript
autoloadsounds()
```

For developer use. This command loads all sounds required by the current views, library, and model as declared by "sound" attributes on the objects. This command may take some time to complete, if there are many sounds that need to be loaded. This command is not available in C++.

## Example

```flexscript
autoloadsounds();
This reloads all of the sounds needed for the current model.
```


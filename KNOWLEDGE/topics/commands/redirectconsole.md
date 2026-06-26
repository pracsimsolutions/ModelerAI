---
id: redirectconsole
name: "redirectconsole"
kind: command
module: ""
signature: "redirectconsole(num console:output/system/compiler/trace, obj view)"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "Redirects any prints to a given console"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# redirectconsole

```flexscript
redirectconsole(num console:output/system/compiler/trace, obj view)
```

Redirects any prints to a given console

This redirects any prints to a given console so that they will instead be printed to the view text of the specified view.
CONSOLE_OUTPUT redirects the output console, CONSOLE_SYSTEM redirects the system console, CONSOLE_COMPILER redirects the compiler console, CONSOLE_TRANCE redirects the trace console. To redirect it back to its original console, call the command again and specify NULL as the view parameter.

## Example

```flexscript
redirectconsole(2, c);buildnodeflexscript(getvarnode(Model.find("Processor1"), "exittrigger"));redirectconsole(2, NULL);
This redirects the compiler console to the view c and then builds flexscript on a node in the model. Thus, any errors in building flexscript will be directed to the view c instead of to the compiler console.
```


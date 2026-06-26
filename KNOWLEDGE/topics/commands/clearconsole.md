---
id: clearconsole
name: "clearconsole"
kind: command
module: ""
signature: "clearconsole()"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: null
description: "Clears the output console"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# clearconsole

> ⚠ **DEPRECATED.** See description for replacement.

```flexscript
clearconsole()
```

Clears the output console

This command clears the output console if it is open. The output console is where output from the pt(), pf(), and pr() commands is printed.

## Example

```flexscript
if (v_printerror == 1) { clearconsole(); pt("An Error occurred"); }
This clears the output console and prints a simple error message to it if the variable called printerror is equal to 1.
```


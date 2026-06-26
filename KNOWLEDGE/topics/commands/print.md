---
id: print
name: "print"
kind: command
module: ""
signature: "print([var value, var value2, ...])"
aliases: []
tags: ["command", "group-output"]
deprecated: false
replacedBy: "Debug"
description: "Print the contents of the given parameters and a new line to the console"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# print

```flexscript
print([var value, var value2, ...])
```

Print the contents of the given parameters and a new line to the console

This command takes 0 to 20 variants as parameters. Each parameter is printed to the console and separated by a space.
After all the parameters are printed a new line character is printed to the console. The output console must be open to write to it.
To open the console, use Debug > Output Console in the menu.

## Example

```flexscript
print("Number:", 1); print([1, 2]);
This prints "Number: 1" and "Array[2]: {1,2}" on different lines of the output console.
```


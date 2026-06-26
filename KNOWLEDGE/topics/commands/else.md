---
id: else
name: "else"
kind: command
module: ""
signature: "else{ }"
aliases: []
tags: ["command", "group-language"]
deprecated: false
replacedBy: null
description: "FlexScript command: else."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# else

```flexscript
else{ }
```

Used in conjunction with the "if" statement to declare what (if any) code should be executed when the condition for the "if" is false.

## Example

```flexscript
if( item.Weight < 100 ) { msg("TRUE",""); } else { msg("FALSE",""); }

This example will display the TRUE message when the value of the item's Weight label is less than 100, and the FALSE message when it is not.
```


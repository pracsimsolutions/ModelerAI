---
id: if
name: "if"
kind: command
module: ""
signature: "if(num condition) { }"
aliases: []
tags: ["command", "group-language"]
deprecated: false
replacedBy: null
description: "FlexScript command: if."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# if

```flexscript
if(num condition) { }
```

If the condition is true (i.e. the conditional expression returns a 1), then the statement immediately following the closing parenthesis will be executed. If braces { } are what immediately follows the closing parenthesis, then the block of code within the braces will be executed when the condition is true. Nothing is executed if the condition is false (returns a 0), unless an "else" has been defined. Refer to the Users Manual for more information.

## Example

```flexscript
if (item.Weight < 100)msg("TRUE", "");

This example will display a popup message saying "TRUE" when the item's Weight label has a value less than 100
```


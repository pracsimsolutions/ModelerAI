---
id: stringcompare
name: "stringcompare"
kind: command
module: ""
signature: "stringcompare(str text1, str text2) -> num"
aliases: []
tags: ["command", "group-language"]
deprecated: false
replacedBy: null
description: "Compare strings for alphabetical order"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# stringcompare

```flexscript
stringcompare(str text1, str text2) -> num
```

Compare strings for alphabetical order

Compare strings for alphabetical order. The return value is zero if the strings are the same. If text1 is alphabetically before text2, a negative number is returned. If text1 is alphabetically after text2, a positive number is returned. Same as the strcmp() C++ function.

## Example

```flexscript
stringcompare("zenith","beta") returns 1
comparetext("aqua","yellow") returns -1
```


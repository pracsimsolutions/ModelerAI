---
id: stringtoken
name: "stringtoken"
kind: command
module: ""
signature: "stringtoken(str token, str delimit) -> str"
aliases: []
tags: ["command", "group-strings"]
deprecated: false
replacedBy: null
description: "Finds the next token in a string"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# stringtoken

```flexscript
stringtoken(str token, str delimit) -> str
```

Finds the next token in a string

Finds the next token in a string. The first parameter is a string containing delimited tokens. The second parameter is a string with a set of delimiter characters. Works similarly to strtok() in C++.

## Example

```flexscript
string token = "Jacob,Emma,Michael,Isabella,Ethan";
string name = stringtoken(token,",");

while(stringlen(name)>0)
{
pt(name);pr();
name = stringtoken(NULL,",");
}
This prints each name in the list to a different line of the output console.
```


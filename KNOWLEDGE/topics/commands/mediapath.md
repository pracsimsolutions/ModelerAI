---
id: mediapath
name: "mediapath"
kind: command
module: ""
signature: "mediapath(str projectpath, str filepath, int mode) -> str"
aliases: []
tags: ["command"]
deprecated: false
replacedBy: null
description: "FlexScript command: mediapath."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# mediapath

```flexscript
mediapath(str projectpath, str filepath, int mode) -> str
```

Returns strip of the project pathfile from filepath to make it relative, where mode denotes as 1=default, 2=getfile, 3=getpath.

## Example

```flexscript
int mode = 1;
mediapath("C:\\Program Files\\Flexsim4\\program", "C:\\Program Files\\Flexsim4\\fs3d\\operator.3ds", mode);

If mode=1, returns "program"
If mode=2, returns "C:\Program Files\Flexsim4\"
If mode=3, returns "C:\Program Files\Flexsim4\fs3d\operator.3ds"
```


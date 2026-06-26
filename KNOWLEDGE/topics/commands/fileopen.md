---
id: fileopen
name: "fileopen"
kind: command
module: ""
signature: "fileopen(str filename [, str opentype])"
aliases: []
tags: ["command", "group-output"]
deprecated: false
replacedBy: "the"
description: "Opens the FlexSim file interface"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# fileopen

```flexscript
fileopen(str filename [, str opentype])
```

Opens the FlexSim file interface

Opens the FlexSim file interface. Returns 1 if successful. Returns 0 if an error is encountered.
Parameter 2 can be "w" for writing, "r" for reading, or "a" for appending. Default is "w". Use the commands fpt(), fpr(), fpf(), etc.
to write to the opened text file. Use filereadline() to read a line from the file. Using this command, you can only have one file open at a time. To write to multiple
files simultaneously, use the C standard fopen/fclose commands or C++ standard fstream classes in C++ or through a dll.

If in write mode, the contents of the file will be cleared, and the file pointer will be set to the top of the file.
The file must be closed to save any data written to the file.

## Example

```flexscript
fileopen("C:/myfile.txt", "w");
```


---
id: getfilebrowse
name: "getfilebrowse"
kind: command
module: ""
signature: "getfilebrowse(str extension, str extensiondescription[, str defaultreturn, str location, num savemode, node filelist]) -> str"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "Opens a standard windows file browser dialog window"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getfilebrowse

```flexscript
getfilebrowse(str extension, str extensiondescription[, str defaultreturn, str location, num savemode, node filelist]) -> str
```

Opens a standard windows file browser dialog window

This command opens a standard Windows file browser dialog window. The files shown in the window will have the extensions defined in extension. Different extensions should be separated by semicolons (;) in extension. The text passed as description will be used in the file browser to describe the type of file to open. This command returns a string that is the path of the selected file. If no file is selected, or Cancel is pressed, the command returns an empty string. See filebrowse() command.

## Example

```flexscript
string shapefile = getfilebrowse("*.3ds; *.wrl","3D shape files");
This sets the variable shapefile to the path of a file chosen from a file browser that displays files with the .3ds and .wrl extensions.
```


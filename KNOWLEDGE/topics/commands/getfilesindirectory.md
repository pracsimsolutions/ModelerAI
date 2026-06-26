---
id: getfilesindirectory
name: "getfilesindirectory"
kind: command
module: ""
signature: "getfilesindirectory(str directory, str extension, node destnode)"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: getfilesindirectory."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getfilesindirectory

```flexscript
getfilesindirectory(str directory, str extension, node destnode)
```

Retrieves all files in the specified directory that match the specified extension, and populates the content of destnode with nodes
holding the path to each matching file. The command returns the number of matching files that were found. The extension parameter
should be the specific three (or sometimes four) letter extension that you want to get. For example, if you want all .fsm model files, specify the string "fsm".
Also, if you pass "directories" as the extension parameter, then the command will retrieve the list of directories inside the directory. To get all
files, no matter the extension, pass "*" as the extension parameter. File paths will be set on the string data (as opposed to the name) of nodes
inserted into destnode.

## Example

```flexscript
treenode destnode = Model.find("Tools").subnodes.assert("Files");
int nrfiles = getfilesindirectory(documentsdir(), "*", destnode);
for(int i = 1; i <= nrfiles; i++)
{
mpt("file: ");
mpt(destnode.subnodes[i].value);
mpr();
}
```


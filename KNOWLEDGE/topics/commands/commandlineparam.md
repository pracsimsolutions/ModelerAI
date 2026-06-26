---
id: commandlineparam
name: "commandlineparam"
kind: command
module: ""
signature: "commandlineparam(str param) -> str"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "FlexScript command: commandlineparam."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# commandlineparam

```flexscript
commandlineparam(str param) -> str
```

Returns the specified commandline parameter that was passed in if FlexSim is being run from the commandline.

## Example

```flexscript
commandlineparam("nrofops");
This example will return the "nrofops" commandline parameter.
If a batch opens FlexSim with the line: flexsim.exe myfile.fsm -nrofops 8
Then within FlexSim, commandlineparam("nrofops") will return "8".
You can use either the unix-style hyphen (-) or the windows-style forward slash (/) to define a command-line parameter
```


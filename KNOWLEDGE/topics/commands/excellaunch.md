---
id: excellaunch
name: "excellaunch"
kind: command
module: ""
signature: "excellaunch([str launchdir])"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Launch Microsoft Excel"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excellaunch

```flexscript
excellaunch([str launchdir])
```

Launch Microsoft Excel

This command opens Microsoft Excel so that it can be accessed by FlexSim. If launchdir is not specified, the path defined in Tools|Excel...|Global Settings will be used. Otherwise, the launchdir must be an accurate path to the location on the hard drive where EXCEL.EXE is located. It is typically something like "C:/Program Files/Microsoft Office/Office10," but may be different depending on the version of Excel installed. It is generally not required to launch Excel before communicating with it. However, if FlexSim is set to communicate with Excel via DDE (by calling maintenance(1000,1), this is not the default behavior) Excel must be running before any communication can be performed. If excellaunch() is called at the beginning of communication, excelquit() must also be called before the user opens a new model, compiles the current model or closes FlexSim.

## Example

```flexscript
excellaunch(" C:/Program Files/Microsoft Office/Office10"); excelopen("C:/mybook.xls"); excelsetsheet("Sheet1"); double thenum = excelreadnum(1,1); excelclose(true); excelquit();
This starts Excel, opens a workbook called "mybook," stores a value read from row 1, column 1 from that workbook, then closes the file and Excel.
```


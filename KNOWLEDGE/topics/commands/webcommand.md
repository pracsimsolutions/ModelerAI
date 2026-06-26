---
id: webcommand
name: "webcommand"
kind: command
module: ""
signature: "webcommand(str name[, p1, p2, p3, ...])"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "Used to manipulate FlexSim's HTML engine/widgets"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# webcommand

```flexscript
webcommand(str name[, p1, p2, p3, ...])
```

Used to manipulate FlexSim's HTML engine/widgets

A command used for manipulating FlexSim's HTML engine, html widgets, etc.

## Example

```flexscript
string innerhtml = webcommand("getelementinnerhtml", view, "title");
```


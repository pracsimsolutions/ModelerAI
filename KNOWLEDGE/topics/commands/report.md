---
id: report
name: "report"
kind: command
module: ""
signature: "report(obj containerobj, str path+filename, num append [0/1])"
aliases: []
tags: ["command", "group-output"]
deprecated: false
replacedBy: null
description: "FlexScript command: report."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# report

```flexscript
report(obj containerobj, str path+filename, num append [0/1])
```

For developer use. Generate a standard report file in "comma separated value" (csv) format for all objects under containerobj which are currently selected. If append is 1, then the report will be appended to the end of the file specified. If append is 0, then either the file will be created or the existing file will be cleared before the report is written to it.

## Example

```flexscript
report(model());
```


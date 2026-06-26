---
id: runfunction
name: "runfunction"
kind: command
module: ""
signature: "runfunction(num noreset)"
aliases: []
tags: ["command", "group-model-execution"]
deprecated: false
replacedBy: null
description: "FlexScript command: runfunction."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# runfunction

```flexscript
runfunction(num noreset)
```

For developer use. Block user input/output, perform a model run at maximum speed possible by cpu, and return control when stoptime is met. If stoptime = 0, the stoptime is assumed to be 1000. See go().

## Example

```flexscript
runfunction(1)
```


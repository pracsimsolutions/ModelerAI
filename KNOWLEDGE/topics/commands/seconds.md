---
id: seconds
name: "seconds"
kind: command
module: ""
signature: "seconds(num seconds) -> num"
aliases: []
tags: ["command", "group-conversions"]
deprecated: false
replacedBy: null
description: "Gets the number of model time units in the number of seconds"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# seconds

```flexscript
seconds(num seconds) -> num
```

Gets the number of model time units in the number of seconds

Return the number of model time units in the given number of seconds.

## Example

```flexscript
seconds(30);
Returns 0.5 minutes.
```


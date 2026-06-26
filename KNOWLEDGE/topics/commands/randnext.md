---
id: randnext
name: "randnext"
kind: command
module: ""
signature: "randnext(num stream) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "FlexScript command: randnext."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# randnext

```flexscript
randnext(num stream) -> num
```

For development use. Returns the next random number from the stream. This command is used to pull random samples from probability distribution functions.

## Example

```flexscript
randnext(1);
Gets the next random number in the stream of random numbers associated with stream generator number 1.
```


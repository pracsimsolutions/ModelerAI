---
id: randstate
name: "randstate"
kind: command
module: ""
signature: "randstate(num stream[, seednr]) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "Gets the current state value for the random number stream"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# randstate

```flexscript
randstate(num stream[, seednr]) -> num
```

Gets the current state value for the random number stream

Returns the current state value for the specified random number stream. This state value may be useful as the seed value in the randinit() command for the purposes of restarting a saved state model from the same point in the random number stream of the initial run.

seednr is only needed if you are using FlexSim's Combined MRG random number generator, in which case the seednr should be a value 1-6 defining which seed value you want to get the state on. Refer to randinit() for using the Combined MRG.

## Example

```flexscript
int curstate = randstate(5);

Set the curstate variable to the current random integer value stream number 5.
```


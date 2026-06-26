---
id: loglogistic
name: "loglogistic"
kind: command
module: ""
signature: "loglogistic(num location, num scale, num shape[, num stream]) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "Returns a random sample from a log logistic distribution"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# loglogistic

```flexscript
loglogistic(num location, num scale, num shape[, num stream]) -> num
```

Returns a random sample from a log logistic distribution

loglogistic(, , , stream) is a non-negative continuous probability distribution.

Inputs:
is the location parameter ( - , )
is the scale parameter ( 0, )
is the shape parameter ( 0, )
stream is a reference to one of FlexSim's random number streams {0,1,2,...}

Outputs:
range = ( , )
mean = + ( )csc( ) for > 1
variance = ( ){2csc(2 ) ( )[csc( )]} for > 2

Possible Applications:
Used to model the time to perform some task.

## Example

```flexscript
loglogistic(0, 1, 2, 5);

A loglogistic distribution with a location (shift) of 0, a scale of 1, a shape factor of 2 and random stream number 5.
```


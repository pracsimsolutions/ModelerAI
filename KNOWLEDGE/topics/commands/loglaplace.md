---
id: loglaplace
name: "loglaplace"
kind: command
module: ""
signature: "loglaplace(num location, num scale, num shape[, num stream]) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "Returns a random sample from a log laplace distribution"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# loglaplace

```flexscript
loglaplace(num location, num scale, num shape[, num stream]) -> num
```

Returns a random sample from a log laplace distribution

loglaplace( , , , stream ) is a non-negative continuous probability distribution.

Inputs:
is the location parameter ( - , )
is the scale parameter ( > 0 )
is the shape parameter ( > 0 )
stream is a reference to one of FlexSim's random number streams {0,1,2,...}

Outputs:
range = ( , )
mean = + ( 1) for > 1
variance = ( 2 + 1 ) ( - 1 ) ( - 4 ) for > 2

Probability Density Functions:

Possible Applications:
Rarely used in defining processing times.

Comments:
This distribution is also known as the double-exponential distribution because its density curve is essentially two exponential curves back-to-back.

## Example

```flexscript
loglaplace(0, 2, 8, 5);

A loglaplace distribution with a location parameter of 0, a scale parameter of 2, a shape parameter of 8, and random stream number 5.
```


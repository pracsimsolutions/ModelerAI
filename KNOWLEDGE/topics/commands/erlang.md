---
id: erlang
name: "erlang"
kind: command
module: ""
signature: "erlang(num location, num scale, num shape[, num stream]) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "Returns a random sample from an erlang distribution"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# erlang

```flexscript
erlang(num location, num scale, num shape[, num stream]) -> num
```

Returns a random sample from an erlang distribution

erlang( , , , stream ) is a non-negative continuous probability distribution.

Inputs:
is the location parameter ( - , )
is the scale parameter ( > 0 )
is the shape parameter {1, 2, ... }
stream is a reference to one of FlexSim's random number streams {0,1,2,...}

Outputs:
range = ( , )
mean = +
variance =

Possible Applications:
Used to model the time to complete some task such as assembly, customer service and machine repair.

Comments:
When = 0, this distribution is known as the m-Erlang() distribution.
The exponential( , ) and erlang( , , 1 ) distributions are the same.
For a positive integer , the gamma( , , ) and erlang( , , ) distributions are the same.

## Example

```flexscript
double servicetime = erlang(60, 2, 4, 5);

The servicetime variable is set equal to a erlang distribution with a location parameter of 60, a scale parameter of 2, a shape parameter of 4, using random stream number 5. The mean is 100 + (2*4) = 108.
```


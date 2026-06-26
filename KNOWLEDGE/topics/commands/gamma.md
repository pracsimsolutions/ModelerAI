---
id: gamma
name: "gamma"
kind: command
module: ""
signature: "gamma(num location, num scale, num shape[, num stream]) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "Returns a random sample from a gamma distribution"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# gamma

```flexscript
gamma(num location, num scale, num shape[, num stream]) -> num
```

Returns a random sample from a gamma distribution

gamma( , , , stream ) is a non-negative continuous probability distribution.

Inputs:
is the location parameter ( - , )
is the scale parameter ( > 0 )
is the shape parameter ( > 0 )
stream is a reference to one of FlexSim's random number streams {0,1,2,...}

Outputs:
range = ( , )
mean = +
variance =

Probability Density Functions:

Possible Applications:
Used to model the time to complete some task such as assembly, customer service or machine repair.

Comments:
The exponential( , ) and gamma( , , 1 ) distributions are the same.
For a positive integer , the gamma( , , ) and erlang( , , ) distributions are the same.
Shape values less than 1 are rarely used.

## Example

```flexscript
double repairtime = gamma(60, 2, 3.4, 5);

The repairtime variable is set equal to a gamma distribution with a location parameter of 60, a scale parameter of 2, a shape parameter of 4, and using random stream number 5. The mean is 100 + (2*3.4) = 106.8.
```


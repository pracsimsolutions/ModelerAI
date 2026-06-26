---
id: inversegaussian
name: "inversegaussian"
kind: command
module: ""
signature: "inversegaussian(num location, num scale, num shape[, num stream]) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "Returns a random sample from an inversegaussian distribution"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# inversegaussian

```flexscript
inversegaussian(num location, num scale, num shape[, num stream]) -> num
```

Returns a random sample from an inversegaussian distribution

inversegaussian( , , , stream ) is a non-negative continuous probability distribution.

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
Used to model the time to complete some tasks.

Comments:
The inversegaussian( , 1, ) and wald( , ) distributions are the same.
As approaches infinity, the distribution becomes more like the normal (Gaussian) distribution.
The parameter has elements of a shape parameter since it affects the skewness and kurtosis (width).

## Example

```flexscript
double repairtime = inversegaussian(60, 2, 4, 5);

The repairtime variable is set equal to a inversegaussian distribution with a location parameter of 60, a scale parameter of 2, a shape parameter of 4, and using random stream number 5.
```


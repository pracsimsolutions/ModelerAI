---
id: weibull
name: "weibull"
kind: command
module: ""
signature: "weibull(num location, num scale, num shape[, num stream]) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "Returns a random sample from a weibull distribution"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# weibull

```flexscript
weibull(num location, num scale, num shape[, num stream]) -> num
```

Returns a random sample from a weibull distribution

weibull( , , , stream ) is a non-negative continuous probability distribution.

Inputs:
is the location parameter ( - , )
is the scale parameter ( > 0 )
is the shape parameter ( > 0 )
stream is a reference to one of FlexSim's random number streams {0,1,2,...}

Outputs:
range = ( , )
mean = + ( ) (1 ) where () is the Gamma function.
variance = ( ) { 2(2 ) 1 [ (1 ) ] }

Probability Density Functions:

Possible Applications:
The weibull distribution is commonly used when there is not sufficient data available because it can take on so many different shapes depending on the shape factor. It is a very common distribution used for representing the time between equipment failures, as well as the time to complete a manual task.

Comments:
The weibull( , , 1 ) and exponential( , ) distributions are the same.
The weibull( , , 2 ) and rayleigh( , ) distributions are the same.
The weibull distribution is skewed to the left if > 3.6

## Example

```flexscript
weibull(0, 2, 3, 5);

A weibull distribution with a location parameter of 0, a scale parameter of 2, a shape parameter of 3, and random stream number 5.
```


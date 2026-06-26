---
id: normal
name: "normal"
kind: command
module: ""
signature: "normal(num mean, num stddev[, num stream]) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "Returns a random sample from a normal distribution"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# normal

```flexscript
normal(num mean, num stddev[, num stream]) -> num
```

Returns a random sample from a normal distribution

normal(μ, σ, stream) is an unbounded continuous probability distribution.

Inputs:
μ is the location parameter ( - , )
σ is the scale parameter ( 0, )
stream is a reference to one of FlexSim's random number streams {0,1,2,...}

Outputs:
range = ( - , )
mean = μ
variance = σ

Probability Density Functions:

Possible Applications:
Used to model various types of error such as machined hole diameters for large quantities of parts. This distribution is not used much in simulation because variance found in real processes do not typically follow a symmetric distribution such as this.

Comments:
The normal distribution is completely symmetric meaning values below the mean have the same probability for occurance as values above the mean. The mode and mean are equal for the normal distribution. When using this distribution, be aware that negative values can be returned, and therefore truncating the distribution at 0 may be necessary as shown in the example below.

## Example

```flexscript
double thickness = Math.max(0.001, normal(5, 1.2));

The thickness is set a normal distribution having a mean of 5 and a standard deviation of 1.2 that is truncated for values less than 0.001. No random stream has been declared, so the default stream number 0 will be used to generate variates from the distribution.
```


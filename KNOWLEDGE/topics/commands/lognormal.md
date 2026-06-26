---
id: lognormal
name: "lognormal"
kind: command
module: ""
signature: "lognormal(num location, num normalmean, num normalstdev[, num stream]) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "Returns a random sample from a lognormal distribution"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# lognormal

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
lognormal(num location, num normalmean, num normalstdev[, num stream]) -> num
```

Returns a random sample from a lognormal distribution

lognormal() and lognormal2() are similar functions, but with different parameter definitions. lognormal() uses parameter definitions as defined on wikipedia and other sites. lognormal2() uses parameters as defined in the latest versions of Expertfit. Essentially, the parameters can be converted as follows:

lognormal( location, normalmean, normalstdev ) = lognormal2( location, Math.exp( normalmean ), normalstdev )
lognormal2( location, scale, shape ) = lognormal( location, Math.log( scale ), shape )
(Math.exp(x) is e raised to the x power, and Math.log(x) is the natural logarithm of x.)

Please make sure you use the right distribution. Using the wrong distribution could result in bad simulation results.

lognormal( location, normalmean, normalstdev, stream ) is a continuous probability distribution.

Inputs:
location : ( - , )
normalmean such that ( Math.exp( normalmean ) > 0 )
normalstdev such that ( normalstdev > 0 )
stream is a reference to one of FlexSim's random number streams {0,1,2,...}

Outputs:
range = ( location , )
mean = location + Math.exp( normalmean + normalstdev 2)
variance = Math.exp( 2*normalmean + normalstdev ) [ Math.exp( normalstdev ) 1 ]

Probability Density Functions:

Possible Applications:
Used to model the time to perform some task, and to model quantities that are the product of a large number of other quantities. It can also be used as a rough model in the absence of data.

Comments:
The probability density for the lognormal distribution takes on shapes similar to gamma( normalstdev, normalmean ) and weibull( normalstdev, normalmean ) densities for normalstdev > 1, but can have a large spike close to x=0 that is often useful.


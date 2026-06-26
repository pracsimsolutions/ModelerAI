---
id: lognormal2
name: "lognormal2"
kind: command
module: ""
signature: "lognormal2(num location, num scale, num shape[, num stream]) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: "the"
description: "Returns a random sample from a lognormal distribution"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# lognormal2

```flexscript
lognormal2(num location, num scale, num shape[, num stream]) -> num
```

Returns a random sample from a lognormal distribution

lognormal() and lognormal2() are similar functions, but with different parameter definitions. lognormal() uses parameter definitions as defined on wikipedia and other sites. lognormal2() uses parameters as defined in the latest versions of Expertfit. Essentially, the parameters can be converted as follows:

lognormal( location, normalmean, normalstdev ) = lognormal2( location, Math.exp( normalmean ), normalstdev )
lognormal2( location, scale, shape ) = lognormal( location, Math.log( scale ), shape )
(Math.exp(x) is e raised to the x power, and Math.log(x) is the natural logarithm of x.)

Please make sure you use the right distribution. Using the wrong distribution could result in bad simulation results.

## Example

```flexscript
double cycletime = lognormal2(0, 29.8, 0.31, 5);

Set the cycletime variable to a lognormal2 distribution with location parameter 0, scale parameter 29.8, shape parameter 0.31, and random stream number 5.
```


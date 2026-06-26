---
id: lognormalmeanstdev
name: "lognormalmeanstdev"
kind: command
module: ""
signature: "lognormalmeanstdev(num mean, num stdev[, num stream]) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: null
description: "Returns a random sample from a lognormal distribution"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# lognormalmeanstdev

> ⚠ **DEPRECATED.** See description for replacement.

```flexscript
lognormalmeanstdev(num mean, num stdev[, num stream]) -> num
```

Returns a random sample from a lognormal distribution

This command is an alternative to the lognormal2 command, used in the absence of data. You can specify the
mean and standard deviation, and the command will calculate from those parameters the proper location, scale, and shape
parameters for the lognormal2 command, and return a sample from that distribution. Note that the shape of the distribution
is pre-defined to be a bell-shaped curve with a reasonably long right tail. Hence this command isn't as flexible as the
lognormal2 command. This command, similar to the triangular distribution, should be used in the absence of
data, where you know the mean and standard deviation of the distribution, and you assume that the distribution is right-skewed
(it has a longer right tail), but you do not have a real data set to fit to a distribution. Refer to lognormal2
for more information. Relating to the diagram describing the lognormal2 distribution, the lognormalmeanstdev command uses a
normalstdev/shape value of 0.75 to define its shape. Note that since the shape of the
distribution is hard-set, depending on the mean and standard deviation values, this may return a negative value.

## Example

```flexscript
double cycletime = lognormalmeanstdev(10, 2);This example will return a random value that follows a
right-skewed lognormal distribution with a mean of 10 and standard deviation of 2.
```


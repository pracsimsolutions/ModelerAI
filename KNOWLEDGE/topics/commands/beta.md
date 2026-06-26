---
id: beta
name: "beta"
kind: command
module: ""
signature: "beta(num min, num max, num shape1, num shape2[, num stream]) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "Returns a random sample from a beta distribution"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# beta

```flexscript
beta(num min, num max, num shape1, num shape2[, num stream]) -> num
```

Returns a random sample from a beta distribution

beta( , , 1, 2, stream ) is a bounded continuous probability distribution.

Inputs:
is the lower-endpoint parameter ( - , )
is the upper-endpoint parameter( > )
1 is the 1  shape parameter ( 0, )
2 is the 2 shape parameter ( 0, )
stream is a reference to one of FlexSim's random number streams {0,1,2,...}

Outputs:
range = ( , )
mean = + 1( ) (1 + 2)
variance = 12( ) (1 + 2)(1 + 2 + 1)

Probability Density Functions:

Possible Applications:
The beta distribution is often used to model task durations in the absence of real data because it can be definitively bounded between a minimum and maximum value, and it can take on a number of different probability density shapes depending on the two shape parameters used. For most real world task durations, the density shape will have a longer right tail than a left, so if you know the mean and the mode (most likely value) , you can back calculate suitable shape parameters with the following equations:

1 ≅ ( )(2 ) ( )( )

2 ≅ 1( ) ( )

Comments:
The beta( , , 1, 1) and uniform( , ) distributions are the same.
The density is symmetric about ( + ) 2 when 1 = 2.
The density will have a hump and longer right tail when 2 > 1 > 1.
The mean and mode are equal when 1 = 2 > 1.

## Example

```flexscript
double ptime = beta(8, 12, 1.3, 3, 5);

The ptime variable is set equal to a beta distribution having a minimum value of 8, a maximum value of 12, and shape factors of 1.3 and 3 for 1 and 2 respectively. FlexSim's random stream number 5 will be used to generate variates from the distribution. The distribution density will have a nice humped curve with a long right tail.
```


---
id: exponential
name: "exponential"
kind: command
module: ""
signature: "exponential(num location, num scale [, num stream]) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "Returns a random sample from an exponential distribution"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# exponential

```flexscript
exponential(num location, num scale [, num stream]) -> num
```

Returns a random sample from an exponential distribution

exponential( , , stream) is a non-negative continuous probability distribution.

Inputs:
is the location parameter ( - , )
is the scale parameter ( 0, )
stream is a reference to one of FlexSim's random number streams {0,1,2,...}

Outputs:
range = ( , )
mean = +
variance =

Possible Applications:
Often used to model the interarrival times of a continous stream of customers and/or customer orders into a system. It is also a popular distribution for modeling the time between failure of mechanical equipment.

Comments:
Variates less than the mean occur much more frequently than variates greater than the mean.
The exponential distribution is the only continuous distribution with the memoryless property of producing totally independent variates.
If the location parameter () is set to zero, then becomes the mean, and the distribution's variates will range between zero and infinity.
The exponential( , ) and gamma( , , 1 ) distributions are the same.

## Example

```flexscript
exponential(0, 10, 5);

An exponential distribution with a location of 0 and a scale of 10 (mean = 10). FlexSim's random stream number 5 will be used to generate variates from the distribution.
```


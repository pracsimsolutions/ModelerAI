---
id: uniform
name: "uniform"
kind: command
module: ""
signature: "uniform(num min, num max[, num stream]) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "Returns a random number between the min and max values"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# uniform

```flexscript
uniform(num min, num max[, num stream]) -> num
```

Returns a random number between the min and max values

uniform( , , stream ) is a bounded continuous probability distribution.

Inputs:
and are real numbers with <
is the location parameter defining the minimum variate
defines the maximum variate
is the scale parameter
stream is a reference to one of FlexSim's random number streams {0,1,2,...}

Outputs:
range = [ , ]
mean = ( + ) 2
variance = ( ) 12

Possible Applications:
Used to approximate a value that is felt to be randomly varying between and but hardly anything else is known.

Comments:
There is an equal probability that an , or , or any real number inbetween will be returned.
The uniform(0, 1) distribution is essential in generating random variates from all the other distributions and will return a continuous set of random values between and inclusive of 0 and 1.

## Example

```flexscript
uniform(12.3, 27.5, 3);

Random number stream 3 will be used to generate a random variate from a uniform distribution having a minimum of 12.3 and a maximum of 27.5.
```


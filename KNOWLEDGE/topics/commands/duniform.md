---
id: duniform
name: "duniform"
kind: command
module: ""
signature: "duniform(num min, num max[, num stream])"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "Returns random integer between the min and max"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# duniform

```flexscript
duniform(num min, num max[, num stream])
```

Returns random integer between the min and max

duniform( , , stream ) is a bounded discrete probability distribution.

Inputs:
is the lower-endpoint parameter
is the upper-endpoint parameter
and are integer numbers where <
is the scale parameter
stream is a reference to one of FlexSim's random number streams {0,1,2,...}

Outputs:
range = { , +1, ..., }
mean = ( + ) 2
variance = ( ( + 1) 1 ) 12

Possible Applications:
Used to approximate a quantity that is felt to be randomly varying between and , but hardly anything else is known.

Comments:
There is an equal probability that an , or , or any discrete (integer) number inbetween will be returned.

## Example

```flexscript
duniform(1, 6, 3);

Random number stream 3 will be used to generate a random variate from a discrete uniform distribution having a minimum value of 1 and a maximum value of 6. This simulates the roll of a dice.
```


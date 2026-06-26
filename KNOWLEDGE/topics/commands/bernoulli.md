---
id: bernoulli
name: "bernoulli"
kind: command
module: ""
signature: "bernoulli(num prob, num succeed-value, num fail-value[, num stream]) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: "empirical"
description: "Returns one of two possible values, based on the given probability"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# bernoulli

```flexscript
bernoulli(num prob, num succeed-value, num fail-value[, num stream]) -> num
```

Returns one of two possible values, based on the given probability

bernoulli( , , , stream ) is a discrete probability distribution.

Inputs:
is the probability that will be returned;  where ( 0, 100 )
100 is the probability that will be returned.
( - , )
( - , )
stream is a reference to one of FlexSim's random number streams {0,1,2,...}

Outputs:
range = { , }
mean = ( + (100) ) 100
variance = (100)(1 100) when =1 and =0

Possible Applications:
Used to model a random occurrence that has two possible outcomes such as a pass/fail test. It is possible to generate more than two values by nesting bernoulli commands; however, it is better to use empirical distributions in these cases.

Comments:
The bernoulli(, 1, 0) and binomial(1, ) distributions are the same.

## Example

```flexscript
treenode curoperator = current.centerObjects[bernoulli(90,1,2,1)];

The bernoulli command in this example has a 90 percent probability of returning 1 otherwise it will return 2, using random number stream 1. It is used within a centerObject reference such that 90 percent of the time curoperator references the object connected to center port 1 of the current object, otherwise curoperator references the object connected to center port 2.
```


---
id: binomial
name: "binomial"
kind: command
module: ""
signature: "binomial(num trials, num prob[, num stream]) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "Returns a random sample from a binomial distribution"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# binomial

```flexscript
binomial(num trials, num prob[, num stream]) -> num
```

Returns a random sample from a binomial distribution

binomial( t, , stream ) is a discrete probability distribution used to represent the number of successes in t independent Bernoulli trials with probability of sucess on each trial.

Inputs:
t is the number of independent trials { 1, 2, ..., }.
is the probability of success for any given trial (0, 100)
stream is a reference to one of FlexSim's random number streams {0,1,2,...}

Outputs:
range = { 0, 1, 2, ..., t }
mean = t 100
variance = t ( 100 )( 1 100 )

Possible Applications:
The binomial distribution is useful for modeling the number of defective parts in a batch of size t, or the actual number of people in random size groups, or maybe the number of items ordered from inventory.

Comments:
The binomial(1, ) and bernoulli( , 1, 0 ) distributions are the same.
The binomial( t, ) distribution is symmetric when = 50 percent.

## Example

```flexscript
int fparts = binomial(item.batchsize, 10, 5);

The number of failed parts will be written to the fparts variable based on a 10% probability that any given part will fail within a batch quantity defined by a label on the flowitem named "batchsize". FlexSim's random stream number 5 will be used to generate variates from the distribution.
```


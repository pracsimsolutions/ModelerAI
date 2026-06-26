---
id: getdatastat
name: "getdatastat"
kind: command
module: ""
signature: "getdatastat(num stat, num/node nrSamples/parentNode, num sampler [, num p1, num p2, num p3]) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: "the"
description: "FlexScript command: getdatastat."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getdatastat

```flexscript
getdatastat(num stat, num/node nrSamples/parentNode, num sampler [, num p1, num p2, num p3]) -> num
```

Calculates and returns a certain statistic of a data set.
stat - Can be one of the following values

STAT_MEAN - calculates the sample mean of the set
STAT_MIN - calculates the minimum value in a defined range of values. p1 is the first element in the
range (default is the first element in the set), p2 is the last element in the range (default
is the last element in the set), and p3 defines a stride, allowing you to look at
every nth value (default 1). If either of p1, p2, or p3 is 0, then it will revert to that value's
respective default.
STAT_MAX - calculates the maximum element in a defined range of values. p1, p2 and p3 are the same as for STAT_MIN
STAT_VARIANCE - calculates the unbiased variance of the data set
STAT_STD_DEV - calculates the unbiased standard deviation of the set
STAT_SUM - calculates the sum of the set
STAT_CONF_INTERVAL_HALF_WIDTH - calculates the confidence interval half-width of the set.
Pass in p1 to define the desired
confidence percentage. FlexSim will return precise values for percentages at 80%, 90%, 95%, 98%, 99%, and 99.8% and for
sample sizes at or below 100 degrees of freedom. For percentages not in this set, FlexSim will "clamp" it to the nearest
percentage ABOVE the percentage passed in. So if you pass in a 75%, it will return an 80% confidence interval. Since an
80% confidence interval will be a "wider" interval, we clamp up to be "safer" with the wider interval. Confidence
intervals above 99.8 will simply return 0 (i.e. not valid). For degrees
of freedom above above 100, it will assume 100 degrees of freedom.

nrSamples - defines the number of samples in the set.
Note: parameter 2 as a parentNode is deprecated and you should use the parameter 2 as nrSamples option because it
encompasses the other and is more flexible. The following deprecated functionality:
getdatastat(STAT_MEAN, myNode, 0, 0, 0, 0)
can be represented using the newer option with:
getdatastat(STAT_MEAN, myNode.subnodes.length, myNode.subnodes[count].value, 0, 0, 0)

sampler - an expression that gets the nth value in the sample.

Use count for determining which value is to be retrieved. Below are several examples:
For calculating the mean of all values in column 5 of global table GT1:
getdatastat(STAT_MEAN, Table("GT1").numRows, Table("GT1")[count][5])
For calculating the sum of all values in field "Time" of a bundle:
getdatastat(STAT_SUM, Table("MyBundle").numRows, Table("MyBundle")[count]["Time"])


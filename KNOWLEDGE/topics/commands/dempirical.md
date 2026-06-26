---
id: dempirical
name: "dempirical"
kind: command
module: ""
signature: "dempirical(str/node/num table[, num stream])"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "Returns a random sample from a discrete empirical distribution"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# dempirical

```flexscript
dempirical(str/node/num table[, num stream])
```

Returns a random sample from a discrete empirical distribution

A discrete empirical distribution.

The table referenced must contain a list of values and probability percentages associated with each of the values. Probability percentages must be entered in column 1 starting with row 1 of the table, and their associated values are entered in column 2. The table may have as many rows as needed to define as many values as desired. The percents are entered as numbers between 0 and 100, and should add up to a total of 100 percent; otherwise any values defined after a cumulative percentage of 100 is reached will never be returned.

A detailed description of the three empirical distributions used in FlexSim and their differences is included with the cempirical() command.

## Example

```flexscript
int ptype = dempirical("prodtypes", 5);

Assigns a value to the ptype variable using a discrete empirical distribution defined in a global table named "prodtypes" using random number stream 5.

dempirical(current.labels["route"], 5);

Returns a random variate from a discrete empirical distribution defined in a node table defined on the label named "route" of the current object, and using random number stream 5.
```


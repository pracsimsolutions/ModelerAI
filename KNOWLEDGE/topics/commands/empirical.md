---
id: empirical
name: "empirical"
kind: command
module: ""
signature: "empirical(str/node/num table[, num stream]) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "FlexScript command: empirical."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# empirical

```flexscript
empirical(str/node/num table[, num stream]) -> num
```

A continuous empirical distribution.

The table referenced must contain a list of values and probability percentages associated with each of the values. Probability percentages must be entered in column 1 starting with row 1 of the table, and their associated values are entered in column 2. The table may have as many rows as needed to define as many values as desired. The percents are entered as numbers between 0 and 100, and should add up to a total of 100 percent; otherwise any values defined after a cumulative percentage of 100 is reached will never be returned.

A detailed description of the three empirical distributions used in FlexSim and their differences is included with the cempirical() command.

## Example

```flexscript
empirical("mytable", 5);

Returns a random variate from a continuous empirical distribution defined in a global table named "mytable" using random number stream 5.

empirical(current.labels["ptime"], 5);

Returns a random variate from a continuous empirical distribution defined in a table defined on a label named "ptime" on the current object using random number stream 5.
```


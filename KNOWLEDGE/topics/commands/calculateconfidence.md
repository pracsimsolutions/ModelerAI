---
id: calculateconfidence
name: "calculateconfidence"
kind: command
module: ""
signature: "calculateconfidence(num stddev, num nrofdatapoints, num confidence)"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "FlexScript command: calculateconfidence."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# calculateconfidence

```flexscript
calculateconfidence(num stddev, num nrofdatapoints, num confidence)
```

For developer use. Calculates the confidence interval for the given standard deviation and number of data points. The return value is the distance on one side of the interval from the sample mean. This uses the t distribution to calculate the confidence interval. Confidence values must be either 90, 95, or 99. This is used in the experimenter to calculate the confidence intervals for the mean of performance measures for each scenario.

## Example

```flexscript
calculateconfidence(5.82, nrofsamples, 90)
```


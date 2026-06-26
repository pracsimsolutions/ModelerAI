---
id: randantithetic
name: "randantithetic"
kind: command
module: ""
signature: "randantithetic(num on/off)"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "FlexScript command: randantithetic."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# randantithetic

```flexscript
randantithetic(num on/off)
```

Makes number streams antithetic. Using this command, users can switch random streams to generate numbers between 1 and 0; rather than the default of 0 to 1. This is helpful for determining if random numbers are having a big affect on the results of the model. It's also useful when experimenting with massive models and there isn't sufficient time to run the model for multiple iterations as required for good statistical sampling. The user can then choose to do just one regular run and one antithetic run of the model to obtain two diametrically apposed results. These two results can be averaged for an approximation that is somewhat better than if each run of the model used random stream generators without antithetic sampling.

## Example

```flexscript
randantithetic(1);
```


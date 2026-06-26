---
id: requestdecision
name: "requestdecision"
kind: command
module: ""
signature: "requestdecision(str/node/num object)"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: "this"
description: "FlexScript command: requestdecision."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# requestdecision

```flexscript
requestdecision(str/node/num object)
```

Request a decision from a ReinforcementLearning object.

When training, this will cause the simulation to return a reward for the previous action, make a new observation, and (if the episode is not done) wait for an action from the algorithm.

When running normally (not training), this will make an observation and execute the On Request Action trigger of the ReinforcementLearning object in order to make a heuristic decision or predict an action using a trained AI model.

Use this command to specify exactly when the AI should make a decision.

## Example

```flexscript
requestdecision("ReinforcementLearning1");
```


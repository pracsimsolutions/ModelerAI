---
id: endreplication
name: "endreplication"
kind: command
module: ""
signature: "endreplication(num onlyifexperimenting)"
aliases: []
tags: ["command", "group-model-execution"]
deprecated: false
replacedBy: "this"
description: "Ends the current experimenter replication"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# endreplication

```flexscript
endreplication(num onlyifexperimenting)
```

Ends the current experimenter replication

Will end the current experimenter replication. Use this if your experiment's end time is dependent on a certain condition becoming true instead of a set end time. Specify 1 for onlyifexperimenting to make sure that you only stop if you are running the experiment.

## Example

```flexscript
if(current.stats.output >= 5000) endreplication(1);
```


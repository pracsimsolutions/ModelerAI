---
id: dispatcher
name: "Dispatcher"
kind: class
module: ""
signature: "Dispatcher"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class that represents an Object with a task sequence queue, and implements logic to receive and dispatch TaskSequences."
seeAlso: []
source: "help\\FlexScriptAPIReference\\TaskExecuter\\Dispatcher.xml"
---

# Dispatcher

A class that represents an Object with a task sequence queue, and implements logic to receive and dispatch TaskSequences.

## Properties

### taskSequences

```flexscript
Dispatcher.taskSequences (readonly)
```

Accesses the task sequences in the dispatcher's task sequence queue. This member can be accessed like an array of TaskSequences.

Dispatcher disp = Model.find("Dispatcher1");
int numTaskSequences = disp.taskSequences.length;
for (int i = 1; i <= numTaskSequences; i++) {
TaskSequence ts = disp.taskSequences[i];
...
}


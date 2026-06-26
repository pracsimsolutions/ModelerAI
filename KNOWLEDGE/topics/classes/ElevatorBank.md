---
id: elevatorbank
name: "ElevatorBank"
kind: class
module: ""
signature: "ElevatorBank"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class that represents a bank of elevators."
seeAlso: []
source: "help\\FlexScriptAPIReference\\People\\ElevatorBank.xml"
---

# ElevatorBank

A class that represents a bank of elevators.

## Properties

### elevators

```flexscript
ElevatorBank.elevators (readonly)
```

An array accessor to the set of ElevatorBank.Elevators in this bank.

ElevatorBank bank = Model.find("ElevatorBank1");
for (int i = 1; i <= bank.elevators.length; i++) {
ElevatorBank.Elevator elevator = bank.elevators[i];
...
}

### requests

```flexscript
ElevatorBank.requests (readonly)
```

An array accessor to the set of ElevatorBank.Requests in this bank.

for (int i = 1; i <= bank.requests.length; i++) {
ElevatorBank.Request request = bank.requests[i];
...
}


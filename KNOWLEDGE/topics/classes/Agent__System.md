---
id: agent__system
name: "Agent::System"
kind: class
module: "Agent"
signature: "Agent::System"
aliases: []
tags: ["class", "agent-module"]
deprecated: false
replacedBy: null
description: "Provides access to data and methods related to a System of an Agents."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Agent\\Agent.System.xml"
---

# Agent::System

Provides access to data and methods related to a System of an Agents.

## Methods

### createAgent

```flexscript
Agent Agent::System.createAgent(Object object)
```

Creates a new agent that is associated with the passed object.

**Parameters:**
- `object` *(Object)* — The model object that you want the agent to be associated with.

**Returns:** A newly created agent

Agent newAgent = Agent.System("ProximitySystem").createAgent(current)

### getAgent

```flexscript
Agent Agent::System.getAgent(Object object)
```

Returns the agent that is associated with the passed object, if one exists.

**Parameters:**
- `object` *(Object)* — The model object.

**Returns:** The Agent associated with the object. If there is no such agent member, it will return null.

Agent agent = Agent.System("ProximitySystem").getAgent(current)

### System

```flexscript
Agent.System Agent::System.System(string name)
```

Accesses a system in the model with a given name.

**Parameters:**
- `name` *(string)* — The name of the system.

**Returns:** An Agent.System object

Agent.System("ProximitySystem").getAgent(current)


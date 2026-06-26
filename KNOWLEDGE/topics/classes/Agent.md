---
id: agent
name: "Agent"
kind: class
module: ""
signature: "Agent"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "Provides access to data and methods related to Agents who are members of an Agent System."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Agent\\Agent.xml"
---

# Agent

Provides access to data and methods related to Agents who are members of an Agent System.

## Methods

### activateBehavior

```flexscript
Agent.activateBehavior(string name)
```

Activates a named behavior of an agent.

**Parameters:**
- `name` *(string)* — The name of the behavior you want to activate

agent.activateBehavior("ProsimityBehavior");

### Agent

```flexscript
Agent Agent.Agent(Object object)
```

Gets access to the Agent associated with an Object.

**Parameters:**
- `object` *(Object)* — An object that is a member of an agent system.

**Returns:** An AGV object

To access the Agent associated with an object, you must either use this constructor, or call
Agent.System.getAgent. If an object is a member
of multiple agent systems, then calling this constructor will access the first system the object is a member of.
Casting from a treenode is not correct.
You should use this constructor instead of the as() casting method. Using as() on an object will not work
properly.
Agent agent = Agent(object); // correct
Agent agent = object; // auto-downcast: incorrect
Agent agent = object.as(Agent); // explicit cast: incorrect

### deactivateBehavior

```flexscript
Agent.deactivateBehavior(string name)
```

Deactivates a named behavior of an agent.

**Parameters:**
- `name` *(string)* — The name of the behavior you want to deactivate

agent.deactivateBehavior("ProsimityBehavior");

### neighbors

```flexscript
Agent.neighbors(string behaviorID = nullvar)
```

Retrieves the set of in-proximity neighbors associated with a behavior.

**Parameters:**
- `behaviorID` *(string, default nullvar)* — Usually this is the name of the behavior you want to deactivate. If you do not pass this behavior in, then it will use the first activated proximity behavior it finds. If you have a reference to the behavior node, you can pass this as well, and it will return the neighbors for that behavior.

var neighbors = agent.neighbors("ProsimityBehavior1");

## Properties

### object

```flexscript
Object Agent.object
```

Gets the object associated with the agent.

### socialForce

```flexscript
Agent.SocialForceState Agent.socialForce
```

Accesses the social force state of the agent.

If the agent does not have a social force behavior activated, this property will be null.


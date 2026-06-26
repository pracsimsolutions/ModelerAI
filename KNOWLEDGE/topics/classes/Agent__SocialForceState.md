---
id: agent__socialforcestate
name: "Agent::SocialForceState"
kind: class
module: "Agent"
signature: "Agent::SocialForceState"
aliases: []
tags: ["class", "agent-module"]
deprecated: false
replacedBy: null
description: "Provides access to data and methods related to an agent's social force state."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Agent\\Agent.SocialForceState.xml"
---

# Agent::SocialForceState

Provides access to data and methods related to an agent's social force state.

This can be used in custom social force fields to get/set forces on the
agent. It can also be used in other contexts to retrieve information about the current
social forces acting upon the agent.

## Properties

### center

```flexscript
Vec3 Agent::SocialForceState.center (readonly)
```

The agent's current center point.

### force

```flexscript
Vec3 Agent::SocialForceState.force
```

The agent's current force.

You can only set this force as part of a custom social force.

### friction

```flexscript
double Agent::SocialForceState.friction (readonly)
```

The scalar value of the agent's current friction force. This is only applicable if the agent has a friction force applied.

### goal

```flexscript
Vec3 Agent::SocialForceState.goal (readonly)
```

The agent's current goal location.

### hasGoal

```flexscript
int Agent::SocialForceState.hasGoal (readonly)
```

1 if the agent currently has a goal, 0 otherwise.

### velocity

```flexscript
Vec3 Agent::SocialForceState.velocity (readonly)
```

The agent's current velocity.


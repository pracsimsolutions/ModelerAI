---
id: reference_tools_agentsystem_socialforcebehavior_socialforcebehavior
name: "Social Force Behavior"
kind: manual
breadcrumb: ["Reference","Tools","AgentSystem","SocialForceBehavior","SocialForceBehavior"]
tags: ["manual","reference","tools","agentsystem"]
source: "manual/Reference/Tools/AgentSystem/SocialForceBehavior/SocialForceBehavior.html"
---

# Social Force Behavior

## Overview and Key Concepts

 

A social force behavior is a behavior that can be activated on agents in an agent system. Social force behaviors apply sets of forces on agents within a system to 'push' the agents in different directions based on various factors. A social force behavior is made up of one or more forces. Forces are calculated individually, and then aggregated into a cumulative force affecting the agent.

 

Example forces include things like attractive forces, which motivate an agent to 'stay in the group', or repulsive forces, which motivate an agent to not get too close to its neighbors. You may have an alignment force that motivates the agent to move in the same direction as other agents. You also may have a goal-based force that motivates the agent to move toward a destination or along an A* path.

 

 

When each force of an agent is calculated and aggregated, the cumulative force will effectively cause the agent to accelerate in the direction of the force. If the agent is a task executer, then the acceleration will be bounded by the task executer's acceleration or deceleration property, depending on whether the force is aligned with, or against, the agent's current direction of motion. The speed of the agent will also be bounded by the max speed of the task executer.

 

 

 

## Behavior Settings

 

The Behaviors Tab has the following Properties:

 

### Update Interval

 

Defines how often the system will update the force equations based on the selected time unit.

 

### Forces as Speeds

 

If this box is checked, then instead of applying the cumulative force as an acceleration on the agent, the behavior will instead immediately set the agent's speed to the result force. In other words, when the box is checked, the cumulatively calculated force is not technically a force, but a resultant agent speed. Thus, 'momentum' of the agent, i.e. its previous speed, is disregarded, and the agent gets a newly calculated speed on each update interval.

 

### Threading

 

There are two options:

 

- **Single-Threaded** - When the behavior is single-threaded, it will update each agent's forces in order, on a single CPU thread.
- **Multi-Threaded (Experimental)** - When the behavior is multi-threaded, it will use multiple CPU threads to update agent forces. Using multi-threading can be beneficial if there are many agents that must be updated. Note, however, that as of the initial release of the Agent System, if a random force is part of the set of forces, then the model will not be repeatable. If you need repeatability, you will need to use single-threading, at least for now.

 

### Forces

 

Clicking the button will open a menu that will allow the selection and application of a new force to the system. For a reference on the various forces available: Social Forces.

 

### Active Rules

 

Active Rules can be used to define criteria for applying a force. For example, you might want a force to only be applied relative to certain types of neighbors, such neighbors who have the same behavior activated.

 

Clicking the button will create a new active rule. Once an active rule is selected, the name of the rule will be displayed in the **Active Rules** field. The rule can be removed by clicking the **X** button next to the name of the rule. The rules and categories are as follows:

 

**Self-based** active rules rules apply to the agent based upon its own properties. There are the following options:

 

- **Only When Traveling** - This option will only apply a force when the agent is a Task Executer that is currently performing a travel task.
- **Only When Idle** - This option will only apply a force when the agent is a Task Executer that is not performing a travel task.
- **Label Value** - This option will only apply a force when an agent has a certain label with a certain value. When you select this option, it will bring up a field for the **Label Name**, and a field for the **Value** of the label that it needs to match.

 

**Neighbor-based** active rules apply based upon certain neighbors. There are the following options:

 

- **Neighbors with Behavior** - This option will only apply a force for neighbors with the defined behavior, or no behavior at all.
- **Neighbors with Matching Label ** - This option will only apply a force for neighbors who have a label that matches the agent's label value.
- **Neighbors with Label Value ** - This option will only apply a force for neighbors who have the selected label with the defined value.

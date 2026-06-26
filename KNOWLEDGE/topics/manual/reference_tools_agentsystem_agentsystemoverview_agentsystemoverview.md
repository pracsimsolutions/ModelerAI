---
id: reference_tools_agentsystem_agentsystemoverview_agentsystemoverview
name: "Agent System"
kind: manual
breadcrumb: ["Reference","Tools","AgentSystem","AgentSystemOverview","AgentSystemOverview"]
tags: ["manual","reference","tools","agentsystem"]
source: "manual/Reference/Tools/AgentSystem/AgentSystemOverview/AgentSystemOverview.html"
---

# Agent System

## Overview and Key Concepts

 

An agent system is composed primarily of two things:

 

1. A set of objects, or *agents*
1. A set of behaviors

 

The agent system maintains ongoing proximity relationships between the agents, and each agent can have one or more behaviors activated at any time. These behaviors allow the agents to make decisions based on other agents in their vicinity.

 

### Agent System vs. Agent-Based Modeling

 

FlexSim's Agent System relates closely, but not exactly, to the well-known concept of agent-based modeling. While a universally accepted definition of agent-based modeling is hard to come by, agent-based models often have one or more of the following aspects.

 

- **An agent-based model is made up of a set of autonomous agents** - From an object-oriented perspective, these agents are essentially objects.
- **Agents behave according to some set of rules or decision-making logic** - Sometimes these rules may be simple, but they are not required to be. The behavior mapping may also be heterogeneous and dynamic. Each agent may have a different set of rules/behaviors than its neighbor agents, and may also dynamically acquire and relinquish behaviors.
- **Agents are aware of their environment** - This may be a spatial awareness — an agent is aware of other agents that are spatially close — or it may be some other type of inter-agent awareness, such as familial or occupational relationships. These agent awareness environments are often referred to as *agent topologies*.
- **Agent behavior is generally defined from the perspective of the agent** - In other words, agent-based modeling is inherently object-oriented.
- **Agent-based modeling can involve a very large number of agents** - Agent-based models often analyze population dynamics, where lots of agents are interacting with lots of other agents.

 

Before FlexSim's Agent module was introduced, FlexSim already had strong capabilities for many of the aspects of agent-based modeling. For example, using an object process flow, you can easily define logic using an object-oriented, or agent-based, perspective. Further, you can easily define non-spatial *topologies* using labels that point from one object or token, to other objects or tokens. FlexSim also provides incredible scalability to simulate large numbers of agents. This is especially true if you use process flow's lightweight tokens to represent agents, instead of full 3D objects. And finally, FlexSim's AGV, Conveyor, and A* travel mechanisms all provided high-fidelity proximity-dependent behavior within specific problem domains, such as conveyor systems or AGV travel systems.

 

What FlexSim's new Agent System provides, beyond what was previously available, is a scalable, general purpose, spatial component to an agent's environmental awareness. An agent system maintains a [bounding volume hierarchy](https://en.wikipedia.org/wiki/Bounding_volume_hierarchy) of all of its agents' locations. This is a data structure that allows for more scalable detection of neighbors within an object's spatial proximity. Previously, in order to detect spatial proximity, one would have to compare an object's location with every other object in the entire model. This becomes computationally infeasible when the number of agents reaches into the thousands. Using a bounding volume hierarchy, however, spatial proximity can be detected much more efficiently, allowing the number of agents to be scaled up more easily.

 

### Calculating Proximity

 

The proximity detection mechanism calculates proximity distance by measuring the distance from the center of the agent to the closest point on the bounding box of a neighbor agent.

 

 

Each agent makes its own proximity calculation with each of its neighbor agents. In other words, since the proximity calculation is between an agent's center and the neighbor's bounding box, the agent's proximity calculation with that neighbor may be slightly different than the neighbor's proximity calculation with the agent.

 

 

The proximity calculation also works for 'static' neighbor agents. Static agents are agents that have no behavior assigned to them. The most common examples of this are A* walls and barriers. These are technically agents in that they make up the detectable environment of other agents, but they are static in that they do not themselves have behaviors. In other words, active agents calculate proximity with walls and barriers as part of their behaviors, but not the other way around, because those barriers do not themselves have any behaviors.

 

 

 

 

## The Behaviors Tab

 

As mentioned above, an agent system is composed of a set of agents and a set of behaviors. An individual agent can have one or more behaviors active at any time, and those behaviors can be activated and deactivated dynamically. You define which behaviors make up the agent system using the Behaviors tab.

 

 

### Adding, Removing, and Renaming Behaviors

 

The field at the top and the controls next to it allow you to select, create, remove, rename, and reorder behaviors that are part of the system. Clicking the button will add a new behavior. Click in the text field box to rename the behavior. The Up and Down arrows will rearrange the behavior in the list. The button will delete the behavior.

 

There are two types of agent behaviors in FlexSim: proximity behaviors, and social force behaviors.

 

### Proximity Behavior

 

A proximity behavior is a simple behavior that detects proximity between an agent and its neighbor agents.

 

See Proximity Behavior for more information on proximity behaviors.

 

### Social Force Behavior

 

A social force behavior defines a set of forces that 'push' the agent in various directions, based on the neighbors in its environment. This behavior follows a methodology called [social force modeling](https://en.wikipedia.org/w/index.php?title=Social_force_model&redirect=no).

 

See Social Force Behavior for more information on social force behaviors:

 

  

 

## The Agents Tab

 

The Agents Tab has a list of all the current agents that are part of that system, as well as a setting, for each behavior, whether that behavior should be activated on that agent when the simulation starts.

 

### Agent Column

 

The Agent column displays the path name to the 3D object agent.

 

### Behavior Columns

 

For each behavior in the system, the table will display a column showing whether that behavior should be activated when the simulation starts. By clicking on a cell in this column, a menu selection arrow will appear next to the column. Either *on* or *off* can be selected.

 

 

 

## The General Tab

 

The General Tab has the following properties:

 

### Show Bounding Volume Hierarchy

 

When checked this will display the bounding volume hierarchy wireframes of the agents in the proximity system.

 

### Show Agent State Info

 

When checked, the system will draw a bounding box wireframe around the agent 3D objects, as well as a circle around the agents indicating the proximity radius that was defined.

 

### Resolve Distances and Forces on the X/Y Plane Only

 

When checked, this will resolve proximity measurements along the X and Y axis only. In this case, the proximity bounding volume of an agent will resemble a cylinder that is the height of the agent, with the behavior's defined radius. Further, forces of a social force behavior will be clamped to the XY plane. When the box is unchecked, an agent's proximity bounding volume is a sphere around its center with the behavior's radius. This is checked by default.

 

 

 

## Integrating with Other FlexSim Logic

 

You can integrate agent system functionality with other elements of a FlexSim model, like Process Flow or statistics collection, through event listening. As of the initial release of the Agent System, the best way to listen to agent and behavior events is by sampling them in the tree view.

 

### Listening to Agent Events

 

You can listen to individual agent proximity events if those agents have a proximity behavior enabled. This is especially useful if you are defining the logic of an agent in Process Flow. To listen to a proximity behavior event of a specific agent:

 

1. Add an Event Triggered Source or Wait For Event activity to your process flow.
1. To define the event's target object, call the Agent() constructor in the **Object** field, passing in the agent object. In an object process flow, this would likely be *current*, or it may be a token label value.
1. To define the target event, first open a tree view, and find the agent system in the model, then expand to the list of agents at *>variables/agents* relative to the agent system.
1. Once you have the list expanded, press the button in the process flow activity properties, and then click on one of the agents in the list. You should get a list of events like **On Enter Proximity**.
1. Choose the desired event.

 

 

### Listening to Behavior Events

 

You can also listen to an event for an entire proximity behavior. This is often useful specifically for statistics collection. To listen to a proximity behavior event in a statistics collector:

 

1. Add an Statistics Collector to your toolbox.
1. To define the target event, first open a tree view, and find the agent system in the model, then expand to the list of behaviors at *>variables/behaviors* relative to the agent system.
1. Once you have the list expanded, press the button in the statistics collector's properties, and then click on one of the behaviors in the list. You should get a list of events like **On Enter Proximity**.
1. Choose the desired event.

 

 

This will listen to an event associated with an entire proximity behavior.

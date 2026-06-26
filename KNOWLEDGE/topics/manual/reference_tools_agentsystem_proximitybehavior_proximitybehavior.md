---
id: reference_tools_agentsystem_proximitybehavior_proximitybehavior
name: "Proximity Behavior"
kind: manual
breadcrumb: ["Reference","Tools","AgentSystem","ProximityBehavior","ProximityBehavior"]
tags: ["manual","reference","tools","agentsystem"]
source: "manual/Reference/Tools/AgentSystem/ProximityBehavior/ProximityBehavior.html"
---

# Proximity Behavior

## Overview and Key Concepts

 

A proximity behavior is a behavior that can be activated on agents in an agent system. This behavior measures proximity distances between each agent and its neighbors, and then fires triggers when user-defined proximity distances are breached and subsequently re-established.

 

See Calculating Proximity for more information on how the behavior calculates proximity between agents.

 

 

 

## Behavior Settings

 

 

The Behaviors Tab has the following Properties:

 

### Update Interval

 

Defines how often the system will check to see if agents are within the defined proximity to each other based on the selected time unit.

 

### Proximity Radius

 

This number defines the amount of distance that will trigger a proximity detection in the selected measurement unit.

 

### Threading

 

There are two options:

 

- **Single-Threaded** - When the behavior is single-threaded, it will update each agent in order, on a single CPU thread.
- **Multi-Threaded (Experimental)** - When the behavior is multi-threaded, it will use multiple CPU threads to update each agent. Using multi-threading can be beneficial if there are many agents that must be updated.

 

### Active Rules

 

Active Rules are rules that can be selected to apply the behavior during specific circumstances or conditions. For example, you might want the behavior to only detect proximity with certain types of agents, or with agents that have a certain state or behavior activated.

 

Clicking the button will create a new active rule. Once an active rule is selected, the name of the rule will be displayed in the **Active Rules** field. The rule can be removed by clicking the **X** button next to the name of the rule. The rules and categories are as follows:

 

**Self-based** active rules rules apply to the agent based upon its own properties. There are the following options:

 

- **Only When Traveling** - This option will only apply the proximity detection when the agent is a Task Executer that is currently performing a travel task.
- **Only When Idle** - This option will only apply the proximity detection when the agent is a Task Executer that is not performing a travel task.
- **Label Value** - This option will only apply the proximity detection when an agent has a certain label with a certain value. When you select this option, it will bring up a field for the **Label Name**, and a field for the **Value** of the label that it needs to match.

 

**Neighbor-based** active rules apply based upon certain neighbors. There are the following options:

 

- **Neighbors with Behavior** - This option will only perform proximity detection for neighbors with the defined behavior, or no behavior at all.
- **Neighbors with Matching Label ** - This option will only perform proximity detection for neighbors who have a label that matches the agent's label value.
- **Neighbors with Label Value ** - This option will only perform proximity detection for neighbors who have the selected label with the defined value.

 

### Triggers

 

Triggers for the proximity behavior are defined here. Clicking the button will open a menu of the available triggers. The set of triggers you can add correspond with the set of events that the behavior supports.

 

 

 

## Events

 

Below are the set of events that a proximity behavior supports. See Agent System : Integrating with Other FlexSim Logic for more information on listening to proximity behavior events.

 

### On Enter Proximity

 

This event fires when a neighbor agent is first detected within the proximity radius.

  

#### Narrow Phase Proximity Detection and Aborting Proximity Entry

 

If you return 1 from the On Enter Proximity trigger, then the proximity entry will be aborted, meaning the system will treat the agent as if it is still not in proximity with the neighbor. This can be used to write more finely-tuned [narrow phase](https://www.google.com/search?q=narrow+phase+collision+detection) proximity detection logic, and abort proximity entry when it is determined that the neighbor is not in the more narrowly-defined proximity.

 

### On Exit Proximity

 

This event fires when a neighbor agent is first detected as having left the proximity radius.

 

### On In Proximity

 

This event fires on each update interval, for every neighbor agent that is currently in proximity with the agent.

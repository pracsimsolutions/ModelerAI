---
id: tools
name: "Tools"
kind: class
module: ""
signature: "Tools"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class for creating and getting Tool nodes."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\Tools.xml"
---

# Tools

A class for creating and getting Tool nodes.

Tool Types
The Tools methods take only the following tool types:

AgentSystem
CalculatedTable
ColorPalette
DatabaseConnector
DownBehavior
EmpiricalDistribution
FlowItem
GlobalTable
GlobalVariable
Group
List
MilestoneCollector
ModelBackground
ModelTrigger
MTBFMTTR
ParameterTable
PerformanceMeasureTable
PropertyTable
ReinforcementLearning
StateTable
StatisticsCollector
TimeTable
UserEvent
TrackedVariable

Tool Sub Types
Some tool types have sub types that can be used in the create() method to make a preconfigured version of
that tool. Only the ModelTrigger tools require a sub type to be specified. For the other tools, not specifying a
sub type will make a basic version of that tool with out any preconfiguration.

AgentSystem

Proximity
SocialForce

ColorPalette

Standard
ByNumber
Viridis
Plasma
Rainbow

FlowItem

Container
TaskExecuter
Man
Woman
Boy
Girl
EmptyBottle
EmptyBottleWithLabel
FullBottle
FullBottleWithLabel
FullBottleWithCap
FullBottleWithLabelAndCap
AllBottles

List

FixedResource
Item
TaskExecuter
TaskSequence

ModelTrigger

OnModelOpen
OnModelReset
OnRunStart
OnRunStop

StateTable

Default
PhotoEye

## Methods

### create

```flexscript
treenode (static) Tools.create(string type, string subType = '')
```

Creates a new copy of a tool node.

**Parameters:**
- `type` *(string)* — The name of the type of tool to create. See Tool Types for the full list.
- `subType` *(string, default '')* — The name of the sub type of tool to create. See Sub Tool Types for the full list.

**Returns:** A new tool node.

treenode basicList = Tools.create("List");
treenode teList = Tools.create("List", "TaskExecuter");

### get

```flexscript
treenode (static) Tools.get(string type, string name)
```

Gets a reference to an exisiting tool node.

**Parameters:**
- `type` *(string)* — The name of the type of tool to get. See Tool Types for the full list.
- `name` *(string)* — The name of the tool to get.

**Returns:** A reference to an existing tool node.

treenode timeTable = Tools.get("TimeTable", "Shift1");


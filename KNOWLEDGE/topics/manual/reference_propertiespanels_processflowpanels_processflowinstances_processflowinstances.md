---
id: reference_propertiespanels_processflowpanels_processflowinstances_processflowinstances
name: "Process Flow Instances"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","ProcessFlowPanels","ProcessFlowInstances","ProcessFlowInstances"]
tags: ["manual","reference","propertiespanels","processflowpanels"]
source: "manual/Reference/PropertiesPanels/ProcessFlowPanels/ProcessFlowInstances/ProcessFlowInstances.html"
---

# Process Flow Instances

The Process Flow Instances panel displays which objects are attached to this process flow. This properties panel is not available for General Process Flows. See Process Flow Instances for more information about instances and related concepts.

 

 

The following properties are on the Process Flow Instances panel:

 

### Instance Creation

 

The Instance Creation menu is only available on process flows that are using the Sub Flow type. This menu defines when the FlexSim system will create an instance of a sub flow. This can possibly affect whether shared assets (Resources, Lists, and Zones) are global or local to instances. The menu has the following options:

 

- **Global** - If this option is selected, there will only be one instance of a sub flow running at a time. All shared assets will be shared within that sub flow.
- **Per Instance** - In this context, an instance refers to an object that is running the sub flow. So, for example, if two processors are using the same sub flow, each processor would be considered a separate instance. By default, when you select this option all shared assets will be local to each instance, meaning they won't share assets.
- **Per Block** - You might have activity blocks that each have a Run Sub Flow activity in them. Each Run Sub Flow activity might be connected to the same sub flow. When this option is selected, each activity block that runs a sub flow would be considered a separate instance. By default, when you select this option all shared assets will be local to each instance, meaning they won't share assets.
- **Per Token** - When this option is selected, each token that runs inside the sub flow will be a separate instance. By default, when you select this option all shared assets will be local to each instance, meaning they won't share assets.

  

#### Sharing Assets Between Instances

 

Each shared asset (Resources, Lists, and Zones) has a Type menu in its Quick Properties. You can use this menu to control whether shared assets are global or local to instances. The menu has two options:

 

- **Global** - Shared assets will be globally accessible by all instances. For example, if you have a Resource with a count of 3, any instance that uses that Resource will deplete the total count. If one instance uses all three Resources, another instance won't be able to use it.
- **Local** - Shared assets will only be locally accessible to instances. For example, if you have a Resource with a count of 3, each instance will have a local copy of that Resource with a count of 3. When an instance uses a Resource will only deplete the local count. If one instance uses all three of its Resources, another instance will still have access to its three Resources as well.

  

### Attached Objects

 

You will use the Attached Objects properties to add, remove, and display attached instance objects. These options are only available for the Sub Flow, Fixed Resource, and Task Executer types of process flows. See Types of Process Flows for more information.

---
id: reference_tools_globallists_fields_fields
name: "The Fields Tab"
kind: manual
breadcrumb: ["Reference","Tools","GlobalLists","Fields","Fields"]
tags: ["manual","reference","tools","globallists"]
source: "manual/Reference/Tools/GlobalLists/Fields/Fields.html"
---

# The Fields Tab

## About the Fields Tab

 

The Fields tab is where you define the set of fields that can be used in a pull query or back order queue strategy. To get to the Fields tab of a List, double-click on the desired List in the Toolbox. Then select the Fields tab.

 

Depending on the type of list you create, a default set of fields will be added. You can easily remove these fields by pressing the next to the field, and/or add your own if needed. The following image shows the default set of fields available for an item list.

 

 

 

 

## Adding Fields

 

Click on the button for a list of available fields to add. The available add options will include, at a minimum, the following set of options.

 

- **Expression** - An expression field will evaluate a FlexScript function to retrieve the field's value. Enter the name you want to call the field in **Expression Field**. Then enter the expression directly in **Expression**, or use the buttons to the right of the edit to choose various options, sample for the desired value, or edit the code in a code window.
- **Label** - A label field will retrieve its value from the same-named label on the entry's value object. Enter the name of the label in **Label Field**. Note that this will only work if a list entry's value is an object reference, not a number or string.
- **Push Argument** - A push argument field is only used if you are manually pushing values onto the list using the listpush() command. The listpush() command allows you define additional arguments after the partition ID argument. In the Fields tab you add a Push Argument field, define its name and the number of the additional argument associated with the field. For example, if your listpush() call were: `listpush("ItemList1", item, partitionID, up(item));`, you could then add a push argument field, in **Push Argument Field** give it the name parentObject, and under **Argument**, enter 1, meaning the first additional argument after the partition ID of the listpush() command.
- **age** - The age field is a dynamic customized expression field that returns the amount of simulation time that has elapsed since the entry was added to the list.
- **pushTime** - The pushTime field is a customized expression field that returns the simulation time when the entry was added to the list.

 

 

 Fields for Specific List Types 

In addition to the standard set of fields, pressing the **Add Field** button will provide options specific to the type of list you are using. Note that all of these are just customized **Expression** fields. This means that if the given set of automatic fields don't fit your needs, you can easily view and customize the code associated with each field. Available add options may include the following:

 

### Item List Fields

 

- **itemType** - The item type of the item.
- **distance** - This is the straight-line distance from the item to the puller object. Using in the case of item lists, the puller will be a fixed resource that is pulling from the list, trying to determine which item to receive next.
- **queueSize** - The size of the object that containers the item. For example, if the item is in a queue, this is the total number of items currently in the queue.
- **ageInQueue** - The time that has elapsed since the item first entered the object that currently contains it.
- **totalAge** - The time that has elapsed since the item was first created.
- **meetsPullRequirement** - Used in fixed resource pulling. This evaluates to 1 if the item satisfies the puller object's pull requirement.

 

### Fixed Resource List Fields

 

- **queueSize** - The number of items currently in the fixed resource.
- **distance** - The straight-line distance from the fixed resource value to a puller object (usually an item).
- **throughput** - The throughput of the fixed resource.
- **isIdle** - 1 if the fixed resource's state is STATE_IDLE, 0 otherwise. Note that you may need to adjust this field based on the state profile of the objects that will be on the list. For example, queues don't use STATE_IDLE as part of their state profile, so if queues will be on the list, the expression may need to be adjusted.

 

### Task Sequence List Fields

 

- **priority** - The priority value of the task sequence.
- **preempt** - The preempt value of the task sequence.
- **distance** - The travel distance from the puller task executer to the task sequence's first travel destination.
- **isTransportTS** - 1 if the task sequence is a standard transportation task sequence to move an item from one fixed resource to another, 0 otherwise.
- **isUtilizeTS** - 1 if the task sequence is a utilization task sequence, in other words, a task sequence to go and be utilized at a Processor object, 0 otherwise.
- **loadToUnloadDistance** - For transport task sequences, the straight-line distance from the point where item will be picked up to the point where the item will be dropped off.
- **loadStation** - For transport task sequences, the object that the item will be loaded from.
- **unloadStation** - For transport task sequences, the object that the item will be unloaded to.
- **transportItem** - For transport task sequences, the item that will transported.

 

### Task Executer List Fields

 

- **utilization** - The non-idle state percentage of the task executer.
- **totalTravel** - The total travel distance of the task executer.
- **distance** - Assuming the puller is a task sequence, the travel distance from the task executer to the Task Sequence's first travel destination.
- **queueSize** - The number of items currently in the task executer.
- **throughput** - The task executer's current output.
- **isIdle** - 1 if the task executer's current state is idle, 0 otherwise.
- **isPullerPreempting** - Assuming the puller is a task sequence, 1 if the task sequence is preempting, 0 otherwise.
- **pullerPriority** - Assuming the puller is a task sequence, the priority value of the task sequence.

 

 

 Dynamic Fields 

Each field you add (except push arguments) can be a dynamic field by checking the Dynamic checkbox. A dynamic field is a field whose source value may change while an entry is on the List. While a non-dynamic field's value will be evaluated only once when the entry is added to the List, a dynamic field's value will be re-evaluated every time the list is queried with a pull request. For this reason, non-dynamic fields are faster in execution, but dynamic fields offer more power in querying lists whose source values may change. Also, all puller-dependent fields must be designated as dynamic.

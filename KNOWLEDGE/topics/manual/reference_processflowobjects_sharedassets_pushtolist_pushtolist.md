---
id: reference_processflowobjects_sharedassets_pushtolist_pushtolist
name: "Push to List"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","SharedAssets","PushToList","PushToList"]
tags: ["manual","reference","processflowobjects","sharedassets"]
source: "manual/Reference/ProcessFlowObjects/SharedAssets/PushToList/PushToList.html"
---

# Push to List

## Overview and Key Concepts

 

This activity will add something (tokens, flow items, task executers, task sequences, numbers, strings, etc.) to a list. For detailed information on how Lists work, see the List shared asset and Global Lists.

 

 

When this activity is first added to a process flow, a red exclamation mark shows up to the right of the activity notifying you that a link to a List is required for this activity to function correctly. To link this activity to a List, click the exclamation point and then click on a list object to select it. Alternatively, you can create a dynamic reference by setting the value for the **List** property (such as using a label value or looking up a value in a global table).

 

By default, this activity does not finish until the entry is pulled from the list. This means that a token will remain at a Push to List activity until the **Push Value** has been pulled off the list. The default **Push Value** is the entering token. Pushing the entering token to a list and waiting for it to be pulled allows for easy token synchronization. (See Why Use Lists in Process Flow for more information about token synchronization.) Both tokens sync with each other through the pushing/pulling mechanism, and the pushing token gets access to the token that pulled it (through the **Assign Puller To** property), and vice versa (through the Pull From List's **Assign To** property).

 

You can use the **Max Wait Timer** to cause the token to push a value to a list and then move on to additional activities.

 

 

 

## Connectors

 

The Push to List activity allows multiple connectors out. However, tokens automatically released from this activity will be released through the first connector. Only manually released tokens have the opportunity to exit out a different connector. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Push to List activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### List

 

The **List** box displays the name of the List to which this activity will push. If you linked to a List by clicking on the red exclamation mark next to the Push to List activity, then the name of the List you linked to will show up in this box. You can also manually type in the name of the List or create a dynamic reference here in this box.

 

### Push Value

 

The object or value that is pushed to the list. By default, the entering token will be pushed to the list. But it is also possible to push an object, number, string, node, or array to the list when the token enters the activity instead.

 

### Assign Puller To

 

Assigns a reference on the specified label/node to the object that pulled the entry from the list. See the Assign To section of Common Properties for more information.

 

### Partition ID

 

This will push the value to a certain partition within the list.

 

### Use Max Wait Timer

 

The max wait timer will be evaluated if the token has not pulled its required amount after the specified time. See Max Wait Timer for more information.

 

### Keep On List On Early Release

 

If checked and the token is manually released before it's **Push Value** was pulled from the list, the pushed value will be kept on the list to be pulled at a later time.

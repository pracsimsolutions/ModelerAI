---
id: modellogic_labels_labelmatching_labelmatching
name: "Label Matching"
kind: manual
breadcrumb: ["ModelLogic","Labels","LabelMatching","LabelMatching"]
tags: ["manual","modellogic","labels","labelmatching"]
source: "manual/ModelLogic/Labels/LabelMatching/LabelMatching.html"
---

# Label Matching

## Key Concepts About Label Matching

 

Before reading this topic, make sure you are familiar with the key concepts discussed in the Event Listening topic. Label matching is very important for event listening. Event listening is typically used to link a process flow to a 3D model or to gather statistics from a model using the Statistics Collector. Label matching allows you to gather important data from events and preserve that information in a label.

 

### Event Parameters

 

Standard, object-based events have specific parameters associated with them. These parameters contain basic information about the event, such as which flow item or token triggered the event. You'll usually want to preserve some of this information by putting it into a label that can be referenced by other process flow activities or by a statistics collector's data table. The most commonly used event parameters are:

 

| Event | Explanation |
| --- | --- |
| Entering/Exiting Item | This parameter tracks which item triggered the event. This parameter will contain a reference to the item in the 3D model. You'll want to create a label to reference this item in the future. |
| Station or Destination | Another commonly used parameter is the station or destination that the flow item is going to. |

 

If you would like to know about specific event parameters that are associated with specific events, you should refer to the reference page for the 3D object or process flow activity that you are setting up event-listening for.

 

 

 

## Tools That Use Label Matching or Label Assignment

 

The Process Flow tool and the Statistics Collector tool both use label matching or assignment while listening to events. See Event Listening for more information.

 

 

 

## Overview of the Label Matching Table

 

You'll use the Label Matching table to assign or match a label from an event parameter. This table will appear after you've set up event listening on a process flow activity or a statistics collector. Be aware that it could also sometimes be named the *Label Matching/Assignment* table.

 

 

You can leave any of the event parameters blank if they're not useful for your simulation project.

 

The following sections will explain the different elements of the label matching table.

 

### Parameter Rows

 

The rows will list the event parameters that are available for that specific event. This will change depending on which event you're listening to. If you would like to know about specific event parameters that are associated with specific events, you should refer to the reference page for the 3D object or process flow activity that you are setting up event-listening for. Most of the time, the name of the row will be descriptive enough to give you a good idea of its reference point.

 

For example, in the previous image, the event being listened to is an On Entry event on a fixed resource. The On Entry event has two parameters: *entering item* (which keeps a reference to the item that triggered the event) and *input port* tracks which input port the item entered from.

 

### Label Name

 

Use the cell under the Label Name column to assign (or match) the name of the label that will be created on the created token for this reference point.

 

For example, in the previous example you could type *MyItem* in the Entering Item row under the Label Name column to assign create a label containing the reference to the item that triggered the event.

  

#### Label Names

 

You are not limited to a label on the entering token. Periods separating label names can be used to reference labels on objects that the token has a reference to. For example, if the entering token has a label called *operator* which references another token or a task executer object in the model you can type *operator.item* to reference a label on the operator. Be aware that you can usually only have one dot per expression, so you can't nest labels too deeply.

  

### Operation

 

The Operation column is only available for the Wait for Event process flow activity or the statistics collector. If you click on any cell under the Operation column, it will open a menu. The option you choose will determine which operation will be performed on the given label. The menu has the following options:

 

| Operation | Explanation |
| --- | --- |
| none | The default value. Does nothing with the label. |
| match | This will match the label's existing value to the parameter that is being referenced. The token will only finish the activity when the event fires AND the event's parameter matches the value of the token's label. For example, you may be tracking an item through the system. The token's *item* label points to the flow item in the model. You want to catch when that specific item enters a fixed resource (like a queue) in the model. To do that, you can subscribe to the OnEntry event of the fixed resource, and then for the Entering Item row of the table, type *item* in the Label Name column, and *match* as the operation. |
| assign | This will assign the involved parameter value to the token's label value. |
| insert at front | This operation is like the assign, except that if the label already has a value, it will make the label value into an array and push the new value to the front of the array. You can use this option if perhaps you are looping and you want to accumulate values into an array on the label. |

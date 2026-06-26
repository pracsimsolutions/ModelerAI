---
id: reference_processflowobjects_basic_waitforevent_waitforevent
name: "Wait for Event"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","Basic","WaitForEvent","WaitForEvent"]
tags: ["manual","reference","processflowobjects","basic"]
source: "manual/Reference/ProcessFlowObjects/Basic/WaitForEvent/WaitForEvent.html"
---

# Wait for Event

## Overview and Key Concepts

 

The Wait for Event activity will hold the token until a certain event is triggered. This activity will listen for that event to occur in the simulation model. When that event occurs, it will release the token.

 

 

To get a deeper understanding of how the Wait for Event activity works, you might want to consider reading these topics first:

 

- Key Concepts About Events
- <_a href="../../../../ModelLogic/ProcessFlowBasics/LinkingModels/LinkingModels.html">Linking Process Flows to 3D Models

 

As a more advanced feature, the Wait For Event also has the ability to override the return value of the event it is listening to.

 

When this activity is first created, a red exclamation mark shows up to the right of the activity notifying you that a link to an event is required for this activity to function. This link may be a direct pointer which can be created by clicking on the exclamation mark and then clicking on an activity or object and selecting an event, or the reference may be set through the quick properties.

 

 

 

## Connectors

 

The Wait for Event activity allows multiple outgoing connectors. However, tokens automatically released from this activity will be released through the first connector. Only manually released tokens can exit out a different connector. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Wait for Event activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Object

 

Use the **Object** box to select the object that the Wait for Event activity will listen to. You could possibly listen to:

 

- An object in the model.
- An activity in the Process Flow.
- The current instance object (See Process Flow Instances for more information.)
- A group of objects. In this case, the activity will listen to the defined event on all members of the group.

 

You could select the event you want to listen to by either using the pull-down arrow next to the **Object** box or using the **Sampler** button to select the object.

 

### Event

 

Use the **Event** box to select the event that the Wait for Event activity will listen to. The **Event** box is connected to the object you selected in the **Object** box. When you use the Sampler button to select an object for the **Object** box, a menu will appear that lists all the different types of events that are available for that object. (This menu is created dynamically based on all the possible events related to that object.) The event that you select from this menu will automatically be listed in the **Event** box.

 

You can listen to more than one event on an object. To add an event in addition to the original event, press the **Add Event** button . Additional events will have a **Delete** button button if you need to remove them.

 

### Label Matching/Assignment and Change Rule

 

The **Label Matching/Assignment** or **Change Rule** properties will appear after you've selected an object and event to listen to. See Label Matching/Assignment for more information about these sets of properties.

 

### OnListenerInitialized

 

This is a field that is evaluated after the event listening mechanism has been set up. You may want to use this field if, for example, in a Fixed Resource Process Flow you want to both release an item and wait for the item to leave the object. Here you would want to first set up the exit listening mechanism, and then release the item (since if you do it in the opposite order, the item may leave before you can set up the exit listening). You can use the Wait for Event to listen for the exit, and then once the listening is set up, release the item.

 

### Assign Event Object To

 

Assigns a reference to the event-firing object to the specified label or node. This is useful if you are listening to multiple objects (perhaps from a Group) and need to know which object fired the event. This box is not required and can be left empty.

 

### Send Token To

 

By default, the token is released to the next activity when the triggering event occurs in the simulation model. However, you can use the **Send Token To** field to build some more complex logic into the Wait for Event activity so that it acts more like a **Decide** activity. See the Decide activity for more information about the **Send Token To** field.

 

### Use Max Wait Timer

 

If you check the **Use Max Wait Timer** to release the token after a certain amount of time has passed, regardless of whether the event it was waiting for occurred. You can also determine what should happen to this token if it is released because the wait timer expired. See Max Wait Timer for more information about this property.

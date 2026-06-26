---
id: reference_processflowobjects_subflow_finish_finish
name: "Finish"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","SubFlow","Finish","Finish"]
tags: ["manual","reference","processflowobjects","subflow"]
source: "manual/Reference/ProcessFlowObjects/SubFlow/Finish/Finish.html"
---

# Finish

## Overview and Key Concepts

 

The Finish activity marks the end of a sub flow. The Finish activity essentially acts the same as a Sink in that it destroys tokens that enter it.

 

 

The Finish activity has a few additional functions depending on what kind of activity or event that triggered the sub flow to run:

 

- **Run Sub Flow** - If the sub flow is being run through a Run Sub Flow activity, each token entering the Finish will notify its parent token that is waiting in the Run Sub Flow that it has finished its sub flow. Once all tokens have finished for that parent, the parent will be released from the Run Sub Flow activity. In this case, the Return Value property is not used.
- **Will Override Return Value** - The Wait For Event and Event Triggered Source activities have a Will Override Return Value property. This property allows the token to give a new return value to the event that was triggered. For example, overriding the *Process Time* event of a processor would allow you to perform steps using process flow to define the process time of that processor. The processor will ignore whatever value was originally returned in the *Process Time* box. When overriding a return value, the token will move through the defined set of activities until it hits a Finish activity where it will evaluate the return value for the event. (A return value might be data from a calculation, a new label value, etc.)
- **Execute Sub Flow command** - If you are using any kind of code that uses the `executesubflow()` command to run a sub flow (some picklist options use this command), the Finish activity can return a value when the sub flow is finished being run such that `double processTime = executesubflow(processFlow);` will give you the value evaluated from the **Return Value** box.

 

### About Sub Flows

 

Consider reading Sub Process Flows for more in-depth information.

 

### Overriding a Return Value

 

Though Finish activities are generally thought of as the end of a sub flow, they are also used to mark the end of a **Will Override Return Value** option from the Wait For Event or Event Triggered Source activity or the end of an `executesubflow()` command. In both of these cases, one token will be used to execute a series of activities ending with a Finish activity. Upon arriving at the Finish activity, the **Return Value** box will be evaluated for the token and the result will be returned to the calling function. There are picklists available through standard FlexSim objects that execute sub flows in order to get return values for properties such as a processor's Processing Time.

 

If the Finish marks the end of a sub flow, the **Return Value** will not be evaluated.

 

 

 

## Connectors

 

The Finish activity marks the end of a set of activities. No activity may be connected to a Finish.

 

 

 

## Properties

 

The following image shows properties for the Finish activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Return Value

 

The **Return Value** property can be a static or dynamically-generated value. A return value can be anything you need it to be. For example, you could return a calculation based on data in a label on the token or you could return a reference to an object. See Overriding a Return Value for more information.

 

### Deallocate Shared Assets

 

If checked, when a token is destroyed, any Shared Assets that are allocated by the token will be deallocated. This means, any resources that have been acquired by the token will be released and any entries or backorders on lists associated with the token will be deleted. If the token is in a zone, the zone will only update its content and not update any of its other statistics. If unchecked, the token will remain in the Finish activity until its shared assets have been deallocated.

  

#### Exiting a Zone

 

In order to properly remove a token from a zone and have the zone's output, staytime and custom statistics updated, send the token through an Exit Zone activity.

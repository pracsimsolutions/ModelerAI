---
id: reference_peopleobjects_processflowactivities_basic_process_process
name: "Process"
kind: manual
breadcrumb: ["Reference","PeopleObjects","ProcessFlowActivities","Basic","Process","Process"]
tags: ["manual","reference","peopleobjects","processflowactivities"]
source: "manual/Reference/PeopleObjects/ProcessFlowActivities/Basic/Process/Process.html"
---

# Process

## Overview and Key Concepts

 

The Process activity, like the Delay activity, will hold the token for a certain length of time. You can use a fixed time or you can create the delay time dynamically using a label value on a token, a statistical distribution, etc.

 

Additionally, the Process activity manages the resources involved in the process. These resources will have their state set to the user-defined state. For those resources marked as preemptable, the resources will be made available to be acquired by requests in a higher priority tier. The Process activity also allows you to define what should happen if a resource is preempted away.

 

 

 

 

## Connectors

 

The Process activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Process activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Process Time

 

The **Process Time** defines the amount of time that the token will be delayed.

 

### Involved Resources

 

The **Involved Resources** panel defines who and what is involved in the process. When you add an Involved Resource you have the option to add a Non-Preemptable or Preemptable resource. Both have the following properties:

 

- Object - The object(s) that make up this class of resource.
- State - The state that the objects defined above will be put into after starting the process.

 

Preemptable Resources have the following additional properties.

 

- On Preempt - Here you define what to do if one of the objects defined above is preempted.
- On Resume - Here you define what to do when a preempted resource is reacquired or replaced.

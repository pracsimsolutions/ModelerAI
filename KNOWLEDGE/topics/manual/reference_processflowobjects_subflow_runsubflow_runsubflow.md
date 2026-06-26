---
id: reference_processflowobjects_subflow_runsubflow_runsubflow
name: "Run Sub Flow"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","SubFlow","RunSubFlow","RunSubFlow"]
tags: ["manual","reference","processflowobjects","subflow"]
source: "manual/Reference/ProcessFlowObjects/SubFlow/RunSubFlow/RunSubFlow.html"
---

# Run Sub Flow

## Overview and Key Concepts

 

The Run Sub Flow activity can be used to initiate a sub process flow. The entering token will remain in the Run Sub Flow activity until all of its child tokens have completed their sub flow (by exiting a Finish activity).

 

 

Consider reading Sub Process Flows for more in-depth information.

 

When this activity is first created, a red exclamation mark shows up to the right of the activity. This is notifying you that a link to the Run Sub Flow activity is required. This can be either to a Process Flow Sub Flow object that has a Start activity in it or to a Start activity in the same process flow. This link may be a direct pointer which can be created by clicking on the exclamation mark and then clicking on a Sub Flow object or Start activity, or the reference may be dynamic by setting the value for the **Destination** property.

 

### Starting and Finishing a Sub Flow

 

The Run Sub Flow activity requires a Start activity as the first activity in the sub flow and a Finish activity as the final activity. As child tokens enter the Finish activity, they notify the parent token (in the Run Sub Flow) that the child is complete. Once all children have completed the sub flow, the parent token will be released from the Run Sub Flow.

 

If the Run Sub Flow references a Process Flow Sub Flow object, it will search the Sub Flow for the first Start activity and use that as the start of the sub flow. If your Sub Flow has multiple Start activities, you can use the Sampler to a specific Start activity or enter `SubFlowName: StartName` in the **Destination** box.

 

By default, the token that enters the RunSubFlow activity will create all the children needed at the same time. You can optionally choose to have the token create its children one at a time. In this case, the parent token will one child token and wait for that token to arrive in the finish activity before creating the next child token.

 

 

 

## Connectors

 

The Run Sub Flow activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Run Sub Flow activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Destination

 

Use the **Destination** box to link to a sub flow. This can be a Process Flow Sub Flow object, or it can be a Start activity. See Linking to Sub Flows for more information.

 

### Quantity

 

The number of child tokens that will be created in the sub flow.

 

### Run Tokens One at a Time

 

Check this box to indicate that child tokens should run one at a time, rather than all at the same time.

 

### Label Access

 

The Run Sub Flow activity's label-related properties are nearly identical to the Create Tokens activity with only two exceptions: 1) there is no Create Tokens As menu, and 2) the Copy Labels to Tokens on Create property is named Copy Labels to Children on Create instead.

 

For information on the **Label Access** property, see the Create Tokens activity.

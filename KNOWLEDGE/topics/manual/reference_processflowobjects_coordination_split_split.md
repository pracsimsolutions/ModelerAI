---
id: reference_processflowobjects_coordination_split_split
name: "Split"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","Coordination","Split","Split"]
tags: ["manual","reference","processflowobjects","coordination"]
source: "manual/Reference/ProcessFlowObjects/Coordination/Split/Split.html"
---

# Split

## Overview and Key Concepts

 

The Split activity splits the token into multiple tokens and sends one out each outgoing connector. This activity is similar to the Create Tokens activity, except that the quantity of tokens to create and the destination of each token is determined by the number of outgoing connectors.

 

A Split ID (a reference to the original token) can be added to a user-specified label on each token. This is useful for coordinating with the other split tokens. See Coordination for information on coordinating tokens.

 

 

 

 

## Connectors

 

The Split activity allows any number of connectors out. The Split does not allow other activities to be snapped to it as doing so would remove all connectors from the Split.

 

The number of outgoing connectors will affect the behavior of this activity. See Connectors and Coordination Activities for more about the unique behavior of connectors in Coordination activities.

 

 

 

## Properties

 

The following image shows properties for the Split activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Assign Split ID To

 

A reference to the original token will be stored on the label specified in the **Assign Split ID To** box on each token.

 

### Create As

 

Defines the relationship of the original token to the created token(s).

 

- **Independent Tokens** - Created tokens will have no association with the original token.
- **Child Tokens** - Created tokens will be child tokens of the original token.
- **Sibling Tokens** - Created tokens will be sibling tokens of the original token. In other words, they will have the same parent as the original token.

 

### Label Access

 

For information on Label Access, see the Label Access section of the Sub Process Flows page.

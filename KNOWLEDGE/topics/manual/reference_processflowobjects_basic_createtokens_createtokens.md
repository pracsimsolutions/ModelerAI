---
id: reference_processflowobjects_basic_createtokens_createtokens
name: "Create Tokens"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","Basic","CreateTokens","CreateTokens"]
tags: ["manual","reference","processflowobjects","basic"]
source: "manual/Reference/ProcessFlowObjects/Basic/CreateTokens/CreateTokens.html"
---

# Create Tokens

## Overview and Key Concepts

 

When the Create Tokens activity receives an incoming token, it will create one or more new tokens and automatically send them to a different activity or sub flow (referred to here as the *destination activity*). This activity is similar to the Run Sub Flow activity, except this activity does not hold on to the entering token once the created tokens are released. This activity is also not restricted to only sending tokens to a sub flow or Start activity. Tokens can be created in any activity to include activities in other General Process Flows.

 

 

Consider reading Sub Process Flows for more in-depth information.

 

When this activity is first created, a red exclamation mark shows up to the right of the activity notifying you that a link to another activity or Process Flow Sub Flow object is required for this activity to function. This link may be a direct pointer which can be created by clicking on the exclamation mark and then clicking on an activity or Sub Flow object, or the reference may be dynamic by setting the value for the **Destination** property.

 

 

 

## Connectors

 

The Create Tokens activity can have any number of connectors out. Created tokens will exit this activity based upon the **Destination**. The entering token will always exit through the first connector out (or the next activity if part of a stacked block). See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows the available properties for the Create Tokens activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Destination

 

Indicates the destination activity that the newly created tokens will be automatically sent to. This can be the index of a connector, a connector name, or an activity. Unlike the Run Sub Flow activity, the destination is not restricted to being a Start activity or a Sub Flow. This activity can send the newly created tokens to any activity, whether it be in a sub flow, a General process flow, or any activity in the same process flow. See Linking to Sub Flows for information about linking to a sub flow from this property.

 

### Quantity

 

The number of tokens that will be created every time the Create Tokens activity receives an entering token.

 

### Assign To

 

Assigns a reference on the specified label/node to the created tokens. See the Assign To section of Common Properties for more information. 

### Create As

 Defines the relationship of the original token to the created token(s):

 

- **Independent Tokens** - Created tokens will have no association with the original token.
- **Child Tokens** - Created tokens will be child tokens of the original token.
- **Sibling Tokens** - Created tokens will be sibling tokens of the original token. In other words, they will have the same parent as the original token.

 

### Label Access on Parent Only

 

By default, this checkbox is cleared on the Create Tokens activity and checked on the Run Sub Flow activity. When it is cleared, any activity can create, modify, or read labels on either the parent or the child tokens.

 

When you check the Label Access on Parent Only checkbox, access to the labels on the child tokens will be restricted. In other words, activities will only be able to create, modify, or read labels on the parent token. This restriction essentially causes your parent token to be in multiple activities at the same time because child tokens will just be duplicates of the parent token (as opposed to being independent tokens).

  

#### Child Label Access

 

This option is the only way a child token can add labels to its parent token.

  

When the Label Access on Parent Only checkbox is checked, the next two properties will not be available.

 

### Parent Label Access

 

This menu determines if and how child tokens may access labels owned by their parent. The menu has three available options:

 

- **None** - The child tokens have no access to their parent's labels. All reading, writing and adding of labels are done on the child. Select this option if you want the child to be a completely separate entity from the parent.
- **Read** - This option only allows child tokens to get information from labels on their parent; the child tokens will not be able the add or set the parent's labels. Select this option if you want to set and add labels on the child rather than the parent. If an activity in a sub flow tries to set a label on the child token that is owned by the parent token, the child will add a label to itself with the given label name and value.
- **Read / Write** - This option allows child tokens to read or set the parent's labels, but the child will not be able to add labels to the parent token. Setting labels that are not on the child or parent will cause a label to be added to the child with the given label name and value.

  

#### Parent / Child Relationships

 

When accessing labels on a token, the token will first look at labels owned by itself. If the label doesn't exist and the token has read access of its parent's labels, it will try and access the label on the parent. If the label doesn't exist on the parent, it will check the parent's label access and if able, look up to the next parent's labels, and so forth.

  

### Copy Labels to Tokens/Children on Create

 

By default, the Copy Labels to Tokens/Children on Create is cleared. When this checkbox is checked, the entering or parent token will copy all of its labels to the created tokens/children. On a Create Tokens activity, if the Create As is set to Sibling Tokens, only labels owned by the entering token will be copied. Otherwise, all labels from all ancestors of the token will be copied (based upon each token's label access type). The copied labels can be read or written on the child regardless of the option you selected in the Parent Label Access menu. However, new labels added to the parent may still be read or written if the label access type allows it.

 

### Assign Labels to Created Tokens

 

You can use this group of properties to set labels on the created tokens/children. Each label value will be evaluated for each created token/child. The functionality is identical to the Assign Labels activity. See the Assign Labels activity for more information about these properties.

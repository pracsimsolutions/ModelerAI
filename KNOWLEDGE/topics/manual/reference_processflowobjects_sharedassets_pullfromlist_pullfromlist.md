---
id: reference_processflowobjects_sharedassets_pullfromlist_pullfromlist
name: "Pull From List"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","SharedAssets","PullFromList","PullFromList"]
tags: ["manual","reference","processflowobjects","sharedassets"]
source: "manual/Reference/ProcessFlowObjects/SharedAssets/PullFromList/PullFromList.html"
---

# Pull From List

## Overview and Key Concepts

 

The Pull From List activity will pull one or more values from a list.

 

 

When this activity is first created, a red exclamation mark shows up to the right of the activity notifying you that a link to a List is required for this activity to function. This link may be a direct pointer which can be created by clicking on the exclamation mark and then clicking on a List object, or the reference may be dynamic by setting the value for the **List** property.

 

By default, this activity does not finish until the it has successfully pulled everything that it required from the list. This means that a token will remain at a Pull From List activity until the **Require Number** of values has been pulled off the list.

 

You can use the **Max Wait Timer** to cause the token to attempt to pull a value from the list and then move on to additional activities.

 

The process of pulling a value from a list does not cause that token or object to be physically moved. Instead, a reference to the pulled value can be stored on the pulling token. This reference can be used later in other activities.

 

### Pulling From a Resource

 

The Pull From List activity can also be used with a Resource shared asset. The Resource must be using a list in order for a Pull From List activity to be able to connect to the Resource. This can be done by one of the following:

 

- Defining a **Queue Strategy** on the Resource shared asset.
- Defining a **Default Query** on the Resource shared asset.
- Linking the Resource shared asset to a Global List or a List shared asset.

 

Using a Pull From List activity instead of an Acquire Resource activity will give you more options for what and how you want to acquire the resource(s). Use a Release Resource activity to released the resource that was pulled.

 

 

 

## Connectors

 

The Pull From List activity allows multiple connectors out. However, tokens automatically released from this activity will be released through the first connector. Only manually released tokens can exit out a different connector. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Pull From List activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### List Reference

 

The reference to the List that this activity is pulling from.

 

### Request Number

 

This defines how many values you *want* to pull. The request number must be greater than or equal to the **Require Number**. If more values than the require number are available when the token initially pulls, then the token will pull those additional values, up to the request number, and then immediately release the token.

 

If the request number is zero, then the pull operation will become a mere querying operation. The pull will retrieve all objects on the list that meet the query filter, but it will not remove any values from the list.

 

### Require Number

 

This defines the minimum number of values that you *need* to pull. If the list has insufficient values for the require number, a back order will be created and the token will wait in the activity until the required number of values is available.

 

If the require number is zero then the activity will always finish immediately, returning the amount that it successfully pulled. In this case no back order will be created.

 

### Assign To

 

Assigns a reference on the specified label/node to the value(s) were pulled from the list. See the Assign To section of Common Properties for more information.

 

### Query / Object / Array

 

The conditions which specify which values to pull. This takes one of the following forms:

 

- SQL Query - You can use the button for help in constructing this query.
- Object - this will attempt to pull the specified 3D object. This can also be a reference to any other sort of treenode you wish to pull.
- Array - the array of possible values you are trying to pull. Only values explicitly listed in the Array will be pulled.

 

### Partition ID

 

Defines which partition in the list to pull from.

 

### Puller

 

Defines the puller object. This may be important if the query accesses fields that are dynamic and based upon the puller.

 

### All or Nothing

 

If checked, no values will be pulled from the list until the entire **Require Number** of values can be pulled at one time. Otherwise, values will be pulled as they become available.

 

### Leave Entries On List

 

If checked, all values that are 'pulled' from the list will remain on the list. The values will still be assigned to the **Assign To** label node. This can be useful for querying the list when specific data is added, or for using the list as an informational database. This option behaves like the **All or Nothing** in that no values will be 'pulled' until all values are available.

 

### Use Max Wait Timer

 

The max wait timer will be evaluated if the token has not pulled its required amount after the specified time. See Max Wait Timer for more information.

 

### Use Max Idle Timer

 

The max idle timer will be evaluated each time a value is pulled from the list by the token. See Max Idle Timer for more information.

 

### Keep Back Order On Early Release

 

If checked and the token is manually released before it was able to pull its required amount, the back order will be kept on the list to be fulfilled at a later time.

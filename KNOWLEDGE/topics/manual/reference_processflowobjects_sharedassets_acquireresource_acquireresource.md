---
id: reference_processflowobjects_sharedassets_acquireresource_acquireresource
name: "Acquire Resource"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","SharedAssets","AcquireResource","AcquireResource"]
tags: ["manual","reference","processflowobjects","sharedassets"]
source: "manual/Reference/ProcessFlowObjects/SharedAssets/AcquireResource/AcquireResource.html"
---

# Acquire Resource

## Overview and Key Concepts

 

This activity acquires resources from a Resource shared asset.

 

 

When this activity is first created, a red exclamation mark shows up to the right of the activity notifying you that a link to a Resource is required for this activity to function. This link may be a direct pointer which can be created by clicking on the exclamation mark and then clicking on a Resource object, or the reference may be dynamic by setting the value for the **Resource Reference** field.

 

When acquiring a resource, the acquired value (number, task executer, fixed resource, etc.) will be assigned to a label on the acquiring token. This value or reference can be used in other activities to affect the logic of the process flow.

 

### Acquiring Multiple Resources

 

If multiple resources are acquired by a single activity or if multiple Acquire activities share the same **Assign To Label**, an array will be created on the label with references to all of the acquired resources. The most recent acquired resource will appear as the first index in the array (index 1) and previously acquired resources will be pushed to the end of the array in sequential order.

 

 

 

## Connectors

 

The Acquire Resource activity allows multiple connectors out. However, tokens that successfully acquire a resource and are automatically released from this activity will be released through the first connector. Only manually released tokens have the opportunity to exit out a different connector. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Acquire Resource activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Resource Reference

 

The Resource shared asset this activity is linked to.

 

### Quantity

 

The number of resources to acquire. If the Resource does not have sufficient resources available, the token will wait in this activity until the token is able to acquire the total number of resources returned by this field. If the Resource is in object mode then this field will be read as an integer. If Resource is operating in numeric mode then this field will be evaluated as a floating point or real number.

 

### Assign To Label

 

Once the token has acquired from the resource, a reference to what was acquired will be added to the label on the token as defined by the **Assign To Label** box as either a single reference or an array of references. If data already exists on the label, the references will be prepended to the beginning of the array. This must be a token label as the label will be used to later release the resource using a Release Resource activity. However, it does not have to be a label on the entering token and could reference a parent, sibling or other token's label.

 

If the Resource is in numeric mode, the label will be assigned the quantity acquired which will always be the same as the **Quantity** box.

 

### Query / Object / Array

 

The conditions which specify which resources to acquire. This takes the form of an SQL query, which can be constructed from the drop down menu. Only the WHERE and ORDER BY SQL keywords are available. This is only applicable in object mode. For more information see the SQL Queries page.

 

### Use Max Wait Timer

 

The max wait timer will be evaluated if the token has not acquired its resources after the specified time. See Max Wait Timer for more information.

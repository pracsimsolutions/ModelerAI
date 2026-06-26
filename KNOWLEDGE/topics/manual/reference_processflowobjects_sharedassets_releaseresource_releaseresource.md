---
id: reference_processflowobjects_sharedassets_releaseresource_releaseresource
name: "Release Resource"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","SharedAssets","ReleaseResource","ReleaseResource"]
tags: ["manual","reference","processflowobjects","sharedassets"]
source: "manual/Reference/ProcessFlowObjects/SharedAssets/ReleaseResource/ReleaseResource.html"
---

# Release Resource

## Overview and Key Concepts

 

 

This activity releases resources from a token that have been acquired by an Acquire Resource activity. The resources will automatically return to the Resource activity they were acquired from. If the**Resource(s) Assigned To** does not return a label that has a resource assigned to it, this activity will do nothing and the token will be immediately released to the next activity.

 

 

Once a resource is released back to a Resource shared asset, the Resource will go through and check any tokens waiting in an Acquire activity, attempting to acquire a resource, and see if it's able to acquire it's requested resource.

 

### Releasing Resources When Multiple Were Acquired

 

If multiple resources were acquired by a single Acquire activity while in object mode, a treenodearray of references to the acquired resources will be created. When releasing these resources using the Release Resource activity, not all of acquired resources will be released by default. By default, the **Resource(s) to Release** box is set to *Last Acquired*. If two Operators were acquired by one Acquire activity, the *Last Acquired* option will only release one of the operators. This can allow you to easily begin an operation with multiple resources and release them one at a time.

 

You can acquire resources from multiple Resource shared assets and assign them to the same label. Doing so will create an array where the last acquired resource will be index 1 of the array. For example, a label with the resources {/Operator3, /Operator2, /Operator1} acquired Operator3 last and Operator1 first. Releasing the *Last Acquired* will release Operator3 leaving you with the array {/Operator2, /Operator1}. When Operator1 and Operator2 are released, the label storing these resources will be deleted.

 

### Accessing Resources from an Array

 

As you acquire multiple resources on the same label, as in the example above, you may need to access those resources to give them tasks to complete. If your resources are assigned to the label *resource* with the value {/Operator3, /Operator2, /Operator1}, using `Label: resource` or `getlabel(token, "resource")` in a property like **Executer / Task Sequence** of one of the Task Sequence activities will return the reference to the resource at the first index of the array. In this case, Operator3. To access one of the other operators, you can use a *Value/Object From Array* pick option.

 

### Releasing Numeric Resources

 

Multiple numeric resources can also be acquired and assigned to the same label. Doing so will create a double array on the label. For example, if three Acquire activities are used to acquire 1 resource from three different Resource shared assets, and each resource is assigned to the label *resource*, the resource label on the token will display an array of {1.00, 1.00, 1.00}.

 

In this case where quantities are equal, it is not guaranteed that the resources will be released in the correct order when using pick options like *First Acquired* and *Array/Range of Resources*. To ensure the correct resource is released you can pass a reference to the Resource shared asset into the **Resource(s) to Release** property.

 

However, if these same Acquire activities acquire different quantities, say 1, 2 and 3, the Release Resource activity will be able to correctly identify which resource is being released using the *First Acquired* and *Array/Range of Resources* pick options.

  

#### Array Type Mismatch

 

If you try to acquire a numeric resource on the same label as a previously acquired object resource, you'll get an Array Type Mismatch error. Resources must be of the same type to be acquired on the same label. This is due to the nature of arrays only handling one type each (treenodearray, intarray, doublearray, stringarray).

  

 

 

## Connectors

 

The Release Resource activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Release Resource activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Resource(s) Assigned To

 

Where the resource(s) to release are located. The return value of this field should be a label on a token where an Acquire Resource activity assigned the acquired resource to.

 

### Resource(s) to Release

 

The conditions which specify which resources to release. If multiple resources were acquired and assigned to the same label, the label will display an array of all the acquired resources. To release a specific resource from the array, return the index of the array or an intarray of multiple indexes. You can also return a reference to the Resource shared asset that was acquired from. Doing so will search the array for resources acquired from the shared asset and release all of them. This can be useful when you are unsure of which index to release, or when releasing Numeric Resources as described above in Releasing Numeric Resources.

 

### Assign Released Resource(s) To

 

Assigns a reference on the specified label/node to the released resource or resources. This can be helpful if you need to maintain a reference to a resource for use in a later Acquire activity. See the Assign To section of Common Properties for more information.

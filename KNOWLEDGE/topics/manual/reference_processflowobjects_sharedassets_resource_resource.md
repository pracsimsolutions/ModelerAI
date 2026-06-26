---
id: reference_processflowobjects_sharedassets_resource_resource
name: "Resource"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","SharedAssets","Resource","Resource"]
tags: ["manual","reference","processflowobjects","sharedassets"]
source: "manual/Reference/ProcessFlowObjects/SharedAssets/Resource/Resource.html"
---

# Resource

## Overview and Key Concepts

 

This shared asset represents an asset with a limited supply that can be acquired and released. For example, three manufacturing stations might all share one tool. The tool is modeled here as a Resource. When a station uses the tool, it would be acquiring the Resource. Once the Resource has been acquired, nothing else can acquire it until the acquiring token has released it. Resources are useful for modeling timing constraints, where multiple tokens need to use a limited pool of assets.

 

 

A Resource is acquired by an Acquire Resource activity and released by a Release Resource activity.

 

### Resource Operation Modes

 

The Resource can operate in two modes, as follows:

 

**Numeric Mode**

 

When the Resource operates in numeric mode, it simply keeps track of a number in determining its availability. This number starts at zero. When activities acquire the Resource, it increments the number, and when activities release the Resource, it decrements the number. If an Acquire activity is requesting a quantity that is greater than the Resource has available as defined by the **Count** property, then the token will be unable to acquire the resource.

 

To put the Resource into numeric mode, define its **Reference** property as *Numeric*.

 

When in numeric mode, the **Count** property can dynamically change during the simulation. It will be reevaluated every time a token attempts to acquire a resource and whenever a resource is released. This can be useful if you want to simulate a resource that changes based upon the time of day or other model parameters. Even if the total resource's count drops below the number of resources that have been acquired, tokens will keep their acquired value until they release their resource in a Release activity.

 

Numeric mode can also be used to simulate non-discrete resource quantities. For example, you can give the Resource a **Count** of 5.5, and acquire quantities like 1.5, etc.

 

**Object Mode**

 

When the Resource operates in object mode, it links to a discrete set of objects in the model. This may be a task executer, fixed resource or other 3D object, an array (objects, strings or numbers) or a Group of objects. When using a TE or FR object, the **Count** property can duplicate objects when needed. When using arrays, a Dispatcher or Groups, the **Count** property defines how many objects in the set can be used. When tokens acquire the Resource, the Resource gives them back a reference to specific acquired object(s), instead of just managing an availability number. In this mode, the **Count** is established at the start of the simulation and cannot change after that.

 

### Duplicating Objects

 

If the Resource is in object mode and its **Reference** property references an object in the model, the **Count** property will be used to duplicate the object by `count - 1`, where the referenced object is the first resource available. Once duplicated, the task executer or fixed resource objects may be moved in the 3D view and their properties may be individually modified. Properties set on the linked resource will NOT be pushed down to the duplicated objects once they have been created. To propagate changes from the referenced object to the duplicated objects (that is, labels), set the **Count** to 1, reset the model, then set the **Count** back to the original value. Alternatively, you can select all of the objects and modify values in Quick Properties or use the Edit Selected Objects window.

 

It may be useful to tie the **Count** property to a Process Flow Variable so that the count may be changed through the Experimenter over multiple replications. The resource will add new objects to the model and remove unused objects as needed.

  

#### Adding and Removing Resources

 

If the Count is greater than the previous Count, the referenced object will be duplicated and the new object will be at the same location as the referenced object. If the Count is less than the previous Count, the last duplicated objects will be deleted. This means if you duplicated objects and then placed them throughout the model, the Experimenter should only remove resources not add them as doing so may cause your objects to be in incorrect locations.

  

### Using A List

 

By default, acquire requests are processed in FIFO (first in first out) order, and in object mode the resource objects themselves are distributed in FIFO order. If a **Default Query** or a **Queue Strategy** is used on the Resource or if any Acquire Resource activity that references this Resource uses a **Query**, the Resource will switch over to using a List. This List will be internally owned by the Resource. This allows the Resource to utilize the built in SQL functionality of the List to determine which tokens to allocate resources to first and which resources should be allocated. Through the **Advanced** button, you can set up additional fields for the List that can be used in the **Query** and **Queue Strategy** properties.

 

The Resource can also be used as a pass-through to a List shared asset by referencing the List in the **Reference** field. You can also reference a Global List.

 

If a Resource is using a list then the Pull From List activity can be used like an Acquire activity. This gives you more options for what and how you will allocate resources.

 

 

 

## Connectors

 

Only Acquire Resource and Release Resource activities can be connected to a Resource. If the Resource is using a List to manage its assets, then a Pull From List activity can also be connected to the Resource. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Resource shared asset:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Reference

 

This box will only be evaluated on reset. It may be either *Numeric*, a reference to an object in the 3D model, a Group, an array (objects, strings or numbers) or any other object or node.

 

### Count

 

The number of resources that are available to acquire. If the **Reference** property is set to a Group, a Dispatcher (team) or array (objects, strings or numbers), the count should be less than or equal to the number of objects in the Group/Team/array. If a Group/Team/array has 10 objects in it and the count is set to 5, only the first 5 objects in will be used. Reorder the objects to specify which 5 objects will be used. This can be a very useful features when implemented with the Experimenter. The **Count** can be set to a Process Flow Variable which is then set as an Experimenter variable which changes for each scenario. If Reference is not *Numeric*, this property will only be evaluated on reset. If Reference is a direct reference (that is, to a task executer or fixed resource), the object will be duplicated. See Duplicating Objects for more information.

 

### Type

 

Resources can be defined globally or locally. If the Resource is in a General Process Flow or connected to a Group or an array, the Resource will be globally available and this property will be unavailable. Otherwise, it can be set to Local so only tokens in the same Process Flow instance will be competing for the same resources. If Type is set to Local and Reference is linked to a Dispatcher, Task Executer or Fixed Resource object, the object will be duplicated for each instance of the Process Flow.

 

### Queue Strategy

 

If a number of activities are trying to acquire resources and no resources are available, their acquire requests will stack up in a queue. By default this is FIFO (first in first out or first come first serve). Defining a Queue Strategy gives you control of who is given resources first. The queue then becomes back orders on a list and a SQL command is used to determine the order of the queue.

  

#### Using a Queue Strategy with Numbers

 

If the Resource is set to *Numeric*, the Queue Strategy will be grayed out. If you want to use a Queue Strategy but you want to use numbers, or the value of the acquired resource is not important, you can use an array of numbers. In this case you would set the **Reference** field to an array. For example,

 

```
[1, 2, 3, 4, 5]
```

 

or

 

```
[1, 1, 1, 1, 1]
```

 

Then set the **Count** field to be either 5 (for this example) or choose the *Array Size* option from the drop down. Setting the Reference to an array of numbers will push each number to a list that is internal to the Resource. The Queue Strategy can then be used to specify which tokens have priortiy for acquiring the next resource.

  

### Default Query

 

You can define a default query that will be used if no query is used by an Acquire activity. These queries allow you to define what resource should be allocated.

 

### View Entries

 

Only valid if the Resource is using a list (see Using a List) this will display all of the available resources.

 

### View Back Orders

 

Only valid if the Resource is using a list (see Using a List) this will display all of the tokens attempting to acquire resources.

 

### Advanced

 

Only valid if the Resource is using a list (see Using a List). Opens the List properties page.

---
id: reference_processflowobjects_sharedassets_list_list
name: "List"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","SharedAssets","List","List"]
tags: ["manual","reference","processflowobjects","sharedassets"]
source: "manual/Reference/ProcessFlowObjects/SharedAssets/List/List.html"
---

# List

## Overview and Key Concepts

 

The List is a shared asset that can represent a list of tokens, flow items, task executers, task sequences, numbers, strings, etc. Using the Push To List and Pull From List activities, the contents of a list can be dynamically updated during a simulation run. For example, a list could represent flow items or tokens that are waiting to go to the next available object or activity. When an object or activity becomes available, the flow item or token can be pulled from the list and sent to the downstream object or activity.

 

Lists can be either be internal to a process flow or they can link to a Global List in the 3D simulation model.

 

 

### Why Use Lists in Process Flow

 

Using Lists, you can:

 

- Sync tokens or objects
- Organize groups of objects
- Track custom statistics
- Use search concepts like filtering and prioritization for making choices in the model

 

Lists have a lot of functional overlap with other shared assets. However, one of the unique advantages of using Lists within Process Flow is for token synchronization. Token synchronization means that tokens can wait for each other at defined points by having one activity push a token onto a List, while another activity pulls the token from the List.

 

To give a real-world example of a business system that would need token synchronization, imagine you want to simulate the loading dock of an order picking warehouse. When the warehouse receives notice that a truck is on its way, the employees need to begin staging the orders that have already been picked for shipment. It's possible that the truck could arrive before the employees are finished staging the orders on pallets, in which case the truck will need to wait until the orders are ready. However, it's also possible that the employees might finish staging the orders before the truck arrives, in which case the orders will need to wait until the truck arrives.

 

If you represent the truck as a single token and each order as a single token, then the problem is as simple as sending each order token to a Push To List activity that pushes the order token to a list and sending the truck token to a Pull From List activity that will pull all of the order tokens from the list. If the orders arrive first, the order tokens will remain in the **Push To List** activity until the truck arrives and pulls them from the list. If the truck arrives first, it will wait at its **Pull From List** activity until it has pulled all of the orders from the list.

 

### Creating, Linking, and Accessing Lists

 

A List shared asset can be used to create either internal lists or link to global lists:

 

- **Global List** - Links to a Global List that has already been created in the 3D simulation model. Use a Global List if your list needs to be linked to a list used by objects in the 3D model.
- **Internal List** - The default method. A list that is created and owned by the List shared asset. Use an internal list if your list does not need to be accessible to objects in the 3D model.

 

When using a local list, the list can either be defined globally or locally to the process flow.

 

### Debugging Lists in Process Flow

 

Normally, when you need to debug code in one of your pick options, you would click in the far left column of the code editor to create a breakpoint:

 

 

However, if you are using an Internal List in a List Shared Asset and you need to debug a field, you need to use the debug() command:

 

 

This also applies to the Back Order Reevaluation Events.

 

 

 

## Connectors

 

Only Push to List activities and Pull From List activities can be connected to a List.

 

 

 

## Properties

 

The following image shows properties for the List shared asset:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### List

 

Use the **List** box to link to a Global list or create an internal list. See Creating, Linking, and Accessing Lists for more information about the difference between a global list and an internal list.

 

By default, all lists are internal, which means that this box should say *Use Internal List*. If you want to use an internal list and the box has text other than this in it, click the remove button to revert back to using an internal list.

 

To link to a Global List, click the arrow next to this box to open a menu. Point to **Global List** and select the name of the list from the menu or use the sampler and sample the list from the Toolbox.

 

### Type

 

The **Type** menu determines whether the list will be define locally or globally. This menu is only available if you have created an Internal list and the List shared asset is not contained in a General Process Flow. See Creating, Linking, and Accessing Lists for more information.

 

### View Entries...

 

The **View Entries...** button will display all of the values currently on the list. This button functions the same as the **View Entries...** button on the **General** tab on the Global List Properties.

 

### View Back Orders...

 

The **View Back Orders...** button will display all the back orders who are pulling from the list. This button functions the same as the **View Back Orders...** button on the **General** tab on the Global List Properties.

 

### Advanced

 

The **Advanced** button opens the List properties page. Use this page to edit the list's properties.

---
id: using3dobjects_workingwithflowitems_keyconceptsflowitems_keyconceptsflowitems
name: "Key Concepts About Flow Items"
kind: manual
breadcrumb: ["Using3DObjects","WorkingWithFlowItems","KeyConceptsFlowItems","KeyConceptsFlowItems"]
tags: ["manual","using3dobjects","workingwithflowitems","keyconceptsflowitems"]
source: "manual/Using3DObjects/WorkingWithFlowItems/KeyConceptsFlowItems/KeyConceptsFlowItems.html"
---

# Key Concepts About Flow Items

## Introduction to Flow Items

 

Flow items are the items that will flow through your simulation model from the beginning of your simulation until the end. Flow items can represent many different things in your simulation model such as:

 

- Materials moving through an assembly line
- Customers walking through a service center
- Products moving to different locations in a warehouse
- Orders getting transferred from one station to another

 

By default, flow items look like brown boxes, as shown in the following image:

 

 

However, FlexSim has a variety of other shapes available for flow items: people, cylinders, spheres, pallets, totes, trucks, etc. You can easily change and customize the visual appearance of the flow items in your simulation model using a tool called the Flow Item Bin. Using this tool you can:

 

- Change the color, size, or rotation of the basic flow item shapes
- Add your own custom 3D flow items
- Set default labels or itemtypes on flow items
- Create different shape frames for the basic flow item shapes so that their appearance can change at different stages in the simulation model

 

See Using the Flow Item Bin and Creating Custom Flow Items for more information about changing the basic appearance of flow items.

 

 

 

## Getting Flow Items into a 3D Model

 

The most common way to put flow items into a 3D model is to use a source object, as shown in the following image:

 

 

In this model, a source creates flow items and puts them into a queue at random intervals. You can set the source to create flow items at any rate that you want.

 

Alternatively, you can add flow items to a model using the Create Object activity in the Process Flow tool. For example, the process flow in the following image can act just like a source:

 

 

In this example, the Create Object activity is set to create a box flow item and put it inside the queue in the 3D model. See the chapter on Building the Model's Logic for more information about the Process Flow tool.

 

 

 

## Types of Flow Items

 

There are three different types of flow items: basic, container, and TaskExecuter. Each type of flow item behaves a little differently in the simulation model and has different properties or unique logic, as explained in the following table:

 

| Type | Description | Flow Items |
| --- | --- | --- |
| Basic | Basic flow items are simple shapes that can flow through the simulation model. You can change the visual appearance of these shapes so that they have a custom color, size, or rotation. | - Box - Cylinder - Sphere - Plane - Circle |
| Container | You can pack a container flow item with basic flow items, possibly preparing the container and its contents for transportation or storage. You can customize how flow items get packed into the container flow items using the Flow Item Bin. See Packing Methods for Containers for more information. | - Pallet - Tote - Truck |
| TaskExecuter | TaskExecuter flow items look like an operator object, which is helpful if you want to simulate customers as flow items. These flow items act like actual task executer objects, meaning you can assign them tasks to do and they can walk from one object to another in the simulation model. | - Person |

 

 

 

## Flow Item Labels

 

In FlexSim, labels store specific pieces of information on objects such as flow items. Labels are key to the overall functionality of FlexSim because they can track important information or dynamically change the model's logic or behavior. See Key Concepts About Labels for more information.

 

For example, imagine you want to track a flow item's weight as it moves throughout the simulation model. You would create a label on the flow item named *Weight* and perhaps set its initial value to 25 (meaning 25 pounds or grams). As the flow item moves through the simulation system, different processes could change the value of that label.

 

As another example, imagine you want different flow items to undergo different processes based on their specific item type. Product A should be diverted to one set of processes while Product B should be diverted to a different set of processes, and so forth. To create this kind of logic, you'd create a label named *Type* on the flow item. A flow item could be assigned a value of 1 to represent Product A and a value of 2 to represent Product B. As the flow item moves through the simulation system, it could reach a point in the simulation model where an object checks the label on the flow item. If the value of the label is 1, it might send that item to the set of processes for Product A. If the value of the label is 2, it might send the flow item to the processes for Product B instead.

 

Using the Flow Item Bin, you can add a default label to a flow item. See Adding Labels to Flow Items for specific instructions.

  

#### Labels Can Be Added Later

 

You do not necessarily need to use the Flow Item Bin to add labels to a flow item. You can set up your model's logic so that labels are added while a flow item moves through the simulation model. However, it can be useful for label management to set the initial values in the Flow Item Bin.

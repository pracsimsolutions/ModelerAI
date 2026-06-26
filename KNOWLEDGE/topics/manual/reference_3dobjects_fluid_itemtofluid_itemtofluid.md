---
id: reference_3dobjects_fluid_itemtofluid_itemtofluid
name: "Item to Fluid"
kind: manual
breadcrumb: ["Reference","3DObjects","Fluid","ItemToFluid","ItemToFluid"]
tags: ["manual","reference","3dobjects","fluid"]
source: "manual/Reference/3DObjects/Fluid/ItemToFluid/ItemToFluid.html"
---

# Item to Fluid

## Overview and Key Concepts

 

The ItemToFluid is an object that is used to interface between the fluid objects and the discrete objects. It receives flow items and converts them to fluid material.

 

 

The ItemToFluid is a Fixed Resource object that is designed to interface between the discrete objects and the fluid objects. When it receives a flow item, it destroys the flow item and creates fluid that can be sent to any of the other fluid objects. The amount of fluid created is based on two values that the modeller specifies. The first is the amount of fluid that each discrete unit creates. The second is the number of discrete units each flow item represents. Generally this value will be 1, but often the modeller will use a flow item to represent multiple physical objects. In that case, the number should be set to the number of objects represented by each flow item. The total number of fluid units that are created for each flow item that enters is found by multiplying these two values together. For example, a single flow item may represent 10 bags that each contain 25 pounds of a fluid material. The discrete units per flow item in this case is 10, and the fluid units per discrete unit is 25. Each time one of these flow items enters the ItemToFluid, 250 pounds of fluid are created.

 

The ItemToFluid has a maximum capacity that the modeller defines. The object will not accept any flow items if there is not at least enough empty space in it to hold all of the material that flow item will create. The modeller can also define the ProductID, and sub-component mix of the fluid that is created and sent out.

 

The flow items entering can be controlled using standard FixedResource pull logic. The output of fluid is completely controlled by the modeller. They can edit the maximum object rate, maximum port rate and the port scale factors using the GUI and the AdjustOutputRates function.

 

 

 

## States

 

For statistical purposes, the fluid blender will be in one of the following states at various points during a simulation run:

 

### Empty

 

The ItemToFluid has no material in it.

 

### Not Empty

 

The ItemToFluid has fluid material in it that has not been pulled out yet.

 

### Full

 

The ItemToFluid's maximum capacity has been reached.

 

 

 

## Properties

 

The item to fluid object has seven tabs with various properties. The last five tabs are the standard tabs that are common to most fluid objects. For more information about the properties on those tabs, see:

 

- The Triggers Tab
- The Fluid Level Display Tab
- The Labels Tab
- The General Tab

 

The Statistics tab is obsolete and is not supported for this object any more. Use the Statistics group in the object's Quick Properties instead.

 

The Flow tab is common to many fixed resources objects. See The Flow Tab for more information. Only the Fluid to Item tab is unique to the item to fluid object. The properties on this tab will be explained in more detail in the following sections.

 

The Fluid to Item tab has the following properties:

 

 

### Maximum Content

 

The maximum amount of fluid material that this object can hold at any time.

 

### Initial Product

 

This opens the Initial Product Window which that allows the modeler to define the ProductID and sub-component mix of the material that is created by this object.

 

### Adjust Output Rates

 

This function is called every tick and allows the user to change the output rates and scale factors during a model run. There are access variables that are references to the nodes that hold the rates and scale factors. To read the current rate (object or port) the getnodenum() command should be used. To change the rate the setnodenum() should be used. To read or change the scale factors getnodenum() and setnodenum() should be used in conjunction with the rank() command.

 

### Input Ports

 

These properties affect how the object receives material from upstream objects:

 

- **Fluid per Discrete Unit** - This is the number of units of fluid material that are in a single discrete unit in the flow item. For example: 10 pounds per bag.
- **Discrete Units per Flowitem** - This is the number of discrete units of material that are in each flowitem. For example: 5 bags per pallet, where a flow item is a single pallet.
- **Flowitem Recycling** - The modeler uses this drop-down list to decide where to store flow items that need to be recycled. They should send flow items back to the section of the flow item bin that they originally came from.

 

### Output Ports

 

These properties affect how the object sends material to downstream objects:

 

- **Maximum Object Rate** - The maximum rate that material will leave this object through all output ports combined.
- **Maximum Port Rate** - The maximum rate that material will leave this object through any one port.
- **Output port scale factor** - Each row of the table is the scale factor for a single output port. The value is multiplied by the maximum port rate to determine the actual maximum rate for that specific port. This allows the modeler to change the rate of individual output ports during a model run.

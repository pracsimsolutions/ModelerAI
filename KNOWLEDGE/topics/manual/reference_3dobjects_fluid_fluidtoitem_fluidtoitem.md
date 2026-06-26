---
id: reference_3dobjects_fluid_fluidtoitem_fluidtoitem
name: "Fluid to Item"
kind: manual
breadcrumb: ["Reference","3DObjects","Fluid","FluidToItem","FluidToItem"]
tags: ["manual","reference","3dobjects","fluid"]
source: "manual/Reference/3DObjects/Fluid/FluidToItem/FluidToItem.html"
---

# Fluid to Item

## Overview and Key Concepts

 

The FluidToItem is an object that is designed to interface between the fluid objects and the discrete objects. It receives fluid and converts it to flow items that it sends downstream.

 

 

The FluidToItem is used to convert fluid to flow items that can be sent to any Fixed Resource. The modeller chooses the flow item class, the default itemtype and name for the flow items that are created. The modeller also has to specify the amount of fluid material that must be collected before a flow item can be created. This is done by editing two values. The first is the amount of fluid in each discrete unit. The second is the number of discrete units that each flow item represents. The total amount of fluid that will be collected before a flow item is created is found by multiplying these two values together. For example, if a single flow item represents 20 cans and each can holds 5 gallons of fluid, the number of discrete units per flow item is 20 and the number of fluid units per discrete unit is 5. Therefore, for each 100 gallons of fluid collected in the FluidToItem a single flow item will be sent out.

 

Once a flow item is created, the standard Send To Port logic is used to send it downstream. This means that the FluidToItem can send flow items to any of the discrete objects and can call operators to do the transporting. The modeller can define how the input to the FluidToItem works by changing the maximum object input rate, maximum port input rate and the port scale factors using the GUI and the AdjustInputRates function.

 

 

 

## States

 

For statistical purposes, the fluid blender will be in one of the following states at various points during a simulation run:

 

### Empty

 

The FluidToItem has no material in it.

 

### Blocked

 

The FluidToItem has material in it that it is unable to send downstream.

 

### Collecting

 

The FluidToItem is collecting material.

 

 

 

## Properties

 

The fluid to item object has seven tabs with various properties. The last five tabs are the standard tabs that are common to most fluid objects. For more information about the properties on those tabs, see:

 

- The Triggers Tab
- The Fluid Level Display Tab
- The Labels Tab
- The General Tab

 

The Statistics tab is obsolete and is not supported for this object any more. Use the Statistics group in the object's Quick Properties instead.

 

The Flow tab is common to many fixed resources objects. See The Flow Tab for more information. Only the Fluid to Item tab is unique to the fluid to item object. The properties on this tab will be explained in more detail in the following sections.

 

The Fluid to Item tab has the following properties:

 

 

### Maximum Content

 

The maximum amount of fluid material that this object can hold at any time.

 

### Flowitem

 

This is the class of flow item that the FluidToItem will create.

 

### Adjust Input Rates

 

This function is called every tick and allows the user to change the input rates and scale factors during a model run. There are access variables that are references to the nodes that hold the rates and scale factors. To read the current rate (object or port) the getnodenum() command should be used. To change the rate the setnodenum() should be used. To read or change the scale factors getnodenum() and setnodenum() should be used in conjunction with the rank() command.

 

### Input Ports

 

These properties affect how the object receives material from upstream objects:

 

- **Maximum Object Rate** - The maximum rate that material will enter this object through all output ports combined.
- **Maximum Port Rate** - The maximum rate that material will enter this object through any one port.
- **Input port scale factor** - Each row of the table is the scale factor for a single input port. The value is multiplied by the maximum port rate to determine the actual maximum rate for that specific port. This allows the modeler to change the rate of individual input ports during a model run.

 

### Flowitem Output

 

These properties define when the FluidToItem creates a flow item and some information that will be defined on the flow item when it is created:

 

- **Fluid per Discrete Unit** - This is the number of units of fluid material that are in a single discrete unit in the flow item. For example: 5 gallons per can.
- **Discrete Units per Flowitem** - This is the number of discrete units of material that are in each flow item. For example: 10 cans per case, where a flow item is a single case.
- **Flowitem ItemType** - This value will be assigned to the itemtype of the flow items as they are created. It can be changed using the OnCreation or OnExit triggers.
- Flowitem Name - This name will be assigned to the flow items that are created. It can be changed later using triggers.

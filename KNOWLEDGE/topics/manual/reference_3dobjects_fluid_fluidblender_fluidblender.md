---
id: reference_3dobjects_fluid_fluidblender_fluidblender
name: "Fluid Blender"
kind: manual
breadcrumb: ["Reference","3DObjects","Fluid","FluidBlender","FluidBlender"]
tags: ["manual","reference","3dobjects","fluid"]
source: "manual/Reference/3DObjects/Fluid/FluidBlender/FluidBlender.html"
---

# Fluid Blender

## Overview and Key Concepts

 

The FluidBlender is used to mix materials from multiple input ports based on percentages the user defines (not fixed amounts). It is most commonly used for in-line blending where the mixing is not done in batches.

 

 

The FluidBlender receives material from more than one input port based on a series of percentages that the modeller defines. Once the material has been pulled into the Blender it is ready to be pulled out immediately by downstream objects. The modeller defines the ProductID of the material that is released by the Blender. The sub-components of the mix will be a mixture of the sub-components of the material that was pulled in. The sub-component percentages are based on how much of each incoming product is mixed together.

 

The percentages that the Blender uses are defined in a table called the Blender Recipe. There is one row in the table for each input port that the Blender has. The rows in the table are not visible in the Properties window until there are objects connected to the Blender's input ports. Each row has two columns: the ingredient name and the percentage. The name is a text string that the modeller uses to identify the material that is being pulled in. It is for the modeller's benefit only, it does not affect how the Blender will work. The percentage value is a number between 0 and 100, indicating what percentage of the incoming material will come from the port represented by the row.

 

The Blender always makes sure it is pulling the correct percentages. Each tick, the Blender will calculate how much from each port it needs to either fill itself or pull at its maximum input rate. If there is not enough of a material to meet this demand, the amount it pulls from the other ports will be reduced to keep the percentages correct. If there is not enough empty space in the Blender to pull at the full input rate, the Blender will lower the input rate (while still maintaining the defined percentages) to fill itself. It recommended, therefore, that the maximum content of the Blender be at least twice the input rate. This will allow the Blender to receive at the maximum input rate, as long as material is flowing out at least as fast.

 

Because the Blender controls how much it pulls from each port at any point in time, the modeller does not have access to the maximum port rate or the input port scale factors. They can, however, edit the object's maximum input rate. The user has complete control over the output rates and scale factors. They can change these values with the AdjustOutputRates function, which fires every tick. This allows them to update the output rates and scale factors as the model is running.

 

 

 

## States

 

For statistical purposes, the fluid blender will be in one of the following states at various points during a simulation run:

 

### Empty

 

The Blender has no material in it.

 

### Mixing

 

The Blender has received material that it mixed together.

 

### Blocked

 

The Blender has received material, but it cannot send it downstream.

 

 

 

## Properties

 

The fluid blender object has seven tabs with various properties. The last five tabs are the standard tabs that are common to most fluid objects. For more information about the properties on those tabs, see:

 

- The Triggers Tab
- The Fluid Level Display Tab
- The Labels Tab
- The General Tab

 

The Statistics tab is obsolete and is not supported for this object any more. Use the Statistics group in the object's Quick Properties instead.

 

The remaining two tabs, the Blender tab and the Recipe tab are unique to the fluid blender object. The properties on these tabs will be explained in more detail in the next two sections.

 

 

 

## The Fluid Blender Tab

 

The Fluid Blender tab has the following properties:

 

 

### Maximum Content

 

The maximum amount of fluid material that this object can hold at any time.

 

### Target Product ID

 

The ProductID that will be assigned to the material that leaves this object.

  

#### Adjust Output Rates

 

This function is called every tick and allows the user to change the output rates and scale factors during a model run. There are access variables that are references to the nodes that hold the rates and scale factors. To read the current rate (object or port) the getnodenum() command should be used. To change the rate the setnodenum() should be used. To read or change the scale factors getnodenum() and setnodenum() should be used in conjunction with the rank() command.

  

### Input Ports

 

These properties affect how the object receives material from upstream objects.

 

### Maximum Input Rate

 

The maximum rate that material will be allowed into this object through all input ports combined. The actual input rate is based on the amount of material available upstream and the space available in this object.

 

### Output Ports

 

These properties affect how the object sends material to downstream objects.

 

### Maximum Object Rate

 

The maximum rate that material will leave this object through all output ports combined.

 

### Maximum Port Rate

 

The maximum rate that material will leave this object through any one port.

  

#### Outside Port Scale Factor

 

Each row of the table is the scale factor for a single output port. The value is multiplied by the maximum port rate to determine the actual maximum rate for that specific port. This allows the modeler to change the rate of individual output ports during a model run.

  

 

 

## The Recipe Tab

 

The Recipe tab has the following properties:

 

 

### Blender Recipe

 

Each row of this table represents a single input port. The rows do not appear in the table unless the object is already connected to upstream objects when the Properties GUI is opened. There are two columns that the modeler can change in the table:

 

- **Ingredient** - This is a text description of the ingredient coming from the port the row represents. This is for the modeler's use only, the Blender will ignore this value.
- **Percent** - This is a number between 0 and 100 that is the percentage of the total incoming material that should come from the port represented by the row. The Blender will adjust the actual amount of material pulled from each port to make sure these percentages are correct, even when there is not enough material or space available to pull at the maximum rate.

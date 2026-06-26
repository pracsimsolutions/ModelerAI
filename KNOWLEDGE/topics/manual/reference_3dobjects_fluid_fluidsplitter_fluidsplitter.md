---
id: reference_3dobjects_fluid_fluidsplitter_fluidsplitter
name: "Fluid Splitter"
kind: manual
breadcrumb: ["Reference","3DObjects","Fluid","FluidSplitter","FluidSplitter"]
tags: ["manual","reference","3dobjects","fluid"]
source: "manual/Reference/3DObjects/Fluid/FluidSplitter/FluidSplitter.html"
---

# Fluid Splitter

## Overview and Key Concepts

 

The Splitter is used to send material to multiple output ports in percentages that the modeller specifies. These percentages are specified in a table called the Splitter Percents Table. Each row in the table corresponds to an output port. There are columns that allow the modeller to enter a description of each port (for the modeller's use only) and the percentage (from 0-100) of the outgoing material that will go to each port. When the Splitter sends material out, it will always send in the percentages specified. If the Splitter can not send all the material that it is trying to send, it will reduce the amount that it sends to the other ports to keep the percentages equal at all times. The modeller has complete control over the input rates and scale factors using the AdjustInputPorts function. They also have control over the total output rate. The port output rate and the scale factors will be adjusted by the object itself as needed. It should be noted that the FluidSplitter may not adjust the amount of material to go out correctly if the downstream objects are full, or nearly full. Due to the timing of the calculations, the Splitter may decide there is no room for material it is trying to send even if the downstream object will release enough material to make room for the Splitter's material.

 

 

The FluidBlender receives material in a normal manner, but sends it out differently. It sends material out in percentages that the user specifies in a table called the Splitter Percents. The table is not visible in the Properties window until the Splitter is connected to downstream objects. Each row of the table corresponds to a single output port, and has two columns: a description and the percentage. The description is a text string that describes the material being sent to that port, it is not used by the object and is for the modeller's benefit only. The percentage is a number between 0 and 100 that indicates what percentage of the outgoing material should be sent to that port.

 

Every tick, the Splitter calculates the amount of material that should be sent to each of the downstream objects, based on rates and capacities. If there is not enough room in one of the downstream objects to receive the amount the Splitter has calculated, the amount sent to each port is reduced to keep the percentages correct.

 

Because the Splitter controls how much it sends to each port at any point in time, the modeller does not have access to the maximum port rate or the output port scale factors. They can, however, edit the object's maximum output rate. The user has complete control over the input rates and scale factors. They can change these values with the AdjustInputRates function, which fires every tick. This allows them to update the input rates and scale factors as the model is running.

 

 

 

## States

 

For statistical purposes, the fluid splitter will be in one of the following states at various points during a simulation run:

 

### Empty

 

The Splitter has nothing in it.

 

### Not Empty

 

The Splitter has material in it that can be sent out.

 

 

 

## Properties

 

The fluid splitter object has seven tabs with various properties. The last five tabs are the standard tabs that are common to most fluid objects. For more information about the properties on those tabs, see:

 

- The Triggers Tab
- The Fluid Level Display Tab
- The Labels Tab
- The General Tab

 

The Statistics tab is obsolete and is not supported for this object any more. Use the Statistics group in the object's Quick Properties instead.

 

The remaining two tabs, the Splitter tab and the Percents tab are unique to the fluid splitter object. The properties on these tabs will be explained in more detail in the next two sections.

 

 

 

## The Splitter Tab

 

The Splitter tab has the following properties:

 

 

### Maximum Content

 

The maximum amount of fluid material that this object can hold at any time.

 

### Adjust Input Rates

 

This function is called every tick and allows the user to change the input rates and scale factors during a model run. There are access variables that are references to the nodes that hold the rates and scale factors. To read the current rate (object or port) the getnodenum() command should be used. To change the rate the setnodenum() should be used. To read or change the scale factors getnodenum() and setnodenum() should be used in conjunction with the rank() command.

 

### Input Ports

 

These properties affect how the object receives material from upstream objects:

 

- **Maximum Object Rate** - The maximum rate that material will enter this object through all output ports combined.
- **Maximum Port Rate** - The maximum rate that material will enter this object through any one port.
- **Input port scale factor** - Each row of the table is the scale factor for a single input port. The value is multiplied by the maximum port rate to determine the actual maximum rate for that specific port. This allows the modeler to change the rate of individual input ports during a model run.

 

### Output Ports

 

These properties affect how the object sends material to downstream objects:

 

- **Maximum Output Rate** - The maximum rate that material will leave this object through all output ports combined.

 

 

 

## The Percents Tab

 

The Percents tab has the following properties:

 

 

### Splitter Percents

 

Each row of this table represents a single output port. The rows do not appear in the table unless the object is already connected to downstream objects when this window is opened. There are two columns that the modeler can change in the table:

 

- **Ingredient** - This is a text description of the material going to the port the row represents. This is for the modeler's use only, the Splitter will ignore this value.
- **Percent** - This is a number between 0 and 100 that is the percentage of the total outgoing material that should go to the port represented by the row. The Splitter will adjust the actual amount of material sent to each port to make sure these percentages are correct, even when there is not enough material or space available to send at the maximum rate.

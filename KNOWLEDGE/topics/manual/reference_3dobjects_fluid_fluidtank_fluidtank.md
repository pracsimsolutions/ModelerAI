---
id: reference_3dobjects_fluid_fluidtank_fluidtank
name: "Fluid Tank"
kind: manual
breadcrumb: ["Reference","3DObjects","Fluid","FluidTank","FluidTank"]
tags: ["manual","reference","3dobjects","fluid"]
source: "manual/Reference/3DObjects/Fluid/FluidTank/FluidTank.html"
---

# Fluid Tank

## Overview and Key Concepts

 

The FluidTank is a simple Fluid Object that can receive and send material at the same time. The modeller decides the maximum capacity of the Tank and up to three points (called "marks") that will cause triggers to fire when the content in the Tank reaches them.

 

 

The FluidTank is the most generic of the Fluid Objects. It can receive and send material at the same time. The modeller has complete access to the variables that control the input and output rates. They are also given two functions that fire at the end of every tick. These functions are called "AdjustOutputRates" and "AdjustInputRates". They are used to changed the values of the input or output rates during a model run.

 

The Tank can start a model with no content, or with a set amount. If the tank begins with content, that is all it will create during a run. It may continue to receive material from upstream, however. Starting with a fixed content value is very useful for models that have a fixed amount of material that will enter. If the modeller wants a constant or infinite stream of incoming material, they should use a FluidGenerator instead.

 

The Tank has a maximum capacity that the modeller defines. The content of the Tank will never go above this value. If, at the end of a tick, the Ticker calculates that the Tank should receive more than it can currently hold, only the material required to fill the Tank will be transferred.

 

The modeller can define three points that will cause triggers to fire when the content of the Tank reaches them. These points are called "Marks". Whenever the content of a Tank passes one of these marks (either rising or falling), a trigger is fired. The user can use that trigger to open or close ports, send messages, change rates, or many other things. If two or more marks are set for the same value, the trigger for only one of them will fire. For example, if the low mark and the mid mark are both set to 10, when the content in the Tank changes from 9 to 10 only the trigger for the low mark will fire.

 

 

 

## States

 

For statistical purposes, the fluid tank will be in one of the following states at various points during a simulation run:

 

### Empty

 

The Tank has no material in it.

 

### Not Empty

 

The Tank has some material in it.

 

### Full

 

The Tank's maximum capacity has been reached, it will not receive any material unless it can send some out.

 

 

 

## Properties

 

The fluid tank object has seven tabs with various properties. The last five tabs are the standard tabs that are common to most fluid objects. For more information about the properties on those tabs, see:

 

- The Triggers Tab
- The Fluid Level Display Tab
- The Labels Tab
- The General Tab

 

The Statistics tab is obsolete and is not supported for this object any more. Use the Statistics group in the object's Quick Properties instead.

 

The remaining two tabs, the Tank tab and the Marks tab are unique to the fluid tank object. The properties on these tabs will be explained in more detail in the next two sections.

 

 

 

## The Tank Tab

 

The Tank tab has the following properties:

 

 

### Maximum Content

 

The maximum amount of fluid material that this object can hold at any time.

 

### Initial Content

 

The amount of material that is in the object when the model is reset.

 

### Initial Product

 

This opens the Initial Product Window which that allows the modeler to define the Product ID and sub-component mix of the material that is in this object.

 

### Adjust Input Rates

 

This function is called every tick and allows the user to change the input rates and scale factors during a model run. There are access variables that are references to the nodes that hold the rates and scale factors. To read the current rate (object or port) the getnodenum() command should be used. To change the rate the setnodenum() should be used. To read or change the scale factors getnodenum() and setnodenum() should be used in conjunction with the rank() command.

 

### Adjust Output Rates

 

This function is called every tick and allows the user to change the output rates and scale factors during a model run. There are access variables that are references to the nodes that hold the rates and scale factors. To read the current rate (object or port) the getnodenum() command should be used. To change the rate the setnodenum() should be used. To read or change the scale factors getnodenum() and setnodenum() should be used in conjunction with the rank() command.

 

### Input Ports

 

These properties affect how the object receives material from upstream objects:

 

- **Maximum Object Rate** - The maximum rate that material will enter this object through all output ports combined.
- **Maximum Port Rate** - The maximum rate that material will enter this object through any one port.
- **Input port scale factor** - Each row of the table is the scale factor for a single input port. The value is multiplied by the maximum port rate to determine the actual maximum rate for that specific port. This allows the modeler to change the rate of individual input ports during a model run.

 

### Output Ports

 

These properties affect how the object sends material to downstream objects.

  **Maximum Object Rate** - The maximum rate that material will leave this object through all output ports combined. **Maximum Port Rate** - The maximum rate that material will leave this object through any one port. **Output port scale factor** - Each row of the table is the scale factor for a single output port. The value is multiplied by the maximum port rate to determine the actual maximum rate for that specific port. This allows the modeler to change the rate of individual output ports during a model run. 

 

 

## The Marks Tab

 

The Marks tab has the following properties:

 

 

### Low Mark

 

If the content passes this value (while rising or falling), the PassingLowMark Trigger will fire.

 

### Mid Mark

 

If the content passes this value (while rising or falling), the PassingMidMark Trigger will fire.

 

### High Mark

 

If the content passes this value (while rising or falling), the PassingHighMark Trigger will fire.

 

### Passing Low Mark

 

If the content passes the Low Mark, this trigger fires. Its common uses include opening and closing ports or sending messages. There is an access variable that informs the modeler if the fluid level is rising through the mark or falling.

 

### Passing Mid Mark

 

If the content passes the Mid Mark, this trigger fires. Its common uses include opening and closing ports or sending messages. There is an access variable that informs the modeler if the fluid level is rising through the mark or falling.

 

### Passing High Mark

 

If the content passes the High Mark, this trigger fires. Its common uses include opening and closing ports or sending messages. There is an access variable that informs the modeler if the fluid level is rising through the mark or falling.

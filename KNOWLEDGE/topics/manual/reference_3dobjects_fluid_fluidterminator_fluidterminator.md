---
id: reference_3dobjects_fluid_fluidterminator_fluidterminator
name: "Fluid Terminator"
kind: manual
breadcrumb: ["Reference","3DObjects","Fluid","FluidTerminator","FluidTerminator"]
tags: ["manual","reference","3dobjects","fluid"]
source: "manual/Reference/3DObjects/Fluid/FluidTerminator/FluidTerminator.html"
---

# Fluid Terminator

## Overview and Key Concepts

 

The FluidTerminator is used to destroy fluid material once the model is done with it.

 

 

The FluidTerminator is the object that modellers use when they want to remove fluid material from the model without turning it into flow items. The Terminator keeps track of how much of each different type of material it receives, with a few restrictions. It can only keep track of up to 14 different Product ID's. These should integers that are 1 or greater. The data it collects for each product id is displayed in the Class Statistics section of the Properties GUI's statistics tab.

 

The user has complete control of the input rates of the Terminator. This includes a function called "AdjustInputRates" that fires every tick. This function is used to change the input rates and scale factors during a model run. The modeller is not given control over the output rate because the material the Terminator receives is destroyed and cannot be sent downstream.

 

 

 

## States

 

For statistical purposes, the fluid terminator will be in one of the following states at various points during a simulation run:

 

### Collecting

 

The Terminator is never full and is always able to receive material, so it is always in a collecting state.

 

 

 

## Properties

 

The fluid terminator object has six tabs with various properties. The last five tabs are the standard tabs that are common to most fluid objects. For more information about the properties on those tabs, see:

 

- The Triggers Tab
- The Fluid Level Display Tab
- The Labels Tab
- The General Tab

 

The Statistics tab is obsolete and is not supported for this object any more. Use the Statistics group in the object's Quick Properties instead.

 

Only the Terminator tab is unique to the fluid terminator object. The properties on this tab will be explained in more detail in the following sections.

 

 

### Adjust Input Rates

 

This function is called every tick and allows the user to change the input rates and scale factors during a model run. There are access variables that are references to the nodes that hold the rates and scale factors. To read the current rate (object or port) the getnodenum() command should be used. To change the rate the setnodenum() should be used. To read or change the scale factors getnodenum() and setnodenum() should be used in conjunction with the rank() command.

 

### Input Ports

 

These properties affect how the object receives material from upstream objects.

 

### Maximum Object Rate

 

The maximum rate that material will enter this object through all output ports combined.

 

### Maximum Port Rate

 

The maximum rate that material will enter this object through any one port.

 

### Input port scale factor

 

Each row of the table is the scale factor for a single input port. The value is multiplied by the maximum port rate to determine the actual maximum rate for that specific port. This allows the modeler to change the rate of individual input ports during a model run.

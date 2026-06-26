---
id: reference_3dobjects_fluid_fluidgenerator_fluidgenerator
name: "Fluid Generator"
kind: manual
breadcrumb: ["Reference","3DObjects","Fluid","FluidGenerator","FluidGenerator"]
tags: ["manual","reference","3dobjects","fluid"]
source: "manual/Reference/3DObjects/Fluid/FluidGenerator/FluidGenerator.html"
---

# Fluid Generator

## Overview and Key Concepts

 

The FluidGenerator provides an infinite supply of fluid material for a model. The Generator can be set to refill at a fixed rate (that can be faster or slower than the outgoing rate) or it can refill itself a set amount of time after it becomes empty.

 

 

The FluidGenerator is used to create fluid material for a model. The modeller defines the capacity of the Generator, as well as the amount of material in it when the model is reset. They can also define the ProductID and sub-component mix for the initial product.

 

The Generator creates material in two ways. The first is at a constant rate that the modeller defines. This rate can be faster or slower than the output rate of the object. If it is faster, then the Generator will always be full. If it is slower, then the Generator will eventually become empty. Even if the Generator becomes empty, it will make more material during the next tick, but the downstream objects may not be able to receive it at the full rate.

 

The second way to create material is to fill the Generator to its maximum amount instantly, but only after a certain amount of time has elapsed since the Generator became empty. The modeller can define how long this wait time is. This is used to simulate situations where material is available on a regular basis, but is not available all of the time. For example, trucks full of raw materials that arrive once a day could be simulated with this technique.

 

The modeller is given control over all of the rate variables that affect the Generator's output rate, as well as a function called "AdjustOutputRates". This function fires every tick and allows the modeller to change the output rates during a model run. The modeller is not given control over the variables controlling the input rate, as no material ever comes into a Generator.

 

 

 

## States

 

For statistical purposes, the fluid generator will be in one of the following states at various points during a simulation run:

 

### Empty

 

The Generator has no material in it.

 

### Not Empty

 

The Generator has some material in it.

 

### Full

 

The Generator's maximum capacity has been reached.

 

 

 

## Properties

 

The fluid generator object has six tabs with various properties. The last five tabs are the standard tabs that are common to most fluid objects. For more information about the properties on those tabs, see:

 

- The Triggers Tab
- The Fluid Level Display Tab
- The Labels Tab
- The General Tab

 

The Statistics tab is obsolete and is not supported for this object any more. Use the Statistics group in the object's Quick Properties instead.

 

Only the Generator tab is unique to the fluid generator object. The properties on this tab will be explained in more detail in the following sections.

 

 

### Maximum Content

 

The maximum amount of fluid material that this object can hold at any time.

 

### Initial Content

 

The amount of material that is in the object when the model is reset.

 

### Initial Product

 

This opens the Initial Product Window which allows the modeler to define the Product ID and sub-component mix of the material that is in this object.

 

### Adjust Output Rates

 

This function is called every tick and allows the user to change the output rates and scale factors during a model run. There are access variables that are references to the nodes that hold the rates and scale factors. To read the current rate (object or port) the getnodenum() command should be used. To change the rate the setnodenum() should be used. To read or change the scale factors getnodenum() and setnodenum() should be used in conjunction with the rank() command.

 

### Generator Refill

 

These properties affect how the object refills itself as the model runs.

 

### Refill Mode

 

This selects the type of refill the Generator performs. It can refill itself continuously (at a specified rate) or it can refill itself completely after it becomes empty.

 

### Refill Rate

 

The rate at which the Generator refills itself. This is available if Continuous Refill is selected in the Refill Mode drop-down list.

 

### Delay Time

 

The time that the Generator waits after becoming empty before it completely refills itself. This is available if Refill When Empty is selected in the Refill Mode drop-down list.

 

### Output Ports

 

These properties affect how the object sends material to downstream objects.

 

### Maximum Object Rate

 

The maximum rate that material will leave this object through all output ports combined.

 

### Maximum Port Rate

 

The maximum rate that material will leave this object through any one port.

 

### Output port scale factor

 

Each row of the table is the scale factor for a single output port. The value is multiplied by the maximum port rate to determine the actual maximum rate for that specific port. This allows the modeler to change the rate of individual output ports during a model run.

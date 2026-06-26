---
id: reference_3dobjects_fluid_fluidprocessor_fluidprocessor
name: "Fluid Processor"
kind: manual
breadcrumb: ["Reference","3DObjects","Fluid","FluidProcessor","FluidProcessor"]
tags: ["manual","reference","3dobjects","fluid"]
source: "manual/Reference/3DObjects/Fluid/FluidProcessor/FluidProcessor.html"
---

# Fluid Processor

## Overview and Key Concepts

 

The FluidProcessor is used to simulate a processing step that continuously receives and sends fluid material (such as a continuous cooker).

 

 

The FluidProcessor receives and sends material based on an over-all rate that the modeller specifies. The modeller specifies the maximum output rate of the Processor. This value is used to determine the input rate as well. The actual output rate is based on the rate that material came in. Material will leave at the same rate that it entered, unless the output is, for some reason, lowered (such as the downstream object closing its input ports or breaking down). If this happens, the material will "back up" in the Processor and more will be available to send when the downstream object can receive more again. Once the downstream object has received all of the "backed up" material, the output rate will go back to the input rate. The amount of time that material spends in the Processor is based on the Processor's maximum output rate and its maximum capacity.

 

The modeller is also able to specify a loss value. This value is a number from 0-1 that represents the percentage of incoming material that is lost due to machine inefficiency, evaporation, or any other reason. Whenever any material enters the Processor, it is immediately reduced by this percentage.

 

Unlike other objects, the Processor can only receive from one port and send to one port during each tick. The Processor has functions called "Receive Port Number" and "Destination Port Number" that the modeller uses to decide which ports will be used. If these functions returns 0, the first input or output port that has material or has available space will be used. If another value is returned it is the number of the input or output port that will be used. This is the more common use of these two fields.

 

 

 

## States

 

For statistical purposes, the fluid processor will be in one of the following states at various points during a simulation run:

 

### Empty

 

The Processor has no material in it.

 

### Processing

 

The Processor has material in it that it is trying to send downstream, or that has not been in the Processor long enough to be sent.

 

### Blocked

 

The Processor has material in it that it is unable to send downstream.

 

 

 

## Properties

 

The fluid processor object has six tabs with various properties. The last five tabs are the standard tabs that are common to most fluid objects. For more information about the properties on those tabs, see:

 

- The Triggers Tab
- The Fluid Level Display Tab
- The Labels Tab
- The General Tab

 

The Statistics tab is obsolete and is not supported for this object any more. Use the Statistics group in the object's Quick Properties instead.

 

Only the FluidProcessor tab is unique to the fluid processor object. The properties on this tab will be explained in more detail in the following sections.

 

 

### Maximum Content

 

The maximum amount of material that this object can hold.

 

### Loss Amount

 

A value between 0 and 1 that represents the percentage of material that is lost going through the Processor. This loss could be due to evaporation, inefficiency or many other factors. A value of 0 means that there is no material lost, a value of 1 means that all material is lost. This loss is applied as soon as material is pulled into the Processor.

 

### Maximum Output Rate

 

The maximum rate that material will leave this object through all of the output ports combined. The actual rate will be determined by the rate of material coming into the Processor.

 

### Receive Port

 

If this field returns a 0, the Processor will receive material from all input ports. If it returns a number greater than zero, the Processor will only receive material from that input port.

 

### Destination Port

 

If this field returns a 0, the Processor will allow material to leave from all of its output ports. If it returns a number greater than zero, the Processor will only allow material out that output port.

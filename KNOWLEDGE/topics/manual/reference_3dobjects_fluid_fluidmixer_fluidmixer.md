---
id: reference_3dobjects_fluid_fluidmixer_fluidmixer
name: "Fluid Mixer"
kind: manual
breadcrumb: ["Reference","3DObjects","Fluid","FluidMixer","FluidMixer"]
tags: ["manual","reference","3dobjects","fluid"]
source: "manual/Reference/3DObjects/Fluid/FluidMixer/FluidMixer.html"
---

# Fluid Mixer

## Overview and Key Concepts

 

The FluidMixer is used to combine products together into a single, new product. The different materials can either be pulled sequentially or in parallel. The Mixer always works in batches. It does not send any material until it has received and processed all the material that it was set to receive.

 

 

The FluidMixer pulls material from one or more input ports and mixes it together. The modeller defines the ProductID of the material that is released by the Mixer. The sub-components of the mix will be a mixture of the sub-components of the material that was pulled in. The sub-component percentages are based on how much of each incoming product is mixed together.

 

The modeller defines a series of steps that the Mixer will go through. These steps are defined in the Step Table. Each step can pull material from zero or more input ports at the same time. A delay time can also be defined that begins after all the material for the step has been collected. The next step will not begin until after the delay time is over. In addition, the modeller is given a trigger that fires before the delay at a step a trigger that fires after the delay (but before the next step starts). These triggers can be used for things like calling an operator to perform work during the delay. The modeller can assign a text description to each of the steps in the table. This description is displayed near the object's name in the model view window. It does not affect the behavior of the object.

 

The modeller defines how much material comes in by using the Mixer's Recipe Table. Each row of the table represents material coming from a single input port during a single step. Each row has four columns: the ingredient name, the port number, the step number and the amount. The name is a string that describes the material being pulled in by that row. It is for the modeller's benefit only. The Mixer ignores the value. The port number is the input port that the material will be pulled from. The step number is the step in the Step Table the Mixer must be in to pull this material. Once the Mixer has pulled the correct amount for a given row, it will not pull any more material for that row, even if the other rows in the same step are not complete yet. The amount is the actual amount of material that will be pulled from the specified port during the specified step.

 

Different materials will be pulled in parallel if they have the same step number. They will be pulled in series if they have different step numbers. It is possible (and often very useful) to have a recipe that calls for some ingredients to be pulled in parallel and others to be pulled in series. There is no limit on the number of steps or ingredients that can be defined. There is also no limit on the number of ingredients that can be pulled during any single step. If the modeller wishes to have material pulled from the same input port during multiple steps, they have to define multiple rows in the Recipe Table.

 

Because the Mixer controls which ports it will pull from at any point in time, the modeller does not have access to the input port scale factors. They can, however, edit the object's maximum input rate and maximum port rate. It is very important that they make sure that the maximum object rate is high enough to allow input from multiple ports if their recipe requires that. Once the delay after the final step is complete, the user has control over the output rates and scale factors. They can change these values with the AdjustOutputRates function. This function is not called until the Mixer has finished collecting everything. Once the Mixer is finished collecting and processing a batch, the function is called during every tick. It is not called while the Mixer is still working through the Step Table.

 

The Mixer provides a visual display of the material that has been received at any point in the process. The level indicator bar will not show the Mixer's color until the delay time of the last step is complete. Before that time, the level indicator is a series of layers of different colors. There is one layer for each ingredient in the Recipe. The colors of the layers are the colors of the upstream objects that the Mixer is receiving material from. The size of each layer is the percentage of the total batch that has been pulled for that ingredient. This multi-color bar is a good indicator of what is happening in a Mixer at any given time.

 

 

 

## States

 

For statistical purposes, the fluid blender will be in one of the following states at various points during a simulation run:

 

### Empty

 

The Mixer has nothing in it and is waiting to start step 1.

 

### Filling

 

The Mixer is receiving material for its current step.

 

### Starved

 

The Mixer has not completed its Step Table, but there is no material coming into it.

 

### Releasing

 

The Mixer has completed the Step Table and is sending the finished product downstream.

 

### Blocked

 

The Mixer has completed the Step Table, but is unable to send material downstream.

 

 

 

## Properties

 

The fluid mixer object has seven tabs with various properties. The last five tabs are the standard tabs that are common to most fluid objects. For more information about the properties on those tabs, see:

 

- The Triggers Tab
- The Fluid Level Display Tab
- The Labels Tab
- The General Tab

 

The Statistics tab is obsolete and is not supported for this object any more. Use the Statistics group in the object's Quick Properties instead.

 

The remaining two tabs, the Mixer tab and the Steps tab are unique to the fluid mixer object. The properties on these tabs will be explained in more detail in the next two sections.

 

 

 

## The Mixer Tab

 

The Mixer tab has the following properties:

 

 

### Target Product ID

 

The ProductID that will be assigned to the material that leaves this object.

 

### Adjust Output Rates

 

This function is called every tick and allows the user to change the output rates and scale factors during a model run. There are access variables that are references to the nodes that hold the rates and scale factors. To read the current rate (object or port) the getnodenum() command should be used. To change the rate the setnodenum() should be used. To read or change the scale factors getnodenum() and setnodenum() should be used in conjunction with the rank() command.

 

### Input Ports

 

These properties affect how the object receives material from upstream objects.

 

- **Maximum Object Rate** - The maximum rate that material will be allowed into this object through all input ports combined. The actual input rate is based on the amount of material available upstream and the space available in this object.
- **Maximum Port Rate** - The maximum rate that material will be allowed into this object through any single input port.

 

### Output Ports

 

These properties affect how the object sends material to downstream objects.

 

- **Maximum Object Rate** - The maximum rate that material will leave this object through all output ports combined.
- **Maximum Port Rate** - The maximum rate that material will leave this object through any one port.

 

### Output port scale factor

 

Each row of the table is the scale factor for a single output port. The value is multiplied by the maximum port rate to determine the actual maximum rate for that specific port. This allows the modeler to change the rate of individual output ports during a model run.

 

 

 

## The Steps Tab

 

The Steps tab has the following properties:

 

 

### Before Step Delay Trigger

 

This trigger fires after all of the material for a step has been collected, but before the step's delay time begins. This gives the modeler a chance to do things like call an operator for the delay.

 

### After Step Delay Trigger

 

This trigger fires after the delay for a step is complete. It gives the modeler a chance release an operator or send messages to other objects.

 

### Mixer Steps

 

This group has the following properties:

 

- **Number of Steps** - This is the number of steps that the Mixer will go through for every batch of material that it makes.
- **Update** - Pressing this button updates the Step Table so that it has the number of rows specified by the modeler.
- **Steps Table** - The Step Table shows all of the steps that the Mixer must go through for each batch. Each step has two columns that the modeler must fill out:
- **Description** - This is a text description of the step. It is displayed by the Mixer's name in the model view window when the Mixer is on the step.
- **Delay** - This is the amount of time that the Mixer must wait after collected all of the ingredients for the step before it can go on to the next step.

   

### Mixer Recipe

 

This group has the following properties:

 

- **Number of Ingredients** - This is the number of ingredients that the Mixer will pull as it goes through its Step Table.
- **Update** - Pressing this button updates the Ingredients Table so that it has the number of rows specified by the modeler.
- **Ingredients Table** - The Ingredients Table show all of the ingredients that the Mixer pulls as it goes through its Step Table. If a single ingredient needs to be pulled in more than one step, it should appear in more than one row in the table. The table has four columns that the modeler must fill out:
- **Ingredient** - This is a text description of the ingredient that the row represents. It is only to help the modeler document their model. It does not affect the Mixer's behavior.
- **Port** - This is the input port that the ingredient will be pulled from.
- **Amount** - This is the amount of the ingredient that will be pulled.
- **Step** - This is the step number that the Mixer must be in for this ingredient to be pulled.

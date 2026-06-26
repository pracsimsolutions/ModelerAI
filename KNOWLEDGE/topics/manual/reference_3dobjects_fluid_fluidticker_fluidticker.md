---
id: reference_3dobjects_fluid_fluidticker_fluidticker
name: "Fluid Ticker"
kind: manual
breadcrumb: ["Reference","3DObjects","Fluid","FluidTicker","FluidTicker"]
tags: ["manual","reference","3dobjects","fluid"]
source: "manual/Reference/3DObjects/Fluid/FluidTicker/FluidTicker.html"
---

# Fluid Ticker

## Overview and Key Concepts

 

The Ticker is responsible for breaking time into small, evenly spaced units called *ticks*. The modeller can define the length of a tick. The Ticker is the object that controls all of the Fluid Objects in a model. For this reason, in any model that uses Fluid Objects, there should always be exactly one Ticker. The modeller also uses the Ticker to define the global list of sub-components that make up the fluid material in the model.

 

 

Any model that uses the Fluid Objects must have a Ticker. The Ticker must be named *TheTicker*. The modeller can create a Ticker by dragging it from the library icon grid to the model view window. However, this is an easy step to forget, so when the modeller drags any Fluid Object into a model, a Ticker is created if there is not one already in the model. If the modeller tries to create another Ticker, a warning message is given saying that a Ticker already exists and that the new Ticker should be deleted.

 

The Ticker does not do much that the modeller can see, but it is very important behind-the-scenes. It is responsible for calculating how much material is transferred between Fluid Objects at the end of every tick. When the model is reset, the Ticker builds a list of all of the Fluid Objects in the model. This objects in this list are sorted based on how many fluid objects are upstream and downstream of them. At the end of each tick, the Ticker begins with the fluid object farthest downstream and calculates how much material it received during that tick. Then the Ticker calculates how much material moved into the object upstream of that starting object, and so on until it reaches the starting point of the fluid portion of the model.

 

The Ticker only has a few values that the modeller can change. The most important of these is the tick time. This is the length of time that the Ticker will wait between updates to the Fluid Objects. A very small tick time will result in a large number of events firing as the model runs (at least one per tick) and may slow down the model. It will also, in many cases, increase the accuracy of the Fluid Objects' behavior. A longer tick time will generally result in a faster running model, but at a cost in accuracy. It is up to the modeller to find the appropriate balance of speed and accuracy for their model.

 

The modeller can also use the Ticker to define a set of sub-components that will make up the fluid material in the model. Each of the sub-components should be given a name. These names are to help the modeller understand what is happening in the model, they have no affect on the objects' behavior or accuracy. All fluid material has the same list of sub-components. It is not required that any given fluid material use all the sub-components, however. The Fluid Objects keep track of the percentages of the sub-components that make up the material that they are currently processing. If fluid material from two different sources is mixed together, the sub-components' percentages are adjusted accordingly. There is no limit to the number of sub-components that can be defined. Just remember that the list applies to all fluids.

 

 

 

## States

 

For statistical purposes, the fluid ticker will be in the following state at various points during a simulation run:

 

### Idle

 

The Ticker is always in an idle state.

 

 

 

## Properties

 

The fluid ticker object has four tabs with various properties. The last three tabs are the standard tabs that are common to most fluid objects. For more information about the properties on those tabs, see:

 

- The Triggers Tab
- The Labels Tab
- The General Tab

 

Only the Ticker tab is unique to the fluid ticker object. The properties on this tab will be explained in more detail in the following sections.

 

 

### Tick Time

 

This is length of time in each tick. At the end of a tick, the Ticker calculates how much fluid moved between the fluid objects in the model.

 

### Optimize object list resorting

 

The Ticker keeps an internal list of the order that the Fluid Objects should be evaluated. if this box is not checked, the order in which certain objects are evaluated may be different in different runs of the model. This can cause a model to give different results, even if nothing in the model has actually changed. Typically, this box should be checked.

 

### Product Components

 

This is the number of sub-components available to all of the fluid objects in the model. All of the objects use the same list of sub-components, although they do not have to specify a value greater than 0 for all of the components.

 

### Update

 

Pressing this button updates the list of component names so that there are the number that the modeler specified.

 

### Component Names

 

This table lists the names of the sub-components that are available to all of the fluid objects in the model.

 

### Controlled Fluid Objects

 

This table lists the names of the fluid objects that are controlled by this Ticker.

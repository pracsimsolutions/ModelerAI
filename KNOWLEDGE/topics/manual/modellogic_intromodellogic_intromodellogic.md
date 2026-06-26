---
id: modellogic_intromodellogic_intromodellogic
name: "Introduction to Model Logic"
kind: manual
breadcrumb: ["ModelLogic","IntroModelLogic","IntroModelLogic"]
tags: ["manual","modellogic","intromodellogic","intromodellogic"]
source: "manual/ModelLogic/IntroModelLogic/IntroModelLogic.html"
---

# Introduction to Model Logic

## What is Model Logic?

 

In this chapter, you'll learn how to build your simulation's model logic. In FlexSim, the term *logic* refers to the overall behavior and interaction of the 3D objects and other simulation elements while the simulation model runs. In other words, model logic is what controls the series of events that will occur from the time a simulation model begins running until the simulation ends.

 

As you can imagine, building your simulation model's logic is occasionally challenging, depending on the complexity of the business system you're trying to model. The rest of this chapter is designed to make logic building as easy as possible. This chapter will begin by explaining some of the key concepts that you'll need in order to understand how model logic works in FlexSim in general. Then this chapter will explain how to work with the various tools that are involved in building a model's logic.

 

Be aware that more topics are planned for this chapter in future releases of the User Manual and that you should consider checking back in from time to time for more helpful topics.

 

 

 

## Tools Used in Creating Model Logic

 

Nearly all of the FlexSim tools are involved in building the model's logic in some way. But some tools are more central to this process than others. This section will discuss some of the major ones.

 

### Model Parameters

 

A model parameter is an input variable for a model. Model parameters usually control some aspect of a model, such as: 

- Number of workers
- Customer arrival rate
- Rework likelihood
- The sequence of jobs to do

 These are just a few examples. Your model can have as many parameters as you require, and you can usually configure a parameter to control any aspect of the model. For more information about how to use model parameters, see the Model Parameters topic.

 

### Object Properties

 

You're likely aware by now that editing the properties of 3D objects gives you control over their behavior and visual appearance.

 

 

To edit an object's properties, you'll primarily use its Properties, which is open, by default on the right pane.

 

### The Process Flow Tool

 

The Process Flow tool is the main tool that you'll use to build the logic for your simulation model. The Process Flow tool allows you to build a flowchart of your model's logic. Inside that flowchart, you'll add *activities* that contain small chunks of pre-programmed logic. When a simulation model runs, small green circles called *tokens* move through the process flow activities, executing each activity's logic.

 

You can edit the activity's properties to link these activities to the objects in the 3D model or you can create a self-contained simulation purely inside the Process Flow tool itself. For example, you can link a token to a flow item in the simulation model if needed.

 

 

There are several advantages to using the Process Flow tool to control your model's logic:

 

- **User-friendliness** - One of the main advantages of FlexSim is the ability to create custom objects that are capable of simulating the underlying logic of complex business processes. Up until the Process Flow tool was introduced, most of this customization required you to write code in FlexScript (FlexSim's internal scripting language). While many advanced users appreciate being able to tinker directly with the code, some users might prefer a simple, user-friendly interface that writes the code behind the scenes for them. That's what the Process Flow tool is designed to do. For that reason, the Process Flow tool is significantly more intuitive, especially for non-programmers.
- **Greater flexibility** - The Process Flow tool can flexibly adapt to many different kinds of simulation projects and different approaches to the task of simulation. Rather than asking you to learn one particular approach toward simulation and then adapt your simulation model to this style, the Process Flow tool will adapt to you and your approach to simulation. Because there are many possible ways to solve the same problem in the Process Flow tool, you can build the logic of your simulation in a way that makes sense to you. This flexibility makes it possible for you to address the challenges that are unique to your simulation project.
- **Working with stakeholders** - The Process Flow tool is ideal for presenting simulation models to decision makers. Stakeholders are more likely to catch your vision when you can present the deeper logic of your business system using easy-to-understand, visually-engaging flowcharts. You can also move and connect activities so that they are spatially organized and grouped in whatever way best communicates their meaning.
- **Easier de-bugging** - Besides simply making compelling presentations, these engaging visuals also makes it easy to debug your simulation since the Process Flow tool helps you to see the deeper logic of your business system right before your eyes.

 

### FlexScript

 

FlexScript is FlexSim's internal scripting language. While the Process Flow tool can handle nearly any kind of simulation logic, more advanced users will find that it's helpful to learn how to code model logic using FlexScript instead. Learning FlexScript can potentially help you take your simulation project to the next level or help you understand better what is happening under the surface of your simulation model. This chapter will eventually include an introduction to the basics of coding in FlexScript, but for now, you can start by referring to the Coding in FlexSim topics found in the Reference section of this manual.

 

 

 

## Best Practices for Building Model Logic

 

The following sections provide a few general guidelines for building a model's logic:

 

### Flowchart Your System

 

Many FlexSim users typically begin the simulation process by creating a simple flowchart of their business system. You can design this kind of flowchart in the Process Flow tool while you're in an early model-building stage and then possibly add more complex logic to that same flowchart to control the logic for your simulation model in the later model-building stages.

 

### Build Your Logic in Phases

 

The Process Flow tool has many features that can easily scale for use in projects of any size and complexity. A simulation project generally involves large, complex problems that can be broken down into sets of smaller problems. Consider breaking up large simulation models into small, manageable pieces and working on one element at a time. Build the logic for that one small element, then test it by running the simulation model to see if it behaved as expected. After you've got one element working well, then you can build out more logic from there.

 

With the Process Flow tool, you can begin building the logic of your simulation earlier in a project life cycle from a perspective that makes sense to you. This means you will be able to build your ideas into the simulation more quickly. It also means that you can adapt your simulation more easily and efficiently later in the simulation life cycle as your knowledge changes or you get new information about your business system.

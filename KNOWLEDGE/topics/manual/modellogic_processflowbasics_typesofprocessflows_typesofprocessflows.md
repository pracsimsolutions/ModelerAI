---
id: modellogic_processflowbasics_typesofprocessflows_typesofprocessflows
name: "Types of Process Flows"
kind: manual
breadcrumb: ["ModelLogic","ProcessFlowBasics","TypesOfProcessFlows","TypesOfProcessFlows"]
tags: ["manual","modellogic","processflowbasics","typesofprocessflows"]
source: "manual/ModelLogic/ProcessFlowBasics/TypesOfProcessFlows/TypesOfProcessFlows.html"
---

# Types of Process Flows

## Introduction to Types of Process Flows

 

When you add a new process flow to a simulation model, the first choice you have to make is which type of process flow you will build.

 

 

This topic will provide a high-level overview of the three types of process flows you can build to help you decide which type of process flow is best for your model.

 

 

 

## Types of Process Flows

 

The main purpose of a process flow is to control the basic logic of your simulation model. In this respect, you could think of process flows as the "brains" of a simulation model. Therefore, when you decide which type of process flow to create, it is like answering the question: whose brain is it that you are implementing?

 

### General

 

Choose this type of process flow if you want to create a centralized brain for the entire model. General types of process flows are ideal for creating process flow logic that will need to be globally accessible to many objects in the simulation model. A general process flow is a good place to put any labels, variables, or logic that needs to be used by multiple objects in the model.

 

### Object

 

Choose this type of process flow if you want to create the brain of one or more objects. You'd use this type of process flow if you want to create logic for custom FR objects or if you simply want an object to have additional functionality beyond the standard FlexSim logic for that object.

 

When defining an object process flow, you are defining logic from the perspective of the object itself. This perspective is generally synonymous with [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming). It also coincides with the concept of agent-based modeling.

  

#### Process Flow Templates

 

The Object process flow type comes with pre-built templates for commonly used logical processes. Feel free to experiment with some of them.

  

### Sub-Flow

 

Choose this type of process flow if you want to create a sub flow, which is like a small part of a brain that will get used by many different objects. A sub flow is a separate process flow that begins running when it is triggered by another activity or event in a different process flow. Sub flows are chunks of self-contained logic that will get executed when they are triggered by certain events in the simulation model or general process flow. If you are familiar with programming terms, you could think of a sub flow as a *function* or a *subroutine*.) See Sub Process Flows for more information.

 

If you are unsure what type of process flow to start with, choose a General Process Flow. Be aware that the type of process flow you choose could affect the picklist options that are available to link that process flow to objects in the 3D simulation model. <!-- See <_a href="../../ProcessFlowBasics/LinkingModels/LinkingModels.html">Linking Process Flows to 3D Models for more information.-->

 

### Person

 

Person flows are a specialized version of the Object flow that also creates a general flow to create people instances.

 

 

 

## Adding and Changing a Process Flow Type

 

To add a process flow, click the Process Flow button on the main toolbar and select the appropriate process flow type from the menu.

 

After you create a process flow, there is no method for changing a process flow's type (such as in its general properties). But if you begin working on a process flow and realize you should have chosen a different process flow type, simply create a new process flow and copy the activities and stacked blocks from the old process flow into the new one. The activities will retain all the same settings and properties when they are copied. See Copying and Pasting Activities or Stacked Blocks for more information.

 

 

 

## Instances

 

In using different types of process flows, you will come across the idea of instances. Instances are specific copies of a process flow's 'brain'. For example, as mentioned before, a fixed resource process flow would implement the brain of a fixed resource. Once you've implemented this brain, you can attach that process flow to one or more fixed resources in the model. Each fixed resource will then become a separate instance of the process flow.

 

If you are using a general process flow, there will only be one instance, namely the process flow itself. If you use a fixed resource or task executer process flow, there will be one instance for each object that is attached to the process flow. If you use a sub flow, instances are created dynamically during the model run, based on rules that you define on the process flow.

 

See Process Flow Instances for more information on instances.

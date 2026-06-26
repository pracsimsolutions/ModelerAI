---
id: modellogic_additionalconcepts_processflowinstances_processflowinstances
name: "Process Flow Instances"
kind: manual
breadcrumb: ["ModelLogic","AdditionalConcepts","ProcessFlowInstances","ProcessFlowInstances"]
tags: ["manual","modellogic","additionalconcepts","processflowinstances"]
source: "manual/ModelLogic/AdditionalConcepts/ProcessFlowInstances/ProcessFlowInstances.html"
---

# Process Flow Instances

## Introduction to Process Flow Instances

 

In the Process Flow tool, an instance is a single occurrence of a sub flow, fixed resource process flow, or task executer sub flow. Usually, you'll create one of these kinds of process flows to act as a basic template. When an object (such as a fixed resource or task executer) uses the process flow during a simulation run, it will become a separate instance of that process flow. For that reason, there might be multiple instances of the same process flow running at the same time during a simulation run.

  

#### General Process Flows Are the Exception

 

General process flows can only have one instance at a time. See Process Flow Types for more information.

  

 

 

## About Process Flow Instances

 

The concept of instances is somewhat abstract. For that reason it might help to compare it to something concrete, such as cooking with a recipe. When you build any kind of process flow other than a general process flow, it's as though you are creating a basic recipe for a particular meal. Individual chefs can then make that recipe on their own. Each time someone makes the recipe, they are making an *instance* of that recipe. Several individuals might be cooking the meal using the recipe at the same time. But they are all using the same basic recipe as their starting point. Also, if you change the recipe, you will change all instances that use that recipe from that point forward.

 

In that same vein, when you build a process flow, you are building a "recipe" for the basic logic that will control the objects in your simulation model. During a simulation run, many different objects could use the same process flow logic (the recipe), but each one would be a separate instance. If you change the main process flow (the recipe) process flow, it will change all the instances of that process flow as well.

 

The Task Sequence Tutorial provides a useful example of process flow instances. In this tutorial, you create a sub flow containing a simple task sequence for loading and unloading flow items from a processor. The model has two processors, each with its own operator. Both operators use the task sequence in the main process flow to load and unload the flow items. The following image shows what happens during a simulation run:

 

 

In this case, the main process flow is the recipe because it contains the basic task sequence that will be used by both operators. Because of the way the general properties were defined on the main process flow, each operator that runs the task sequence in the sub flow will be considered a separate instance of that process flow. During a simulation run, each token in the main process flow represents one of the operators, as shown in the following image:

 

 

The following image shows what it would look like if you were viewing the main process flow and each individual instance at the same time:

 

If you were to make a change to the task sequence in the main process flow, it would immediately update all the instances of that process flow as well.

 

 

 

## Reasons to Use Instances

 

There are a two advantages in using process flow instances:

 

1. Being able to manage shared assets across instances
1. Creating a dynamic reference to an instance

 

Each of these concepts will be explained in more detail in this section.

 

### Managing Shared Assets Across Instances

 

The most important reason for understanding and using process flow instances has to do with managing shared assets across instances. As a reminder, a shared asset is a special type of process flow object that functions differently from other activities in a process flow. There are a few types of shared assets:

 

| Shared Asset | Explanation |
| --- | --- |
| List | Allows you to push or pull tokens, flow items, task executers, numbers, strings, etc. to a list. This is a useful asset for syncing multiple tokens within a process flow or as a more dynamic resource. Process flows can use a list that is local to the process flow itself or could be tied to a Global List in the simulation model. |
| Resource | Represents a limited supply of some resource that can be acquired and released. It can be used to simulate a supply of goods, services, time, materials, employees, etc. |
| Variable | Allows you to store data in a centralized location accessible throughout the flow. The value of that data can be read and changed freely by tokens. |
| Zone | Can collect statistical information not available for standard activities. It can also restrict access to a section of the process flow based on certain statistics or other criteria. |

 

Whenever a process flow can have multiple instances, you can determine how any shared assets are distributed among the instances of that process flow. For example, imagine you build a sub flow that uses a Resource with a count of 3. The Resource can be managed one of two ways:

 

- **Global** - Shared assets will be globally accessible by all instances. For example, if you have a Resource with a count of 3, any instance that uses that Resource will deplete the total count. If one instance uses all three Resources, another instance won't be able to use it.
- **Local** - Shared assets will only be locally accessible to instances. For example, if you have a Resource with a count of 3, each instance will have a local copy of that Resource with a count of 3. When an instance uses a Resource it will only deplete the local count. If one instance uses all three of its Resources, another instance will still have access to its three Resources as well.

 

You can change these settings on each shared asset you add to your process flow. Each shared asset has a *Type* menu in its Properties. You can use this menu to control whether shared assets are global or local to instances. Note that this setting is only available if the process flow type is not general. For general process flows, there is always just one instance, so shared asset access is always global.

 

### Referencing the Instance

 

Another advantage of using process flow instances is that you can easily reference the object associated with the instance. When using Process Flow instances, the `current` keyword gives a reference to the token's instance object. Technically, this is not a critical feature because you can always store labels on the token that point to the object that you would otherwise use as the instance object. Nevertheless, using `current` can be more intuitive.

 

### Instance Class Type

 

When referencing the `current` in picklists and code editors, an instanced process flow defaults to the Object class type. This allows you easy access to the methods and properties of the Object class. You can change the object class of the instanced process flow in the properties. Changing the object class to a different class type, for example a Dispatcher or TaskExecuter will update all of the code headers through the process flow and give you access to the new object class methods and properties. For a complete list of available classes see the FlexScript Class Reference.

  

#### Selecting A Valid Object Class

 

Be sure to use a valid object class to avoid getting exceptions when running your model. Attaching a Processor to your process flow and then trying to treat that as a TaskExecuter will give you runtime errors.

  

 

 

## Properties Related to Instances

 

The properties that affect instances depend on what type of process flow it is: 

- **General** process flow types cannot have more than one instance.
- Each fixed resource that runs a **Fixed Resource** process flow type will be a separate instance of that process flow.
- Each task executer that is attached to a **Task Executer** process flow type will be a separate instance of that process flow.
- **Sub Flow** process flow types have a special property called **Instance Creation** that allow you to define when the FlexSim system will create an instance of a sub flow. This can affect the way local shared assets are used.

 See Process Flow Types for more information.

 

Additionally, you can use the *Type* menu on Shared Assets objects (Resource, List, and Zone) to determine whether these assets are global or local to an instance.

 

 

 

## Viewing Instances

 

To view an instance of a process flow:

 

1. Click in a blank spot in the process flow to ensure nothing is selected.
1. In Properties, find the **Process Flow Instances** panel. This lists the objects that are attached to the process flow, as shown in the following image:
1. Click one of the objects in the **Attached Objects (instances)** list to select it. Then click the **Open PF View** button to open the process flow view for that specific instance. This view will only show the tokens associated with that instance.

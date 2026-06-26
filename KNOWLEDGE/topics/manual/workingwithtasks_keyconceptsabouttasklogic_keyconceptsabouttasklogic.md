---
id: workingwithtasks_keyconceptsabouttasklogic_keyconceptsabouttasklogic
name: "Key Concepts About Task Sequence Logic"
kind: manual
breadcrumb: ["WorkingWithTasks","KeyConceptsAboutTaskLogic","KeyConceptsAboutTaskLogic"]
tags: ["manual","workingwithtasks","keyconceptsabouttasklogic","keyconceptsabouttasklogic"]
source: "manual/WorkingWithTasks/KeyConceptsAboutTaskLogic/KeyConceptsAboutTaskLogic.html"
---

# Key Concepts About Task Sequence Logic

## Overview of Task Sequence Logic Systems

 

As you'll recall from the chapter about Using 3D Objects, task executers are 3D objects that perform task sequences. They can move throughout the model, load and unload flow items, be utilized at fixed resource stations, etc.

 

 

They are called task executers because they can be assigned task sequences to complete in the simulation model. Task sequence logic is any kind of logic that will involve task executers and task sequences. FlexSim provides you with several different options for defining task sequence logic, so that your solution can cater to specifically to your individual simulation problem.

 

There are four primary facets to defining task sequence logic, as follows.

 

| Task Sequence Generation | How are task sequences defined? |
| --- | --- |

 

| Job Dispatching | Who or what will perform what jobs? |
| --- | --- |

 

| Item Flows and Routing | Where do the items need to go? |
| --- | --- |

 

| Transportation or Travel | How do task executers move around? |
| --- | --- |

 

Each of these problems can be resolved according to your simulation's needs. Your simulation model might have a very simple answer to one or more of these problems, in which case it will be simple and straightforward to set up the logic. On the other hand, many simulation models require customized solutions to one or more of these problems. FlexSim's features give you ultimate flexibility in defining this customization. This topic will discuss each of these different problems in more depth and will explain which tools and processes you can use to solve them.

  

#### If You Understand Task Sequence Logic, You Understand FlexSim

 

FlexSim's task sequence logic capabilities are robust and feature-rich. Consequently, if you are new to FlexSim, they might be a little intimidating. However, the good news is that once you've mastered these principles, you can build nearly any kind of simulation model in FlexSim. If you can create task sequence logic, you can build any kind of logic in FlexSim.

  

 

 

## Task Sequence Generation

 

| Task Sequence Generation | How are task sequences defined? |
| --- | --- |

 

One of the first problems you need to address when designing task sequence logic is the obvious question of how to define the task sequences to be worked on by task executers. A *task sequence* is a specific sequence to *tasks* that are to be performed together as part single "job" or unit of work. A *task* is a single instruction or action that is performed as part of a *task sequence*. An example task sequence, with its associated individual tasks, might be the following:

 

1. Travel to a queue
1. Load a flow item from the queue
1. Travel to a processor
1. Unload the item into the processor

 

See Task Sequence Concepts for more information on task sequences.

 

#### Viewing a TaskExecuter's Task Sequences

 

You can view the state of a TaskExecuter's currently active task sequence and its task sequence queue by right-clicking on the object and selecting **View Task Sequence Queue**.

 

### 3D Object-Based Task Sequence Generation

 

Transportation tasks tend to be the most common tasks. For that reason, the standard logic on 3D objects has some simple and straightforward ways to set up a transportation task. For example, after setting up your port connections, sometimes all you need to do is check the *Use Transport* button on a fixed resource's Output pane to set it up. The 3D object will automatically create a task sequence and dispatch it to the task executer or dispatcher connected to its center port.

 

 

Processors also include an automatic task sequence generation mechanism. You can check the *Use Operator(s) for Setup* or *Use Operator(s) for Process* boxes in a processor/separator/combiner's *Processor* pane. This will cause an automatic task sequence to be generated that tells a task executer to travel to the processor and "utilize" while the item is processed.

 

### Process Flow-Based Task Sequence Generation

 

You can also use a process flow to generate task sequences. The main difference is that, whereas when using 3D object logic the task sequences are defined automatically, in process flow you have full control over the sequence of tasks to perform. For example, the process flow equivalent of a simple transportation task sequence would look similar to the following image:

 

 

### Examples of Process Flow Task Sequence Generation

 

The Process Flow tool has a library of commonly used task sequence activities that you can combine together to create any number of custom task sequences.

 

 

One thing to keep in mind about the process flow tool is that usually most tasks can be represented by simple travel, delay, and acquire activities. For example, imagine you wanted to simulate an operator cleaning a machine during a machine changeover. You can simulate this as a series of travels and delays:

 

1. **Travel to Cleaning Supply Storage** - The operator travels to the location where the cleaning supplies are kept
1. **Acquire Supplies** - The operator acquire the supplies
1. **Travel to Machine** - The operator then travels to the machine
1. **Delay for Cleaning** - The operator delays at the machine, simulating the time it takes to clean the machine and prepare it for the changeover
1. **Travel Back to Cleaning Supply Storage** - The operator returns to the location where the cleaning supplies are kept
1. **Release Supplies** - The operator returns the supplies to the supply closet

 

The following image shows how this task sequence logic would work during a simulation run:

 

 

The task activities that are available in the Process Flow library cover most of the common tasks that are used in simulation modeling, such as loading, unloading, traveling, and delays. However, there are several more pre-programmed tasks that are available using the Custom Task activity in the library.

 

If you want to use any of these custom tasks, you should refer to Task Sequences Quick Reference. The Quick Reference can provide you with detailed information about how you should fill in the properties for each custom task type. It also links to more in-depth guides for each task.

 

 

 

## Job Dispatching

 

| Job Dispatching | Who or what will perform what jobs? |
| --- | --- |

 

When designing task sequence logic, you also need to think about how jobs will be dispatched. The following are some related questions:

 

- How will jobs be assigned to task executers?
- If there are two or more task executers, which task executer will perform the job?
- What will happen if there are not enough task executers to complete the jobs?
- Will some jobs take priority over others? Will a more important job be allowed to preempt (interrupt) another job?
- What happens if multiple task executers need to work together to accomplish a job (also known as a coordinated job)?

  

#### What is a *Job*?

 

In describing these concepts, we intentionally use the more general term *job* instead of the more specific term *task sequence*. A job may indeed be represented by a specific task sequence in one model design. However, in other models, instead of task sequences, you may decide to have process flow tokens, entries on a list, items in a queue, etc., embody the representation of a *job*.

 

Whatever implementation you decide to use, the core concept is the same. In dispatching, *jobs* are being matched to *resources* that can perform them.

  

### Core Dispatching Decisions

 

The dispatching problem in FlexSim is broadly analogous to the well-known set of problem-solving techniques within the domain of [scheduling](https://en.wikipedia.org/wiki/Scheduling_(computing)). From a simulation perspective, the dispatching problem can generally be boiled down to two decisions that the simulation must make. They are:

 

| Dispatching Decisions |  |
| --- | --- |
| Resource Selection | When a new job (e.g. a task sequence) needs to be performed, how does the simulation select which available resource (e.g. task executer) should do that job. |
| Job Selection | When a resource (e.g. a task executer) becomes available to do work, how does it select which job (e.g. task sequence) to do next. |

 

While these two core questions are also applicable in non-task-executer-based scenarios (for example, fixed resources perform similar decisions as part of their send-to and pull logic), this topic will explore just the dispatching problem as it relates to task executers. Here we show different options, and explain how they each solve these two decisions.

 

In making job dispatching decisions, it is important to understand how task executers handle task sequences. See How Task Executers Handle Tasks for an in-depth explanation of how this works.

  

#### *Resource* with a lower-case "r"

 

In this topic we generally use the term *resource* inline with its common usage in describing [scheduling problems](https://en.wikipedia.org/wiki/Scheduling_(computing)). This is not to be confused (unless expressed as such) with FlexSim's process flow Resource.

  

### Direct Task Sequence Dispatching

 

Any kind of simulation model in which there is only one task executer will generally be very easy to set up. You can create center port connections to a task executer and use the Use Transport option on the upstream fixed resource.

 

 

The table below shows how the two primary dispatching decisions are made in this scenario.

 

| Single-Resource Dispatching |  |
| --- | --- |
| Resource Selection | There is only one task executer who can do the job, so all task sequences ("jobs") are dispatched to that task executer. |
| Job Selection | When the task executer becomes available to do another task sequence, it uses its Queue Strategy to select a task sequence to perform next from its task sequence queue. |

 

Using more than one task executer is not much more difficult. You can connect the task executers to a dispatcher object, then send task sequences to that dispatcher through the Use Transport option.

. 

 

| Multi-Resource Dispatching Using a Dispatcher |  |
| --- | --- |
| Resource Selection | Task sequences are dispatched to the dispatcher's task sequence queue. The task executer is then chosen via the dispatcher's Pass To logic. |
| Job Selection | When one of the dispatcher's connected task executers becomes available to do another task sequence, it automatically notifies the dispatcher of its availability. The dispatcher will then use its Queue Strategy to select a task sequence to perform next from its (the dispatcher's) task sequence queue. |

 

These scenarios can also be integrated relatively easily with process flows that you define. If there are additional jobs that you'd like to define directly in process flow, you can use the Create Task Sequence activity to create task sequences and dispatch them to a dispatcher or task executer's task sequence queue.

 

The advantage of using these scenarios is that it is very simple to set up. It is mostly done directly through 3D object properties. A potential disadvantage is that it's less visible. Job queueing is internal to the objects, so it's harder to see what is going on (although it is more visible if you define the task sequences in process flow, because you can see the tokens that are defining the task sequences). Additionally, intricate customization of the job selection and resource selection logic can sometimes be more difficult, because you're confined to using the Queue Strategy and Pass To pick options.

 

| Direct Task Sequence Dispatching |  |
| --- | --- |
| Advantages | Disadvantages |
| - Simple setup - Configured directly in 3D object properties - Integrates easily with process flow | - Job queues are less visible - Intricate selection customization is more difficult |

 

### Dispatching Using Process Flow Resources

 

Another option is to use a process flow Resource to solve the dispatching problem. Here you define a Resource shared asset in process flow. You can associate the resource with a group of task executers. Then when you have a job to do, you use an Acquire Resource activity to select a task executer to use. Then you define a task sequence for the task executer to perform. Finally, you use a Release Resource activity to release the task executer back to the pool.

 

| Dispatching Using Process Flow Resources |  |
| --- | --- |
| Resource Selection | The set of available resources (task executers) is managed by the process flow Resource. Selection is done using the Query / Object / Array field of the Acquire Resource activity. |
| Job Selection | Job selection is performed when the task executer is released back to the Resource, when there are multiple other process flow tokens waiting to acquire it. The default is FIFO. However, you can customize this by clicking the Advanced button and defining the internal list's Back Order Queue Strategy. Since the Resource is the gatekeeper to acquiring task executers, an individual task executer's task sequence queueing logic is generally not a factor. |

 

The advantage of using this mechanism is that it can use the powerful selection and prioritization mechanism provided by FlexSim's SQL engine. Further, queueing of jobs is represented visibly by tokens stacking up in the process flow's Acquire Resource activity. A disadvantage is that, if there is other logic in the model that uses direct dispatching for the same set of task executers, that logic will dispatch directly to task executer/dispatcher task sequence queueing logic, and thus will be invisible to the Resource's selection logic. Further, when you use the powerful SQL selection mechanism of a Resource, you are really just using a List in disguise, and it might be worthwhile to use Lists directly for dispatching.

 

| Dispatching Using a Process Flow Resource |  |
| --- | --- |
| Advantages | Disadvantages |
| - Powerful selection using SQL pull queries - Job queueing is visible in process flow | - Direct dispatching is invisible to the Resource - In this configuration, the Resource is really just a List in disguise |

 

### Dispatching Using Lists

 

The most powerful option for dispatching in FlexSim is to use Lists. Lists allow you to customize how resource and job selection is done, with an incredible degree of flexibility and robustness. Lists utilize FlexSim's SQL parser for selection and prioritization. You can also access lists both from process flow as well as directly through 3D object properties, so they can integrate all different sources of logic. See Key Concepts About Lists for more information.

 

While using Lists is incredibly powerful, it does require a good understanding of how lists work. Concepts such as back orders, pull queries, pullers, values, fields, etc. are required to take full advantage of their capabilities. As such, if direct dispatching is sufficient for the problem at hand, by all means use that solution. Nevertheless, lists provide an incredibly powerful alternative to direct dispatching.

 

#### What Goes on the List?

 

For dispatching, you will orient a list in one of two ways. Either the list will represent a list of *resources* (e.g. task executers, process flow tokens, etc.), or it will represent a list of *jobs* (e.g. task sequences, process flow tokens, items in a queue, etc.).

 

#### A List of Resources

 

 

When the list is a list of resources, the resources are pushed onto the list, and the jobs pull those resources from the list. This orientation is used when resource selection is the primary selection priority. When a job requests a resource, it can use a pull query to define constraints and priorities for which resource to select. If, in pulling from the list, the job does not find a suitable resource, then the job will be placed on the list's back order list. When a resource becomes available and is pushed back onto the list, it will fulfill one of the job back orders. Thus, job selection is done primarily using the list's Back Order Queue Strategy.

 

| Dispatching Using a List of Resources |  |
| --- | --- |
| Resource Selection | The set of available resources (task executers) is managed by pushing those resources onto the list. Resource selection is done when a job pulls from the list, wherein the pull query defines selection requirements and prioritization. |
| Job Selection | Job selection is performed when the resource is pushed back onto the list. If there are back orders, those back orders will be fulfilled based on the Back Order Queue Strategy. Since the List is the gatekeeper to acquiring resources, an individual task executer's task sequence queueing logic is generally not a factor. |

 

To implement this option, you can use a task executer's On Resource Available event to push the task executer onto the list. Additionally, you can use a fixed resource's Use Transport field to pull from the list. Or, you can manage all the list pushing and pulling in process flow, using a process flow List. 

#### A List of Jobs

 

When the list is a list of jobs, the jobs are pushed onto the list, and the resources pull those jobs from the list. This orientation is used when job selection is the primary selection priority. When a resource requests a job, it can use a pull query to define constraints and priorities for which job to select. If, in pulling from the list, the resource does not find a suitable job, then the resource will be placed on the list's back order queue. When a new job arrives and is pushed onto the list, it will fulfill one of the resource back orders. Thus, resource selection is done primarily using the list's Back Order Queue Strategy.

 

| Dispatching Using a List of Jobs |  |
| --- | --- |
| Resource Selection | Resource selection is performed when a job arrives and is pushed onto the list. If there are resources waiting on list's back orders, those back orders will be fulfilled based on the Back Order Queue Strategy. |
| Job Selection | The set of active jobs is managed by pushing those jobs onto the list. Job selection is done when a resource pulls from the list, wherein the pull query defines selection requirements and prioritization. Since the List is the gatekeeper to acquiring resources, an individual task executer's task sequence queueing logic is generally not a factor. |

 

To implement this option, you can use a task executer's On Resource Available event to pull from the list of jobs. Additionally, you can use a fixed resource's Use Transport field to push a job to the list. Or, you can manage all the list pushing and pulling in process flow, using a process flow List. See Using Lists for Task Logic for an example of this scenario.

 

In conclusion, using Lists is the most powerful option for doing dispatching in FlexSim.

 

| Dispatching Using a List |  |
| --- | --- |
| Advantages | Disadvantages |
| - Most capable dispatching option - Powerful selection using SQL pull queries - Job/Resource queueing is visible on the list - Can integrate task sequence logic defined in 3D object properties with process flow logic | - Requires a working understanding of how lists work |

 

 

 

## Item Flows and Routing

 

| Item Flows and Routing | Where do the items need to go? |
| --- | --- |

 

Item flows and routing issues are related to task sequence logic if task executers are involved in the transportation of items from one fixed resource to another. The important thing to keep in mind is that the logic for item routing is handled through fixed resources---not through any logic on task executers themselves. In other words, the logic that determines how items flow from one object to another is primarily determined by logic on the fixed resource or through process flows that are listening to events on fixed resources.

 

### Examples of Simple Item Routing

 

The simplest kind of item routing is when one fixed resource is passing flow items directly to a fixed resource that is adjacent to it. Port connections can handle this logic very easily. Ports can also handle one-to-many item routing without too many difficulties.

 

 

### Examples of Complex Item Routing

 

Any many-to-many item routing system is likely to be a little more complex to design. Other examples of complex systems are ones in which there needs to be a special criteria or conditional logic by which an upstream object sends items to different downstream objects.

 

 

Depending on your simulation model's needs, FlexSim has many different systems you can use to route items from one fixed resource to another, including lists, conveyors, and process flows. Consider reading the chapter about Connecting 3D Object Flows for more detailed information about how to build item routing logic using these different systems.

 

 

 

## Transportation or Travel Routing

 

| Transportation or Travel | How do task executers move around? |
| --- | --- |

 

If task executers need to transport items or travel across long distances, you might need to create the logic that tells the task executer which path to take in order to get from one point to another.

 

### Examples of Simple Travel Routing

 

By default, task executers will travel on the shortest path between two locations, even if there are other objects in that path. The default travel mechanisms might be all your system needs if task executers only need to travel short distances between objects.

 

 

### Examples of Complex Travel Routing

 

If task executers have to travel distances that will not be in a simple straight line, you'll probably want to use one of FlexSim's travel systems to help task executers walk on more accurate paths. The good news is that each of these different travel systems operate on the same basic principles. Once you've learned how to use one of the travel systems, it's not terribly difficult to learn how to use the other systems since many of the same basic principles apply. See Key Concepts About Travel for a deeper explanation of the different advantages and disadvantages of the various travel systems.

 

FlexSim has three different travel systems. One is the travel network system:

 

 

Another is the A* navigation system:

 

 

And the last system is for automatic guided vehicle (AGV) networks:

 

 

 

 

## Tutorials and Further Reading

 

To learn more about tasks, consider completing the following relevant tutorials to get hands-on experience with the concepts discussed in this chapter:

 

- Task Logic Tutorial 1 - Task Logic Tools
- Task Logic Tutorial 2 - Coordinated Tasks
- Task Logic Tutorial 3 - Conditional Tasks
- Process Flow Tutorial 2 - Task Sequences
- Process Flow Tutorial 5 - Creating Logic for Custom Objects

 

Consider also reading Reference - Task Executer Concepts for a more in-depth explanation about how task executers work.

 

The rest of this chapter will discuss topics related to tasks in general and specific kinds of task executers.

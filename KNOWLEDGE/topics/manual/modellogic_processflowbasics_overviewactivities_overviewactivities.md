---
id: modellogic_processflowbasics_overviewactivities_overviewactivities
name: "Overview of Activities and Shared Assets"
kind: manual
breadcrumb: ["ModelLogic","ProcessFlowBasics","OverviewActivities","OverviewActivities"]
tags: ["manual","modellogic","processflowbasics","overviewactivities"]
source: "manual/ModelLogic/ProcessFlowBasics/OverviewActivities/OverviewActivities.html"
---

# Overview of Activities and Shared Assets

## Introduction to Activities and Shared Assets

 

The Process Flow tool has a unique set of objects in the Library. When you have the process flow view open and active in FlexSim, the Library will change to display these objects.

 

The following tables will provide a high-level overview of these objects. Each object has a link to its object reference page for more information. The categories and objects are presented in the same order as they appear in the Library.

 

 

 

## Token Creation

 

Similar to sources in the 3D simulation model, these activities create the tokens that will flow through the process flow. Most process flows will begin with some kind of Token Creation activity.

 

| Name and Icon | Description |
| --- | --- |
| Inter-Arrival Source | Creates new tokens according to a specific interval of time. You can use a fixed number to set an exact interval of time between token creations or you can use a statistical distribution to randomly calculate the time between arrivals. See the Inter-Arrival Source activity for more information. |
| Schedule Source | Creates new tokens at a rate specified in its Arrivals table. This table defines the time (in model units) that tokens should be created, the name that will be assigned to the new tokens, and the number of tokens to create. See the Schedule Source activity for more information. |
| Date Time Source | Creates new tokens as specified in its Arrivals table. This table defines when and how many tokens should be created. You can use a date-based method or a repeated table (with daily or weekly schedules). See the Date Time Source activity for more information. |
| Event-Triggered Source | Creates tokens in response to an event during a simulation run. This source will listen for that event to occur in the simulation model. When that event occurs, it will create a token. See the Event-Triggered Source activity for more information. |

 

 

 

## Basic

 

The basic activities have many general uses in a wide variety of process flows.

 

| Name and Icon | Description |
| --- | --- |
| Assign Labels | Creates or modifies labels on various objects. Labels can be used to store important data about various objects. See the Assign Labels activity for more information. |
| Delay | Holds the token for a certain length of time. You can use a fixed time or you can create the delay time dynamically using a label value on a token, a statistical distribution, etc. See the Delay activity for more information. |
| Custom Code | Can create custom behavior in the process flow module. You can create custom codes by selecting pre-defined picklist options or by writing your own code in FlexScript. When a token enters the Custom Code activity, it will evaluate the user-defined code and then immediately be released to the next activity without any time passing. See the Custom Code activity for more information. |
| Decide | Send a token to one of two or more possible activities based on conditions that you define. In other words, the Decide activity can determine the next activity that a token should be diverted to. See the Decide activity for more information. |
| Batch | Collects incoming tokens and sorts them into groups of tokens (batches). When a batch is ready, the Batch activity will release it to a downstream token. Using the Batch activity for simple grouping and releasing is relatively straightforward. However, the Batch activity has some additional features that allow you to collect, sort, and release batches in fairly complex ways. See the Batch activity for more information. |
| Wait for Event | Holds the token until a certain event is triggered. This activity will listen for that event to occur in the simulation model. When that event occurs, it will release the token. See the Wait for Event activity for more information. |
| Create Tokens | Creates one or more new tokens and automatically sends them to a different activity or sub flow. The token will then immediately continue to the next downstream activity. See the Create Tokens activity for more information. |
| Sink | Destroys tokens, removing all data stored on those tokens. Typically put at the end of a process flow. See the Sink activity for more information. |

 

 

 

## Sub Flow

 

Sub flow activities are needed in order to create and run sub flows. See Sub Process Flows for additional information.

 

| Name and Icon | Description |
| --- | --- |
| Run Sub Flow | Initiates a sub process flow. When a token enters this activity, it will create one or more child tokens and send them to the Start activity in a sub flow. The token will wait at this activity until the child token(s) reach the Finish activity in the sub flow. See the Run Sub Flow activity for more information. |
| Start | The Start activity is usually the first activity in a sub flow. However, the Start activity only marks the beginning of a sub flow. It does not perform any other logic. After a token enters the Start activity, it will immediately move to the next connected activity in the sub flow. See the Start activity for more information. |
| Finish | The Finish activity marks the end of a sub flow. The Finish activity essentially acts the same as a Sink in that it destroys tokens that enter it. See the Finish activity for more information. |

 

 

 

## Visual

 

These activities can be used to change the visuals of an object or run an animation in the 3D model.

 

| Name and Icon | Description |
| --- | --- |
| Change Visual | Changes the appearance of an object in the 3D model. Generally this is used for changing a flow item's visuals, but it may also be used to adjust any 3D object's visual properties. See the Change Visual activity for more information. |
| Run Animation | Triggers an animation on a 3D object in the simulation model. You can use any of the animations that come pre-programmed with the standard FlexSim objects or you can create your own custom animations. See the Run Animation activity for more information. |

 

 

 

## Objects

 

These activities can create, move, or destroy objects such as flow items, fixed resources, task executers, etc. in the 3D simulation model.

 

| Name and Icon | Description |
| --- | --- |
| Create Object | Creates one or more objects in the 3D simulation model. This activity can also create TaskExecuters and TaskExecuter flow items and connect them to a travel network. See the Create Object activity for more information. |
| Move Object | Moves an object or multiple objects to another place in a simulation model. See the Move Object activity for more information. |
| Destroy Object | Removes one or more objects in the 3D simulation model. See the Destroy Object activity for more information. |

 

 

 

## Task Sequences These activities can be used to build task sequences that can be assigned to task executers (such as operators) in the simulation model.   Name and Icon Description    Travel   Makes a task executer travel to a specific object (such as a fixed resource) in the 3D simulation model. See the Travel activity for more information.    Load   Makes a task executer load an object in the 3D simulation model. For example, you can make a task executer pick up an object, possibly to transport it to another destination. See the Load activity for more information.    Unload   Makes a task executer unload an object in the 3D simulation model. For example, you can make a task executer drop off an object at a particular destination. See the Unload activity for more information.    Delay   Makes a task executer delay for a specific period of time in the 3D simulation model. For example, a delay could represent a task that takes a specific amount of time to complete such as cleaning an object, assembling a product, etc. See the Delay (Task Sequence) activity for more information.    Travel to Loc   Makes a task executer travel to specific X, Y, and Z coordinates in the 3D simulation model. See the Travel to Location activity for more information.    Custom Task   If you need to give a task executer a task that isn't currently available in the process flow library, you can use the Custom Task activity to build your own custom task. See the Custom Task activity for more information.    Create Task Sequence   Creates an empty task sequence. Tasks can be assigned to this task sequence later in the process flow. See the Create Task Sequences activity for more information.    Dispatch Task Sequence   You could use a Dispatch Task Sequence if you would prefer instead to build a series of task sequences first and dispatch them later. See the Dispatch Task Sequence activity for more information.     Shared Assets

 

These activities can be used to create and manage lists, resources and zones in the simulation model.

 

| Name and Icon | Description |
| --- | --- |
| List | Represents a list of tokens, flow items, task executers, numbers, strings etc. Process flows can use a list that is local to the process flow itself or could be tied to a Global List in the simulation model. See the List shared asset for more information. |
| Push to List | Can be used to add tokens, flow items, task executers, numbers, strings, etc. to a list. See the Push to List activity for more information. |
| Pull from List | Can be used to retrieve tokens, flow items, task executers, numbers, strings, etc. from a list. See the Pull from List activity for more information. |
| Resource | Represents a limited supply of some resource that can be acquired and released. It can be used to simulate a supply of goods, services, time, materials, employees, etc. See the Resource shared asset for more information. |
| Acquire Resource | Used to acquire a resource at some point during a process flow. When a resource has been acquired, it reduces the supply of that resource by a specified amount. See the Acquire Resource activity for more information. |
| Release Resource | Used to release or return a resource at some point during a process flow. When a resource has been released, it increased the supply of that resource by a specified amount. See the Release Resource activity for more information. |
| Variable | The Variable can be defined to stand in as other elements such as a label on a token or other object. The variable acts as a central reference to that value that can be easily accessed throughout a process flow. You can listen to events related to the variable such as when it is changed or even when it is read. See the Variable shared asset for more information. |
| Get Variable | Used to read the current value of a Variable shared asset. You can then store that value in a label before the token is sent to the next downstream activity. See the Get Variable activity for more information. |
| Set Variable | Used to change the current value of a Variable shared asset. See the Set Variable activity for more information. |
| Zone | Can collect statistical information not available for standard activities. It can also restrict access to a section of the process flow based on certain statistics or other criteria. See the Zone shared asset for more information. |
| Enter Zone | Used to define the point in the process flow where the token will either enter or request access to enter the zone. See the Enter Zone activity for more information. |
| Exit Zone | Used to define the point in the process flow where the token will exit the zone. See the Exit Zone activity for more information. |

 

 

 

## Coordination

 

These activities can be used to coordinate multiple tokens in process flow.

 

| Name and Icon | Description |
| --- | --- |
| Split | Splits the token into multiple tokens and sends one out each outgoing connector. See the Split activity for more information. |
| Join | Tokens wait at the Join activity until one token arrives from each incoming connector. The token from the first connector is then released out the outgoing connector and the other tokens destroyed. See the Join activity for more information. |
| Synchronize | Tokens wait at the Synchronize activity until one token arrives from each incoming connector. The tokens then leave through the outgoing connector corresponding to the incoming connecter they came in through. See the Synchronize activity for more information. |

 

 

 

## Preemption

 

These activities can be used to manage preemption systems in process flow.

 

| Name and Icon | Description |
| --- | --- |
| Save Token Context | Before preempting, this activity saves the token's current context in the process flow so that it can possibly return to this context after it finishes the preempting activities. See the Save Token Context activity for more information. |
| Release Token | Aborts the token's current activity and releases it to a new activity, either to do something else, or to just wait. See the Release Token activity for more information. |
| Restore Token Context | After completing the operation that required the preemption, this activity restores the token back to its saved context. See the Restore Token Context activity for more information. |

 

 

 

## Display

 

These objects have no actual function in the process flow and are available for visual display only.

 

| Name and Icon | Description |
| --- | --- |
| Text | The Text object can act like a custom text box that can be placed anywhere inside a process flow. See the Text object for more information. |
| Arrow | The Arrow object is an arrow you can put anywhere in your process flow. See the Arrow object for more information. |
| Image | Use the Image object for a number of different purposes such as adding a custom image somewhere in your process flow, adding a background image to a process flow, or adding a background image to Flow Chart object. See the Image object for more information. |

 

 

 Flowchart 

There are many different flowchart objects available in the Process Flow Library. You can use these shapes to build a classic flowchart in your process flow. You could also possibly use them as visual containers that help organize and visualize the activities in your process flow. See the Changing Process Flow Visuals for more information.

---
id: workingwithtasks_coordinatedtasks_coordinatedtasks
name: "Coordinated Tasks"
kind: manual
breadcrumb: ["WorkingWithTasks","CoordinatedTasks","CoordinatedTasks"]
tags: ["manual","workingwithtasks","coordinatedtasks","coordinatedtasks"]
source: "manual/WorkingWithTasks/CoordinatedTasks/CoordinatedTasks.html"
---

# Coordinated Tasks

## Introduction to Coordinated Tasks

 

Coordinated tasks are any tasks that require more than one task executer. Some common scenarios in which coordinated tasks are used:

 

- When more than one operator is needed to transport an item
- When multiple operators are needed for setup or for processing
- When an operator needs to hand off something to another operator
- When two or more operators need to work on separate tasks concurrently

 

Nearly all coordinated task sequences require you to build custom task logic.

 

The following image shows an example of a coordinated task sequence in which two operators are occasionally needed to transport heavy boxes:

 

  

#### There's a Tutorial For That

 

The example from the previous image comes from Task Logic Tutorial 2 - Coordinated Tasks. Consider doing this tutorial if you'd like an in-depth, hands-on approach to the concepts discussed in this topic.

  

 

 

## Tools Used to Create Coordinated Tasks

 

The Process Flow tool is the ideal tool for building custom task sequences, including coordinated task sequences. The most common activities you will use to build coordinated tasks are the Coordination activities. These activities are specifically designed to coordinate tasks, processes, and logic that needs to happen simultaneously. See Process Flow - Coordination for more information.

 

 

In some cases, it could also be helpful to coordinate task sequences using sub flows activities. See Sub Process Flows for more information.

 

 

The list and resource shared assets can also manage the availability of task executers to work on tasks as well. See Key Concepts About Shared Assets for more information.

 

 

 

## Creating Coordinated Task Logic

 

This section will provide an overview of the basic steps involved in building coordinated tasks. The following image shows an example of coordinated task logic and the different steps involved in the process:

 

 

### 1. Start the Coordinated Task

 

In general, when creating coordinated tasks, the task that needs to be performed should correlate with a token in a process flow.

 

You should start the coordinated task sequence with an activity that responds to an event in the 3D model that creates the need for a task. For example, you could use an Event-Triggered Source that listens for the event in the 3D model that triggers the need for the coordinated task sequence.

 

### 2. Split the Token

 

Once the token is created in the task sequence, you should usually start with a Split activity that splits the token into however many task executers are needed for the task sequence.

  

#### Keeping Your Process Flow Organized

 

In general, it helps to create a separate container (a process flow shape) for each task executer that is needed for the coordinated tasks. To help avoid confusion, the containers should be parallel to each other and contain the tasks that are performed by that task executer alone.

  

### 3. Acquire the Task Executers

 

Use an Acquire Resource or Pull from List to acquire task executers for each set of tasks. You'll need to acquire or pull each of the needed task executers separately, ideally on their own container track.

 

### 4. Build and Synchronize the Tasks

 

Next, build out the actual tasks that are needed for the coordinated tasks. Usually the main task executer performs the actual tasks while any assisting task executers merely move along at the same time the main task executer moves in the 3D model.

 

After each task, you should include a Synchronize activity to ensure that the task executers remain in sync with each other. Otherwise, their timing might begin to diverge and one task executer could get ahead of the other.

  

#### Keeping Your Process Flow Organized

 

As a best practice, consider making the Synchronize activity span across all the coordinated task containers since this activity is used by multiple task executers.

  

### 5. Release the Task Executers

 

After the tasks that needed to be coordinated are complete, release the task executers.

 

### 6. Join or Destroy the Assisting Task Executer Tokens

 

After releasing the task executers, you need to destroy the tokens that are associated with the assisting task executers. You could also use a Join activity to merge the tokens back into one token. However, be aware that the label information from the tokens for the assisting task executers might not be preserved. See Process Flow - Coordination for more information about the interaction of labels between parent, child, and sibling tokens.

 

 

 

## Tutorials and Further Reading

 

To learn more about coordinated tasks, consider completing the following relevant tutorials to get hands-on experience with the concepts discussed in this chapter:

 

- Task Logic Tutorial 1 - Task Logic Tools
- Task Logic Tutorial 2 - Coordinated Tasks

 

Consider also reading Process Flow - Coordination and Sub Process Flows for a more in-depth explanation about the process flow activities that are involved in building coordinated tasks.

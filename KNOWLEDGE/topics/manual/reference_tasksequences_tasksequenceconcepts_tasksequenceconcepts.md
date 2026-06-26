---
id: reference_tasksequences_tasksequenceconcepts_tasksequenceconcepts
name: "Task Sequence Concepts"
kind: manual
breadcrumb: ["Reference","TaskSequences","TaskSequenceConcepts","TaskSequenceConcepts"]
tags: ["manual","reference","tasksequences","tasksequenceconcepts"]
source: "manual/Reference/TaskSequences/TaskSequenceConcepts/TaskSequenceConcepts.html"
---

# Task Sequence Concepts

## Custom Built Task Sequences

 

You can create custom task sequences using 3 simple commands:

 

[TaskSequence.create](../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=TaskSequence&Method=create)()

 

[TaskSequence.addTask](../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=TaskSequence&Method=addTask)()

 

[TaskSequence.dispatch](../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=TaskSequence&Method=dispatch)()

 

First, create a task sequence by using TaskSequence.create(). Then add tasks to the task sequence by successive addTask() commands. Finally dispatch the task sequence with dispatch().

 

The following example tells a forklift to travel to an object, referenced as "station", then load a flow item, referenced as "item."

 

```
		TaskSequence ts = TaskSequence.create(forklift, 0, 0);
		ts.addTask(TASKTYPE_TRAVEL, station);
		ts.addTask(TASKTYPE_LOAD, item, station, 2);
		ts.dispatch();
```

 

The TaskSequence.create() command takes three parameters. The first parameter is the object that will handle the task sequence. This should be a dispatcher or task executer object. The second and third parameters are numbers, and specify the task sequence's priority and preempting values, respectively. The command returns a reference to the task sequence that was created.

 

The addTask method add a task to the end of the task sequence. Each task that you add has several values associated with it. First, it has a type value, which defines what type of task it is. It also has a reference to two involved objects for the task, referred to as involved1 and involved2. These involved objects and what they mean depend upon the task type. For some task types both involved parameters are needed and have meaning, whereas for others, the involved objects are not used. Some task types may use one involved object, and some have involved objects which are optional. Refer to the documentation on task types for information on what a specific task type's involved objects represent. The task can also have up to four additional values. These are task variables, referred to as var1, var2, var3, and var4. Again, their meaning depends on the task type. For the load task below, notice that var1 was specified as 1. For a load task, this specifies the output port through which the item will leave the station.

 

 

The addTask command takes two or more parameters, which specify the task's values. The first parameter is a reference to the task sequence into which the task is inserted. The second is the type of task. This can be chosen from an enumerated list of task types. The third and fourth parameters reference the two involved objects. If a specific involved object is not used or is optional for a task type, then you can simply pass NULL into the addTask method, or even leave that parameter out if there are no number variables that you need to specify either. The fifth through ninth parameters are optional, and define var1-var4. By default, these values are zero.

  

#### Optional Parameters

 

Even though many of the parameters of the addTask method are technically optional, depending on the task type, you will still need to specify them. Also, parameters need to still be specified in their correct order. If, for example, you want to specify var1 of the task, but don't care what involved1 or involved2 are, you will still need to pass the NULL value into parameters 3 and 4, even though they are optional, in order to correctly pass var1 in as parameter 5.

  

 

 

## Task Sequence Preempting

 

Every task sequence has a preempting value. Preempting is used to break a task executer away from its current operation to execute a more important operation. For example, operator A's most important responsibility is to repair machines. When there are no machines to repair, however, it should also transport material throughout the model. If a machine breaks down while operator A is in the middle of transporting a flowitem somewhere, then the operator should stop whatever he is doing and repair the machine, instead of finishing the transport operation. To do this, you will need to use a preempting task sequence to break the operator away from his current operation.

 

To create a preempting task sequence, specify a non-zero value in the preempt parameter of the TaskSequence.create() command.

 

TaskSequence.create(operator, 0, PREEMPT_ONLY);

 

There are four possible preempt values. These values tell the task executer what to do with the original task sequence(s) that have been preempted.

 

- 0 - **PREEMPT_NOT** - This value is non-preempting.
- 1 - **PREEMPT_ONLY** - If a task sequence has this value, then the task executer will preempt the currently active task sequence and put it back in its task sequence queue to be finished later. When a task sequence is preempted, it is automatically placed at the front of the task sequence queue. When the task executer eventually comes back to the original task sequence, the current task in that task sequence will be done over again, since it was not finished. Also, you can specify a series of tasks to do over again when it comes back to the task sequence using the TASKTYPE_MILESTONE task. This preempt value is the most common used.
- 2 - **PREEMPT_AND_ABORT_ACTIVE** - If a task sequence has this value, then the task executer will stop the currently active task sequence and destroy it, so that it will never come back to that original task sequence.
- 3 - **PREEMPT_AND_ABORT_ALL** - If a task sequence has this value, then the task executer will stop the currently active task sequence, destroy it, and destroy all task sequences in its task sequence queue.

 

To query or change the preempting and/or priority values of a task sequence, you can use the getpreempt(), setpreempt(), getpriority(), and setpriority() commands. For more information on these commands, refer to the Commands Helper.

 

### Interaction Between Multiple Preempting Task Sequences

 

If a task executer is currently working on a preempting task sequence, and it receives a new task sequence that is also preempting, it will use the priority value of the task sequence to determine which task sequence to do. If the priority value of the new task sequence is higher than the priority value of the one it is currently working on, the task executer will preempt its current task sequence and execute the new one. If the priority value of the new task sequence is less than or equal to the priority of the task sequence it is currently working on, then the task executer will not preempt the active task sequence, but will queue up the new task sequence just like any other task sequence it receives. If it must queue up the task sequence, it will not take the preempt value into account for its queueing logic unless you explicitly tell it to in the queue strategy.

  

#### Queueing a Preempting Task Sequence

 

If a preempting task sequence does not actually preempt a task executer, then it will be queued up like any other task sequence. If you want to have preempting task sequence be brought to the front of the queue, then either make your preempting task sequences have higher priority than all other task sequences, or take preempting into account in the queue strategy.

  

### Preempting Travel Tasks on Networks with Traffic Controls

 

Preemption can have some undesirable side effects if a task executer is traveling along a network with TrafficControls when the preemption happens. If preemption occurs when a task executer is traveling along a network edge, then the task executer will be "taken off" that edge and connected in an "inactive" travel state (the red line) to the next network node he would have arrived at had he finished traveling the edge. If the network node is a member of a traffic control whose area starts at that node (in other words, the task executer wasn't in the area yet when he was preempted), then the task executer will be "forced" into the traffic control area, meaning this may cause the number of objects in the area to exceed the maximum for that traffic control area. To avoid this, special logic has been added to the task executer's preemption mechanism, so that if he is preempted from a travel task directly to another travel task, then instead of being taken off the edge and connected to the next node, the preemption will call redirectnetworktraveler(), which will essentially keep him traveling as he was before on the edge, but his final destination will be changed so that when he arrives at the end of the edge, he will continue on a new path to the new destination. Note that this will only happen if the preemption mechanism can detect that the first durative task that the task executer will perform after the preemption is another travel task. By durative we mean any task that will take some amount of time to perform.

 

The following tasks are durative:

 

- TASKTYPE_TRAVEL
- TASKTYPE_LOAD
- TASKTYPE_UTILIZE
- TASKTYPE_DELAY

 

Whereas the following are not:

 

- TASKTYPE_SETNODENUM
- TASKTYPE_TAG
- TASKTYPE_MOVEOBJECT
- TASKTYPE_SENDMESSAGE
- TASKTYPE_NODEFUNCTION

 

If the preemption detects that the next durative task is *not* a travel task, then it will take the task executer off the edge as described above, in which case the task executer may be forced into a traffic control area.

 

### Preempting With Dispatchers

 

If a preempting task sequence is given to a dispatcher, the dispatcher will not consider the preempt value of the task sequence unless you explicitly tell it to. If the dispatcher is set to dispatch to the first available task executer, then it will do just that, and not send the preempting task sequence immediately to a task executer. If you want the dispatcher to dispatch preempting task sequences immediately, then you will need to specify such logic in its Pass To function.

 

A task executer may be connected back to a dispatcher by dragging an A-connect from the task executer to the dispatcher. If this is done, then when the TaskExecuter receives a preempting task it will pass its current task back to the dispatcher. The dispatcher will then redistribute that task according to its dispatching logic. Tasks returned to the dispatcher in this manner are returned in their current state so that the next task executer will begin where the previous task executer left off. This may cause some odd behavior that you should take into consideration when assigning preempting tasks. For example, if a task executer is traveling with an item when it is preempted, the task executer that receives this task will perform the travel and unload tasks without ever picking up the item from the first task executer. Instead, the item will "magically" appear at the right location when the unload completes. In order to prevent these odd behaviors you may want to query the task executer's state to determine if it is in a "preemptable" state (as you define it) before assigning the new task.

 

 

 

## Coordinated Task Sequences

 

Coordinated task sequences are a deprecated feature of FlexSim. We advise that you use Process Flow for the coordination of multiple task executers. The documentation below is included for legacy support.

 

Coordinated task sequences are used for operations which require sophisticated coordination between two or more task executers. These task sequences implement concepts like allocation and de-allocation of task executers, as well as synchronizing several operations being done in parallel.

 

### Commands

 

Coordinated task sequences are built and dispatched using a set of commands which are mutually exclusive from the default task sequence commands. The commands for coordinated task sequences are as follows.

 

`createcoordinatedtasksequence()`

 

`insertallocatetask()`

 

`insertdeallocatetask()`

 

`insertsynctask()`

 

`insertproxytask()`

 

`dispatchcoordinatedtasksequence()`

 

### createcoordinatedtasksequence

 

The createcoordinatedtasksequence command takes one parameter, namely a reference to an object. This object is designated as the task coordinator who holds the task sequence, as well as coordinates the tasks. The task coordinator can also be one of the objects that is allocated within the task sequence. It can be any dispatcher or task executer object. Note that selecting a task coordinator doesn't mean allocating that task coordinator. A task coordinator can be coordinating any number of coordinated task sequences at any one time. Also, unlike regular task sequences, coordinated task sequences are not queued up. The task coordinator will start executing the coordinated task sequence immediately when you dispatch it, no matter how many other coordinated task sequences it is coordinating.

 

### insertallocatetask

 

The insertallocatetask command takes four parameters. The first is the task sequence. Second is the task executer or dispatcher to give an allocated task to. When the task coordinator gets to an allocate task, it will actually create a separate task sequence with an allocated task in it, and pass that task sequence to the specified task executer or dispatcher. In the case that it is a dispatcher, meaning you want to allocate any one of several task executers, then you can use the return value of this command as a key to reference the specific one that gets allocated, since you don't know exactly which one it is at the time that you build the task sequence. The third and fourth parameters are the priority and preempting values of the separate task sequence that will be created. The fifth parameter is optional, and specifies whether the task is blocking. By default (0), the task is blocking. If 1 is passed in, then the task will not be blocking.

 

### insertproxytask

 

The insertproxytask command is similar to the addTask method, with one parameter, the second, added. The second parameter specifies which allocated object you want to do the task. As the task coordinator is the one actually executing the task sequence, once he gets to a proxy task, he will instruct the allocated object to do the task by proxy. Notice that for involved1 and involved2, you can either pass in a key or a straight reference to an object.

 

### insertsynctask

 

The insertsync task halts execution of the task sequence until a specified task, referenced by its key, is finished. It takes two parameters: the task sequence, and a key value of a given proxy task. It is important to note that proxy tasks which are specified for different task executers, by default, will be done in parallel, unless a sync task is specified, whereas proxy tasks given to the same task executer will automatically be done in sequential order, without the need for a sync task.

 

### insertdeallocatetask

 

The insertdeallocatetask command de-allocates a specific task executer, referenced by its key. The first parameter references the coordinated task sequence. The second parameter is the allocation key for the resource you want to de-allocate. The third parameter is optional, and specifies whether the task is blocking. By default (0), the task is blocking. If 1 is passed in, then the task will not be blocking.

 

The above code creates a coordinated task sequence that organizes the two task sequences, as shown in the diagram below.

 

Coordinated Task Sequence

 

 

### Example

 

A team of three operators share two forklifts. An operation needs one operator and one forklift. The operator should travel to the forklift, and the forklift should then move the operator into itself. Then the forklift should travel to the load location, pick an item, then travel to an unload location and drop off the item. Then the forklift should travel to its parking location, and unload the operator. Doing this using simple task sequences would be very difficult, because it deals with two different resources that work in a very coordinated fashion. Coordinated task sequences make this example much easier to simulate. The diagram below illustrates the two task sequences that need to be done for the forklift and operator. Notice that there are some parts where one resource needs to wait and do nothing while the other operates.

 

 

### Code

 

The code to build the task sequence would be written as follows. It is assumed that references called operatorteam and forkliftteam have been established. These reference dispatchers to three Operator objects, and two Transporter objects, respectively. References have also been established for a loadstation from which to load, an unloadstation to unload to, and the item.

 

```
		treenode ts = createcoordinatedtasksequence(operatorteam);
		int opkey = insertallocatetask(ts, operatorteam, 0, 0);
		int forkliftkey = insertallocatetask(ts, forkliftteam, 0,0);
		int traveltask = insertproxytask(ts, opkey, TASKTYPE_TRAVEL, forkliftkey, NULL);
		insertsynctask(ts, traveltask);
		insertproxytask(ts, forkliftkey, TASKTYPE_MOVEOBJECT, opkey, forkliftkey);
		insertproxytask(ts, forkliftkey, TASKTYPE_TRAVEL, loadstation, NULL);
		insertproxytask(ts, forkliftkey, TASKTYPE_LOAD, item, loadstation);
		insertproxytask(ts, forkliftkey, TASKTYPE_TRAVEL, unloadstation, NULL);
		insertproxytask(ts, forkliftkey, TASKTYPE_UNLOAD, item, unloadstation);
		insertproxytask(ts, forkliftkey, TASKTYPE_TRAVEL, forkliftteam, NULL);
		insertproxytask(ts, forkliftkey, TASKTYPE_MOVEOBJECT, opkey, model());
		insertdeallocatetask(ts, forkliftkey);
		insertdeallocatetask(ts, opkey);
		dispatchcoordinatedtasksequence(ts);
```

  

#### About the Above Example

 

There are some model maintenance issues involved here. For example, if you happen to stop and reset the model while the operator is inside of the forklift, you will need to move the operator out of the forklift and back into the model from a reset trigger. Also, whenever you move the operator back into the model, you will need to set its location appropriately, since it is transferring between two different coordinate spaces.

  

### Things to Remember

 

- The first thing you must do before giving any resource proxy tasks is to allocate that resource.
- You must get the key back from each allocate task, because you will use it later. The insertproxytask command takes a key for the executer of the proxy task. This is the key that the allocation task returns. You also will use this key when de-allocating the object.
- While all proxy tasks for the same allocated resource are executed in sequence, proxy tasks for different allocated resources are executed in parallel, unless you explicitly put blocking tasks in the coordinated task sequence.
- Blocking tasks are ones that block the parallel execution of the coordinated task sequence. The task coordinator goes straight through the task sequence, giving proxy tasks to the appropriate allocated resources, until a blocking task is encountered. It will then wait until that task's blocking requirement is met before continuing the task sequence. In other words, execution of all tasks occurring after that blocking task (regardless of which resource they apply to) will be stopped until the blocking task's requirement is met . The blocking tasks and their blocking requirements are as follows:
- Allocation Task: By default this task will block until the specified resource has been allocated. However, if the fifth parameter of insertallocatetask is 1, then the allocate task will not block.
- Sync Task: This task will block until the proxy task specified by its key is finished.
- De-allocation Task: By default this task will block until the specified resource has finished all its proxy tasks and is de-allocated. However, if the third parameter of insertdeallocatetask is 1, then the de-allocate task will not block.
- The order in which you insert your tasks can have subtle yet important implications. This is especially true for placing your proxy tasks in relation to blocking tasks. Proxy tasks placed after certain blocking tasks can be executed very differently than if those proxy tasks were inserted before the blocking tasks.
- Make sure that you de-allocate all objects that you allocate, or the task sequence won't properly release the objects it has allocated.
- Once you have de-allocated a resource, do not give it any more proxy tasks.

  

#### Non-blocking De-allocate and Allocate Tasks

 

The functionality for allowing these tasks to be non-blocking is still in the beta state. Although we encourage you to use this feature, and there are no known bugs at the time of writing, know that you may run into some problems because this functionality hasn't yet been used extensively.

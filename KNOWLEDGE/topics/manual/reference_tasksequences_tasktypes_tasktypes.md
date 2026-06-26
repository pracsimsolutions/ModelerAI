---
id: reference_tasksequences_tasktypes_tasktypes
name: "Task Types"
kind: manual
breadcrumb: ["Reference","TaskSequences","TaskTypes","TaskTypes"]
tags: ["manual","reference","tasksequences","tasktypes"]
source: "manual/Reference/TaskSequences/TaskTypes/TaskTypes.html"
---

# Task Types

## Introduction to Task Types

 

You can also refer to the task type reference guide for quick reference. The following sections give detailed explanations of each task type.

 

  

 

## TASKTYPE_TRAVEL

 

Here the task executer travels to the object specified. This is done by making a travel request to the navigator that it is connected to. The navigator then takes over control of the task executer, and pushes it like a pawn on a chessboard, according to the navigator's own logic, until the task executer reaches the destination and the navigator notifies the task executer that the travel task is finished. How the task executer is pushed is dependent on the type of navigator. If the task executer is connected to a network, then its associated navigator is a network navigator and will push the task executer along network paths.

  

#### Task Executers and Navigators

 

Some objects by default are not connected to a navigator at all. If the task executer is not connected to a navigator, then it will do nothing for the travel task. The following objects do not connect to any navigators by default: ASRSvehicle, Elevator, Robot, Crane.

  

**involved1** - The object to travel to.

 

**involved2** - This value's meaning is only applicable to AGV travel. If the task executer is using an AGV network for travel, then involved2 defines the AGV trailer relative to whom the AGV should arrive at its destination. See Customizing AGV Travel Tasks for more information.

 

**var1** - This specifies the desired end speed for the travel operation. If 0, then the desired end speed will be the maximum speed of the task executer. A positive value will use that value directly as the end speed. If the end speed is negative, then the functionality is dependent on the navigator's logic. The standard navigators, i.e. the default navigator and the network navigator will interpret negative end speeds as an end speed of 0. Other non-standard navigators may however use a negative end speed value to customize the logic for how to get the task executer to the destination, allowing the modeler to use different negative end speeds on travel tasks to effect customized travel logic.

 

**var2** - This value's meaning is specific to the navigator that is handling the travel task. 

- **Network Navigator** - If the task executer is using a normal node network for travel, then the value determines whether to "force" the traveler back to its current network node. If this value is 1, then the object will travel to the destination node even he is already connected to it.
- **AGV Navigator** - If the task executer is using an AGV network for travel, then var2 defines a target offset distance, along the path of the destination control point, for the traveler to arrive at. See Customizing AGV Travel Tasks for more information.

 

**var3** - This value's meaning is only applicable to AGV travel. If the task executer is using an AGV network for travel, then var3 defines a target AGV body offset, i.e. where along the AGV's trailer body it should arrive at its destination. See Customizing AGV Travel Tasks for more information.

 

**var4** - If this value is greater than 0, it represents a *target* travel time for the travel task. The navigator will determine the travel distance for the travel task, and will set the task executer's speed to be the travel distance divided by the target travel time, with acceleration and deceleration of 0. It will then send the traveler to the destination as it would normally. If the task executer is able to travel unimpeded to the destination, then the travel task will take the defined time to finish.

 

Note here that the travel time is a *target*. If the traveler cannot travel unimpeded, then the actual travel time will be higher than the travel time defined by this variable. Consequently, if you want your traveler to take exactly the time defined in this variable, you should design your travel network to avoid travel delays. To do this you may need to:

 

- Set the **Max Allocations** properties of all AGV Control Points and Control Areas on the traveler's path to 0, meaning unlimited allocations.
- Make all AGV paths the traveler travels on use **No Accumulation**.
- Make all network travel paths passing.
- Do not use traffic controls in a node network.
- Uncheck **Collision Avoidance** in the A* network properties.

 

Note also that this will not work properly when using elevators in the A* network. Since it can't pre-determine the exact time it will take to move between floows on an elevator, the total travel time cannot be pre-determined. Here you should use bridges instead of elevators.

 

### Example

 

`ts.addTask(TASKTYPE_TRAVEL, outobject(current,1))`

 

 

 

## TASKTYPE_LOAD

 

This task causes the task executer to load an item from a station. If the task executer's "Travel Offsets for Load/Unload Tasks" checkbox is checked in its Properties page, then it will travel to the location of the given flow item by querying the location from the station and using offset travel. Then the task executer will figure out the load time. At the end of the load time, the task executer will move the item into itself. If loading into a fixed resource, it will notify the fixed resource right before it moves the item, so that the fixed resource can update its own tracking data.

 

**involved1** - the object to load (usually a flow item).

 

**involved2** - the object to load from.

 

**var1** - this is the output port through which the object will exit the station. Usually a 0 is fine.

 

**var2** - The requested end speed for the task.

 

**var3** - N/A

 

**var4** - N/A

 

### Example

 

`ts.addTask(TASKTYPE_LOAD, item, current)`

 

 

 

## TASKTYPE_UNLOAD

 

This task causes the task executer to unload an item to a station. If the task executer's "Travel Offsets for Load/Unload Tasks" checkbox is checked in its Properties page, then it will travel to a drop-off location by querying the station and using offset travel. Then the task executer will figure out the unload time. At the end of the unload time, the task executer will move the item into the station. If unloading from a fixed resource, it will notify the fixed resource right before it moves the item, so that the fixed resource can update its own tracking data.

 

**involved1** - the object to unload (usually a flow item).

 

**involved2** - the object to unload to.

 

**var1** - this is the input port through which the object will enter the station. Usually a 0 is fine, unless you are unloading to a Combiner or a Separator. The Combiner needs to know which input port the item is coming through in order to update its input table. The Separator just needs a value that is greater than 0.

 

**var2** - The requested end speed for the task.

 

**var3** - N/A

 

**var4** - N/A

 

### Example

 

`ts.addTask(TASKTYPE_UNLOAD, item, outobject(current, 1))`   

## TASKTYPE_BREAK

  This task causes the task executer to "break" from its currently active task sequence to a new task sequence as the diagram below illustrates.

 

 

The involved objects and variables allow you to customize how to find the task sequence to break to. In the default case, the task executer will call its "Break To Requirement" function. This function should return a reference to the task sequence that you want the task executer to break to. In your break logic you may search through the task executer's task sequence queue by using task sequence query commands, or you can create the task sequence explicitly using createemptytasksequence. If you don't want the task executer to break at all, then return NULL.

 

**involved1** - If involved1 is specified, then it should be a reference to some object in the model. Instead of calling the "Break To Requirement" the task executer will send a message to this object. The only difference here is the place in which you place your logic for finding a task sequence to break to. By default, the logic executes in the Break To Requirement, but if this parameter is specified, then you will write your logic in a message trigger. Again, the return value of the message should be a reference to the task sequence. You would most likely use this feature if you want to centralize your logic through messages to a central "Model Control Center."

 

**involved2** - If involved2 is specified, then it is interpreted as a straight reference to the task sequence that the task executer should break to. This would only be used if you know exactly which task sequence you want to break to at the time that you create the task sequence with the break in it. This parameter is not used very often, because if you know exactly which task sequence to break to when you create a task sequence, then you can just add the tasks into the original task sequence when you create it.  

#### Using Both Involved Parameters

  If the involved1 parameter of this task is specified, then involved2 should be NULL. Likewise, if involved2 is specified, then involved1 should be NULL. These parameters are mutually exclusive. You can also just use the default case by specifying both of the involved parameters as NULL.

  

**var1** - This parameter specifies whether or not the content of the task executer should be screened before performing the break task. By default (0), the task executer will only perform a break task if its current content is less than the maximum content specified in its Properties page. If var1 is not 0, however, then the task executer will ignore its current content, and perform the break task anyway. This parameter is also passed in as parval(3) if it is to call its Break To Requirement function, and as msgparam(1) if it is to send a message.

 

**var2** - This parameter specifies whether or not the task executer should check to receive task sequences from an upstream Dispatcher. By default (0), the task executer will see if it has any task sequences in its queue. If the queue is empty, or if all of the task sequences in its queue are task sequences that have already been started and broken out of, then it will open its input ports and receive a task sequence from an upstream dispatcher. However, if var2 is not zero, then the task executer will not receive anything from an upstream dispatcher before calling its break logic. This parameter is also passed in as parval(4) if it is to call its Break To Requirement function, and as msgparam(2) if it is to send a message.

 

**var3** - These parameters are passed into the Break To Requirement as parval(5) and parval(6), and var 3 is passed into the message as msgparam(3) if the task is to send a message.

 

**var4** - Same as var3

 

### Example

 

`ts.addTask(TASKTYPE_BREAK, NULL, NULL)` //a basic break. the "Break To" Requirement on the task executer tab will fire

 

`ts.addTask(TASKTYPE_BREAK, centerobect(current, 1), NULL)` //Sends a message to the referenced object, where your logic will be written in the OnMessage trigger of the object

 

`ts.addTask(TASKTYPE_BREAK, NULL, specificTaskSequence)` //breaks to a specified task sequence

 

 

 

## TASKTYPE_CALLSUBTASKS

 

This task is just like the break task, except that it ensures that as soon as the second task sequence is finished, it will return immediately to the next task of the original task sequence. The following illustration shows how this works.

 

As the diagram shows, Task Sequence A comes to a call sub tasks type, upon which it breaks to Task Sequence B. Immediately after Task Sequence B is finished, it returns to the next task of Task Sequence A. Often Task Sequence B won't be created until Task Sequence A actually gets to the call sub tasks task. This is because often when you create Task Sequence A, you don't know exactly what you want the task executer to do when he gets to the point of the call sub tasks task, or you don't have an up-front reference to the objects you need. For example, what if you want the task executer to travel to one part of the model, then load an item, then travel to another part of the model and unload the item, but you won't have a reference to the item that you want to load until the task executer arrives at the other side of the model. In this case, you want the task executer to travel to that portion of the model, then figure out which item to load. Here you would use call sub tasks so that you can resolve the reference to the item at the time the task executer arrives at the load location. Call sub tasks can also be hierarchical. This means that Task Sequence B can also have a call sub tasks type in it.

 

If you decide to create a task sequence when the task executer gets to the call sub tasks task, then the involved object you call sub tasks on will receive a message. In that object's OnMessage trigger, you will need to create a new task sequence using createemptytasksequence(), and insert tasks using inserttask(), but DO NOT dispatch the task sequence, instead, simply return the reference to your newly created task sequence.

  

#### The Break To Requirement Function

 

When the task executer comes to this task type, by default, he will call his "Break To Requirement" function. He will pass in a 1 as parval(2), so that within the function you can tell that it is a call sub tasks instead of the usual break task.

   

#### Coordinated Task Sequences

 

Using the diagram above, if Task Sequence B is a coordinated task sequence, then the task executer that executes the call sub tasks task from Task Sequence A must be the first object to be allocated in the Task Sequence B.

  

**involved1** - If involved1 is specified, then it should be a reference to some object in the model. Instead of calling the "Break To Requirement" the task executer will do one of two things. If involved1 is a reference to an object (a node with object data) then the task executer will send a message from itself to the object specified by the involved1 parameter. If involved1 is a reference to a dll, flexscript, or c++ node (a node with string data) then it will call nodefunction on that node, and it will pass a reference to itself as parnode(1). The only difference here vs. the default case is the place in which you put your logic for finding a task sequence to break to. By default, the logic executes in the Break To Requirement, but if this parameter is specified, then you will write your logic in a message trigger or nodefunction. Again, the return value of the message/nodefunction should be a reference to the task sequence. You would most likely use this feature if you want to centralize your logic through messages to a central "Model Control Center."

 

**involved2** - If involved2 is specified, then it is interpreted as a straight reference to the task sequence that the task executer should break to. This would only be used if you know exactly which task sequence you want to break to at the time that you create the task sequence with the break in it. This parameter is not used very often, because if you know exactly which task sequence to break to when you create the original task sequence, then you should just add the tasks into the task sequence when you create it. It does, however, allow you to specify different priority and preempting values for different portions of your task sequence, so that if you don't want a certain portion of your task sequence to be preempted, then you can have that portion be a sub-routine task sequence with a different priority than the original task sequence.

  

#### Using Both Involved Parameters

 

If the involved1 parameter of this task is specified, then involved2 should be NULL. Likewise, if involved2 is specified, then involved1 should be NULL. These parameters are mutually exclusive. You can also just use the default case by specifying both of the involved parameters as NULL.

  

**var1 - var4** - These parameters are passed into the Break To Requirement as parval(3), parval(4), parval(5) and parval(6). When sending a message, var1, var2, and var3 are passed into the message as msgparam(1), msgparam(2) and msgparam(3) respectively. When calling a nodefunction, var1, var2, var3 and var4 are passed in as parval(2), parval(3), parval(4), and parval(5). In this task type, these parameters are interpreted as legacy doubles, so you will need to use the tonum() command to pass treenodes.

 

### Example

 

`ts.addTask(TASKTYPE_CALLSUBTASKS, NULL, NULL)` // The "Break To" Requirement on the task executer tab will fire

 

`ts.addTask(TASKTYPE_CALLSUBTASKS, centerobect(current, 1), NULL)` // Sends a message to the referenced object, where your logic will be written in the OnMessage trigger of the object

 

`ts.addTask(TASKTYPE_CALLSUBTASKS, NULL, specificTaskSequence)` // breaks to a specified task sequence

 

 

 

## TASKTYPE_UTILIZE

 

This task causes the task executer to go into a given state, and then wait until it is freed from that state with the freeoperators() command. This task is used frequently when you want an operator to "do something" at a station, but at the time you create the task sequence you don't know how long it will take to finish whatever the operator is doing. In such a case, use this task type to cause the operator to go into the state you specify, and then free him when he is finished, using the freeoperators() command. This can be done from a trigger like OnProcessFinish or OnSetupFinish, etc. If you know from the outset how long the operator will have to be "doing something", then you can use the delay task instead.

 

**involved1** - Often this parameter will be a reference to a flow item, if the operator's job has to do with processing a flow item. Sometimes it references a station, for example in the case that a station goes down, and an operator is called. Here, the operator is working on the station, and not a flow item, so the station would be the involved1 parameter. You can even specify this parameter to be NULL if you like. In more specific terms, this parameter is a key for matching with the freeoperators command. For example, if this parameter is a flowitem, then when the freeoperators command is called, the same flowitem must be passed into the second parameter of the freeoperators command in order for the operator to be freed properly. Often you will use a team of operators, any one of which can do the job you want. In such a case you would give the task sequence to a dispatcher, and the dispatcher would give it to a member of the team. At the time you call freeoperators, you really don't know exactly which operator finally came and worked on your job, so you send the freeoperators command to the dispatcher, and in the freeoperators command, you make the second parameter match the involved1 parameter that you specified for this task. This allows the dispatcher to basically say to his team, "Any of you who are doing a Utilize task whose involved1 parameter is this can now finish that task". This makes it so that the dispatcher can free certain operators from the right tasks without freeing other operators from the wrong tasks.

 

**involved2** - This parameter only needs to be specified if it is possible for the operator to be preempted away from his operation. An operator can be preempted away from an operation by a preempting task sequence, or by a stopobject() command, or by a global TimeTable or global MTBF table. If the operator is preempted away from a utilize task, then problems can be caused if the freeoperators command is called before he comes back to the utilize task. If freeoperators is called while he is doing something else, then the operator will simply ignore it, thinking it doesn't apply to him. Then, once he comes back to the operation, he will never be freed because the modeling logic thinks that he's already been freed. This involved2 parameter can be used to help alleviate this problem. If involved2 is specified, then it should point to an object in the model that is responsible for freeing the operator. When the operator is preempted, he will call stopobject() on the specified object, which stop the object, and in most cases thus stop the object from calling freeoperators. Once the operator comes back to the utilize task, he will call resumeobject() on the station, and things will resume as normal, and the operator will eventually be freed. If you would like to know more about preempting, refer to its corresponding help section.

 

**var1** - This is the state into which the operator will go during the utilize task. If it is 0, then the task executer will go into STATE_UTILIZE.

 

**var2** - N/A

 

**var4** - N/A

 

### Example

 

`ts.addTask(TASKTYPE_UTILIZE, item, NULL, STATE_UTILIZE)`

 

 

 

## TASKTYPE_STOPREQUESTBEGIN

 

This task causes the task executer to call stopobject() on the involved1 object. Refer to the stopobject() command documentation for more information.

 

**involved1** - This parameter specifies the object to call stopobject() on. If NULL, then the task executer will call stopobject on himself.

 

**involved2** - Not used. Use NULL for this parameter.

 

**var1** - This is the state to request the stopped object to go into.

 

**var2** - This variable is necessary only if the task executer that executes this task may be preempted. It is also only needed if you are using milestone tasks. If this variable is set to 0, then the task will only be executed once, even if it is within a milestone task's range and the object is preempted within that range. If this variable is set to 1 and the task is within a milestone task's range, then the task will be executed again each time the object is preempted and needs to do the task over again. By default, the value is 0, meaning the task will only be executed once. Note that if it is 0, on the first execution of the command, the task executer will change the variable to 2 as a flag to not execute it again.

 

**var3** - This is the id for the stopobject command

 

**var4** - This is the priority of the stopobject command

 

### Example

 

`ts.addTask(TASKTYPE_STOPREQUESTBEGIN, current)`

 

 

 

## TASKTYPE_STOPREQUESTFINISH

 

This task causes the task executer to call resumeobject() on the involved1 object. Refer to the resumeobject() command documentation for more information.

 

**involved1** - This parameter specifies the object to call resumeobject() on.

 

**involved2** - N/A

 

**var1** - This variable is necessary only if the task executer that executes this task may be preempted. It is also only needed if you are using milestone tasks. If this variable is set to 0, then the task will only be executed once, even if it is within a milestone task's range and the object is preempted within that range. If this variable is set to 1 and the task is within a milestone task's range, then the task will be executed again each time the object is preempted and needs to do the task over again. By default, the value is 0, meaning the task will only be executed once.

 

**var2** - This is the id for the resumeobject() command.

 

**var3** - N/A

 

**var4** - This variable is managed by the task executer, and tells whether this task has already been executed once.

 

### Example

 

`ts.addTask(TASKTYPE_STOPREQUESTFINISH, current)`

 

 

 

## TASKTYPE_SENDMESSAGE

 

This task is deprecated. Use TASKTYPE_MESSAGE instead. This task causes the task executer to send a message to the involved1 object.

 

**involved1** - Involved1 is the object that the message is sent to. If NULL, then a message is sent to the task executer himself.

 

**involved2** - Involved2 specifies msgsendingobject in the message trigger. If NULL, then msgsendingobject is the task executer himself. Usually this will be NULL, because it is the only way that you can access the task executer within the message trigger. However, you may want the message to be sent "from" a different object, so you have the option here.

 

**var1** - This parameter is passed in as msgparam(1) in the message trigger.

 

**var2** - This parameter is passed in as msgparam(2) in the message trigger.

 

**var3** - This parameter is passed in as msgparam(3) in the message trigger.

 

**var4** - This parameter tells whether the message sent is to be a delayed message. If 0, then the message is sent immediately. If -1, then the message is sent delayed in zero time. Otherwise, the message is sent in the specified number of seconds.

 

### Example

 

`ts.addTask(TASKTYPE_SENDMESSAGE, current, NULL, p1,p2,p3, delay)`

 

 

 

## TASKTYPE_MESSAGE

 

This task causes the task executer to send a message to the involved1 object.

 

**involved1** - Involved1 is the object that the message is sent to. If NULL, then a message is sent to the task executer himself.

 

**involved2** - Involved2 specifies msgsendingobject in the message trigger. If NULL, then msgsendingobject is the task executer himself. Usually this will be NULL, because it is the only way that you can access the task executer within the message trigger. However, you may want the message to be sent "from" a different object, so you have the option here.

 

**var1** - This parameter tells whether the message sent is to be a delayed message. If -1, then the message is sent immediately. If 0, then the message is sent delayed in zero time. Otherwise, the message is sent in the specified number of seconds. You might think that delayed message sending is a bit redundant, because if you want to send a delayed message, why not insert a delay task followed by a regular send message task. There is a subtle difference. Say, for example, you want the task executer to wait until a certain number of requirements are met, and the only way you can check those requirements is by executing code. The way that you would do this is, when the task executer gets to the point where he needs to wait for the requirements to be met, he sends a message to some object, and then either does a utilize task or a stop request begin task. When the other object gets the message, he is responsible for checking if the requirements are met. If they are already met, then he is to immediately call resumeobject() or freeoperators() on the task executer. Otherwise he must wait until the requirements are met, and then call resumeobject() or freeoperators(). A problem arises, however, when the requirements are already met and he can immediately allow the task executer to continue. If the message has been sent immediately, then the task executer hasn't started the utilize of stoprequestbegin task yet. He is still working on the send message task. So the other object can't immediately call freeoperators() or resumeobject() because he must wait until the task executer finishes the send message task, and goes on to the utilize or stop request begin. Sending a delayed message in 0 time allows the task executer to do exactly that, and thus allows the other object to immediately free him if the requirements are met.

 

**var2** - This parameter is passed in as msgparam(1) in the message trigger.

 

**var3** - This parameter is passed in as msgparam(2) in the message trigger.

 

**var4** - This parameter is passed in as msgparam(3) in the message trigger.

 

### Example

 

`ts.addTask(TASKTYPE_MESSAGE, current, NULL, delay, p1,p2,p3)`

 

 

 

## TASKTYPE_DELAY

 

This task causes the task executer to go into a given state, and then simply wait for a specified amount of time.

 

**involved1** - Not used. Use NULL for this parameter.

 

**involved2** - Not used. Use NULL for this parameter.

 

**var1** - This is the amount of time that the task executer will wait in the specified state.

 

**var2** - This is the state into which the operator will go during the delay task. If it is 0, then the task executer will remain in the previous state it was in.

 

**var3** - This variable is reserved by the task executer. Do not set this variable yourself, or at least don't expect it to stay the same as what you specified it to be.

 

**var4** - N/A

 

### Example

 

`ts.addTask(TASKTYPE_DELAY, NULL, NULL, time, STATE_NUMBER)`

 

 

 

## TASKTYPE_MOVEOBJECT

 

This task tells the task executer to move a specified object into a specified container. This would be used if you want the task executer to load/unload a flow item without going through the offset travel or the load/unload time. Also, this could be used if you want a flow item to be moved, but not into or out of the task executer.

 

**involved1** - The object to move.

 

**involved2** - The object to move involved1 into.

 

**var1** - The output port of the object that involved1 will exit. 0 is usually fine.

 

**var2** - N/A

 

**var4** - N/A

 

### Example

 

`ts.addTask(TASKTYPE_MOVEOBJECT, item, outobject(current, 1))`

 

 

 

## TASKTYPE_DESTROYOBJECT

 

This task tells the task executer to destroy the specified object. Usually this will be done if a flow item is finished in a model, and is ready to go to a sink. You can destroy the flow item explicitly here. You could also use this to destroy labels. Say for example you have a label that acts as a queue of requests. Once a request has been completed, or is ready to be taken out of the queue, you can destroy it.

 

**involved1** - The object to destroy.

 

**involved2** - Not used. Use NULL for this parameter.

 

**var1** - N/A

 

**var4** - N/A

 

### Example

 

`ts.addTask(TASKTYPE_DESTROYOBJECT, item, NULL)`

 

 

 

## TASKTYPE_SETNODENUM

 

This task causes the task executer to set the value on a specified node. This would be used if you want to set a variable or label on the object.

 

**involved1** - The node to set the value on. This can be something like label(current, "mylabel") or var_s(current, "maxcontent")

 

**involved2** - Not used. Use NULL for this parameter.

 

**var1** - The value to set the node to.

 

**var2** - This parameter allows you to either set the value on the node, or increment the value on the node. By default (0), it will set the value of the node. If 1, then it will increment the value on the node by var1.

 

**var3** - N/A

 

**var4** - N/A

 

### Example

 

`ts.addTask(TASKTYPE_SETNODENUM, theNode, NULL, 42)`

 

 

 

## TASKTYPE_TRAVELTOLOC

 

This task causes the task executer to travel to a specified location using offset travel.

 

**involved1** - Not used. Use NULL for this parameter.

 

**involved2** - Not used. Use NULL for this parameter.

 

**var1** - This is the x location to travel to.

 

**var2** - This is the y location to travel to.

 

**var3** - This is the z location to travel to.

 

**var4** - This is the desired end speed.

 

### Example

 

`ts.addTask(TASKTYPE_TRAVELTOLOC, NULL, NULL, 3,3,3)`

 

 

 

## TASKTYPE_TRAVELRELATIVE

 

This task causes the task executer to travel a specified offset using offset travel. This is like the TravelToLoc task, except that instead of traveling to a location, the task executer offsets from his current location.

 

**involved1** - Not used. Use NULL for this parameter.

 

**involved2** - Not used. Use NULL for this parameter.

 

**var1** - This is the x offset to travel.

 

**var2** - This is the y offset to travel.

 

**var3** - This is the z offset to travel.

 

**var4** - This is the desired end speed.

 

### Example

 

`ts.addTask(TASKTYPE_TRAVELRELATIVE, NULL, NULL, 1,0,0)`

 

 

 

## TASKTYPE_PICKOFFSET

 

This task causes the task executer to execute part or all of the travel offset involved in a load task. This also allows you to sequence the travel operation that the task executer does before doing a load. Let's say, for example, that you have a floor storage area that the task executer is going to pick an item from. The items are organize in bays (x) and rows (y) on the floor. When the task executer arrives at the floor storage area, instead of traveling straight to the product to load it, you want him to first travel in the x direction to the right bay, then travel the y and z offsets to the location of the item. This task type allows you to do this. When the task executer arrives at the floor storage area, you can give him a pick offset task in which you tell to only travel the x portion of the offset. Then you can give him the usual load task, and he will do the y and z offsets once he's finished with the x offset. If you give an object a pick offset task to travel all of the offsets, the effect will be to do the complete travel operation of a load task, without actually loading the object at the end.

 

**involved1** - Just like in a load task this is the reference to the item that would be loaded.

 

**involved2** - Just like in a load task, this is the reference to the station from which the item would be loaded.

 

**var1** - These parameters are usually either a 0 or 1. They correspond respectively to the x, y, and z portions of the offset travel. If 0, then the task executer will travel none of the corresponding offset. If 1, then the task executer will travel all of the corresponding offset. You can also have these values be between 0 and 1. A 0.9 would mean that the task executer would travel 90% of the corresponding offset.

 

**var3** - Same as var1

 

**var4** - This is the desired end speed.

 

### Example

 

`ts.addTask(TASKTYPE_PICKOFFSET, item, current, 1,0,0)`

 

 

 

## TASKTYPE_PLACEOFFSET

 

This task is just like the pick offset task, except that it does part of all of the offset travel involved with an unload task.

 

**involved1** - Just like in an unload task, this is the reference to the item that would be loaded.

 

**involved2** - Just like in an unload task, this is the reference to the station to which the item would be unloaded.

 

**var1** - Same as pick offset task

 

**var4** - Same as pick offset task

 

### Example

 

`ts.addTask(TASKTYPE_PLACEOFFSET, item, outobject(current, 1), 1,0,0)`

 

 

 

## TASKTYPE_TAG

 

This task is exclusively for you to use to "tag" your task sequences. Say for example, that you create 5 general types of task sequences in your model. At certain points in the simulation you need to know which general type a certain task sequence is. By inserting a "tag" task as the first task of all task sequences you create, you can then query that task by using the gettaskinvolved() and gettaskvariable() commands.

 

**involved1** - For your use.

 

**involved2** - For your use.

 

**var1** - For your use.

 

**var4** - For your use.

 

### Example

 

`ts.addTask(TASKTYPE_TAG, current, centerobject(current, 1), 1)`

 

 

 

## TASKTYPE_MILESTONE

 

This task type is only useful for task sequences that may be preempted. It defines a "bookmark" in the task sequence that the task executer can revert back to if it is preempted away from the task sequence. Normally when a task executer is preempted away from a task sequence, it will resume at the same spot it was at once it comes back to the task sequence. The milestone task allows you to tell the task executer to repeat a whole section of tasks if preemption occurs. The task has a defined range of subsequent tasks for which it is responsible. If the task executer is within that range and is preempted, then it will revert back to the milestone task. If it has passed the milestone's range, then it will go back to the default preemption functionality.

  

#### Coordinated Task Sequences

 

The milestone task will not work as a proxy task in a coordinated task sequence. If you want to set bookmarks in a coordinated task sequence, then you should insert a CALLSUBTASKS proxy task, and within the subsequent sub-task sequence, you can insert milestone tasks as needed.

  

**involved1** - Not used. Use NULL for this parameter. **involved2** - Not used. Use NULL for this parameter.  **var1** - This parameter is the range of the milestone task, defined in number of tasks. For example, if var1 is set to 3, and the milestone task is the 5th task in the task sequence, then if the task executer is preempted while executing any one of tasks 6, 7 or 8, then it will revert back to the milestone task.

 

**var2** - N/A

 

**var4** - N/A

 

### Example

 

`ts.addTask(TASKTYPE_MILESTONE, NULL, NULL, 3)`

 

 

 

## TASKTYPE_NODEFUNCTION

 

This task type will call nodefunction() on the specified node.

 

**involved1** - The node to call nodefunction() on.

 

**involved2** - Passed in as parnode(1). If specified as NULL, then when the task executer executes the task, it will pass a reference to itself as parnode(1).

 

**var1 - var4** - These parameters are passed in as parval(2), parval(3), parval(4), and parval(5) in the nodefunction.

 

### Example

 

`ts.addTask(TASKTYPE_NODEFUNCTION, node, NULL, 1)`

 

 

 

## TASKTYPE_STARTANIMATION

 

This task type will call startanimation().

 

**involved1** - Passed as parameter 1 to startanimation(). If NULL, the task executer will pass a reference to itself.

 

**involved2** - Not used. Pass NULL.

 

**var1** - Passed as the animation parameter into startanimation(). Defines the rank of the animation to start.

 

**var2** - Passed as the durationtype parameter into startanimation().

 

**var3** - Passed as the durationvalue parameter into startanimation().

 

**var4** - Not used. Pass 0.

 

### Example

 

`ts.addTask(TASKTYPE_STARTANIMATION, NULL, NULL, 1)`

 

 

 

## TASKTYPE_STOPANIMATION

 

This task type will call stopanimation().

 

**involved1** - Passed as parameter 1 to stopanimation(). If NULL, the task executer will pass a reference to itself.

 

**involved2** - Not used. Pass NULL.

 

**var1** - Passed as the animation parameter into stopanimation(). Defines the rank of the animation to stop.

 

**var2-var4** - Not used. Pass 0.

 

### Example

 

`ts.addTask(TASKTYPE_STOPANIMATION, NULL, NULL, 1)`

 

 

 

## TASKTYPE_FREEOPERATORS

 

This task type will call freeoperators(). This might be used as an alternative or as a supplement to the coordinated task sequence mechanism. It allows you, as part of one task executer's task sequence, to free another task executer from a utilize task.

 

**involved1** - Passed as parameter 1 to freeoperators().

 

**involved2** - Passed as parameter 2 to freeoperators(). If NULL, the task executer will pass a reference to itself.

 

**var1-var4** - Not used. Pass 0.

 

### Example

 

`ts.addTask(TASKTYPE_FREEOPERATORS, centerobject(current, NULL, 1), NULL);`

 

 

 

## TASKTYPE_WAITFORTASK

 

This task type is used if you don't yet have any new tasks to give the object, but you want to disallow the object from finishing the task sequence until you do have tasks to give it. The object will simply wait until the next task is added to the task sequence, and then will finish this task.

 

**involved1** - Should be NULL.

 

**involved2** - Should be NULL.

 

**var1** - Here you can optionally pass a state that you want the object to go into. If 0, STATE_IDLE will be used.

 

**var2-var4** - Not used. Pass 0.

 

### Example

 

`ts.addTask(TASKTYPE_WAITFORTASK, NULL, NULL);`

 

 

 

## TASKTYPE_TE_STOP

  

#### Never Insert This Task Explicitly

 

This task type is for reference purposes only. You should never insert one of these task types explicitly.

  

This task is created when you call stopobject on a task executer. The task executer creates a preempting task sequence of priority 100000, and inserts this stop task into it.

 

**involved1** - Not used. Use NULL for this parameter.

 

**involved2** - Not used. Use NULL for this parameter.

 

**var1** - The state that the task executer should go into when he is down.

 

**var2** - N/A

 

**var4** - N/A

 

 

 

## TASKTYPE_TE_RETURN

  

#### Never Insert This Task Explicitly

 

This task type is for reference purposes only. You should never insert one of these task types explicitly.

  

This task is added onto the end of a task sequence that is returned by a call sub tasks task. It ensures that once the task sequence is finished, it will return to the original task sequence

 

**involved1** - This parameter points to the original task sequence to return to. If the original task sequence is a coordinated task sequence, then this will point to the task sequence with the TE_ALLOCATED task in it.

 

**involved2** - This parameter points to the actual call sub tasks task as a node.

 

**var1- var4** - N/A

 

 

 

## TASKTYPE_TE_ALLOCATED

  

#### Never Insert This Task Explicitly

 

This task type is for reference purposes only. You should never insert one of these task types explicitly.

  

This is a special task specifically used for coordinated task sequences. The task tells the task executer to be allocated meaning when the object comes to this task, it will notify the object coordinating the task sequence, and then simply wait until it is told to do something by that coordinator.

 

**involved1** - The object coordinating the task sequence.

 

**involved2** - This is a reference to the coordinated task sequence being executed.

 

**var1 - var4** - N/A

 

 

 

## TASKTYPE_CT_ALLOCATE

  

#### Never Insert This Task Explicitly

 

This task type is a coordinated task types. You should never insert one of these task types explicitly. You should instead insert them using the insertallocatetask(), insertsynctask(), and insertdeallocatetask() commands.

  

Here the task coordinator will try to allocate some task executer. It is by done by creating a regular task sequence with one TASKTYPE_TE_ALLOCATED task in it, and giving the task sequence to a specified object. This task blocks the continuation of the task sequence until an object has been allocated.

 

**involved1** - This is a reference to a dispatcher to give the task sequence to. It may be a specific task executer, or, if the object to allocate can one of several possible objects, then it can reference a dispatcher that dispatches to those task executers.

 

**involved2** - This is not specified when the task is created, but will be set once the object has been allocated and will reference that object.

 

**var1** - The priority value for the allocation

 

**var2** - The preempt value for the allocation

 

**var3** - This is changed as the task sequence is executed, and is the rank of the front most proxy task that has been given to the allocated resource

 

**var4** - This tells whether the task sequence is blocking. Default (0) is blocking, 1 is non-blocking

 

 

 

## TASKTYPE_CT_SYNC

  

#### Never Insert This Task Explicitly

 

This task type is a coordinated task types. You should never insert one of these task types explicitly. You should instead insert them using the insertallocatetask(), insertsynctask(), and insertdeallocatetask() commands.

  

Here the task coordinator blocks the continuation of the task sequence until some previously specified task (referenced by rank), is finished.

 

**involved1** - Not used. Use NULL for this parameter.

 

**involved2** - Not used. Use NULL for this parameter.

 

**var1** - The rank of the task to sync to.

 

**var2** - N/A

 

**var4** - N/A

 

 

 

## TASKTYPE_CT_DEALLOCATE

  

#### Never Insert This Task Explicitly

 

This task type is a coordinated task types. You should never insert one of these task types explicitly. You should instead insert them using the insertallocatetask(), insertsynctask(), and insertdeallocatetask() commands.

  

Here the task coordinator notifies an object that it can finished its allocated task, and resume to other tasks. The object is specified by the rank of the allocation task that allocated it.

 

**involved1** - Not used. Use NULL for this parameter.

 

**involved2** - Not used. Use NULL for this parameter.

 

**var1** - The rank of the allocation task that allocated the object.

 

**var2** - This variable specifies whether the deallocation task is blocking. Default (0) is blocking. 1 is non-blocking.

 

**var3** - N/A

 

**var4** - N/A

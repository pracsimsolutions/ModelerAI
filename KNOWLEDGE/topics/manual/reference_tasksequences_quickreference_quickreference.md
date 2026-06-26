---
id: reference_tasksequences_quickreference_quickreference
name: "Task Types (Quick Reference)"
kind: manual
breadcrumb: ["Reference","TaskSequences","QuickReference","QuickReference"]
tags: ["manual","reference","tasksequences","quickreference"]
source: "manual/Reference/TaskSequences/QuickReference/QuickReference.html"
---

# Task Types (Quick Reference)

| Task Type | involved1 | involved2 | var1 | var2 | var3 | var4 |
| --- | --- | --- | --- | --- | --- | --- |
| TASKTYPE_TRAVEL | destination | navigator-specific | end speed | navigator-specific | navigator-specific | target travel time |
| TASKTYPE_LOAD | item to load | station | output port | end speed |  |  |
| TASKTYPE_UNLOAD | item to unload | station | input port | end speed |  |  |
| TASKTYPE_UTILIZE | involved | station | state |  |  |  |
| TASKTYPE_DELAY | NULL | NULL | time | state |  |  |
| TASKTYPE_BREAK | send message to | task sequence | check content | check receive | pv(5)* msgp(3)** | pv(6)* |
| TASKTYPE_CALLSUBTASKS | send message to | task sequence | pv(3)* msgp(1)** | pv(4)* msgp(2)** | pv(5)* msgp(3)** | pv(6)* |
| TASKTYPE_STOPREQUESTBEGIN | object to stop | NULL | state | repeat | id | priority |
| TASKTYPE_STOPREQUESTFINISH | object to resume | NULL | repeat | id | already executed |  |
| TASKTYPE_SENDMESSAGE (Deprecated) | to object | from object | msgp(1)** | msgp(2)** | msgp(3)** | delay time |
| TASKTYPE_MESSAGE | to object | from object | delay time | msgp(1)** | msgp(2)** | msgp(3)** |
| TASKTYPE_TRAVELTOLOC | NULL | NULL | x | y | z | end speed |
| TASKTYPE_TRAVELRELATIVE | NULL | NULL | x | y | z | end speed |
| TASKTYPE_PICKOFFSET | item | station | x | y | z | end speed |
| TASKTYPE_PLACEOFFSET | item | station | x | y | z | end speed |
| TASKTYPE_MOVEOBJECT | object to move | container | output port |  |  |  |
| TASKTYPE_DESTROYOBJECT | object to destroy | NULL |  |  |  |  |
| TASKTYPE_SETNODENUM | node to set | NULL | value | increment y/n |  |  |
| TASKTYPE_TAG | user-defined | user-defined | user-defined | user-defined | user-defined | user-defined |
| TASKTYPE_MILESTONE | NULL | NULL | range | N/A | N/A | N/A |
| TASKTYPE_NODEFUNCTION | node | parnode(1) | pv(2)* | pv(3)* | pv(4)* | pv(5)* |
| TASKTYPE_STARTANIMATION | object | NULL | animationnr | durationtype | durationvalue |  |
| TASKTYPE_STOPANIMATION | object | NULL | animationnr |  |  |  |
| TASKTYPE_FREEOPERATORS | object | involved |  |  |  |  |
| TASKTYPE_WAITFORTASK | NULL | NULL | state |  |  |  |

 

* pv = parval

 

** msgp = msgparam

 

The following table explains non-user tasks:

 

| Task Type | involved1 | involved2 | var1 | var2 | var3 | var4 |
| --- | --- | --- | --- | --- | --- | --- |
| TASKTYPE_TE_STOP | NULL | NULL | state |  |  |  |
| TASKTYPE_TE_RETURN | task sequence | task |  |  |  |  |
| TASKTYPE_TE_ALLOCATED | coordinator | task sequence |  |  |  |  |
| TASKTYPE_CT_ALLOCATE | dispatcher | allocated object | priority | preempt | front-most proxy task | blocking, (0)yes,(1)no |
| TASKTYPE_CT_SYNC | NULL | NULL | key or task rank |  |  |  |
| TASKTYPE_CT_DEALLOCATE | NULL | NULL | key or task rank | blocking, (0)yes,(1)no |  |  |

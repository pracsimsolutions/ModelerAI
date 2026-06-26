---
id: modellogic_additionalconcepts_staffresourcepriorities_staffresourcepriorities
name: "HC Staff and Resource Priorities"
kind: manual
breadcrumb: ["ModelLogic","AdditionalConcepts","StaffResourcePriorities","StaffResourcePriorities"]
tags: ["manual","modellogic","additionalconcepts","staffresourcepriorities"]
source: "manual/ModelLogic/AdditionalConcepts/StaffResourcePriorities/StaffResourcePriorities.html"
---

# HC Staff and Resource Priorities

## Overview of Staff Resources and Priorities

 

In FlexSim Healthcare, you have the ability to add different priority levels to staff, location, equipment, and transport resources requests in a patient flow. Priority levels help you determine the best way to allocate resources when there may be a limited supply.

 

 

For example, perhaps staff members have several possible tasks that they could work on at the same time. Priority levels could help the staff members work on more important tasks before they work on less important tasks. You could also use priority levels to allocate locations, transports, or equipment to higher priority patients, such as those with high acuity levels.

 

 

Another important concept related to priority levels is the concept of *preemption*. In simulation model logic, preemption is when one activity is interrupted by an activity that is more important or takes precedence. (See Process Flow Preemption for a more in-depth discussion of preemption in FlexSim generally.)

 

For example, in a hospital emergency room, a doctor might be working on a patient that needs sutures when a gunshot wound victim suddenly arrives. Because the gunshot wound victim has a more life-threatening need, that patient will take precedence over the patient who needs sutures. Therefore, the doctor will be preempted away from the suturing task to work on tasks related to the gunshot wound victim.

 

If a resource is preempted away from another activity, you have to decide what happens when the resource has completed the preempting activity. Will it resume where it left off with the activity that was interrupted? Will it start the activity over from the beginning? Will it skip that activity entirely? These are the kinds of settings you can adjust as needed.

  

#### There's a Tutorial For That

 

If you'd like to see a hands-on example of how the priority and preemption system works for FlexSim Healthcare, check out the FlexSim Healthcare Tutorial - Task 1.6.

  

 

 

## Priority Levels and Tiers

 

FlexSim Healthcare's priority system has two elements: priority levels and tiers. A *priority level* is the number assigned to resource requests when an activity attempts to acquire the resource. If multiple requests are trying to acquire a staff member or location to work on activities, the request with the highest priority level will be able to acquire the resource first.

 

When an acquire resource activity attempts to acquire a resource, the priority level is read from the resource block's Priority field and then assigned to the new resource request. Once the resource is acquired, that priority value is assigned to the acquired resource on its Priority label.

 

Priority levels also have tiers. A *tier* is a range of approximately 100 numbers. When a resource request's priority level is 100 or higher than another priority level, that means it is in a higher tier. Resource requests that are in a higher tier can preempt a resource in a lower tier.

 

For example, if a staff member has been acquired with a priority level of 150, that staff member could be preempted away by a resource request with a priority level of 200 or higher.

 

The following table illustrates the relationship between tiers:

 

| Priority Levels | Tier |
| --- | --- |
| 1-99 | First tier |
| 100-199 | Second tier |
| 200-299 | Third tier |
| 300-399 | Fourth tier |
| Etc. | Etc. |

 

Priority levels 1-99 are in the first tier and are considered to have a lower priority relative to other tiers.

  

#### Priority Level 0

 

Resources that are currently available (and not acquired) have a priority of 0. Thus resource requests must be greater than 0 to properly acquire the resource.

  

 

 

## Key Concepts

 

The following are a few important concepts about the priority system that you should keep in mind:

 

- **You will need to determine what the priority levels mean within your simulation model.** The priority levels are flexible enough to be used in a variety of systems. If your healthcare system uses its own definitions for determining priorities (such as its own acuity system), give some thought about how you could translate your existing system into analogous numerical priority levels in the simulation model.
- **When a resource request has a higher priority level than another request, it will be assigned the resource first.** For example, imagine that there are two requests for a staff member. Perhaps one has a priority level of 150 and the other one has a priority level of 151. Once a staff member becomes available, the staff member will be assigned to the resource request with the priority of 151 first.
- **Resource requests can preempt resources acquired with a lower-tier priority value.** Resource requests will first attempt to acquire resources that are available (their current priority is 0). However, if none are available, the request can preempt a resource that was already acquired with a lower-tier priority value if the resource is tagged as preemptable. Resources are tagged as preemptable if they are listed as a preemptable resource in a process activity's involved resources while the process activity is ongoing.

 

With these concepts in mind, consider planning what priorities you will give to certain activities before designing your patient flows.

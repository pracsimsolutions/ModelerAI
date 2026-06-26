---
id: reference_peopleobjects_tools_peopledownbehaviors_peopledownbehaviors
name: "People Settings"
kind: manual
breadcrumb: ["Reference","PeopleObjects","Tools","PeopleDownBehaviors","PeopleDownBehaviors"]
tags: ["manual","reference","peopleobjects","tools"]
source: "manual/Reference/PeopleObjects/Tools/PeopleDownBehaviors/PeopleDownBehaviors.html"
---

# People Settings

## Overview and Key Concepts

 

People Down Behavior object are pre-configured Down Behavior objects that help provide Shift Schedule functionality. When a Time Table event occurs that uses this Down Behavior a token is created in the Shift Schedule process flow with label values based off the parameters set in the Parameters page. The token in the Shift Schedule flow will attempt to acquire the object going down in order to make them unavailable for acquiring by other things in the model. The parameters on this tab affect various aspects of how the resource is acquired and just how unavailable they are during the break.

 

 

 

 

## Parameters

 

The Parameters tab has the following properties:

  **Condition** - The Condition property allows you to control if the break always happens or only happens under certain conditions.  **Priority** - The Priority value is used when acquiring the resource to put them on break.  **Max Delay** - The Max Delay defines the longest amount of time the break will attempt to acquire the resource for before cancelling the break.  **Break Location** - The Break Location value is where a Staff member will go during a break. It is not used by other resource types.  **State** - The state the resource object will be put into while on break. **Variance** - The Variance value is used to add some extra delay time before the break officially starts.  **Unavailable** - The resource should be marked Unavailable during the break. This will cause requests for this specific object to be redirected to their group.  **Full Break** - If the break is delayed should the break be its full length or should it end at the normal end time. **Preemptable** - If checked the break can be interrupted by requests in a higher priority tier. **Finish Preempted Breaks** - After an interrupted break, if checked the resource will return to taking a break or if unchecked the break will be over. **Color** - This color will be displayed in the time table for events using this Down Behavior. It will also be used to color the box drawn around the resource object while it is on break.

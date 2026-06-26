---
id: reference_peopleobjects_processflowactivities_activitysets_activitysets
name: "Activity Sets"
kind: manual
breadcrumb: ["Reference","PeopleObjects","ProcessFlowActivities","ActivitySets","ActivitySets"]
tags: ["manual","reference","peopleobjects","processflowactivities"]
source: "manual/Reference/PeopleObjects/ProcessFlowActivities/ActivitySets/ActivitySets.html"
---

# Activity Sets

## Overview and Key Concepts

 

 

Activity sets are a set of preconfigured activities bundled in a container that together model a basic people module task. The set contains all the activities necessary to perform a basic task; however, the set is intended to be a starting point for modeling the specific task in your model. You are encouraged to add, remove, and rearrange activities to better suit the task your model is simulating.

 

 

 

## Walk then Process

 

This activity set models a person acquiring a location, traveling to it, spending some time there, and then releasing the location. This could represent, for example, someone signing in at a kiosk when entering a facility.

 

 

The following is an overview of how each activity in the activity set functions:

 

| Activity or Shared Asset | Explanation |
| --- | --- |
| Location | Represents a location object in the 3D model that the person will acquire and interact with in this activity set. |
| Acquire Location | Tokens wait at this activity until a location can be acquired. |
| Walk | The person walks to the acquired location. |
| Process | The person delays an amount of time representing the time required to complete the current process. |
| Release Location | The location acquired at the start of the activity set is released. |

 

 

 

## Wait in Line then Process

 

This activity set models a person waiting their turn in line and then acquiring a location and staff, traveling to the location, spending some time there, and then releasing the staff and location. This could represent, for example, someone waiting in line at a registration desk and then registering.

 

 

The following is an overview of how each activity in the activity set functions:

 

| Activity or Shared Asset | Explanation |
| --- | --- |
| Waiting Line | Points at the waiting line object where the person will stand in line. |
| Location | Represents a location object in the 3D model that the person will acquire and interact with in this activity set. |
| Staff | Represents a staff member in the 3D model that the person will acquire and interact with in this activity set. |
| Wait In Line | The person walks to the waiting line and gets in line. The token stays here until the person reaches the front of the line. |
| Acquire Location | Tokens wait at this activity until a location can be acquired. |
| Acquire Staff | Tokens wait at this activity until a staff member can be acquired. |
| Walk | The person walks to the acquired location. |
| Process | The person delays an amount of time representing the time required to complete the current process. |
| Release Staff | The staff acquired earlier in the activity set is released. |
| Release Location | The location acquired earlier in the the activity set is released. |

 

 

 

## Escort then Process

 

This activity set models a person acquiring a location and staff, being escorted to the location by the staff, spending some time there, and then releasing the staff and location. This could represent, for example, someone being called back by a nurse to check vitals.

 

 

| Activity or Shared Asset | Explanation |
| --- | --- |
| Location | Represents a location object in the 3D model that the person will acquire and interact with in this activity set. |
| Staff | Represents a staff member in the 3D model that the person will acquire and interact with in this activity set. |
| Acquire Location | Tokens wait at this activity until a location can be acquired. |
| Acquire Staff | Tokens wait at this activity until a staff member can be acquired. |
| Escort Person | The acquired staff escorts the person to the acquired location. |
| Process | The person delays an amount of time representing the time required to complete the current process. |
| Release Staff | The staff acquired earlier in the activity set is released. |
| Release Location | The location acquired earlier in the the activity set is released. |

 

 

 

## Transport then Process

 

This activity set models a person acquiring a location, transport, and staff, being transported on the transport to the location by the staff, spending some time there, and then releasing the staff, transport, and location. This could represent, for example, someone being placed on a gurney and taken back to a room in the Emergency Department.

 

 

| Activity or Shared Asset | Explanation |
| --- | --- |
| Location | Represents a location object in the 3D model that the person will acquire and interact with in this activity set. |
| Transport | Represents a transport object in the 3D model that the person will acquire and interact with in this activity set. |
| Staff | Represents a staff member in the 3D model that the person will acquire and interact with in this activity set. |
| Acquire Location | Tokens wait at this activity until a location can be acquired. |
| Acquire Transport | Tokens wait at this activity until a transport can be acquired. |
| Acquire Staff | Tokens wait at this activity until a staff member can be acquired. |
| Transport Person | The acquired staff picks up the acquired transport, walks to the person and then transports the person to the acquired location. |
| Process | The person delays an amount of time representing the time required to complete the current process. |
| Release Staff | The staff acquired earlier in the activity set is released. |
| Release Transport | The transport acquired earlier in the activity set is released. |
| Release Location | The location acquired earlier in the the activity set is released. |

 

 

 

## Move Equipment then Process

 

This activity set models a person acquiring an equipment and staff, the staff bringing the equipment to the person's current location and spending some time there, and then releasing the staff and equipment. This could represent, for example, someone being visited by a nurse to perform an EKG test.

 

 

| Activity or Shared Asset | Explanation |
| --- | --- |
| Equipment | Represents a location object in the 3D model that the person will acquire and interact with in this activity set. |
| Staff | Represents a staff member in the 3D model that the person will acquire and interact with in this activity set. |
| Acquire Equipment | Tokens wait at this activity until a equipment can be acquired. |
| Acquire Staff | Tokens wait at this activity until a staff member can be acquired. |
| Move Equipment | The acquired staff moves the acquired equipment to the patient's location. |
| Process | The person delays an amount of time representing the time required to complete the current process. |
| Release Staff | The staff acquired earlier in the activity set is released. |
| Release Equipment | The equipment acquired earlier in the the activity set is released. |

 

 

 

## Walk or Wait Area

 

This activity set models a person acquiring a location and staff, traveling to the location, spending some time there, and then releasing the staff and location. However, if either the location or staff is not immediately available they will wait in a waiting room until both are acquired. This could represent, for example, someone waiting at the DMV until their number is called.

 

 

| Activity or Shared Asset | Explanation |
| --- | --- |
| Location | Represents a location object in the 3D model that the person will acquire and interact with in this activity set. |
| Staff | Represents a staff member in the 3D model that the person will acquire and interact with in this activity set. |
| Wait Area | Represents a waiting room chair in the 3D model that the person will possibly acquire and interact with in this activity set. |
| Acquire Location | The person attempts to acquire a location. If the location is available the token moves to the Acquire Staff activity. If the location is not available the token moves out the dashed connector to the Acquire Wait Area activity. |
| Acquire Staff | The person attempts to acquire a staff member. If the staff is available the token moves to the Escort Person activity. If the staff is not available the token moves out the dashed connector to the Acquire Wait Area activity. |
| Walk | The person walks to the acquired location. |
| Process | The person delays an amount of time representing the time required to complete the current process. |
| Release Staff | The staff acquired earlier in the activity set is released. |
| Release Location | The location acquired earlier in the the activity set is released. |
| Acquire Wait Area | Tokens wait at this activity until a waiting room chair can be acquired. |
| Walk to Wait Area | The person walks to the acquired waiting room chair. |
| Acquire Location | The person will now be waiting in the wait area. Tokens wait at this activity until a location can be acquired. |
| Acquire Staff | The person will now be waiting in the wait area. Tokens wait at this activity until a staff can be acquired. |
| Release Wait Area | Now that the person has acquired the necessary location and staff, the waiting room chair acquired earlier in the the activity set is released. |

 

 

 

## Escort or Wait Area

 

This activity set works just like the Walk or Wait Area set detailed above except that the person will be escorted by the staff to the location instead of walking there alone.

 

 

| Activity or Shared Asset | Explanation |
| --- | --- |
| Location | Represents a location object in the 3D model that the person will acquire and interact with in this activity set. |
| Staff | Represents a staff member in the 3D model that the person will acquire and interact with in this activity set. |
| Wait Area | Represents a waiting room chair in the 3D model that the person will possibly acquire and interact with in this activity set. |
| Acquire Location | The person attempts to acquire a location. If the location is available the token moves to the Acquire Staff activity. If the location is not available the token moves out the dashed connector to the Acquire Wait Area activity. |
| Acquire Staff | The person attempts to acquire a staff member. If the staff is available the token moves to the Escort Person activity. If the staff is not available the token moves out the dashed connector to the Acquire Wait Area activity. |
| Escort Person | The acquired staff escorts the person to the acquired location. |
| Process | The person delays an amount of time representing the time required to complete the current process. |
| Release Staff | The staff acquired earlier in the activity set is released. |
| Release Location | The location acquired earlier in the the activity set is released. |
| Acquire Wait Area | Tokens wait at this activity until a waiting room chair can be acquired. |
| Walk to Wait Area | The person walks to the acquired waiting room chair. |
| Acquire Location | The person will now be waiting in the wait area. Tokens wait at this activity until a location can be acquired. |
| Acquire Staff | The person will now be waiting in the wait area. Tokens wait at this activity until a staff can be acquired. |
| Release Wait Area | Now that the person has acquired the necessary location and staff, the waiting room chair acquired earlier in the the activity set is released. |

 

 

 

## Process with Staff

 

This activity set models a person acquiring a staff and then having the staff walk to a location and spend some time there before releasing the staff. This could represent, for example, a doctor coming to the patient's exam room or a nurse going into the other room to complete some charting.

 

 

| Activity or Shared Asset | Explanation |
| --- | --- |
| Staff | Represents a staff member in the 3D model will be acquired and utilized in this activity set. |
| Acquire Staff | Tokens wait at this activity until a staff member can be acquired. |
| Walk | The acquired staff walks to the person's location or some other destination. |
| Process | The token delays an amount of time representing the time required to complete the current process. |
| Release Staff | The staff acquired earlier in the activity set is released. |

 

 

 

## Two Staff Meet

 

This activity set models a person acquiring two different staff and then having them meet and spend some time together there before releasing the staff. This could represent, for example, a doctor and nurse meeting in an office to discuss a patient's condition.

 

 

| Activity or Shared Asset | Explanation |
| --- | --- |
| Staff1 | Represents a staff member in the 3D model will be acquired and utilized in this activity set. |
| Staff2 | Represents a second staff member in the 3D model will be acquired and utilized in this activity set. |
| Office | Points at an object in the 3D model where the staff members will meet up. |
| Split | Creates a second token. The first token will go out the bottom connector and the second will go out the connector to the right. |
| Acquire Staff1 | Tokens wait at this activity until the first staff member can be acquired. |
| Staff1 to Office | The acquired first staff walks to the Office object. |
| Acquire Staff2 | Tokens wait at this activity until the second staff member can be acquired. |
| Staff2 to Office | The acquired second staff walks to the Office object. |
| Join | Synchronizes the two tokens that came out of the Split activity. Once both tokens arrive, the token created by the Split activity is destroyed and the original token continues to the Process activity. |
| Process | The token delays an amount of time representing the time required to complete the current process. |
| Release Staff1 | The first staff acquired earlier in the activity set is released. |
| Release Staff2 | The second staff acquired earlier in the activity set is released. |

 

 

 

## Escort to Exit

 

This activity set models a person acquiring a staff to escort them to the exit and then releasing the staff before exiting the model.

 

 

| Activity or Shared Asset | Explanation |
| --- | --- |
| Staff | Represents a staff member in the 3D model that the person will acquire and interact with in this activity set. |
| Exit | Points at an object in the 3D model where the person will exit the model. |
| Acquire Staff | Tokens wait at this activity until a staff member can be acquired. |
| Escort Person | The acquired staff escorts the person to the exit. |
| Release Staff | The staff acquired earlier in the activity set is released. |
| Remove Person | The person is removed from the model. |

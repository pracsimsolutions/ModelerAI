---
id: reference_peopleobjects_processflowactivities_resources_resourceactivities_resourceactivities
name: "People Resource Activities"
kind: manual
breadcrumb: ["Reference","PeopleObjects","ProcessFlowActivities","Resources","ResourceActivities","ResourceActivities"]
tags: ["manual","reference","peopleobjects","processflowactivities"]
source: "manual/Reference/PeopleObjects/ProcessFlowActivities/Resources/ResourceActivities/ResourceActivities.html"
---

# People Resource Activities

## Overview and Key Concepts

 

 

The People Resource Activities facilitate acquiring and releasing resources in a people-based model. For each type of resource (Locations, Staff, Transports, and Equipments) there is a corresponding resources shared asset, acquire, and release activity. A shared asset block is used to represent an object in the 3D model to be acquired. The corresponding acquire activity is used to request the object and waits until the object is acquired. The object is used for a time and then the corresponding release activity is used to release the object.

 

 

These activities use a List in the model to control the acquiring and releasing of resources. Each List in turn uses a Group in the model to define the objects that the List is populated with at the start of the model, and to define all the groups that each object is a member of.

 

 

 

## Resource Properties

 

The following image shows properties for a Resource activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Location / Staff / Transport / Equipment

 

Defines the object to be acquired. A specific object can be referenced using a label, code, or direct reference. Groups can be specified and the activity will attempt to acquire an object that is a member of that group. The object can also be defined using a query just like in a Pull From List activity.

 

Alternatively, this field can be filled with a reference to another people resource of the same type. This makes the current resource a proxy for the referenced resource. Attempting to acquire the proxy resource will actually acquire the reference resource. If you reference another resource in this way the icon will update to a white version of the icon to indicate that it is a proxy resource.

 

 

### Priority

 

Defines the priority of requests when attempting to acquire the specified object. When an object becomes available, it is acquired by the request with the highest priority. If an object is already acquired, but is currently preemptable, another request in a higher priority tier can preempt the resource. Priority tiers are defined as follows:

 

- Tier 1: 0 - 99
- Tier 2: 100 - 199
- Tier 3: 200 - 299
- Tier 4: 300 - 399
- ...

 

For example, a priority 200 request would request an object that was acquired with priority 150 and that is currently preemptable. Objects are only made preemptable during a Process Activity and only if the object is designated as a preemptable resource in the properties of the Process Activity.

 

### Sort By

 

Defines how to sort available objects. For example, entering travelDistance in this field will sort the available objects by the shortest travel distance. By default it sorts in an ascending manner (ASC) such that the object with the lowest value will be chosen first. You can swap the sort order by putting DESC after the sort criteria in order to use a descending sort.

 

### Filter By

 

Defines how to filter available objects. For example, entering Gender = "Female" in this field will filter out any objects that don't have a Gender label with a value of "Female."

 

## Acquire Properties

 

 

### Location / Staff / Transport / Equipment Resource

 

Defines the resource shared asset to acquire from. The object acquired from the shared asset is based on the query value defined on the shared asset.

 

### Use Max Wait Timer

 

The max wait timer will be evaluated if the token has not pulled its required amount after the specified time. See Max Wait Timer for more information.

 

### Assign To

 

Assigns a reference on the specified label/node to the acquired object. See the Assign To section of Common Properties for more information.

 

### Puller

 

Defines the puller object. This may be important if the query accesses fields that are dynamic and based upon the puller.

 

### Same as Last

 

If checked and current token has acquired the shared asset previously, the activity will attempt to acquire the same object that was previously acquired instead of using the query defined on the shared asset.

  

#### Same As Last Example

 

There is a Staff Resource with the Staff field set to "Any member of RNs". The first time the token acquires an object it acquires RN_3 from the RNs group since it requests the first available staff member in the RNs group. The second time this token acquires from this same shared asset it will wait until it can acquire RN_3 and not try to acquire any of the other members of the RNs group.

  

## Release Properties

 

The following image shows properties for a Release activity:

 

 

### Location / Staff / Transport / Equipment Resource

 

Defines the resource shared asset from which you acquired the resource you now wish to release.

 

### Label

 

Where the resource to release is located. The return value of this field should be a label on a token where an earlier Acquire Resource activity assigned the acquired resource to.

 

### Assign Released Resource(s) To

 

Assigns a reference on the specified label/node to the released resource. This can be helpful if you need to maintain a reference to a resource for future use. See the Assign To section of Common Properties for more information.

 

 

 

## Connectors

 

The People Resource Activities only allow one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Groups

 

When any people-based object is added to the model, four Groups are added to the toolbox.

 

 

These Groups are tied to their corresponding List. The purpose of these Groups is to define what objects in the model are available for acquiring and releasing.

  

#### Top level Groups

 

In order to acquire an object using a People Resource Activity, the object must be added, at some level, as a member of the top level Group associated with that resource type. For example, a staff member must be a member of the Staff Group, or a member of a Group whose parent/grandparent/etc. Group is the Staff Group.

  

 

 

## Lists

 

When any people-based object is added to the model, four Lists are added to the toolbox.

 

 

Each set of acquire and release activities and resource shared asset interface with their corresponding list to manage requests for the objects on the list. For example, the Acquire Staff activity acquires from a Staff Resource shared asset which then pulls from the Staff List in order to acquire a staff. When a staff is acquired, its priority column in the list is updated so it is unavailable to other acquire requests unless the staff is in a preemptable state and the new request is of a higher, preempting tier. The Release Staff activity makes the released staff member available to other acquire activities.

 

Each List is tied to one of the top level People Groups. This Group is automatically contained in the List's Initial Contents. When the model is reset all objects contained in the Group are pushed to the list and are then available to be acquired. The Automatically Add Group Fields checkbox is checked so that a Group field is added to the list for each Group containted in the top level Group.

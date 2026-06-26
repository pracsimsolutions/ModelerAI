---
id: connectingflows_lists_keyconceptslists_keyconceptslists
name: "Key Concepts About Lists"
kind: manual
breadcrumb: ["ConnectingFlows","Lists","KeyConceptsLists","KeyConceptsLists"]
tags: ["manual","connectingflows","lists","keyconceptslists"]
source: "manual/ConnectingFlows/Lists/KeyConceptsLists/KeyConceptsLists.html"
---

# Key Concepts About Lists

## Introduction to Lists

 

Lists (which are also sometimes referred to as *global lists*) are a tool that can be used to create more complex flows between 3D objects. However, you should be aware that lists have many other possible purposes in addition to controlling the flow of items in the 3D model:

 

- **Connectionless routing** - Lists can be used in place of ports to connect the flows of fixed resources. This is especially helpful in situations where it might become too unwieldy or difficult to use port connections, such as when you need to do many-to-many routing.
- **Filtering flow items** - Lists can filter and prioritize flow items using a fairly complex set of rules, such as how long the item has been waiting, the item's type, the value of a label on the item, etc. When a fixed resource is ready to pull an item from the list, the list will search for any items that match its search criteria and priorities. For example, a processor could pull items from a list that have a label named *weight* with a value over 50. The list could also prioritize flow items that have been waiting the longest amount of time or which are in the queue with the greatest amount of items.
- **Task management** - Lists can be used in place of a Dispatcher object to assign tasks to task executers. Lists can prioritize which task executer should receive a given task sequence or vice versa. Lists can also ensure that only task executers who meet certain criteria are assigned to work on a particular task sequence or vice versa.
- **Prioritizing fixed resources** - You can also put fixed resources onto a list and prioritize which fixed resources should process a flow item.

 

In the following example, the first queue pushes its flow items onto a list called Available Products. The items wait in the queue until they are pulled from the list by the processors. The processors pull items that match its item type query. For example, Processor1 will only pull type 1 items. Processor2 will only pull type 2 items, etc.

 

 

 

 

## How Lists Work

 

Perhaps you've been to a busy restaurant on a weekend. Typically when you arrive, a host puts your name on a waiting list. The host will monitor the restaurant for a table that matches the size of your party. Once your name is at the top of the list and the host sees there is a matching table, he or she will seat you at the table.

 

 

That process is fairly similar to how lists work in FlexSim. The process begin when something pushes an item, task, or 3D object onto a list. For example, a queue could push flow items onto a list as flow items enter the queue. That item/task/object is then considered a *list entry*. The list can track data about each list entry, such as how long that item/task/object has been on the list, any labels that are associated with it, etc. You can use this data to create custom logic (referred to as a *query*) that will determine which list entries should be pulled from the list first.

 

Eventually, another object (such as a downstream fixed resource or a task executer) will attempt to pull an item/task/object from the list. This object might have additional criteria or restrictions about which type of item/task/object it needs. The list will determine which list entry is the best match for the fixed resource or task executer. When it finds a match, the fixed resource or task executer will pull the matching item/task/object from the list.

 

 

### How Lists Manage Imbalances

 

If there are more items/tasks/objects on the list and there are not enough objects pulling from the list, the list entries will wait on the list until they can be pulled.

 

 

But what if the opposite is true? What if there are more objects that want to pull from the list and there are not enough list entries to meet this demand?

 

Returning again to the restaurant analogy, imagine that you go to the restaurant on a slow day. On slow days, there are many tables available. So in that scenario, a table needs a party. The restaurant host keeps a diagram of the tables that are ready for customers. When a customer arrives, the host determines which table should get the customer based on a few different priorities such as whether the table matches the party size and whether the server in the table's section is ready for a new table.

 

In a similar vein, a fixed resource or task executer might attempt to pull from a list at a moment in time when there aren't any matching item/tasks/objects on the list. When that happens, the list will create a *back order*. A back order is an open request for a list entry that needs to be filled. When a new item/task/object is pushed on the list, the list will evaluate it to see whether it can fulfill any back orders.

 

  

#### Too Many Imbalances Might Indicate Inefficiencies

 

Keep in mind that if there are too many list entries or too many back orders on a list, that could be a sign that your business system is inefficient. You might want to gather statistics from lists to determine if there are too many surpluses or deficits. See Getting Data from Your Model for more information.

  

### Lists are Abstract, Logical Constructs

 

One caution about lists is that you shouldn't think of a list too literally. Don't just assume that because your business system doesn't have some sort of formal mechanism like a list that you won't use them in your simulation model. Keep in mind that lists are abstract constructs that help recreate the implicit logic underlying your business system.

 

For example, sometimes there are situations in business systems in which there are finite resources: maybe not enough employees are available to do all the tasks that need to be done at a given time, maybe not enough workstations are available for the items that need to be processed, etc. In those scenarios, some mechanism is usually in place to determine which tasks or items should take priority. If you are using any kind of logical system besides simple first-in-first-out logic (FIFO), lists can recreate this kind of logic in a simulation model.

 

Another thing to keep in mind is that when an item/task/object is pushed on to a list, it is not literally put on a list. For example, you may have a queue object in your model push an item onto a list. This does not put the item physically or literally onto the list. The 3D item object remains in the queue. Instead, a reference to that item/task/object is added to the list, similar to a host putting your name on a list at a restaurant while you remain in the restaurant waiting area.

 

 

 

## What Gets Pushed Must Get Pulled

 

One of the key concepts to keep in mind while you are designing lists is the concept that "what gets pushed, must get pulled." If an item, task, or object gets pushed onto a list, there needs to be a way for that item, task, or object to get pulled from the list later during the simulation run. If you forget to design some event or mechanism that will pull items, tasks, or objects from a list, they will remain on the list indefinitely. So, every time you create functionality that will push something to a list, make sure you also create the functionality that will pull it from the list as well.

 

 

 

## List Features and Key Terms

 

When you view a list's entries during a simulation run, it will look similar to the following image:

 

 

### List Entries

 

When an item, task, or object is pushed on a list, it becomes an *entry* on that list.

 

### Values

 

The *value* of a list entry is the primary identifier associated with a given list entry. Typically the value will be a reference to an item, task, or object that was pushed on the list. In the example in the previous image, the values are references to boxes inside of three different queues in the simulation model.

 

However, depending on the way you set up your list, the value could also be a number, string, or node reference.

 

### Fields

 

Fields specify which data should be tracked about each list entry. You can use this data to create custom logic (referred to as a *query*) that will determine which list entries should be pulled from the list first. Queries can also restrict and filter which list entries should get pulled.

 

In the example in the previous image, there are three fields:

 

- **itemType** - indicates the item type of each box
- **age** - tracks how long the boxes have been on the list
- **queueSize** - displays the total number of boxes that are currently in this box's containing queue

 

In this example, you could possibly create a list query that prioritizes boxes that are in the longest queue and that have been on the list for the shortest/longest amount of time. The boxes that meet this criteria will get pulled from the list first. You could also restrict a query so that only boxes with a specific item type get pulled.

 

FlexSim is pre-built to track a variety of data, depending on the type of objects that are being pushed to the list. You can use this pre-built data or you can track data in a custom label. If you are comfortable with FlexScript, you can design your own custom expressions to create custom data. It's ultimately up to you to decide which fields you need and which data needs to be tracked so that you can create effective list queries.

 

### Field Values

 

The field values display the current data for each field for a given list entry at a given point in time. In the example in the previous image, the *age* fields show the amount of simulation time units that the boxes have been on the list. According to this list, the box in the first queue has been waiting the longest, for 6.81 time units.

 

 

 

## Types of Lists

 

When you first create a list, you need to decide which type of list you are going to create. There are five types of lists:

 

- Fixed Resource Lists
- Item Lists
- Task Sequence Lists
- Task Executer Lists
- General Lists

 

You should select a type of list primarily based on what is getting pushed onto the list. For example, if you are pushing flow items to the list, you should choose an Item List. If you are pushing a task to the list, you should choose a Task Sequence List. If you're unsure what you want to push to a list, choose a General List.

 

### Do List Types Really Matter?

 

One of the most important aspects of a building a list is to choose the list's fields because they will track data about each list entry. Fields are the building blocks that you will use to create queries to filter and prioritize list items.

 

The type of list you select will affect the kinds of pre-built fields that are available for that type of list on the Fields tab of the List properties window. It will also affect the fields that are included on that list by default. You can easily add, delete, or customize any of the fields available in any list type.

 

You shouldn't worry about which type of list you should choose. All lists ultimately have the same functionality, regardless of what type they are. Even if you choose the wrong type of list, you can change that list so that it can have any of the pre-built fields that are available on the other types of lists.

 

 

 

## Global and Local Lists

 

Whether a list is considered a *global* or *local* list is determined by how accessible it is. Most lists will be globally accessible, meaning that all the objects in a given simulation model can push or pull items/tasks/objects to the list. By contrast, local lists are only accessible to a specific instance of an object in a simulation model. You can set up different instances of a simulation model using the Process Flow tool. For more information, see the following topics and tutorials:

 

- Process Flow Instances
- Advanced Process Flow Tutorials - Tutorial 4 Instances

 

 

 

## List Partitions

 

If needed, you can divide a list into partitions. Partitions are a way of dividing lists into compartments based on some distinguishing factor, called its *partition ID*. For example, imagine your flow items all have a label named *type* on them. This label might have a value ranging between 1 and 10. The type label could potentially act as the partition ID. Items with a type 1 will go into partition 1, items with a type 2 will go into partition 2, etc.

 

You can also create partitions based on a value in a custom label or a field in a list. The partition ID can be a number, a string, or a reference to an object or node in the tree. Partitions are dynamic in that, if you push a value onto a partition that does not yet exist, the partition will be created automatically.

 

Just remember that the rule of "what gets pushed, must get pulled" also applies to pushing and pulling from partitions. If something in your simulation model pushes something onto a partition, something needs to pull it from that partition later during a simulation run.

---
id: reference_tools_globallists_backorders_backorders
name: "The Back Orders Tab"
kind: manual
breadcrumb: ["Reference","Tools","GlobalLists","BackOrders","BackOrders"]
tags: ["manual","reference","tools","globallists"]
source: "manual/Reference/Tools/GlobalLists/BackOrders/BackOrders.html"
---

# The Back Orders Tab

The Back Orders tab is where you define List properties associated with back orders. To get to the Fields tab of a List, double-click on the desired List in the Toolbox. Then select the Back Orders tab.

 

 

This tab has the following properties:

 

### Back Order Queue Strategy

 

The Back Order Queue Strategy defines how back orders will be prioritized for a given value. When a value is pushed onto the List, outstanding back orders are processed for that value in the order defined by this queue strategy. It should be an SQL ORDER BY statement that references at least one puller-dependent field. This is because the back order queue strategy is used to prioritize back orders against a single entry. An ORDER BY statement that gets data only associated with the entry value would consequently have the same result value for all back orders, and would thus be unable to properly prioritize the back orders.

 

A few examples:

 

`ORDER BY Puller.priority DESC` - This will prioritize the pullers who have the "priority" label with the highest value.

 

`ORDER BY pullerPriority DESC` - This example is essentially the same as the previous example, except that here you would need to explicitly add a dynamic puller-dependent expression field named pullerPriority, and give it the expression: `puller.priority`. This expression tells FlexSim to check the value of a label named "priority" on the puller.

 

`ORDER BY distance ASC` - This example will prioritize the back orders with the lowest distance value, which usually defines a distance between the value and the puller.

 

When defining a back order queue strategy, you can also store label values on back orders, and then use a special `BackOrder` keyword in the SQL ORDER BY clause. This allows you to define queue strategies that depend on specific values of back orders. To do this, you first need to store values in back order labels. The following code stores a label named "priority" on a pull result's back order with a value of 4.

 

```
List list = List("List1");
List.PullResult result = list.pull("", 1, 1);
if (result.backOrder)
    result.backOrder.priority = 4;
```

 

Once you have stored label values on back orders, you can access them in the back order queue strategy:

 

`ORDER BY BackOrder.priority DESC`

 

Generally this is not needed since you can store labels on the puller object, and then access them in the back order queue strategy. However, storing and accessing labels on back orders can be useful if the same puller object may have multiple pull requests at the same time, and you want to distinguish between them in the queue strategy.

 

### Unique Pullers Only

 

Check this box to disallow multiple back orders with the same puller. If this is checked and a pull request is made with the same puller as an existing back order, then that pull request will replace the previous pull request.

  

#### Overwriting Back Orders

 

If a back order is replaced by a new pull request using the same puller, the query, request number and require number will also be replaced by the new pull request. Objects that were previously listening for the back order to be fulfilled will continue listening.

  

### Allow Multiple Pushes Before Back Order Fulfillment

 

If this box is checked, the list will wait effectively zero seconds after a push before processing back orders for the pushed value(s). This means that if multiple values are pushed onto the list at the same simulation time, all those values will get on the list before back orders are fulfilled.

 

This setting is useful in several cases. First, if you are pulling from the list with an ORDER BY clause in your pull query, then you will likely want to allow all values onto the list before evaluating the query, so it can consider all values pushed, instead of just pulling as soon as the first one gets on. Second, if you are pulling from the list with a required number less than the requested number, then allowing multiple pushed values onto the list before fulfilling the back orders can get you closer to your requested number.

 

To allow multiple pushes, the list creates an event in effectively zero time after a value is pushed to the list to process back orders. This is "effectively" zero time because the event is not created at exactly the same time as the time the value is pushed. Rather, the event's time will be incremented by the smallest amount representable by a double precision floating point value. This means that all events created at the exact same time as the original push will be executed before the back order fulfillment event, even if those events are created after the original value is pushed. And while the back order fulfillment time is not exactly the same as the push time, it is effectively the same.

 

When this box is checked, the listpush() command will always return null. In other words, since back orders are no longer fulfilled synchronously with pushes, they will never be fulfilled in time for the listpush() command to return a valid value telling who pulled the pushed value.

 

### Reevaluate All Values On Pushes

 

By default, back orders on the list will only evaluate new values that are pushed to the list. Check this box to cause all values (those already on the list and those being pushed to the list) to be evaluated for every back order whenever a new value is pushed to the list. This can be important if there are dynamic fields that may change and cause a value already on the list to be pulled when another value is pushed onto the list.

 

### Sort Back Orders Before Evaluating Entries

 

This setting determines how back orders will be sorted when they are reevaluated for all entries on the list. The default is not checked. Here the list will iterate through each entry on the list, sort back orders associated with that specific entry, then match the highest priority back order to that entry. Alternately, when this box is checked, the list will sort the back orders once, then iterate through each back order and fulfill that back order based on its pull query.

  

#### Leave this Box Unchecked If the Back Order Queue Strategy Relies on Entry-Dependent Fields

 

When this box is checked you are not allowed to include anything in the back order queue strategy that accesses data associated with a list entry. Since the list sorts back orders first for all entries, there is no single entry to compare back orders against.

 

As an example, let's say you are simulating a list of nurses, and back orders for nurses represent the patients who are requesting them. When one or more nurses become available, they may want to sort the requesting patients based on the distance from the nurse to the patient. Such a back order queue strategy might be: ORDER BY distance. In this case the distance field needs both a nurse (an entry) and a patient (a back order) in order to calculate the distance between the two. Here you want the list to go through each newly available nurse, and sort back orders (the requesting patients) by the distance from the nurse to the patient. This desired functionality is enabled when this checkbox is unchecked.

   

#### Check this Box If the Pull Query ORDER BY Clause Should Still Be Used When Multiple Entries Become Available at the Same Time

 

When this box is checked, the list will sort the set of back orders once, then go through each back order and attempt to fulfill it based on its pull query. This scenario allows for the back order's pull query to still be in effect when multiple entries become available at the same time. In going through each back order, the list will evaluate the back order against all of the available entries, meaning the back order pull query's ORDER BY clause will take effect, and available entries will be sorted accordingly.

  

### Back Order Reevaluation Events

 

This pane lets you define when back orders should be reevaluated. This is usually only needed if in your pull request you have a WHERE clause whose result may change while the back order is active. For example, consider the following query: `WHERE queueSize <= 5`. If the pull request is made at a point when queueSize is greater than 5, then you may want to reevaluate that pull request when queueSize becomes less than 5. FlexSim doesn't automatically know when this condition may change, so you need to define events that will trigger reevaluation.

 

To add a reevaluation event, click on the button, and choose an event to add. Then choose the **Event Type** you want to listen for.

 

There are several categories of reevaluation events, as follows:

 

- **Puller Event** - A puller event is an event that happens on the puller. Here the puller must be an object (not a number or string). When a back order is created, the List will listen for the defined event on the puller object, and when the event fires, the back order will be reprocessed for all values in the list.
- **Value Event** - A value event is an event that happens on the value, i.e. the primary value of an entry. Here the value must be an object (not a number or string). When the value is pushed to the List, the List will listen for the defined event on the value object, and when the event fires, all back orders will be reprocessed for that entry.
- **Global Event** - A global event is some event that is not associated with either values or back orders. The List will listen for this event and will reprocess all back orders whenever the event happens.
- **Time Interval** - A timer interval event will cause the List to repeatedly reevaluate back orders at a set time interval. When the first back order is added, the List starts the timer loop and reevaluates back orders after the given time elapses. The timer loop ends when the back order queue is emptied.

---
id: reference_tools_globallists_general_general
name: "The General Tab"
kind: manual
breadcrumb: ["Reference","Tools","GlobalLists","General","General"]
tags: ["manual","reference","tools","globallists"]
source: "manual/Reference/Tools/GlobalLists/General/General.html"
---

# The General Tab

The General tab is where you define general properties for a List. To get to the General tab, double-click on the desired List in the Toolbox. Then select the General tab.

 

 

This tab has the following properties:

 

### Cache Queries

 

If checked, the List will cache parsed queries. This means that if you use the same query multiple times, the query will only be parsed once instead of each time the pull request is made. This can increase simulation run speed, but does require some small memory overhead, depending on how many unique queries are made in a simulation.

 

### Keep Empty Partitions

 

If checked, the list will never remove partitions, even if they are empty. This is useful if you want to keep statistics on a specific partition. However, depending on how many unique partitions are created on the list, this can use significant amounts of memory.

 

### Unique Values Only

 

If checked, the List will disallow multiple entries with the same value. If a value is pushed when there is already an entry with that value, it will replace that value.

 

### Track Number Field Totals

 

If checked, the list will keep statistics on the total (sum) field values of all entries for non-dynamic number fields. This can be used if you want to either listen for changes to a field's total, usually using Process Flow, or if you want to use it to gather model statistical results.

 

### Automatically Add Group Fields

 

If checked, the list will add a field for each group that is part of the list's **Initial Content**, as well as for every group contained by those groups. Entries will have a corresponding field value of 1 if the object is a member of a given group, and 0 if it is not a member of the group. This allows you to easily define pull queries that are based on an object's membership in a group.

 

### Always Leave Entries on List

 

If checked, pushed entries will never be removed from the list, even when pulled.

 

### Assign SELECT Values to Puller Labels

 

If checked, pull queries with a SELECT clause will cause the list to assign the corresponding values to the puller's labels.

 

### Use First SELECT Value as Quantifier

 

If checked, the first expression in the SELECT clause has a special meaning that defines a field/label that quantifies the request and require numbers for pull requests.

 

### View Entries...

 

Press View Entries to open list's entry viewer.

 

### View Back Orders...

 

Press View Back Orders to open list's Back Order Viewer.

 

### Initial Content

 

The set of controls under Initial Content allow you to define a set of values to be pushed onto the List when the simulation starts. Use the buttons above to add, remove and reorder these values. You can add objects individually, or you can associate the initial content with a model group, so that the List's initial content will always be synced with the objects you specify in the model group.

---
id: list
name: "List"
kind: class
module: ""
signature: "List"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class that represents a list."
seeAlso: []
source: "help\\FlexScriptAPIReference\\List\\List.xml"
---

# List

A class that represents a list.

Lists are a powerful tool that can have many applications in FlexSim. At the most basic level, a List is
just that: a list of values. Each value in a list may be either a number, a string, or a reference to an object
in the simulation model. Lists also harness the expressive power of SQL for searching, filtering and prioritizing the
values on the List. Entries in a List can have user-defined fields. These fields essentially make the List into a
dynamic database table. The List can then be filtered and prioritized using SQL queries, as if you were querying
a standard database table.

## Methods

### backOrders

```flexscript
List.backOrders(Variant partitionID = 0)
```

Gets an array of the back orders on a certain partition.

**Parameters:**
- `partitionID` *(Variant, default 0)*

This array is read only, but you can query the length of the array as well as accessing individual
back orders to query their data. See the List.BackOrder class for more information.
List("ItemList").backOrders().length
List("ItemList").backOrders(3)[1].puller

### entries

```flexscript
List.entries(Variant partitionID = 0)
```

Gets an array of the entries on a certain partition.

**Parameters:**
- `partitionID` *(Variant, default 0)*

This array is read only, but you can query the length of the array as well as accessing individual
entries to query their data. See the List.Entry class for more information.
List("ItemList").entries().length
List("ItemList").entries(3)[1].value

### getEntryFromValue

```flexscript
List.Entry List.getEntryFromValue(Variant value, Variant partitionID = 0)
```

Returns the entry associated with a value.

**Parameters:**
- `value` *(Variant)* — The value that was pushed to the list.
- `partitionID` *(Variant, default 0)* — The ID of the partition the value was pushed to.

Searches the partition for an entry that is associated with the value. If there
is no entry associated with the value the method returns null. If Unique Values Only is set to false this method
will always return null.

List stations = List("StationList");
List.Entry entry = stations.getEntryFromValue(current);
stations.reevaluateBackOrders(entry);

### List

```flexscript
List List.List(string name)
```

Creates a reference to a global list.

**Parameters:**
- `name` *(string)* — The name of the global list.

List itemList = List("ItemList");

### pull

```flexscript
List.PullResult List.pull(string sqlQuery, double requestNum, double requireNum = 0, Variant puller = nullvar, Variant partitionID = nullvar, int flags = 0)
```

Attempts to pull values from the list.

**Parameters:**
- `sqlQuery` *(string)* — An sql query that defines what you would like to pull from the list.
- `requestNum` *(double)* — The maximum number of things you want to pull from the list.
- `requireNum` *(double, default 0)* — The minimum number of things you want to pull from the list.
- `puller` *(Variant, default nullvar)* — The thing pulling from the list.
- `partitionID` *(Variant, default nullvar)* — The ID of the partition you want to pull from.
- `flags` *(int, default 0)* — Flags that modify the behavior of the pull.

Pulls things from the list that meet the requirements in the sql query.
Returns a List.PullResult.
List.PullResult pullResult = List("ItemList").pull("", 1, 1);
Object item = List("ItemList").pull("WHERE type = 3 ORDER BY age", 3, 2, current, current.station);
sqlQuery - Defines the SQL query to use in pulling. The SQL query should only use the WHERE, ORDER
BY, SELECT or LIMIT clauses. By default, field names correlate with object label names. For example, if your
query is
WHERE type > 5 ORDER BY priority DESC
then the list will search for objects on the list who have
a "type" label with a value greater than 5, and will order by objects' "priority" label value in descending
order, i.e. it will prioritize higher "priority" label values. If you need more flexibility than just labels, you
can add to the list's defined fields through its properties window.
requestNum - Defines the number of entries that you are requesting to pull from the list. If requestNum
is greater than zero, then entries that satisfy the query will be immediately removed from the list. If
requestNum is zero, the list will do the query and return the result, but it will not remove any satisfying
entries. If you want to query for a limited number of entries, but you also do not want to remove them, pass 0 as
the requestNum, and use the LIMIT clause in the pull query.
requireNum - Defines the number of entries that you require to be pulled from the list. This must be
less than or equal to requestNum. If requireNum is greater than zero and the required number is not met, then a
back order will be created for the pull request, and as items are pushed onto the list, they will be pulled off
by the back order until the back order's requireNum is met, at which point the back order will be removed. If no
back order queue strategy is defined on the list, back orders are fulfilled in FIFO order assuming they do not
allocate all-or-nothing.
If a back order is created and you would like to be notified when the back order is fulfilled, use the
eventlisten() command as follows:
eventlisten(pullResult.backOrder, "OnFulfill", callbackNode, 0, ...)
This will cause callbackNode's code to be evaluated when the back order is fulfilled. If the back order may
be incrementally fulfilled multiple times, the callbackNode's code will be evaluated each time a partial
fulfillment is made. Additional parameters (the ... optional parameters) may be defined for callbackNode's
param(1), param(2), etc. You may replace these parameters with one of the following macros to have that parameter
be replaced with a fulfillment-dependent value when the callback is made.
LIST_ON_FULFILL_VALUE - The value of the entry or entries that were pulled.
LIST_ON_FULFILL_NUM_FULFILLED - The quantity that was fulfilled as part of this fulfillment. Usually 1 unless the query has a SELECT clause.
LIST_ON_FULFILL_TOTAL_FULFILLED - The total quantity fulfilled for the back order up to this point.
LIST_ON_FULFILL_NUM_REQUESTED - The number requested initially when the pull request was made.
LIST_ON_FULFILL_NUM_REQUIRED - The number required initially when the pull request was made.
For example, if you use the command:
eventlisten(pullResult.backOrder, "OnFulfill", callbackNode, 0, LIST_ON_FULFILL_VALUE)
Here, when a fulfillment is made, param(1) of callbackNode's code will be replaced with the value for the
entry that satisfied the fulfillment.
puller - Defines who or what is pulling the entries from the list. This can be 0. Reasons to use a
puller may be:

You can access the puller's labels in the query. For example, a query of WHERE Type = puller.Type
would match entries whose "Type" label is equal to the puller's "Type" label.
The list's pushers can know who is pulling them off the list. The push() method will return the back order
pullers who pulled the item off the list.
If you use the list's custom query fields, some require a valid puller to evaluate properly.

partitionId - Defines the partition id to pull from. Using a partitionId will make the list act like
multiple separate lists, each uniquely identified by its partitionId. If you pushed an entry onto the list with a
defined partition id, you should pull the entry off with the same partitionId.
flags - Defines behavior flags for pulling. Valid values may be one or more of the following:

LIST_PULL_ALL_OR_NOTHING The pull request will only pull entries if it can pull all
required entries. If this value is not specified, the pull/back order will pull entries off the list as they are
available.
LIST_PARSE_QUERY Instead of actually doing a pull operation, the list will
simply parse the defined query. It will return a reference to a node that holds the cached query. This node can
subsequently be passed as the query parameter to subsequent pull() calls. This can improve simulation run speed
because it only has to do the parsing once, instead of every time you call pull(). Ownership of the node is given
to the caller, i.e. it is your responsibility to store and later delete the node once you're done with it. You
should use transfernode() to move the node where you can access it.
LIST_PULL_ENTRY_NODES Instead of returning the pulled values, the command will return
the set of entry nodes that reference those pulled values. Primarily this is useful only if you need to know the
time that the entries were pushed onto the list (getsdtvalue(entryNode, "pushTime")). The list uses the same
rules of whether to remove the entries from the list (requestNum > 0), but in this case when it removes
them, it will put them into a temporary location and return them instead of destroying them. You are required to
destroy them yourself (if they were removed) once you have the needed information.
LIST_DO_NOT_REMOVE Matching entries on the list will be returned by this command,
however, the entries will remain on the list. This is useful when creating a directory or routing that
doesn't dynamically change.
LIST_DO_NOT_BACK_ORDER If the number of matching entries is less than the required
number, no back order will will be created.
LIST_PULL_BACK_ORDERS Instead of querying the entries on the list, this command will
query the back orders and return a list of all back orders that matched the specified query. If the query
contains an ORDER BY, the back orders will be sorted. This will also reevaluate the back orders and will
remove any back orders that are fulfilled.
LIST_DO_NOT_FULFILL Used in conjunction with LIST_PULL_BACK_ORDERS, this will cause the
back orders to not be reevaluated or removed. This allows you to purely query the list's back orders or to sort
back orders without fulfilling any back order.
LIST_RETURN_BACK_ORDER_IF_NOT_FULFILL Use in conjunction with LIST_PULL_ALL_OR_NOTHING.
If the pull is unable to be fulfilled, a back order will be created. A reference to this back order node will be
returned by the pull() method. This can then be used to add a listener for the onFulfill of the backorder using
eventlisten().

Return Value - If the pull is successful, the command will return a List.PullResult
which is a Variant holding the pulled value(s). If there are multiple values pulled, they will be in an Array.
If the pull is not completely successful and a back order is created, you can access the created back order on the
pull result.
pullResult.backOrder

### push

```flexscript
List.PushResult List.push(Variant value)
```

Pushes a value on the list.

**Parameters:**
- `value` *(Variant)* — The value to push on the list.

Puts the value on the list making it available to anyone who can pull from the list.
Returns a List.PushResult.
List.PushResult pushResult = List("ItemList").push(item);
If partitionId is specified then the list will act like multiple lists, each list uniquely identified by its
partitionId. When you push a value onto a list with a given partitionId, you must later pull the value off
the list by using the same partitionId.
List("ItemList").push(item, item.type);
Pushing a nullvar value will just re-evaluate all back orders without pushing anything onto the list.
List("ItemList").push(nullvar);
When you push the item onto the list, if it is immediately pulled by a back order, then the push result
will contain the puller(s) who pulled the item off the list. Otherwise it contains 0. To access the node
that represents the entry pushed onto the list use the entries
property of the returned List.PushResult.
List.Entry entry = List("ItemList").push(item).entries[1];
You may also push an array of values onto the list. This is similar to just looping through each value and
pushing it onto the list, except that all entries will be pushed onto the list BEFORE evaluating back orders.
This means that back orders with all-or-nothing pull requests for multiple entries can be fulfilled before
they are "stolen" away by back orders without all-or-nothing requests or who are requesting fewer entries.

### reevaluateBackOrders

```flexscript
void List.reevaluateBackOrders()
```

Reevaluates back orders against all entries or a specific entry.

If an entry is specified, all the back orders in the same partition will be reevaluated against
the enty. If no entry is specified, all back orders in every partition will be reevaluated against all the entries
in the same partition.
List("ItemList").reevaluateBackOrders()
List("ItemList").reevaluateBackOrders(List("ItemList").entries()[1])

## Properties

### stats

```flexscript
List.stats (readonly)
```

Allows access to an object's statistics.

Allows you to access the statistics of a List as TrackedVariable objects.
This property provides the following properties and methods:

Properties
input - returns the input tracked variable
output - returns the output tracked variable
content - returns the content tracked variable
staytime - returns the staytime tracked variable
backOrderInput - returns the input tracked variable for backOrders
backOrderOutput - returns the output tracked variable for backOrders
backOrderContent - returns the content tracked variable for backOrders
backOrderStaytime - returns the staytime tracked variable for backOrders

// Get the input of a List
List("MyList").stats.input.value;

Methods
fieldTotal(Variant field)
fieldTotalInput(Variant field)
fieldTotalOutput(Variant field)
partitionBackOrderContent(Variant partition)
partitionBackOrderInput(Variant partition)
partitionBackOrderOutput(Variant partition)
partitionBackOrderStaytime(Variant partition)
partitionContent(Variant partition)
partitionInput(Variant partition)
partitionOutput(Variant partition)
partitionStaytime(Variant partition)


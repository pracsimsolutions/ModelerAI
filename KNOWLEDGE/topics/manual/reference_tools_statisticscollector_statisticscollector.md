---
id: reference_tools_statisticscollector_statisticscollector
name: "The Statistics Collector"
kind: manual
breadcrumb: ["Reference","Tools","StatisticsCollector","StatisticsCollector"]
tags: ["manual","reference","tools","statisticscollector"]
source: "manual/Reference/Tools/StatisticsCollector/StatisticsCollector.html"
---

# The Statistics Collector

## Overview and Key Concepts

 

The Statistics Collector is a table that records data from the model. The Statistics Collector listens to events in the model and writes data in its table cells when those events occur. You can configure which events the Statistics Collector listens to, as well as the data that it writes with those events. You can use the table data in a chart, or you can export the data from FlexSim.

 <!--Screenshot of the UI on the first tab here --> 

 

The Statistics Collector can be accessed from the Toolbox. A model can have any number of Statistics Collectors.

 

### General Behavior

 

The Statistics Collector is a table, similar to a Global Table; it is a table that is accessible in the toolbox, and you can write custom values in that table. However, Global Tables are generally used to store input data, or data that the model uses to determine its behavior. Statistics Collectors, on the other hand, record data about the model's behavior as it runs. You can think of Global Tables as input tables and Statistics Collectors as output tables.

 

The Statistics Collector is an observer of the model. When you use a Statistics Collector, you specify what events you want to observe, and what to record when those events happen. Then, as the model runs, the Statistics Collector will listen for the specified events to happen, and update the table when those events occur.

 

The following subsections describe the basic logic of the Statistics Collector. The last subsection (Visualizing the Statistics Collector as a Flowchart) shows two flowcharts. One shows the logic that executes if an observed event fires. The other shows the logic that executes if the table is accessed. It may be helpful to read these subsections, view the flowcharts, and then read these subsections again.

 

### Adding Rows to the Table

 

In order to add rows to the table, you must specify events to listen to. For example, the following Statistics Collector listens to two events:

 

 

Whenever an event in this list occurs, the Statistics Collector creates an entity called `data`. This entity has information about the event such as which object fired the event, and the name of the event. You can also assign labels to this entity. In the previous example, the event parameters *item* and *port* are assigned to `data` as label values.

 

Each event also specifies a **row value**. The Statistics Collector maps each row value to a row. If there is not yet a row for the given row value, then the Statistics Collector will add a row for that value. In the previous example, the event uses `data.item` as the row value. This means that whenever an item enters the Queue, the Statistics Collector will make sure there is a row in the table for that item. In addition, the row value given by the event is assigned to the `data` entity's `rowValue` property.

 

In summary, when and event occurs, it produces a row value. If the Statistics Collector doesn't have a row for that row value, it will add a row to its table. The new row is then tied to the row value. In this example, the row value is a flowitem. However, the row value can be any Variant value. More information can be found in the Row Values section.

 

### Adding Columns to the Table

 

In order to add columns to the table, you specify them directly in the Statistics Collector, using the Columns tab. For example, the following Statistics Collector would have two columns:

 

 

When you add a column, you need to specify a name. You also need to specify what value to write in the column when the column is updated. The **Row Add Value** specifies the value that the column should write when a new row is added. When an event produces a row value that doesn't have a row, the Statistics Collector will make a new row, and fill all columns with their Row Add Value.

 

In this example, the Row Add Value for the Type column is set to `data.rowValue.Type`. Recall that in this example, `data.rowValue` is a flowitem. So when the row is added, the Type column will record the value of the Type label on the flowitem for that row.

 

### Updating Values in the Table

 

You can update values in the table in three ways. The first way is by defining a **Row Add Value**. This allows you to write a value when the Statistics Collector adds a row, as discussed in the previous section.

 

The second way is by connecting an event to one or more columns. When the event occurs, the Statistics Collector will assert a row for the event's row value. Then, the Statistics Collector will visit each of the connected columns on that row, and write the value specified in the **Event Value** field. In the following example, the "Processor1 - On Exit" event is connected to the Staytime column:

 

 

In this example, when the "Processor1 - On Exit" event occurs, the Statistics Collector will assert a row for the row value, writing the **Row Add Value** for each column if a row is added. Then, on that same row, the Statistics Collector will write the **Event Value** for each connected column. In this example, the event is connected to the Staytime column, which is set to `data.Staytime`.

  

#### What is `data.Staytime`?

 

You can set custom labels on the `data` entity. The example above does that. You will learn how to set custom labels later. For now, all you need to know is that that value will be written in the Staytime column when something exits Processor1.

  

The third way you can update a column is by changing the **Update Timing** of the column. There are two options for the Update timing: 

- When the row is added and/or updated by events
- When the value is accessed

 If you set this property to the second value, then the column will not be updated by events at all. Instead, the column will be updated when it is accessed, or viewed. For example, if you are viewing the Statistics Collector's table, this value will be updated every time the view refreshes. Or if you are using this Statistics Collector in a chart, and that chart is visible, this value wil be updated before the chart is repainted. This option for Update Timing can be used to show values that change continuously, such as average content of an object, or time in a particular state.

 

### Sorting Rows

 

By default, when the Statistics Collector adds a new row, the row will be added at the end of the table. However, you can specify a custom sort order. To do this, you can add one or more Sort Values:

 

 

The previous example demonstrates that you can access the row value during the sort step. However, there are some important rules about Sort Values: 

- **Sort Values shouldn't change during the row's lifetime.** The sort values are cached, and compared to the other cached values for the other rows.
- **Sort Values cannot depend on column values.** None of the columns have been written at the point the row is sorted, so they cannot use column values. In some cases, a Sort Value might have the same value as a Row Add Value.

 

### Finishing a Row

 

When an event occurs, you can optionally specify that the event should finish the row for its row value, after the event has added or updated that row. If an event finishes a row, then two things happen to that row: 

- All columns that update when accessed are updated
- The row value is disconnected from the row

 Once a row is finished, it will no longer be updated, either with an event, or by accessing the value. The row is then said to be finished. If a later event produces the same row value, the Statistics Collector will add a new row for the row value.

 

### Visualizing the Statistics Collector as a Flowchart

 

Because the Statistics Collector runs logic in response to events, it can be helpful to see that logic as a flowchart. The following flowchart is made with Process Flow activities, where each type of activity has as specific meaning: 

- - Something happens to start update logic
- - Code or values you have specified
- - A decision checked by the Statistics Collector
- - An action taken by the Statistics Collector
- - Indicates (using a dashed arrow) that some steps happen for each thing
- - The end of the update process.

 In addition, it can be very helpful to think of the `data` entity as a token moving through the flowchart.

 

If one of the Statistics Collector's events occurs, this flow approximates the logic:

 

 

If something accesses the Statistics Collector's table, this flow approximates the logic:

 

 

 

 

## Row Values

 

This section describes the different kinds of row values that can be returned from an event's **Row Values** property, and how they might be used in a Statistics Collector.

 

### Number, Text, and Node Values

 

The most common kind of row values are number values, text values, or node values. The Statistics Collector will make one row per unique text, number, or node value.

 

### Arrays of Values

 

If an event's **Row Values** field returns an array, the Statistics Collector will extract every element of that array as an individual row value. In this way, you can have a single event add and/or update multiple rows. For example, `[1, 2, 3]` will assert one row for the values 1, 2, and 3. However, and array of a single value will only update one row; `1 == [1]` for row values.

 

### Array Row Values

 

In some cases, it can take several values to describe a row value. For example, you may want to create a table that has one row per Fixed Resource per SKU (a label on items flowing through the Fixed Resource). In this case, the row value is a composite value of Fixed Resource and SKU.

 

You can accomplish this using an array. However, since the Statistics Collector uses an array to mean multiple row values, you must put the array inside an array. For example, if an event returns a row value like this:

```
[[current, data.item.SKU]]
```

then the Statistics Collector will take each element of the outer array as a row value. This leaves the inner array as the row value. The Statistics Collector will make one row per unique array, where every element of the array must match. You can access elements of that array in later fields with array syntax: `data.rowValue[1]`

 

### Null Values

 

You can return a null value as the row value from an event. This will always add a new row to the Statistics Collector, and that row will always be finished after the event.

 

### Enumerated Values

 

To understand enumerated values, you need to first recall how the Statistics Collector uses the `data` entity. Whenever one of the events in the Statistics Collector occurs, the `data` entity is created. In addition, the Statistics Collector sets the following properties on the `data` entity: 

- `data.group` - If a single event listens to all objects in a group, this property will be set to that group. Otherwise, it is empty.
- `data.eventNode` - This value is set to the node that fired the event. This value is also known as `current`.
- `data.processFlowInstance` - If an event is listening to an activity in an instanced flow, this property is set to the instance caused the event. This property is also known as `instance`.

 

All events set these three properties. You can return the following from any event as the row value:

 

```
data.enumerate()
```

 

This method returns a special value. When the Statistics Collector sees this special value as the row value, it asserts one row per unique combination of

 

```
[data.group, current, instance]
```

 

It also sets `data.rowValue` to `current`.

 

There is a difference between using `current` as the row value and using `data.enumerate()`. While both cases set `data.rowValue` to be the same value in the end, rows made with `data.enumerate()` also remember the instance and group used to make the row.

 

This difference is important when using columns that update when accessed and need the `instance` value. For example, suppose you want to see the average content of a Delay activity in an instanced flow for each instance. When you access the table, there is no event, and so normal rows cannot access `current`, `instance`, or `data.group`. However, since enumerated rows remember these values, you can use them in code like the following:

 

```
getstat(data.rowValue, "Content", STAT_CURRENT, instance)
```

 

 

 

## Events

 

For information on events, see the Event Listening page.

  

#### Event Disambiguation

 

The events in this section refer to the events that the Statistics Collector generates, not the events that it listens to.

  

The Statistics Collector has the following events:

 

### On Row Added

 

This event fires when a new row is added to the Statistics Collector, after the row is sorted, and after the Row Add Values have been set.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Row Number | int | The row number of the new row |
| Row Value | Variant | The value associated with the new row |

 

### On Row Updated

 

This event fires when one of the events that the Statistics Collector listens to updates one or more columns on a given row.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Row Number | int | The row number of the new row |
| Row Value | Variant | The value associated with the new row |
| Columns Updated | Array | An array containing one value per column. If the nth column was updated, then the nth value of the array will be 1. Otherwise, the value will be zero. |
| Previous Values | Array | An array containing one value per column. If the nth column was updated, then the nth value of the array will contain the value that was in the table before the update. Otherwise, the value is undefined, and should not be used. |

 

### On Row Finished

 

This event fires when an event finishes a row. Any columns updated by the event will be updated before this event fires.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Row Number | int | The row number of the finished row |
| Row Value | Variant | The value associated with the finished row |

 

### On Row Adding

 

This event fires when the Statistics Collector needs to add a new row, but has not actually added the row yet. The trigger associated with this event is often used to initialize row labels on the `data` entity.

 

This event has no parameters.

 

### On Row Updating

 

This event fires when the Statistics Collector is about to update any columns connected to an event. The trigger associated with this event is often used to update row labels on the `data` entity.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Event Added Row | int | If the event that is currently causing an update also added the row, this value is true. |
| Event Finishes Row | int | If the event that is currently causing an update will also finish the row, this value is true. |

  

#### Understanding Statistics Collector Events

 

The descriptions about the events and event parameters assume that you have some previous knowlege about the Statistics Collector. For more information on Statistics Collectors, see Using the Statistics Collector.

  

 

 

## Properties

 

The Statistics Collector has three tabs with various properties. The properties in these tabs will be explained in more detail in next sections.

 

 

 

## Events Tab

 

The Events Tab has the following properties:

 

 

On the left of the Events Tab is the **Event List**. You can use the **Add** button or **Sampler** button to add events to this list. You can use the **Duplicate** button to dupliate the selected event. You can use the **Remove** button to remove the selected event. You can use the **Up** and **Down** buttons to reorder the events in the list.

  

#### Event List Ordering

 

If two Events in the Event List refer to the same model event, then when that model event occurs, the event higher in the list will update the table first.

  

Every event allows you to specify the following properties:

 

- **Name** - The name of the current event.
- **Additional Labels** - This list allows you to add extra labels to the `data` entity as part of this event.

  

#### `data` Label Names Shouldn't Have Spaces

 

If you add spaces in the name of a label on the `data` entity, you won't be able to access that label. This includes labels from event parameters, as well as any additional labels.

  

The following subsections describe the different kinds of events that you can listen to or create with the Statistics Collector, and the properties of each kind of event.

 

### Standard Events

 

The example below shows an example of this kind of event:

 

 

The Statistics Collector can listen to the events of any 3D Object or Process Flow Activity. For example, you could listen to the On Entry of a Processor, or the On Content Change of a Queue. If you listen to a Process Flow Activity in an instanced flow, the Statistics Collector will listen to that activity for all instances attached to the flow.

 

When you listen to one of these kinds of events, you will have the option of assigning the event **Parameters** as labels on the `data` entity.

 

This kind of event also has a **Condition** property. After the Parameters are assigned, the Statistics Collector will evaluate the Condition. If the condition value is false, then the event will not proceed any further in the process, and will not update the table.

 

This kind of event also allows you to specify **Additional Labels** on the `data` entity. These labels are assigned after the Condition is evaluated.

 

### Group Events

 

The example below shows an example of this kind of event:

 

 

If you need to listen to many objects of the same type, you can listen to a Group of objects. The Statistics Collector will listen to all objects in the group.

 

As with Standard Events, this object allows you to assign event **Parameters**, define a **Condition**, and specify **Additional Labels**.

 

### Tracked Variable Events

 

The example below shows an example of this kind of event:

 

 

If you listen to a Tracked Variable, or if you listen to an object event with the word "Change" in it, then you are listening to a Tracked Variable event.

 

When you listen to a Tracked Variable Event, you can specify a **Change Rule**. The Change Rule is like a **Condition**; if the change that happened doesn't match the Change Rule, then the event won't update the table.

 

Depending on which Change Rule you use, you may also need to specify a **Change Value**. This is a value related to the Change Rule. If the Change Rule has the word "Value" in its name, then the Change Value is is used.

 

In addition to the Change Rule, you can specify an addition **Condition**, as well as **Additional Parameters**.

 

### Timer Events

 

The example below shows an example of this kind of event:

 

 

The Statistics Collector can create a Timer event and listen to that, rather than listening to events in the model. There are two main ways to use a timer event. The first way is to create a timer event that does not repeat, and fires when the model begins. This kind of timer event usually creates all the rows the table is expected to have. The second way to use a timer event is with a repeating schedule. This allows you to update or gather data at regular intervals.

 

To use a Timer event, you first specify the **First Time** value. This is a duration in model units. The timer will not begin firing until this time. You can also choose to fire this event when the model is reset, by returning a value that is less than 0.

  

#### Do Not Name the Timer Event "OnReset"

 

OnReset is the name of an attribute, and should not be used for the name of an event. Be sure to add a space (On Reset) or make some other change. Otherwise, the Statistics Collector will not work.

  

Next, you can specify whether the timer event is **Repeating**. If it is repeating, you will also need to specify the **Repeat Interval**. This is a duration in model units. The timer will repeat every Repeat Interval after the First Time.

 

You can optionally define a custom **Tick Pattern**. Most users do not need this option. You can use the tick pattern to fire the timer event multiple times. You can do this by returning an array of values from the Tick Pattern property. For each value in the array, the Statistics Collector will execute the timer event that many model units since the First Time or since the last Repeat Interval occurred.

 

This event also allows you to assign **Additional Labels** to the `data` entity.

 

### Enumerate Events

 

The example below shows an example of this kind of event:

 

 

The Enumerate Event, like the Timer Event, is an event that the Statistics Collector creates for itself, rather than listening to some other object in the model. This kind of event fires when the model is reset.

 

To use this event, you can specify a list of `Objects`. These objects can be 3D Objects, Groups, Process Flow Activities, TrackedVariables, or any other treenode value.

 

This event also allows you to assign **Additional Labels** to the `data` entity.

 

This event does not allow you to specify your own **Row Values**. Instead, the Enumerate event creates row values for all objects in the **Objects** list. If the object is a Group, then the Statistics Collector will make a row value for each object in the group. If the object is an Activity in an instanced Process Flow, the Statistics Collector will make one row value for each instance of the activity. All row values are created using the `data.enumerate()` method. For more information, see the Enumerated Values section.

 

### By Requirement Events

 

The example below shows an example of this kind of event:

 

 

This kind of event is almost identical to a Standard Event, but instead of specifying a specific object or group to listen to, you can specify a **Requirement**. When the Statistics Collector is creating its listeners, it will search through the entire model. It will pass each object into the Requirement property, and listen to those objects that return a true value. In this particular example, the Statistics Collector is listening to the OnExit of all Processor objects.

 

The **Event Sampler** next to the **Event** property allows you to sample an object like the objects you expect the requirement to find.

 

As with Standard Events, this object allows you to assign event **Parameters**, define a **Condition**, and specify **Additional Labels**.

 

### Transient Events

 

The example below shows an example of this kind of event:

 

 

Most events allow you to listen to something that is present when the model is reset. Transient events allow you to listen to objects that are created later, as part of the model run. For example, you might want to listen to TrackedVariable label changes on a token. Or you might want to listen to a flowitem's OnEntering event, which fires whenever the flowitem moves to new objects. Neither of these objects exist when the model is reset.

 

It can be helpful to contrast Transient Events with the other kinds of events. When all other kinds of event fire, they expose a Row Value. When this kind of event fires, it exposes a **Transient object**. In the previous example, the Transient object is a flowitem.

 

To use a Transient event, you also need to configure the event you want to listen to on the Transient object. You can use the **Edit transient object event...** button to open a popup, like the one shown here:

 

 

This popup allows you to edit the **Event**, **Parameters** and **Condition** of the event for the Transient object. You must use the **Event Sampler** to sample an event from an object like the transient object you expect to listen to.

 

Transient events are more complicated than other event types because they really listen to two events: the event where the Transient object is identified, and the event fired later by the Transient object. Here are some tips about how Transient events work: 

- A Transient event does not directly update the table. It attaches a listener to an event on the Transient object. When that event fires, the table will be updated.
- The **Condition** property on the Transient event determines whether the event will attach a listener to the Transient object. The **Condition** property on the Transient object event determines whether the Transient object's event will update the table.
- The Transient event has event Parameters. If you assign those parameters to labels on the `data` entity, those label values will be available when the Transient object event fires later.

 

 

 

## Columns Tab

 

The Columns Tab allows you to add columns to the Statistics Collector. The Columns Tab has the following properties:

 

 

### Column List

 

The Column list allows you to add, duplicate, remove, and reorder columns, using the , , , and buttons. You can select a column in the list to edit the properties of that column.

 

### Name

 

The **Name** property sets the name of the column, as it will appear in the table. Each name should be unique.

 

### Row Add Value

 

The **Row Add Value** property specifies the value to write when a row is added to the table.

 

### Event Value

 

The **Event Value** property specifies the value to write when an event connected to this column occurs. If no events are connected to this column, you don't need to specify this value.

 

### Value

 

The **Value** property is only available if the column is set to update when the value is accessed.

  

#### Row Add Value, Event Value, and Value

 

If a column is set to update with events, then you can specify the **Row Add Value** and the **Event Value** properties. If a column is set to update when accessed, those properties will be hidden, and you will instead be able to specify the **Value** property.

  

### Set Values

 

If you add a Column Set, rather than a single column, then you can specify the **Set Values** property. This property should return an Array of name/value pairs. For each name/value pair, the Statistics Collector will add a column with that name. In addition, the value will be available through `data.colValue`.

 

For example, if you have a group of Queue objects, you could make one column per Queue with the following code:

 

```
/**Custom Code*/
StatisticsCollector collector = ownerobject(c);

Array queues = Group("Queues").toFlatArray();
Array result;
for (int i = 1; i <= queues.length; i++) {
	result.push([queues[i].name, queues[i]]);
}

return result;
```

 

Note that the name of each column is set to the name of the queue, and that the value of each column is the queue. This means that `data.colValue` will be the Queue corresponding to the column.

 

### Update Timing

 

The Update Timing option allows you to set whether the column is updated by events or by accessing the table. This option is described in the Updating Values section.

 

### Storage Type

 

The Storage Type of a column determines what type of value can be recorded in that column, and how much memory that value will require. The following list describes each option:

 

- **Double** - Can store decimal numbers, with about 15 digits of precision. Requires 8 bytes per row.
- **Integer** - Can store integer values between about -2 billion and 2 billion. Requires 4 bytes per row.
- **String** - Can store text values. Requires 4 bytes per row, plus enough memory to store each unique text value in the table.
- **Float** - Can store decmimal numbers, with about 7 digits of precision. Requires 4 bytes per row.
- **Binary** - Can store number values of 1 or 0. Requires 4 bytes per 32 fields per row.

 

### Display Format

 

If a column stores a number value (not a text value), then you can specify a **Display Format**. The display format you choose depends on the kinds of value stored in the column. There are four possible options:

 

- **Raw** - Displays the number as it is stored in the table.
- **Object** - Assumes the number is the ID of an object (present on reset), obtained using the [StatisticsCollector.getID()](../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=StatisticsCollector&Method=getID) method. This format will show the path to the object with the given ID. #### Flowitem or Token IDs Don't Use the Object Format If a column stores the ID of tokens or flowitems, you should use the **Raw** format. When you get the ID of a token or flowitem, The Statistics Collector doesn't remember the path to that ID.
- **Date / Time** - Assumes the number is a [DateTime](../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=DateTime) value. This format will show the date and time represented by the number, formatted according to your windows settings.
- **Percent** - Assumes the number is a percentage. This format multiplies the value by 100 and and adds a `%` symbol.

 

### Index Type

 

Use this box to specify the index type of the value. If you specify an option other than None, then an index will be added to the column, or to all columns of the Column Set. An index can greatly improve query performance. For more information, see the SQL Queries topic.

 

### Event/Column Connections

 

You can use this area to connect events to columns. This area shows the list of all events. You can select an event in the list to connect columns to it.

 

There are two ways to connect an event to a column. The first way is **By direct links**. If you choose this option, you can check the box next to each column you want this event to update. This is the most common way to connect events and columns.

 

The second way to connect events and columns is **By column requirement**. To use this way, you specify a code callback. This callback is called for each column in the Statistics Collector. The `data` entity is available, with the following properties set: 

- `data.colNum`
- `data.colName`
- `data.colValue`

 If this callback returns a 1 (or any "true" value), then the event will be connected to this column. If the callback returns a 0 (or any "false" value), then then event will not be connected to the column.

 

 

 

## The Row Options Tab

 

The Row Options tab has the following properties:

 

 

### Row Sorting

 

The Row Sorting area allows you to specify a list of Sort Values. As rows are added to the Statistics Collector, they will be sorted by the first Sort Value, then by the second Sort Value, and so on. For each Sort Value, you can specify whether the value is ordered Ascending (A-Z) or Descending (Z-A).

 

### Reuse Finished Rows

 

If you are not sorting rows, you can optionally choose to reuse finished rows. This means that when the Statistics Collector adds a row, it will first check if there is a finished row. If so, the row value will be assigned to that row instead of to a new row.

 

This option is generally only used where another Statistics Collector will listen to this Statistics Collector. By reusing the rows, this intermediate collector can minimize memory usage.

 

### Keep values and labels for finished rows

 

This property applies when a row is finished. When this box is cleared, the Statistics Collector will forget the row value and destroy and labels associated with that row. If this box is checked, however, the Statistics Collector will simply disconnect the row value and the labels from the current row, rather than discarding them.

 

### On Warmup

 

The **On Warmup** property specifies the actions the Statistics Collector will take at the model's Warmup Time. There are five possible options:

 

- **Remove all rows** - This option removes all rows from the Statistics Collector.
- **Reset to initial values** - This option goes through all unfinished rows and resets the columns to their Row Add value.
- **Remove finished rows** - This option removes all finished rows from the table.
- **Remove finished rows and reset to initial values** - This option removes all finished rows from the table. All remaining rows are set to their Row Add values.
- **Do nothing** - This option does nothing when the warmup time occurs.

 

 

 

## The Triggers Tab

 

The Triggers tab allows you to specify logic for each of the Statistics Collector's Triggers. This tab is identical to the Triggers tab of other objects in FlexSim. For more information, see the Triggers Tab topic.

  

#### Triggers that end in -ed or -ing

 

In the Statistics Collector, some trigger names end with -ing (e.g. OnRowAdding), and others end with -ed (e.g. OnRowAdded). If the trigger name ends with -ing, then that trigger is fired just before the specified event occurs. If the trigger name ends with -ed, then that trigger is fired just after the specified event occurs.

  

 

 

## The Labels Tab

 

The Labels tab allows you to add labels to the Statistics Collector object. It is identical to the Labels tab of other FlexSim objects. For more information, see the Labels Tab topic.

 

 

 

## The General Tab

 

The General tab has the following properties:

 

 

### Shared Condition

 

This property specifies a condition for all events. This property is evaluated immediately when an event occurs. If the condition returns a 0, or any other false value, then the event won't update the table.

  

#### The Shared Condition can't access the `data` entity

 

The Shared Condition is evaluated when an event occurs, but before and properties or labels are assigned to the `data` entity. The Shared Condition is designed to depend on global information, like the current time or a value in a global table.

   

#### The Shared Condition doesn't apply to events that fire on reset

 

If you use an Enumerate event, or if you use a Timer event that fires on reset, then the shared condition will not be evaluated. On Reset events always update the table.

  

### Process Flow Instance

 

This property only applies if one or more of the events for this Statistics Collector involve Process Flow Activities in object process flows. By default, this option specifies that the Statistics Collector should listen to all instances of that activity. In order to listen to fewer instances, you can use the sampler to set this value to a single object, or to a Group of objects. The Statistics Collector will only listen to the instance objects specified by this property. The remove button will reset this property to the default of All Instances.

 

### Object Format Depth

 

This property applies to columns that use the Object format. The Object format displays a path to each object, and this property controls the maximum number of names that can appear in that path. If this property is 0, then the full path will be shown. If it is 1, then only the name of the object will be shown. If this property is 2, then the parent's name and the object's name will be shown as a partial path. As the value increases, the path will include nodes higher in the tree.

 

### Store data on hard drive

 

If this box is checked, then this Statistics Collector will store its table data on the hard drive, in an internal file. Memory (RAM) use for the data table will be limited to 5 MB.

  

#### Performance impact of storing data on the hard drive

 

If you store a Statistics Collector's data on the hard drive, it will affect the read and write speed of the bundle. The write speed will be slower, although this slowdown is negligable. The read speed, however, will be far slower. The read speed mainly affects how fast charts can draw, and how fast queries can run. Consider testing with and without this option to determine which trade-off works best for you.

  

### View Table

 

Click this button to open a view of the data table for this Statistics Collector.

 

### View Row Values

 

Click this button to open a table-based view. The table in this view shows the row value for each row of the Statistics Collector. If the row was enumerated, then this table will also show the group and instance associated with the row. Active rows show

 

### Export Table

 

You can use this button to save a CSV file of this Statistics Collector's data table.

 

### Connected Dashboard Charts

 

This list shows all the dashboard charts that are drawing data from this Statistics Collector. If no charts require this collector, the list will be empty.

 

### Properties

 

Click this button to open the Properties window of the selected chart in the Connected Dashboard Charts list.

---
id: statisticscollector__collecteddata
name: "StatisticsCollector::CollectedData"
kind: class
module: "StatisticsCollector"
signature: "StatisticsCollector::CollectedData"
aliases: []
tags: ["class", "statisticscollector-module"]
deprecated: false
replacedBy: null
description: "Provides access to data that may be used in Statistics Collector callbacks."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\StatisticsCollector.CollectedData.xml"
---

# StatisticsCollector::CollectedData

Provides access to data that may be used in Statistics Collector callbacks.

Each StatisticsCollector has a CollectedData object.
This object can only be accessed in callbacks called by the Statistics
Collector, through its collectedData property.
Before evaluating one of these callbacks, the Statistics Collector
sets some or all of the properties on this object, so they can be read within the callback.

This object also has dynamic properties, which act like labels.
Event parameters and Event Data Label values are assigned as
dynamic properties.

## Methods

### enumerate

```flexscript
double StatisticsCollector::CollectedData.enumerate()
```

Creates a value that represents the unique combination of object, instance, and group.

**Returns:** A hash of the object, instance, and group values.

If the parameters are not given,
then the current
eventNode,
processFlowInstance,
and
group
property values are used.
This method creates a hash of the object, instance, and group values.
That hash, along with the values used in that hash, are stored in the
Statistics Collector. That hash is the return value. If you use
the hash as a row value, then the Statistics Collector will assert
a row for the hash value, and set
eventNode,
processFlowInstance,
and
group
to the values associated with the hash.

This method is most commonly used in conjunction with an
Enumerate Event,
allowing additional events to update rows created by the Enumerate Event.
You can find additional information in the
Enumerate Row Values
section.

## Properties

### colNum

```flexscript
int StatisticsCollector::CollectedData.colNum (readonly)
```

The position of the current column in the table.

### colValue

```flexscript
Variant StatisticsCollector::CollectedData.colValue (readonly)
```

The value associated with the current column.

This value is set as part of the Set Value callback
for Column Sets.

### colValueIndex

```flexscript
int StatisticsCollector::CollectedData.colValueIndex (readonly)
```

The index of the current column, relative to its column set.

This value determined by the sequence of values
returned from the Set Value callback for Column Sets.

### currentValue

```flexscript
Variant StatisticsCollector::CollectedData.currentValue (readonly)
```

The value in the table at the current row and column.

### eventName

```flexscript
string StatisticsCollector::CollectedData.eventName (readonly)
```

The name of the event that fired, causing the current callback to be evaluated.

### eventNode

```flexscript
treenode StatisticsCollector::CollectedData.eventNode (readonly)
```

The treenode responsible for the event that fired, causing the current callback to be evaluated.

### group

```flexscript
Group StatisticsCollector::CollectedData.group (readonly)
```

The Group associated with the current callback.

This value will be empty unless the Statistics
Collector is listening to a Group, or an enumerated object is
a Group.

### linkedColumns

```flexscript
Array StatisticsCollector::CollectedData.linkedColumns (readonly)
```

An array of column numbers, indicating which columns are linked to the current event.

### processFlowInstance

```flexscript
Object StatisticsCollector::CollectedData.processFlowInstance (readonly)
```

The instance associated with the current callback.

This value will be empty unless the Statistics
Collector is listening to, or has enumerated, a Process Flow
block in a instanced flow.

### row

```flexscript
StatisticsCollector::CollectedData.row (readonly)
```

Accesses labels associated with the current row.

This property is an object that has a labels property,
like
treenode.labels.
The object also has dynamic label access, like
treenode.labelProperties.

For example, you could use a label to store the time a row was added
in the OnRowAdding trigger:
data.row.RowAddTime = Model.time;
Then, you could reference this label in a column's value:
Model.time - data.row.RowAddTime

### rowNum

```flexscript
int StatisticsCollector::CollectedData.rowNum (readonly)
```

The position of the current row in the table.

### rowValue

```flexscript
Variant StatisticsCollector::CollectedData.rowValue (readonly)
```

The value associated with the current row.

This value is the value specified by the
Row Value. If the Row Mode is Enumerate, then the row value
is the object associated with the current row.

### rowValueIndex

```flexscript
int StatisticsCollector::CollectedData.rowValueIndex (readonly)
```

The index of this row value, if the Row Value for the current event is an Array.


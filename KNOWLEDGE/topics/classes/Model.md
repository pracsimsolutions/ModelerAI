---
id: model
name: "Model"
kind: class
module: ""
signature: "Model"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class containing data and functions for the model."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\Model.xml"
---

# Model

A class containing data and functions for the model.

## Methods

### getDateTime

```flexscript
DateTime (static) Model.getDateTime(double modelTime)
```

Returns a date and time in FlexSim time from the given simulation time.

**Parameters:**
- `modelTime` *(double)* — The simulation time.

**Returns:** The DateTime represented by the simulation time.

DateTime dateTime = Model.getDateTime(100);

### getTime

```flexscript
double (static) Model.getTime(DateTime dateTime)
```

Returns the simulation time from the given FlexSim time.

**Parameters:**
- `dateTime` *(DateTime)* — The date and time in FlexSim time.

**Returns:** The simulation time represented by the FlexSim time.

DateTime date = DateTime("8:00:00 AM Wed 19 Jul 2017");
double theTime = Model.getTime(date);

### find

```flexscript
treenode Model.find(string path)
```

Finds a node in the model's subtree by its path.

**Parameters:**
- `path` *(string)* — The path, starting at the model node, that addresses the target node.

**Returns:** If the addressed node exists, the method returns that node, otherwise NULL.

This is the same as calling treenode.find() on the
model node: model().find("ObjectName")

treenode obj = Model.find("ObjectName");

## Properties

### dateTime

```flexscript
DateTime Model.dateTime (readonly)
```

The current date and time in FlexSim time.

DateTime currentDateTime = Model.dateTime;

### nextStopDateTime

```flexscript
DateTime Model.nextStopDateTime (readonly)
```

The date and time in FlexSim time of the next stop time.

DateTime nextStop = Model.nextStopDateTime;

### nextStopTime

```flexscript
double Model.nextStopTime (readonly)
```

The simulation time of the next stop time.

If no stop time is defined this returns 0. If the current simulation time has passed the last stop time this returns the largest double value.
double stopTime = Model.nextStopTime;

### parameters

```flexscript
Model.parameters (readonly)
```

Provides access to the model's parameters.

Getting a parameter value
Model.parameters.Parameter1
Model.parameters["Parameter1"].value
Getting a parameter value, passing in up to three arguments
// Passed-in values are available as param(1), param(2), and param(3)
Model.parameters["Parameter1"].evaluate(param1, param2, param3)
Setting a parameter value
Model.parameters.Parameter1 = 5;
Model.parameters["Parameter1"].value = 5;
List parameter tables
Model.parameters.tableNames // Array[1]: [Parameters]
List parameters
Model.parameters.names() // Array[1]: [Parameter1]
Model.parameters.names("Parameters") // Array[1]: [Parameter1]

### performanceMeasures

```flexscript
Model.performanceMeasures (readonly)
```

Provides access to the model's performance measures.

Getting a performance measure value
Model.performanceMeasures.PerformanceMeasure1
Model.performanceMeasures["PerformanceMeasure1"].value
List performance measure tables
Model.performanceMeasures.tableNames // Array[1]: [PerformanceMeasures]
List performance measures
Model.performanceMeasures.names() // Array[1]: [PerformanceMeasure1]
Model.performanceMeasures.names("PerformanceMeasures") // Array[1]: [PerformanceMeasure1]

### startDateTime

```flexscript
DateTime Model.startDateTime (readonly)
```

The model's start date and time in FlexSim time.

The model start date and time is define in the Model Settings.
DateTime startTime = Model.startDateTime;

### statisticalTime

```flexscript
double Model.statisticalTime (readonly)
```

The current simulation time with respect to when stats were last reset.

If the model has a warmup time, this will return the difference between the simulation time and the warmup time. statisticalTime = time - warmupTime
double statTime = Model.statisticalTime;

### time

```flexscript
double Model.time (readonly)
```

The current simulation time.

double currentTime = Model.time;

### warmupDateTime

```flexscript
DateTime Model.warmupDateTime (readonly)
```

The warmup date and time in FlexSim time.

DateTime warmupTime = Model.warmupDateTime;

### warmupTime

```flexscript
double Model.warmupTime (readonly)
```

The total warmup time.

If no warmup time is defined this returns 0.
When the warmup time is reached, statistics on objects and tracked variables will be reset.
double warmup = Model.warmupTime;


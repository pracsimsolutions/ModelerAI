---
id: trackedvariable
name: "TrackedVariable"
kind: class
module: ""
signature: "TrackedVariable"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class that represents a tracked variable."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\TrackedVariable.xml"
---

# TrackedVariable

A class that represents a tracked variable.

Tracked Variables can be defined as global for the model,
or they can be stored on an object, table or label.

## Methods

### init

```flexscript
TrackedVariable (static) TrackedVariable.init(treenode theNode, int type, double startValue = 0, int flags = -1)
```

Asserts tracked variable data on a node with the designated type and start value.

**Parameters:**
- `theNode` *(treenode)* — The node to assert the tracked variable data on.
- `type` *(int)* — Defines the tracked variable type. See the type property for more information.
- `startValue` *(double, default 0)* — The starting value of the tracked variable. If the tracked variable presists when the model is reset, this will be the reset value.
- `flags` *(int, default -1)* — Specifies options for this tracked variable. The flags can be a bitwise combination of the following three values. By default, none of these options are used: STAT_USE_HISTORY - the tracked variable will keep a history of each value it hold, and the time at which the value is set. STAT_USE_PROFILE - the tracked variable will track a profile of the total time spent at each value. This only works if the set of used values are non-negative integers. STAT_IGNORE_WARMUP - the tracked variable will ignore the resetstats() command. Use this option only if the tracked variable should keep the data collected during warmup time.

TrackedVariable newTV = TrackedVariable.init(aNode, STAT_TYPE_TIME_SERIES, 0.0, STAT_USE_HISTORY | STAT_IGNORE_WARMUP);

### clearListeners

```flexscript
TrackedVariable.clearListeners()
```

Used with Categorical Combo TrackedVariables. Removes all listeners from a Categorical Combo tracked variable.

### getCategoryName

```flexscript
string TrackedVariable.getCategoryName(int catNum)
```

Gets the name of a category from a categorical tracked variable.

**Parameters:**
- `catNum` *(int)* — The category number to get the name of.

string name = trackedVariable.getCategoryName(1);

### getCategoryValue

```flexscript
Variant TrackedVariable.getCategoryValue(int catNum = -1)
```

Gets the value associated with a category from a categorical or pointer tracked variable.

**Parameters:**
- `catNum` *(int, default -1)* — The category number to get the value of.

For categorical tracked variables, this method is identical
to the getCategoryName() method.
Variant categoryValue = trackedVariable.getCategoryValue(strOrNodeCategory);

### getTotalTimeAt

```flexscript
double TrackedVariable.getTotalTimeAt(Variant value)
```

Gets the total time the tracked variable as been at the specified categorical or pointer value.

**Parameters:**
- `value` *(Variant)* — Value should be either the name of the category value to get, the node for the category, or the index number of the category.

The getTotalTimeAt method is only available for Categorical and Pointer type tracked variables.

double processingTime = trackedVariable.getTotalTimeAt("processing");
double processingTime = trackedVariable.getTotalTimeAt(2);

### listenTo

```flexscript
TrackedVariable.listenTo(TrackedVariable other)
```

Used with Categorical Combo TrackedVariables. Adds a listener to a different categorical TrackedVariable.

**Parameters:**
- `other` *(TrackedVariable)* — A categorical TrackedVariable to listen to

The listener allows this TrackedVariable to be notified
when the other TrackedVariable changes value. When any of the other TrackedVariables
that you listen to change value, this TrackedVariable sets its value to a
value that represents the combination of all the other TrackedVariable value.
Since State Profiles are implemented using Categorical TrackedVariables, you can
use this type of TrackedVariable to create a state profile that is the combination
of several other state profiles.

### reset

```flexscript
TrackedVariable.reset(double startValue = 0.0, int type = -1)
```

Resets the tracked variable.

**Parameters:**
- `startValue` *(double, default 0.0)* — The start value of the tracked variable.
- `type` *(int, default -1)* — If no value is defined, the tracked variable will keep its original type. See the type property for more information.

trackedVariable.reset();
trackedVariable.reset(10);
trackedVariable.reset(0, STAT_TYPE_LEVEL);

### TrackedVariable

```flexscript
TrackedVariable TrackedVariable.TrackedVariable(string name)
```

Creates a reference to a global tracked variable.

**Parameters:**
- `name` *(string)* — The name of the global tracked variable.

TrackedVariable wip = TrackedVariable("WorkInProgress");

## Properties

### age

```flexscript
double TrackedVariable.age (readonly)
```

Returns the duration since the last reset time in model units.

### average

```flexscript
double TrackedVariable.average (readonly)
```

Gets the average value of the tracked variable.

The average value is not available for Categorical and Cumulative type tracked variables.
See the type property for more information.
double average = trackedVariable.average;

### count

```flexscript
int TrackedVariable.count (readonly)
```

Gets the number of times the tracked variable has been set.

int count = trackedVariable.count;

### history

```flexscript
Table TrackedVariable.history (readonly)
```

Gets the table of history for the tracked variable.

The history property is only available if the tracked variable flags is set to STAT_USE_HISTORY.
See the init method for information on tracked variable flags.

Table history = trackedVariable.history;
double time = history[1][1];
double value = history[1][2];
double rate = history[1][3]; //Kinetic type

### lastResetTime

```flexscript
double TrackedVariable.lastResetTime (readonly)
```

### lowerBound

```flexscript
double TrackedVariable.lowerBound
```

Gets or sets the lower bound of the tracked variable.

The lowerBound property is only available for Kinetic type tracked variables. When setting the
lower bound, the lower bound must be less than the current value of the tracked variable.

double lower = trackedVariable.lowerBound;
trackedVariable.lowerBound = -100;

### maximum

```flexscript
double TrackedVariable.maximum (readonly)
```

Gets the maximum value of the tracked variable.

double maximum = trackedVariable.maximum;

### minimum

```flexscript
double TrackedVariable.minimum (readonly)
```

Gets the minimum value of the tracked variable.

double minimum = trackedVariable.minimum;

### profile

```flexscript
Table TrackedVariable.profile (readonly)
```

Gets the table of profiles for the tracked variable.

The profile property is only available for Categorical type tracked variables.

Table profile = trackedVariable.profile;
string name = profile[2][1];
double value = profile[2][2];

### rate

```flexscript
double TrackedVariable.rate
```

Gets or sets the current rate of the tracked variable.

The rate property is only available for Kinetic type tracked variables. See the
type property for more information.

double rate = trackedVariable.rate;
trackedVariable.rate = 1.5;

### type

```flexscript
int TrackedVariable.type (readonly)
```

Gets the type of the tracked variable.

The type affects what meta data, like average, will be collected, and how it is calculated.
Type can be one of the following values:
STAT_TYPE_LEVEL - records the variable as a level that can go up and down, such as content. The average
is a time-weighted average. An example of this would be content tracking.
STAT_TYPE_TIME_SERIES - records the variable as a series of independent values. The average is a
non-time-weighted average. An example of this type is staytime tracking.
STAT_TYPE_CUMULATIVE - records the variable as a value that only accumulates over time, such as input
or output. Average is not tracked.
STAT_TYPE_CATEGORICAL - records the variable as a set of string values. Average is not tracked. An example of this is state tracking.
STAT_TYPE_KINETIC_LEVEL - similar to STAT_TYPE_LEVEL, except that you can also give it a rate of change by setting the rate property. An example of this would
be battery level.
STAT_TYPE_POINTER - similar to STAT_TYPE_CATEGORICAL, except that instead of tracking
string values, it tracks node values.
STAT_TYPE_CATEGORICAL_COMBO - used with two or more other STAT_TYPE_CATEGORICAL
TrackedVariables. When any of the other tracked variables changes its state,
this variable sets its state to a string representing the combination of all current states.
This can be used to record the combined state of multiple state profiles.
int type = trackedVariable.type;

### upperBound

```flexscript
double TrackedVariable.upperBound
```

Gets or sets the upper bound of the tracked variable.

The upperBound property is only available for Kinetic type tracked variables. When setting the
upper bound, the upper bound must be greater than the current value of the tracked variable.

double upper = trackedVariable.upperBound;
trackedVariable.upperBound = 100;

### value

```flexscript
Variant TrackedVariable.value
```

Gets or sets the current value of the tracked variable.

The value of the tracked variable should be a number or it may be a string for categorical type
tracked variables.

double value = trackedVariable.value;
trackedVariable.value = 32;
trackedVariable.value = "processing"; //Categorical type only
trackedVariable.value = Model.find("Queue1"); //Pointer type only

### valueString

```flexscript
Variant TrackedVariable.valueString
```

Gets or sets the current value of the categorical tracked variable.

The valueString is only available for the categorical type tracked variables.

string valueStr = trackedVariable.valueString;
trackedVariable.valueString = "processing";


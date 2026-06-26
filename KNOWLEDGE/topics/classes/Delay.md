---
id: delay
name: "Delay"
kind: class
module: ""
signature: "Delay"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "An opaque awaitable type, specifically used in coroutines, representing an abstract delay time."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\Delay.xml"
---

# Delay

An opaque awaitable type, specifically used in coroutines, representing an abstract delay time.

await Delay(5); // awaits 5 model time units
var delay = Delay.seconds(22); // represents a 22 second delay
await delay; // wait for 22 seconds
await delay; // wait again for another 22 seconds

A Delay represents an 'abstract' delay time in that the suspend/resume mechanism is
not started when the
Delay object is created, but each time the await statement is invoked,
resulting in possibly multiple different delay actions.

## Methods

### days

```flexscript
Delay (static) Delay.days(double numDays)
```

Returns a delay object representing a delay time in days.

**Parameters:**
- `numDays` *(double)* — The number of days to delay.

**Returns:** The resulting delay.

await Delay.days(5);

### hours

```flexscript
Delay (static) Delay.hours(double numHours)
```

Returns a delay object representing a delay time in hours.

**Parameters:**
- `numHours` *(double)* — The number of hours to delay.

**Returns:** The resulting delay.

await Delay.hours(5);

### minutes

```flexscript
Delay (static) Delay.minutes(double numMinutes)
```

Returns a delay object representing a delay time in minutes.

**Parameters:**
- `numMinutes` *(double)* — The number of minutes to delay.

**Returns:** The resulting delay.

await Delay.minutes(5);

### realTime

```flexscript
Delay (static) Delay.realTime(double numSeconds)
```

Returns a delay object representing a real-time delay in seconds.

**Parameters:**
- `numSeconds` *(double)* — The number of real-time seconds to delay.

**Returns:** The resulting delay.

await Delay.realTime(0);
This is mostly used as part of UI development by FlexSim developers, but it is nevertheless available to users.
You should not use this in your simulation logic or else you will have repeatability issues. Rather, awaiting
real time delays can be useful for things like auto-build scripts. It allows you
to execute logic asynchronously. Awaiting
a real time delay of 0 will post a message to FlexSim's message queue. The logic that called the FlexScript code will finish,
and when FlexSim continues processing its message queue and reaches the posted message, it will then resume the coroutine.
If you await a real time delay
greater than 0, then FlexSim will create a platform-dependent timer, will wait the defined time, and then will resume the
coroutine.

### seconds

```flexscript
Delay (static) Delay.seconds(double numSeconds)
```

Returns a delay object representing a delay time in seconds.

**Parameters:**
- `numSeconds` *(double)* — The number of seconds to delay.

**Returns:** The resulting delay.

await Delay.seconds(5);

### Delay

```flexscript
Delay Delay.Delay(double timeUnits)
```

Returns a delay object representing a delay time in model time units.

**Parameters:**
- `timeUnits` *(double)* — The number of model time units to delay.

**Returns:** The resulting delay.

await Delay(5);


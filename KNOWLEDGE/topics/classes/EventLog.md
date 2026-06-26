---
id: eventlog
name: "EventLog"
kind: class
module: ""
signature: "EventLog"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class that provides access to the event log."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\EventLog.xml"
---

# EventLog

A class that provides access to the event log.

Using the EventLog class, you can add custom events to the Event Log.

EventLog.info("MyCustomEvent");
if (/*something is unusual*/) {
EventLog.warn("MyCustomEvent", 0, 0, "Something is unusual");
}
if (/*something is wrong*/) {
EventLog.error("MyCustomEvent", 0, 0, "Something is wrong");
}
if (/*something is terribly wrong*/) {
EventLog.fatal("MyCustomEvent", 0, 0, "Something is terribly wrong");
}

The logging methods are:

info()
warn()
error()
fatal()

When used, each method logs a new event in the event log at the current model
time and at the current level. The event is logged in any active log: the model
log, the system log, or both. If no event logs are active, then these methods
do nothing.

The only required parameter for the logging methods is the event
parameter. It is best to use a short but meaningful name so that filtering by the
event makes sense. The remainder of the parameters (two node parameters and four
string parameters) are optional and arbitrary.

You can check whether any logs are active using the enabled property:

if (EventLog.enabled) {
string p1 = /*expensive computation*/;
EventLog.info("MyCustomEvent", 0, 0, p1);
}

## Methods

### error

```flexscript
(static) EventLog.error(string event, treenode object = 0, treenode involved = 0, string p1 = "", string p2 = "", string p3 = "", string p4 = "")
```

Log an Error-level event at the current time.

**Parameters:**
- `event` *(string)* — The name of the event.
- `object` *(treenode, default 0)* — The primary object associated with the event.
- `involved` *(treenode, default 0)* — The involved object associated with the event.
- `p1` *(string, default "")* — Data for P1 associated with the event.
- `p2` *(string, default "")* — Data for P2 associated with the event.
- `p3` *(string, default "")* — Data for P3 associated with the event.
- `p4` *(string, default "")* — Data for P4 associated with the event.

Error-level events are often used to indicate that the simulation
cannot continue or that the results are invalid. Consider stopping
the simulation manually if you log an Error-level event.

### fatal

```flexscript
(static) EventLog.fatal(string event, treenode object = 0, treenode involved = 0, string p1 = "", string p2 = "", string p3 = "", string p4 = "")
```

Log a Fatal-level event at the current time.

**Parameters:**
- `event` *(string)* — The name of the event.
- `object` *(treenode, default 0)* — The primary object associated with the event.
- `involved` *(treenode, default 0)* — The involved object associated with the event.
- `p1` *(string, default "")* — Data for P1 associated with the event.
- `p2` *(string, default "")* — Data for P2 associated with the event.
- `p3` *(string, default "")* — Data for P3 associated with the event.
- `p4` *(string, default "")* — Data for P4 associated with the event.

Fatal-level events are often used to indicate that data loss or corruption
has occurred.

### info

```flexscript
(static) EventLog.info(string event, treenode object = 0, treenode involved = 0, string p1 = "", string p2 = "", string p3 = "", string p4 = "")
```

Log an Info-level event at the current time.

**Parameters:**
- `event` *(string)* — The name of the event.
- `object` *(treenode, default 0)* — The primary object associated with the event.
- `involved` *(treenode, default 0)* — The involved object associated with the event.
- `p1` *(string, default "")* — Data for P1 associated with the event.
- `p2` *(string, default "")* — Data for P2 associated with the event.
- `p3` *(string, default "")* — Data for P3 associated with the event.
- `p4` *(string, default "")* — Data for P4 associated with the event.

Info-level events are often used to indicate that something
normal or expected has occurred. All model logic events are Info-level events.

### warn

```flexscript
(static) EventLog.warn(string event, treenode object = 0, treenode involved = 0, string p1 = "", string p2 = "", string p3 = "", string p4 = "")
```

Log a Warn-level event at the current time.

**Parameters:**
- `event` *(string)* — The name of the event.
- `object` *(treenode, default 0)* — The primary object associated with the event.
- `involved` *(treenode, default 0)* — The involved object associated with the event.
- `p1` *(string, default "")* — Data for P1 associated with the event.
- `p2` *(string, default "")* — Data for P2 associated with the event.
- `p3` *(string, default "")* — Data for P3 associated with the event.
- `p4` *(string, default "")* — Data for P4 associated with the event.

Warn-level events are often used to indicate that something unexpected
has occurred but that the simulation can continue to operate.

## Properties

### enabled

```flexscript
int EventLog.enabled (readonly)
```

Returns whether logging is currently enabled.

If true, then events are being logged to the model Event Log,
the system Event Log, or both.


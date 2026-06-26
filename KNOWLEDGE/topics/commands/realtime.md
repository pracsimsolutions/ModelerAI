---
id: realtime
name: "realtime"
kind: command
module: ""
signature: "realtime([num timecode = 0]) -> num/str"
aliases: []
tags: ["command", "group-model-execution"]
deprecated: false
replacedBy: null
description: "Gets the real clock time"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# realtime

```flexscript
realtime([num timecode = 0]) -> num/str
```

Gets the real clock time

Returns the real clock time in a variety of formats based on the timecode:
0 = the number of seconds and milliseconds since midnight, Jan 1, 1970 1 = the number of seconds since midnight, Jan 1, 1970 2 = the number of milliseconds since the last second change 10 = a time string with format hh:mm:ss 11 = a date string with format mm/dd/yy 12 = a date/time string with format Mon Nov 15 15:49:12 2004

## Example

```flexscript
string curdatetime = realtime(12);
```


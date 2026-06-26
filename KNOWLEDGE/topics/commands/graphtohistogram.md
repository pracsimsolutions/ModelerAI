---
id: graphtohistogram
name: "graphtohistogram"
kind: command
module: ""
signature: "graphtohistogram(obj graph, obj histogram, num start, num end, num res, num xyformat)"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "FlexScript command: graphtohistogram."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# graphtohistogram

```flexscript
graphtohistogram(obj graph, obj histogram, num start, num end, num res, num xyformat)
```

For developer use. Generate histogram data from a list of values. This command will generate a histogram inside the node histogram based on the occurrence table given by graph. Mainly used internally by objects. The starting value of the histogram is specified as start. The ending value of the histogram is specified as end. The number of buckets in the histogram is specified as res. If xyformat is 0, it assumes that the graph is a y format occurrence table. If xyformat is 1, it assumed the graph is an xy format occurrence table. Similar to makehisto(). Refer to makehisto() for more information on these similarities.

## Example

```flexscript
graphtohistogram(so(),so().next,1,10,5,0)
```


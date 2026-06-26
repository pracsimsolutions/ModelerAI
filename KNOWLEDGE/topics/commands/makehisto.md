---
id: makehisto
name: "makehisto"
kind: command
module: ""
signature: "makehisto(obj datalist, obj histocontainer, num nrofbuckets)"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "FlexScript command: makehisto."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# makehisto

```flexscript
makehisto(obj datalist, obj histocontainer, num nrofbuckets)
```

For developer use. Builds a histogram structure from the data list and puts it in the destination container node with the given number of buckets. This is similar to graphtohistogram() except that this command creates a more complete structure. To illustrate the operator instead., view a FlexSim object's attribute tree. The makehisto command will create the entire stucture of object>stats/staytime/staytimehisto, whereas graphtohistogram will only populate object>stats/staytime/staytimehisto/data. makehisto also assumes a standard, y data series structure, meaning each sub-node of datalist is its own data point (they are not paired as x/y values).

## Example

```flexscript
makehisto(c.find("@>objectfocus+"), c.find(">histogram"), 20)
```


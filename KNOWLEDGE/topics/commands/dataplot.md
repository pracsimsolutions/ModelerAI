---
id: dataplot
name: "dataplot"
kind: command
module: ""
signature: "dataplot(obj tree, num type, num action, num x, num y, num z)"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "FlexScript command: dataplot."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# dataplot

```flexscript
dataplot(obj tree, num type, num action, num x, num y, num z)
```

For developer use. This command lets you create, reset or add data to a histogram, x/y chart, or a data series. The first three parameters define the associated node in the tree as well as the operation to do. The last three parameters, x,y and z, define the appropriate data for the operation. The first parameter should be the main node in the tree that holds the graph/histo data. The second parameter should be either 1, 2, or 3, and defines the type of data that the node holds. 1 means graphx data. For this data type the node contains a list of sub-nodes, and each sub-node contains a number that is a unique point in a data series. 2 means graphxy. For this data type, the node again contains a list of sub-nodes but this time the sub-nodes are paired together as x/y pairs. Nodes ranked 1 and 2 hold the x and y values for one data point, nodes 3 and 4 hold x and y values for the next data point, etc. A type parameter of 3 means histo data. Here the node holds data associated with a histogram. The action parameter should be either 1, 2, or 3, and defines the action that you want to do. 1 means a create operation. This is only needed for histogram type data, as it needs to set up the node's sub-tree structure. To create a histogram plot, pass the minimum value for the histogram as the x parameter, the maximum value of the histogram as the y value, and the number of divisions, or buckets, as the z parameter. An action parameter value of 2 means you want to reset the data. This will clear the content of the node for graphx or graphxy data, and will reset the histogram values for histogram data. An action parameter value of 3 means you want to add a data point. For graphx and histo data, pass the data point as the x parameter. For graphxy data, pass the as the x and y paramters. In summary: Type: 1-graphx/2-graphxy/3-histo. Action: 1-create (only needed for histo data)/2-reset/3-add point. Data: x,y are data points [histo create: x=start, y=end, z=nr of divisions].

## Example

```flexscript
dataplot(stats_contentgraph(model.find("Processor2")),2,3,8.6,5,0); This adds a point to Processor2's content graph for time 8.6 and content 5. This example is only for demonstration purposes. You should never need to do this since the Processor automatically maintains its content graph.
```


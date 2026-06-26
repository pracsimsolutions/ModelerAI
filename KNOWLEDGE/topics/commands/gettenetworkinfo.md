---
id: gettenetworkinfo
name: "gettenetworkinfo"
kind: command
module: ""
signature: "gettenetworkinfo(obj taskexecuter, obj destobj, num info) -> num"
aliases: []
tags: ["command", "group-networknodes"]
deprecated: false
replacedBy: null
description: "Gets information regarding the current state of a taskexecuter on a node network"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# gettenetworkinfo

```flexscript
gettenetworkinfo(obj taskexecuter, obj destobj, num info) -> num
```

Gets information regarding the current state of a taskexecuter on a node network

Returns information regarding the current state of a taskexecuter on a node network. Valid queries are as follows:

NETWORK_INFO_DEST_NET_NODE_RANK - returns the destination column/rank of the network node that the TE would travel to were he
to be given a travel task to destobj. Usually this will simply be the network node attached to destobj, but if multiple network
nodes are attached to destobj, the command will find the closest one.
NETWORK_INFO_ORIGIN_NET_NODE_RANK - returns the origin column/rank of the network node that the TE would start from were he
to be given a travel task to destobj. Usually this will simply be the current network node where the TE is located, but if that
network node has virtual exits, the command will find the virtual exit that renders the shortest travel distance.
NETWORK_INFO_DISTANCE - returns the shortest travel distance to the destination object through the network.
NETWORK_INFO_CUR_DIST_ALONG_EDGE - Only valid when the TE is actively doing a travel task. This returns the current distance
that the TE has traveled along its current network edge. For this query, destobj should be NULL.
NETWORK_INFO_CUR_TRAVEL_ORIGIN_RANK - Only valid when the TE is actively doing a travel task. This returns the rank of the
origin node at which the TE starts its current travel operation. For this query, destobj should be NULL.
NETWORK_INFO_CUR_TRAVEL_DEST_RANK - Only valid when the TE is actively doing a travel task. This returns the rank of the
destination node to which the TE is traveling for its current travel operation. For this query, destobj should be NULL.

## Example

```flexscript
int destrank = gettenetworkinfo(current, NULL, NETWORK_INFO_CUR_TRAVEL_DEST_RANK);
```


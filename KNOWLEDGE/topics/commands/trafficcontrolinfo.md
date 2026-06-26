---
id: trafficcontrolinfo
name: "trafficcontrolinfo"
kind: command
module: ""
signature: "trafficcontrolinfo(obj trafficcontrol, num info [, num a, num b]) -> num/node"
aliases: []
tags: ["command", "group-networknodes"]
deprecated: false
replacedBy: "the"
description: "Gets information on a traffic controller"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# trafficcontrolinfo

```flexscript
trafficcontrolinfo(obj trafficcontrol, num info [, num a, num b]) -> num/node
```

Gets information on a traffic controller

This command gives you access to various information on a traffic control, as well as lets you direct the traffic control
to perform logic. Logic/data that is performed/returned is based on the info parameter, as follows:

trafficcontrolinfo(tc, TCI_NR_ACTIVE_TRAVELERS) - returns the total number of active travelers in the traffic control.

trafficcontrolinfo(tc, TCI_ACTIVE_TRAVELER, num travelnr) - returns a reference to the nth active traveler in the traffic control.

trafficcontrolinfo(tc, TCI_ACTIVE_TRAVELER_COUPLING, num travelernr) - returns a reference to a coupling node, stored in the
Traffic Control's variables tree, that is associated with the nth active traveler. This coupling can be used to stored custom
user information regarding that active traveler. To do this, add sub-nodes to the coupling node with nodeinsertinto(), add data,
etc. Note that once the active traveler leaves the traffic control area, the coupling, and its sub-nodes, will automatically be
destroyed.

trafficcontrolinfo(tc, TCI_ACTIVE_TRAVELER_RANK_FROM_TRAVELER, obj traveler) - This command takes a reference to a traveler
object and returns which index in the traffic control's active travelers is associated with that traveler. If zero is returned
then the object is not an active traveler in the traffic control.

trafficcontrolinfo(tc, TCI_NR_MEMBERS) - Returns the total number of network nodes that are members of the traffic control area

trafficcontrolinfo(tc, TCI_MEMBER, num membernr) - Returns a reference the nth network node member of the traffic control

trafficcontrolinfo(tc, TCI_MEMBER_COUPLING, num membernr) - Returns a reference to a coupling node, stored in the traffic control's
variables, that is associated with the nth network node member of the traffic control. Like TCI_ACTIVE_TRAVELER_COUPLING, this
node may be used to store data associated with that network node.

trafficcontrolinfo(tc, TCI_MEMBER_RANK_FROM_NETWORK_NODE, obj netnode) - Takes a network node reference and returns the
member index of that network node in the traffic control. If zero is returned then the network node is not a member of the
traffic control.

trafficcontrolinfo(tc, TCI_NR_ENTRY_REQUESTS) - Returns the number of entry requests currently in queue waiting to
enter the traffic control area.

trafficcontrolinfo(tc, TCI_ENTRY_REQUEST_TRAVELER, num requestnr) - Returns a reference to the traveler associated with
the nth entry request, who is waiting get into the traffic control area.

trafficcontrolinfo(tc, TCI_ENTRY_REQUEST_NETWORK_NODE, num requestnr) - Returns a reference to the network node where
the traveler is waiting, associated with the nth entry request

trafficcontrolinfo(tc, TCI_ENTRY_REQUEST_NODE, num requestnr) - Returns a reference to a node associated with the
nth entry request. Like TCI_ACTIVE_TRAVELER_COUPLING, you may add sub-nodes to store data associated with the request.
Do not, however, change the data on this node, as this is used by the traffic control logic.

trafficcontrolinfo(tc, TCI_ALLOW_ENTRY, num requestnr) - Calling this command will instruct the traffic control
to allow the traveler associated with the nth entry request to enter the traffic control area.

trafficcontrolinfo(tc, TCI_SET_ENTRY_REQUEST_RANK, num requestnr, num settorank) - Calling this command will
cause the traffic control to rerank its nth entry request to the specified rank.

Also, a note regarding using the traffic control triggers to override the traffic control's default logic. The traffic control's
OnEntryRequest trigger, which fires when a traveler first requests entry into the traffic control area, can take 3 different
return values:

TC_ON_ENTRY_REQUEST_DEFAULT - If this value is returned from the trigger, the traffic control will perform its normal
filtering logic.

TC_ON_ENTRY_REQUEST_ALLOW - If this value is returned, the traffic control will ignore its default logic and simply
allow the traveler into the area. Note that if this is used, the traffic control will ignore its maximum capacity variable,
so depending on the logic you define, the number of travelers in the area may exceed the maximum capacity variable
you specify on the traffic control.

TC_ON_ENTRY_REQUEST_QUEUE - If this value is returned, the traffic control will ignore its default logic and add
the request to its request queue, disallowing the traveler from entering the area at that time.
The traffic control's OnExit trigger may take one of two return values:

0 - If this value is returned, the traffic control will perform its default logic, allow other travelers to enter, etc.

1 - If this value is returned, the traffic control will do nothing. In this case the user should use the trigger to
explicitly tell the traffic control which traveler(s) to receive using TCI_ALLOW_ENTRY.

If you plan on implementing your own custom logic for allowing travelers into the area, then it is recommended that you
use the traffic control's default "Mutual Exclusion" mode.

## Example

```flexscript
treenode netnode = trafficcontrolinfo(tc, TCI_MEMBER, 2);
```


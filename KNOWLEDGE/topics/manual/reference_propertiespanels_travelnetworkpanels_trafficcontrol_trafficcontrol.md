---
id: reference_propertiespanels_travelnetworkpanels_trafficcontrol_trafficcontrol
name: "Traffic Control"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","TravelNetworkPanels","TrafficControl","TrafficControl"]
tags: ["manual","reference","propertiespanels","travelnetworkpanels"]
source: "manual/Reference/PropertiesPanels/TravelNetworkPanels/TrafficControl/TrafficControl.html"
---

# Traffic Control

The Traffic Control panel has options for controlling the Traffic Control's connections.

 

 

The following properties are on the Traffic Control panel:

 

### Mode

 

This menu defines the method by which the object controls traffic. There are two options:

 

- **Mutual Exclusion** - Only allows a given number of travelers into the Traffic Control's area, regardless of which paths they are on.
- **Untimed Traffic Modes** - Used if you want to control traffic based on each individual path in the object's traffic control area. Click the Edit Traffic Modes button to open the Traffic Modes window.

 

### Max Travelers

 

Mutual exclusion is used to only allow a given number of travelers into the Traffic Control's area, regardless of which paths they are on. Here you simply specify the maximum number value.

 

### Adjust Speeds as Content Increase

 

If this option is checked, then the table will be used to adjust speeds of travelers within the traffic control object's area.

  

#### Manipulating Speeds with Traffic Controls

 

You can also use Traffic Controls to modify speeds of travelers as an area becomes more crowded. As the traffic control's content increases, entering travelers will modify their speeds based on the Traffic Control's speed table. For example, if you have entered a row in the table that is a 0.6 speed multiple for a content of 3, then as soon as the content of the traffic control's area becomes 3 or greater, all travelers' max speeds will be decreased to 60% of their normal max speed. Note that the speed change does not take effect until the traveler reaches its next node in the network. If you have an area with multiple traffic controls, then the minimum speed multiple of all of the traffic controls will be applied.

  

### Edit Network Nodes

 

This button opens the Network Nodes window.

 

 

 

## The Traffic Modes Window

 

Untimed traffic modes are used if you want to control traffic based on each individual path in the object's traffic control area. A mode is defined by one row in the modes table. For each mode you can define a set of paths between nodes in the traffic control area. When the traffic control is in a given mode, travelers are only allowed into the traffic control area if the path they are entering on is one of the paths defined in the current mode. The traffic control will stay in the same mode until there are no travelers left in the traffic control area, after which it will take the first traveler that arrives and find a mode that contains that traveler's requested entry path. This is why it is an un-timed mode. There is no limit on the amount of time that a traffic control may stay in the same mode.

 

 

The Traffic Modes window has the following properties:

 

### Modes

 

This is the number of modes, or rows in the table. Enter the number of modes you want, and click the Apply button, and the appropriate number of rows will be created.

 

### Entries

 

This is the maximum number of From/To entries (or columns) that you will need for your modes. If some modes don't need all of the columns, just leave them blank.

 

### Search for Best Mode

 

If this box is checked, then whenever the traffic control gets an entry request for an entry path that is not in its current mode, it will search through its modes to see if there are any other modes that include all paths already entered as well as the new path. If so, it will change to that new mode and allow the traveler's entry. Note that this may slow down the model, since the traffic control must search the table every time an entry request is made.

 

### Mode Table Entries

 

The mode table has the following columns:

 

- **Max_Nr** - This value specifies a maximum number of travelers allowed in when the traffic control is in that given mode. It is much like the maximum number value in mutual exclusion mode.
- **n_a** - This value is reserved for future development, when timed traffic modes are implemented.
- **From/To Entries** - For each path of a mode, you specify the node from which the path extends, and the node to which the path extends. Enter the name of the nodes. Note that one entry describes only one direction of a node-to-node connection. Thus to specify both directions of a path, you will need to make two From/To entries, one in each direction.

 

 

 

## The Network Nodes Window

 

 

The NetworkNode page of the traffic control object lets you edit a traffic control's connections to network nodes. The panel on the left shows all NetworkNodes that are connected to the traffic control object. Select one of these nodes, and the panel on the right will refresh to a new list of traffic controls. The panel on the right shows all traffic controls that are connected to the NetworkNode you have selected on the left. The traffic control you are editing will have an extra " -- This TrafficControl" text, letting you know how this traffic control is situated among other traffic controls. The ranking of traffic controls in the right panel can have a significant effect on the behavior of the network.

---
id: reference_processflowobjects_sharedassets_exitzone_exitzone
name: "Exit Zone"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","SharedAssets","ExitZone","ExitZone"]
tags: ["manual","reference","processflowobjects","sharedassets"]
source: "manual/Reference/ProcessFlowObjects/SharedAssets/ExitZone/ExitZone.html"
---

# Exit Zone

## Overview and Key Concepts

 

This activity allows tokens to exit a Zone shared asset.

 

 

When this activity is first created, a red exclamation mark shows up to the right of the activity notifying you that a link to a Zone is required for this activity to function. This link may be a direct pointer which can be created by clicking on the exclamation mark and then clicking on a Zone object, or the reference may be dynamic by setting the value for the **Zone Reference** field.

 

 

 

## Connectors

 

The Exit Zone activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Exit Zone activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Zone Reference

 

The reference to the Zone shared asset that the token is exiting.

 

### Exit For

 

You can control which tokens will exit the Zone when the entering token arrives at this activity using the **Exit For** box.

 

### Non-Member Exit

 

Define what should happen if the exiting token never entered the Zone. The possible options are:

 

- **Do Not Allow** - An error will be thrown and the model stopped.
- **Ignore** - The exiting token will be ignored.
- **Update Statistics** - The Zone will update its statistics to reflect a token exiting.

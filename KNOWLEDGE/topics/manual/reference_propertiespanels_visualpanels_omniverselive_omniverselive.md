---
id: reference_propertiespanels_visualpanels_omniverselive_omniverselive
name: "Omniverse Live"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","VisualPanels","OmniverseLive","OmniverseLive"]
tags: ["manual","reference","propertiespanels","visualpanels"]
source: "manual/Reference/PropertiesPanels/VisualPanels/OmniverseLive/OmniverseLive.html"
---

# Omniverse Live

The Omniverse Live panel displays options for collaborative editing of USD stages on Nvidia Omniverse Nucleus servers.

 

 

The following properties are on the Omniverse Live panel:

 

### Session Name

 

Existing sessions can be chosen from the dropdown menu. To create a new session, type a session name into the field.

 

### Create

 

If the specified session name doesn't exist, a Create button is available to create a new session with that name.

 

### Join

 

If the specified session name does exist, a Join button is available to join the session with that name.

 

### Leave

 

If you are currently connected to a session, a Leave button is available to leave the session and optionally apply changes from the live session to the original stage.

 

**Leave Session**. This will leave the live session and reload the original stage, ignoring changes made in the live session.

 

**End and merge to root layer**. This will leave the live session and apply any changes from the live session to the original stage's root layer.

 

**End and merge to new layer**. This will leave the live session and apply any changes from the live session as a new layer within the original stage without modifying the original stage's root layer.

 

### Process Live Updates

 

If you are currently connected to a session, live updates to the usd stage should automatically be sent to the server and received from the server. This button is available to explicitly send and receive those updates immediately.

 

The Save button on the Stage Panel can be used to apply local changes from FlexSim to the stage and send those updates to the live stage.

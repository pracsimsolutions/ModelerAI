---
id: reference_propertiespanels_processflowpanels_processflow_processflow
name: "Process Flow"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","ProcessFlowPanels","ProcessFlow","ProcessFlow"]
tags: ["manual","reference","propertiespanels","processflowpanels"]
source: "manual/Reference/PropertiesPanels/ProcessFlowPanels/ProcessFlow/ProcessFlow.html"
---

# Process Flow

The Process Flow panel has general settings for Process Flows.

 

 

The following properties are on the Process Flow panel:

 

### Image

 

The Image box sets the icon image for the process flow. This icon is displayed on the tab of the process flow and in the Toolbox. By default, all process flows use the flowchart icon .

 

To change the icon, you can click the Browse button to navigate to the icon file in your computer.

 

### Theme

 

A theme is a group of visual settings that can be applied to an entire process flow. A theme can affect:

 

- The color of the background, activities, shared assets, tokens, etc.
- The styles of the connectors
- The font and style of the fonts
- The color and visual functionality of the containers (flow chart objects)

 

See Changing Process Flow Visuals for more information.

 

### More Theme Settings

 

Use the More Theme Settings button to open the Process Flow Visual Theme properties window. See Process Flow Visual Theme Properties for an explanation of its properties.

 

### Edit Mode

 

The Edit Mode checkbox turns on and off the Edit Mode, which changes the Process Flow to be more user friendly for simply running a model. Turning Edit Mode off is mainly intended for use after creating a Process Flow. It will hide variables that are not set as *User Accessible* and it will make it so the default action when selecting a Process Flow is to open its properties window and not the model view window.

 

### Snap to Grid

 

The Snap to Grid checkbox turns snapping to the grid on and off. By default, activities and display objects will automatically align to one another as they are being dragged. The grid offers an additional mechanism to neatly lay out objects.

 

### Show Grid

 

The Show Grid checkbox displays or hides the grid.

 

### Always Stack Tokens

 

If checked, tokens will always be displayed as a single stack of tokens with a number indicating how many tokens are in the stack. When not checked, tokens will be displayed side-by-side unless there is not enough room in the activity to display this way and then they will be displayed in a stack.

 

### Trace Histories Off

 

If checked, trace histories for all activities will be turned off even if some activities are set to display them.

 

### Ticker-Based Event Management

 

Check this box and define a tick interval to make the process flow use ticker-based event management instead of the standard event management. This can be useful in situations where there are many tokens (i.e. thousands+) that are processing events at a high rate. When there is a large population of tokens, and each token is creating events independently, sortation of the event queue can become very slow. Ticker-based event management can alleviate this issue.

 

When using ticker-based event management, events that are created through the process flow will round their times up to the nearest "tick". Events in a given "tick" will be executed in the order that they were created. This can significantly save sorting time when there are a lot of events in the same tick.

 

Ticker-based event management can be expecially useful in agent-based simulations, where there are large populations of agents, and the agents are updating their state, based on their environment, at regular time intervals.

 

### View Tokens

 

You can click the View Tokens button to open a window where you can see an overview of all tokens during a simulation run. The View Tokens window shows the labels and details associated with all the tokens in the Process Flow. This is particularly useful for troubleshooting your process flow. See Troubleshooting Process Flows for more information.

 

### View Activities

 

You can click the View Activities button to open window where you can see an overview of all the activities in your process flow. You can also use this window to change the statistics settings of the activities in the Process Flow and to set up tracing options, which can help with troubleshooting your process flow. See Troubleshooting Process Flows for more information.

 

### More Properties

 

You can click the More Properties button to open additional general properties for this process flow. You can use this window to set the variables used in the Process Flow as well as visual settings, labels, and triggers.

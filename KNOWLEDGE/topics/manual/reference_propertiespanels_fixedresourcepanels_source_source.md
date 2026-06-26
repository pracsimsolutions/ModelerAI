---
id: reference_propertiespanels_fixedresourcepanels_source_source
name: "Source"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","FixedResourcePanels","Source","Source"]
tags: ["manual","reference","propertiespanels","fixedresourcepanels"]
source: "manual/Reference/PropertiesPanels/FixedResourcePanels/Source/Source.html"
---

# Source

The Source panel defines what items the source creates and when they're created.

 

 

The following properties are on the Source panel:

 

### FlowItem Class

 

This is used to define what class of flow item the source will create. To view and edit flow item classes, press the **Flowitem Bin** button to the right of the field, or open the Flowitem Bin using the Toolbox.

 

### Arrival Style

 

This menu specifies the way that the source creates flow items. It has the following options:

 

- **Inter-Arrival time** - After a set period of time, the source creates one flow item. This repeats until the model is stopped.
- **Arrival Schedule** - The source follows a table that defines when flow items are created, how many there are, and what labels are assigned to them.
- **Arrival Sequence** - The source follows a table that defines what order flow items are created in. Flow items are created as fast as the source can move them downstream.

 

## Inter-Arrival Time Properties

 

The following properties only appear when **Inter-Arrival Time** is selected as the Arrival Style:

 

### Arrival at time 0

 

If this is checked, one flow item will be created at time 0. The next will be created at the end of the first inter-arrival time.

 

### Inter-Arrivaltime

 

A function that returns the amount of time the source should wait before creating the next flow item.

 

## Arrival Schedule/Sequence Properties

 

 

The following properties only appear when **Arrival Schedule** or **Arrival Sequence** is selected as the Arrival Style:

 

### Repeat Table

 

If this box is checked, the schedule or sequence will continually repeat itself until the model is stopped. In the case of a schedule, the arrival defined in row 1 will occur 0 seconds after the time defined for the arrival in the last row. This means that the arrival time for the very first row of the table is only used once for a given simulation. Note that this initial delay can be used as a warm-up period. If you would like to specify an interval time between the arrival time of the last row and the arrival time of the first for when repeated, then add another row to the end of the table and specify a quantity of 0.

  

### Edit Table

 

This opens the Arrivals window where you can edit the arrivals in a table view.

 

## The Arrivals Window

 

 

The Arrivals window has the following properties:

 

### Arrivals

 

This specifies how many rows are in the arrival table.

 

### Labels

 

This specifies how many columns are in the arrival table. Additional columns will be added for labels and their initial values that will be added to the flow items when they are created.

 

### Add to Excel Importer

 

- This button adds the table as a row in the Excel Importer.

 

### ArrivalTime

 

This is the time in model time units that the arrival will occur (Arrival Schedule only).

 

### ItemName

 

Specifies the name of the flow item when it's created.

 

### Quantity

 

This number specifies how many flow items will be created during this arrival.

 

### Label Columns

 

These columns specify the labels that will be added to created flow item and their initial values.

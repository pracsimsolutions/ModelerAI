---
id: reference_processflowobjects_sharedassets_zone_zone
name: "Zone"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","SharedAssets","Zone","Zone"]
tags: ["manual","reference","processflowobjects","sharedassets"]
source: "manual/Reference/ProcessFlowObjects/SharedAssets/Zone/Zone.html"
---

# Zone

## Overview and Key Concepts

 

The Zone keeps statistics for a group of activities within process flow. Optionally, it can restrict access to those activities, based on the tokens within those activities.

 

 

 

 

## Connectors

 

Only Enter Zone and Exit Zone activities can be connected to a Zone. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows the property for the Zone shared asset:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Type

 

Zones can be available globally or locally. The Zone keeps statistics and enforces its Max Content and Constraints per instance.

 

### Use Max Content

 

The **Use Max Content** checkbox determines whether or not the Zone will use a maximum content. The **Max Content** box is enabled when this box is checked, and disabled when it is not.

 

### Max Content

 

If **Use Max Content** is checked, the value specified in this box determines the maximum number of tokens that can be in the Zone at any given time.

 

### More Properties

 

The **More Properties** button opens the Zone Properties Window. This window allows you to create and edit Subsets, Calculations, and Constraints. It also allows you to manage some of the Zone's advanced features. It has five tabs, which are explained in the following sections.

 

 

 

## The Subsets Tab

 

The following image shows the **Subsets** tab:

 

 

Each property is described in the following sections.

 

### Add Subset

 

The **Add Subset** button adds a new Subset to the Zone. It is the topmost button on the tab.

 

### Subset Name

 

The **Subset Name** box allows you to edit the name of the subset.

 

### Token Selection Criteria

 

The **Token Selection Criteria** box allows you to edit the Token Selection Criteria for the Subset.

 

### Add Calculation

 

The **Add Calculation** button allows you to add a new calculation to the current Subset. It is found inside the **Subset Calculations** area.

 

### Evaluate Calculations On Exit

 

Turns on the Evaluate Calculations On Exit option for the Zone.

 

### Calculation Name

 

You can edit the name of each Calculation by editing the text in this column.

 

### Label/expression to sum

 

You can edit the expression to sum by edition the text in this column.

 

 

 

## The Subset Constraints Tab

 

The following image shows the **Subset Constraints** tab:

 

 

Each property is described in the following sections.

 

### Add Constraint

 

Click the **Add Constraint** button to add a Constraint to a specific Subset.

 

### Calculation

 

You can choose which Subset-related value to constrain using the **Calculation** drop-down list box.

 

### Value

 

Enter a limit for the value specified in the **Calculation** drop-down list box.

 

 

 

## The Partitions Tab

 

The following image shows the **Partitions** tab:

 

 

Each property is described in the following sections.

 

### Partition By

 

You can use this field to specify how the Zone should partition its tokens.

 

### Add Calculation

 

The **Add Calculation** button allows you to add a new calculation to all Partitions. It is found inside the **Partition Calculations** area.

 

### Calculation name

 

You can edit the name of each Calculation by editing the text in this column.

 

### Label/expression to sum

 

You can edit the expression to sum by edition the text in this column.

 

 

 

## The Partition Constraints Tab

 

The following image shows the **Partition Constraints** tab:

 

 

Each property is described in the following sections.

 

### Add Partition Constraint

 

The **Add Partition Constraint** button adds a new Partition Constraint to the Zone. It is the topmost button on the tab.

 

### Calculation

 

You can choose which Partition-related value to constrain (for all Partitions) using the **Calculation** drop-down list box.

 

### Add Exception

 

You can use the **Add Exception** button to add an exception to the Partition Constraint.

 

### Rows

 

You can use this field to specify how many exceptions there are to this Partition Constraint. This is especially useful if you are copying the data from an Excel spreadsheet. Changing this field will add or remove exceptions as necessary.

 

### Partition

 

This field specifies which particular partition will be affected by the exception. Note that this field accepts text or number data.

 

### Limit

 

This field the limit for the partition involved in the exception.

 

 

 

## The Advanced Tab

 

The following image shows the **Advanced** tab:

 

 

Each property is described in the following sections.

 

### Queue Strategy

 

The **Queue Strategy** box allows you to edit the expression for the Queue Strategy, which should be an ORDER BY expression.

 

### Enforce Queue Order

 

Check the **Enforce Queue Order** box to have the Zone enforce the request queue order.

 

### Wait for Simultaneous Arrivals Before Entry

 

Check the **Wait for Simultaneous Arrivals Before Entry** box to have the Zone allow multiple tokens to arrive in the same instant, before evaluating the Queue Strategy.

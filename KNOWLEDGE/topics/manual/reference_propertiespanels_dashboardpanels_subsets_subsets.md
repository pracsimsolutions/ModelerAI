---
id: reference_propertiespanels_dashboardpanels_subsets_subsets
name: "Subsets"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","DashboardPanels","Subsets","Subsets"]
tags: ["manual","reference","propertiespanels","dashboardpanels"]
source: "manual/Reference/PropertiesPanels/DashboardPanels/Subsets/Subsets.html"
---

# Subsets

The Subsets panel contains options for creating subsets.

 

  

#### Subset Limits

 

All People Templates have a Subsets panel. It is identical to the Subsets tab on the Calculated Table, with one exception: some People Templates will not allow you to make Subsets, if you have a certain number of partitions. Details about this exception are documented with each particular template.

  

The following properties are on the Subsets panel:

 

### Subset List

 

- **Subset Number** - Displays the rank of the subset currently being viewed
- **Subset Name** - Displays and allows you to edit the name of the selected subset
- - Adds a new subset. There are two possible options:
- Add subsets for People Groups or Objects - Adds a special kind of subset that makes it easy to compare group and individualstatistics for People/HC groups and objects.
- Add basic subset - Adds a general use subset.

 - Duplicates the selected subset - Moves the selected subset up in the list - Moves the selected subset down in the list - Deletes the selected subset  

### From Group

 

Allows you to specify which People/HC resource group to use. This field will be set to the correct value if the list of tables includes one of the resource group tables - StaffGroups, LocationGroups, TransportGroups, or EquipmentGroups.

 

### Include data for

 

Select what kind of data for each selected group should be included. There are three possible options:

 

- **Each group as a whole** - creates one subset per selected group that only include rows that deal with objects from the whole group.
- **Each member of each group** - creates one subset per member of each selected group. Each created subset only includes rows that deal with the specific member.
- **Each group and each member** - creates one subset per group as well as one subset per member of the group.

 

### Subgroups

 

Use the checkboxes to choose which groups to include in the final table.

 

### Select all subgroups

 

Press this button to select all subgroups shown in the list except the overall group.

 

### Clear all subgroups

 

Press this button to clear all subgroups shown in the list except the overall group.

 

### Additional Filters

 

If you require filters in addition to showing only group or individual rows, you can use this section to add them. They will be combined with the group or individual filters using an AND statement.

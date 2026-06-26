---
id: reference_propertiespanels_warehousingpanels_storageoptions_storageoptions
name: "Storage Options"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","WarehousingPanels","StorageOptions","StorageOptions"]
tags: ["manual","reference","propertiespanels","warehousingpanels"]
source: "manual/Reference/PropertiesPanels/WarehousingPanels/StorageOptions/StorageOptions.html"
---

# Storage Options

The Storage Options panel defines some extra options for the storage object.

 

 

The following properties are on the Storage Options panel:

 

### Mark Slots with Outbound Items

 

This checkbox will highlight slots that have outbound items in a red color.

 

### Virtualize Items

 

If this box is checked, entering items will be virtualized. Essential label values and visual properties will be preserved, but the main item will be destroyed. The item will be recreated the next time it is accessed.

 

### Extend Columns

 

If this box is checked, the object's support columns will be drawn extending above the top shelf. This property is not applicable with Floor Storage objects.

 

### Shelf Tilt Amount

 

This number defines the amount of tilt of items placed in a given slot of the rack. This mostly used in Gravity Flow and Push Back Racks, products slide down from the back of the rack to the front. This property is not applicable with Floor Storage objects.

 

### Pick/Place Y Offset

 

This value is used to configure how close transport objects come to the storage object when they drop off or pick up flow items from the rack. This is especially useful if operators are used to drop off and pick up from the rack, because often they will walk into the middle of the rack to get a flow item. Specify a value of 1, for example, and the operators will keep better distance from the rack when dropping off and picking up flow items. This property is not applicable with Floor Storage objects.

 

### Flow Speed

 

Flow Speed is only visible on Gravity Flow or Push Back Racks. This property defines the speed by which items flow down the rack. Unless using fixed resource logic, this property is purely for animation purposes, and does not affect model results.

 

### Drive Through

 

Drive Through is only visible on Drive In Racks. This checkbox will cause items to be filled from on side and picked on the other.

---
id: reference_propertiespanels_fixedresourcepanels_combiner_combiner
name: "Combiner"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","FixedResourcePanels","Combiner","Combiner"]
tags: ["manual","reference","propertiespanels","fixedresourcepanels"]
source: "manual/Reference/PropertiesPanels/FixedResourcePanels/Combiner/Combiner.html"
---

# Combiner

The Combiner panel defines how a Combiner object combines flow items.

 

 

The following properties are on the Combiner panel:

 

### Combine Mode

 

Selects the mode that the combiner is operating in. The container flow item is the item received through port 1. All other flow items are received through ports 2 and higher. It has the following modes:

 

- **Pack** - All other flow items are moved into the container flow item and then the container flow item is released.
- **Join** - All other flow items are destroyed and then the container flow item is released.
- **Batch** - All flow items are released when the batch is collected.

 

### Components List

 

This table is used to define how many of each type of flow item the combiner will collect before sending the completed collection downstream. The combiner will use the flow item that arrives through input port one as the container object and will only accept one of them. Each row in the table represents arrivals from input ports numbered two and above. To update this list dynamically during a model run, use the Update Combiner Component List picklist option in the OnEntry trigger.

 

### Target Quantity

 

The required number of flow items to be received through the associated input port for each operation.

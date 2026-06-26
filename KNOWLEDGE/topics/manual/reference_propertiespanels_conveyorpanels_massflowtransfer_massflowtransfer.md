---
id: reference_propertiespanels_conveyorpanels_massflowtransfer_massflowtransfer
name: "Mass Flow Transfer"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","ConveyorPanels","MassFlowTransfer","MassFlowTransfer"]
tags: ["manual","reference","propertiespanels","conveyorpanels"]
source: "manual/Reference/PropertiesPanels/ConveyorPanels/MassFlowTransfer/MassFlowTransfer.html"
---

# Mass Flow Transfer

The Mass Flow Transfer panel defines properties for a mass flow transfer.

 

  

### Properties Only Apply to Downstream Transfers

 

This panel will only be visible on downstream mass flow transfers, namely the transfers with input ports. Upstream transfers that have output ports do not use these properties.

  

The following properties are on the Mass Flow Transfer panel.:

 

### Convert To Unit

 

This allows you to optionally convert from a given upstream flow unit to a different downstream unit. For example, you may have a cartonizer in your model that receives bottles and packs them into cartons. Traditionally you might 'discretize' at this point, i.e. use a Mass Flow Exit Transfer to translate flow into discrete flow items. However, depending on the required throughput rate of such a machine, simulating each carton as a discrete item could significantly slow your simulation run speed. So instead of discretizing at this point, you could let the flow remain 'fluid', continuing to use mass flow conveyors, but convert to a 'carton' flow unit.

 

### Conversion Scale

 

Defines the 'scale' factor for converting from one flow unit to a different unit. This property is only used if you define the Convert To Unit property. It is the target number of output (downstream) units divided by the number of input (upstream) units. In the example cited above, if you pack 6 bottles into a single carton, then the Conversion Scale should be 1 / 6.

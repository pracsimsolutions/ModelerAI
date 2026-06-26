---
id: reference_propertiespanels_fixedresourcepanels_input_input
name: "Input"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","FixedResourcePanels","Input","Input"]
tags: ["manual","reference","propertiespanels","fixedresourcepanels"]
source: "manual/Reference/PropertiesPanels/FixedResourcePanels/Input/Input.html"
---

# Input

The Input panel defines how an object pulls flow items from upstream objects.

 

 

The following properties are on the Input panel:

 

### Pull

 

If this box is checked, the object will pull flow items from upstream objects. The upstream objects should open all their output ports to allow the object to pull the flow items it needs.

 

### Pull Strategy

 

This parameter is only visible if **Pull** is checked. This picklist returns the input port number connected to the object that the next flow item is to be pulled from. This field is evaluated only on reset of the model and when the pulling object becomes ready to receive its next flow item. For a Processor with a capacity of 1, this means that the Pull Strategy field will only be evaluated once right after each flow item exits the Processor. See also Fixed Resource Concepts: Pull Strategy.

 

### Pull Requirement

 

This parameter is only accessible if **Pull** is checked. This picklist needs to return either a true or a false (1 or 0). This field is evaluated when considering whether or not to pull in a particular flow item from the upstream object that was defined by the "Pull from port" field. This field will only be evaluated for flow items that are in the "ready" state (i.e.FRSTATE_READY) meaning the flow items are ready to leave the upstream object. Basically, the "Pull Requirement" field is evaluated for every "ready" flow item immediately after the "Pull from port" field gets evaluated. The field is evaluated again for each new flow item that later becomes ready in the upstream object. See also Fixed Resource Concepts: Pull Requirement.

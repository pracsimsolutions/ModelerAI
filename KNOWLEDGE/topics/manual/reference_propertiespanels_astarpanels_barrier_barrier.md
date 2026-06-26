---
id: reference_propertiespanels_astarpanels_barrier_barrier
name: "Barrier"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","AStarPanels","Barrier","Barrier"]
tags: ["manual","reference","propertiespanels","astarpanels"]
source: "manual/Reference/PropertiesPanels/AStarPanels/Barrier/Barrier.html"
---

# Barrier

The Barrier panel defines the position of the barrier and lets you set a condition.

 

 

The following properties are on the Barrier panel:

 

### Position and Size (Barriers)

 

Specify the size and position of the barrier.

 

### Conditional Rule

 

Define the condition that is required for the barrier to be applied. When a barrier is conditional, its condition is reevaluated at the beginning of every travel task. If the condition returns false (0), then the barrier will not be applied in the route calculation algorithm, and the traveler will travel as if the barrier did not exist. Conditional barriers let you define certain barriers as only applicable for certain travelers, or based on certain model conditions.

 

You can add conditional rules through the A* Properties window.

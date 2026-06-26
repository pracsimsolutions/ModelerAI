---
id: connectingflows_conveyors_keyconceptsconveyors_keyconceptsconveyors
name: "Key Concepts About Conveyors"
kind: manual
breadcrumb: ["ConnectingFlows","Conveyors","KeyConceptsConveyors","KeyConceptsConveyors"]
tags: ["manual","connectingflows","conveyors","keyconceptsconveyors"]
source: "manual/ConnectingFlows/Conveyors/KeyConceptsConveyors/KeyConceptsConveyors.html"
---

# Key Concepts About Conveyors

## Why Use Conveyors?

 

Installing a conveyor system at a business facility generally involves significant costs. It can also be costly to change an existing conveyor system. With FlexSim, you can experiment risk-free until you find the ideal way to configure your conveyor system and maximize its throughput.

 

 

FlexSim includes two main types of conveyors: discrete item conveyors and mass flow conveyors. Discrete item conveyors, which we usually just refer to as conveyors, simulate the movement of discrete flow items. Mass flow conveyors let you build high-volume high-speed manufacturing models, such as bottling or food production systems, by simulating flow rates and the movement of product densities through a system, instead of simulating individual items.

 

### Discrete Item Conveyor Features

 

FlexSim's conveyor objects have several useful features:

 

- **Photo Eyes, Decision Points, and Stations** - You can use photo eyes, decision points, and stations to place routing/decision/control logic anywhere in your conveyor system. From simple movement/rotation logic to area restriction or triggering sophisticated process flows, executing logic at defined points in your conveyor system is incredibly easy.
- **Flexible Movement Controls** - FlexSim's conveyor system has many possible options for controlling how items transfer between the conveyors, including speed, delay time, and pop-up distance. You can also adjust a roller skew angle on a conveyor, so that items will accumulate on one side of the conveyor if needed. You can also tilt, translate, and rotate items while moving along the conveyor.
- **Item Orientation** - You can define any orientation for items on a conveyor. This orientation will automatically persist across multiple sections, and will update its calculations to match the object's orientation when the item transfers through side transfers.
- **Power and Free** - With the conveyor system you can simulate power and free systems, which simulate fixed interval movement of items on a dog chain.
- **Slug Building and Merge Control** - You can create conveyors that will accumulate a slug of items on a conveyor and then release it once the slug is ready. Using this feature in conjunction with the merge controller object and/or the Process Flow tool, you can easily implement sawtooth merges without writing any custom code.
- **Range-Based Transfer Points** - Operators can pick up or drop off items from a range of possible transfer points along the conveyor rather than a single fixed point, making it easier to simulate picking operations. When picking items from a conveyor, operators will automatically predict a proper pick up point on the conveyor based on item speed, operator speed, and distance.

 

### Mass Flow Conveyor Features

 

FlexSim's mass flow conveyor objects have several useful features:

 

- **High Speed Simulation** - Since FlexSim's mass flow conveyors do not simulate individual items, they enable you to easily simulate the production of any number (hundreds, thousands, millions, ...) of bottles/cartons/cans, etc. In this design, run speed is only constrained by how often flow rates change, not by how many 'units' you are producing. Generally, given this, a whole day's production can be simulated in a few seconds or less.
- **High Fidelity Graphics** - While FlexSim's mass flow conveyors do not track individual items, the mass flow conveyor system includes an innovative rendering algorithm that will draw individual bottles/cartons/etc. flowing down the conveyor, even though they are not individually tracked. This graphic fidelity goes a long way in troubleshooting as well as selling your ideas to decision makers.
- **Sophisticated Flow Rate Splitting** - When you want to split flows between multiple conveyors, FlexSim's flow distribution algorithm works intuitively in that wider conveyors will naturally receive a larger proportion of flow. Additionally, you can custom configure priorities for splitting flows downstream.
- **Multiple Configurable Bottle/Can/Carton Types** - FlexSim's mass flow units let you configure the geometry of different types of product. When you then send these different types of product into your system, the conveyors automatically calculate flow densities, capacities, and flow rates based on these flow unit dimensions and attributes.
- **Photo Eyes** - You can use FlexSim's standard conveyor photo eye on mass flow conveyors. Here some of the properties you enter change, but the photo eye still allows you to trigger logic when certain flow rates occur, or when the system accumulates to some point.
- **Easy Flow Generation** - You can easily generate flow directly from an individual conveyor, or you can convert discrete items into mass flow rates using a mass flow entry transfer.

 

Many conveyor model-building concepts work for both mass flow conveyors and regular conveyors. For example, the methods for creating and connecting conveyors are the same for both types of conveyors. However, for other concepts like creating conveyor logic, this user manual section deals primarily with regular discrete item conveyors. For information on mass flow conveyors, see the reference topic on Mass Flow Conveyors.

 

 

 

## Conveyor System Settings

 

The Conveyor System properties window can be used to set the default properties for all conveyor objects in your simulation model. For example, you can create custom conveyor types and then use the conveyor system tool to ensure that each new conveyor you create is automatically assigned that type. You can also use the Conveyor System properties to change the visual settings for the conveyor system. See Reference - Conveyor System for more information.

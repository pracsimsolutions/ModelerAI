---
id: connectingflows_warehousing_racktypes_racktypes
name: "Types of Racks and Storage Options"
kind: manual
breadcrumb: ["ConnectingFlows","Warehousing","RackTypes","RackTypes"]
tags: ["manual","connectingflows","warehousing","racktypes"]
source: "manual/ConnectingFlows/Warehousing/RackTypes/RackTypes.html"
---

# Types of Racks and Storage Options

## What Types of Racks are Available?

 

One of the key design choices is what types of racks and storage space you want to use in your model. FlexSim offers multiple different types of racks and storage options, that can be used in a variety of ways. Each of the different racks provide different visualization and interaction options with operators and transports.

 

The different rack type options include:

 

- **Rack** The most basic and simple shelving structure. This is just a basic Rack with shelves. By default it will load in a First in First Out order.
- **Floor Storage** Storage space on the floor that utilizes the same matrix organizational tools for racks. This type of Storage Object is used often for large areas of pallets, or staging areas either in receiving or shipping. Any type of organized floor storage area would best be represented with this type of object.
- **Gravity Flow Rack** A rack that operates with rollers. Operators and Transports will load onto the back of the rack, while the pick-face of the rack is on the front. This type of rack will visualize objects rolling to the front pick-face of the rack. The nature of this Rack Type is to be a "first in first out" loading scheme. This type of rack also has a small animation of the items rolling to the front of the pick-face of the rack.
- **Push Back Rack** A rack that operates similarly to the Gravity Flow Rack but will be loaded both to and from the pick-face of the rack. This rack is meant to simulate the "first in last out style" of a Push Back Rack both visually and logically. Also like the Gravity Flow Rack, the Push Back Rack has animations for the items as they are pushed into the slots. One reason to use a Push Back Rack as opposed to a Gravity Flow Rack, is if your rack were against a wall, and required both loading and unloading from the pick-face.
- **Drive-in Rack** This rack visually represents the framework of a rack meant to allow forklift trucks to drive into the storage area to drop off pallets.

 

 

 

## Rack Visualization

 

Each type of Rack has its own unique visuals. All of these components can be edited and altered to whatever your warehouse model requires. Whenever a new Rack or Storage Object is dragged out into the model, it creates a Storage System object in your Tools. When opening the Storage System object's properties window, you will see a Visualization tab. It is within this tab that all the visual elements of your racks can be edited. For more information, please see the Visualization Tab section in the Storage System documentation.

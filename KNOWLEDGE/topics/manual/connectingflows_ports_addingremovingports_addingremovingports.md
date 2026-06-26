---
id: connectingflows_ports_addingremovingports_addingremovingports
name: "Adding and Removing Ports"
kind: manual
breadcrumb: ["ConnectingFlows","Ports","AddingRemovingPorts","AddingRemovingPorts"]
tags: ["manual","connectingflows","ports","addingremovingports"]
source: "manual/ConnectingFlows/Ports/AddingRemovingPorts/AddingRemovingPorts.html"
---

# Adding and Removing Ports

## Adding Ports

 

The easiest way to create port connections between two objects is to use the connection icons on the orb of the selected object. The triangle icon creates output port connections and the square icon creates center port connections.

 

 

To create port connections click on the icon and then drag to another object.

 

 

### Connection Mode

 

Another way to create port connections is to turn on *connection mode* in FlexSim. When you are in connection mode, your mouse pointer will change to a plus sign with a chain link symbol next to it. While in connection mode, you can connect two objects together:

 

 

One way to turn on connection mode is to use the Connect Objects button:

 

1. Click the **Connect Objects** button on the toolbar to open a menu. Select the type of port connection you want to make from the menu to turn on connection mode. See Types of Port Connections for information these options.
1. When you are in connection mode, your mouse pointer will change to a plus sign with a chain link symbol next to it:
1. Once you are in connection mode, you can connect two objects together. Click the first object you want to connect in your model. You will notice as you move your mouse that a yellow line will appear between the object you clicked and your cursor.
1. Click a second object to create the port connection between the two objects. NOTE: If you are making an input/output port connection, FlexSim will create an output port on the first object you click and an input port on the second object you click.
1. You will still be in connection mode even after you connect two objects together. You can continue connecting objects together if needed.

 

You can use a few different methods to turn off connection mode when you are using the Connect Objects tool:

 

- Press the Esc key.
- Right-click the mouse.
- Click the **Pointer** button on the toolbar.

  

#### Keyboard Shortcuts

 

You can also use keyboard shortcuts to turn connection mode on or off. You will use a different keyboard shortcut based on the type of port connection you want to make between two objects:

 

- Input/Output Connections: Press and hold down the A key.
- Center Port Connections: Press and hold down the S key.
- Extended Connections: Press and hold down the D key.

 

Connection mode will automatically turn off as soon as you stop pressing the shortcut key. Continue pressing and holding down the key if you want to continue connecting more objects together.

   

 

 

## Removing Ports

 

The easiset way to remove port connections is to select the connection by clicking on it and then pressing the delete key.

 

 

### Disconnection Mode

 

Another way to remove port connections is to turn on disconnection mode. One way to turn on disconnection mode is to use the Disconnect Objects tool:

 

1. Click the **Disconnect Objects** menu on the toolbar to open a menu. Select the type of port connection you want to remove from the menu.
1. When you are in disconnection mode, your mouse cursor will change to a plus sign with a broken chain link symbol next to it:
1. Once you are in disconnection mode, you can remove a connection between two objects. Click the first object you want to disconnect. You will notice as you move your mouse that a yellow line will appear between the object you clicked and your cursor.
1. Click the object to which it is connected. The port connection will disappear.
1. You will still be in disconnection mode even after you remove the connection between two objects. You can continue disconnecting objects together if needed.

 

You can use a few different methods to turn off disconnection mode when you are using the Disconnect Objects tool:

 

- Press the Esc key.
- Right-click the mouse.
- Click the **Pointer** button on the toolbar.

  

#### Keyboard Shortcuts

 

You can also use keyboard shortcuts to turn disconnection mode on or off. You will use a different keyboard shortcut based on the type of port connection you want to make between two objects:

 

- Input/Output Connections: Press and hold down the Q key.
- Center Port Connections: Press and hold down the W key.
- Extended Connections: Press and hold down the E key.

 

Disconnection mode will automatically turn off as soon as soon as you stop pressing the shortcut key. Continue pressing and holding down the key if you want to continue removing port connections.

  

 

 

## Shortcuts

 

FlexSim has some additional shortcuts and tricks that you can use to create port connections, even between multiple objects at a time.

 

### Quick Library

 

You can automatically add 3D objects to a model using the Quick Library. (See Adding 3D Objects to a Model for more information.) Fortunately, you can also create port connections between the objects that you create using the Quick Library:

 

1. Create a port connection as usual, but instead of connecting to an existing object end the connection at a blank space in the model where you want to place the new object.
1. Click the icon for the object you want to add.

 

### Connecting One Object to Multiple Objects

 

To connect one object to several other objects at the same time:

 

1. Hold down the shift key and draw a box around the downstream objects to select the objects you want to connect.
1. Create a connection from the upstream object to any one of the downstream objects. All downstream objects will automatically be connected to the upstream at the same time.

 

### Connecting the Same Type of Object

 You can add several copies of the same object to a model and connect them at the same time: 

1. While pressing and holding the R key, click an object in the Library to enter object creation mode.
1. While continuing to hold the R key, click a blank space in the 3D model to add an object.
1. Continue to hold the R key as you click blank spaces in the model to add any additional objects.

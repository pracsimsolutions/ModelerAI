---
id: object
name: "Object"
kind: class
module: ""
signature: "Object"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class that represents objects in the model."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Tree\\Object.xml"
---

# Object

A class that represents objects in the model.

An Object is a treenode that has object data. Objects have all the functionality of a treenode and also provide
functionality for accessing object data and manipulating 3D objects.

## Methods

### create

```flexscript
Object (static) Object.create(string className)
```

Creates a new instance of a library object.

**Parameters:**
- `className` *(string)* — The class name of a library object.

**Returns:** A new instance of a library object.

Object newProcessor = Object.create("Processor");
Object newConveyor = Object.create("Conveyor::StraightConveyor");

### applyProperties

```flexscript
void Object.applyProperties(string propertyName = "")
```

Commits property change(s) to template instances and overrides.

**Parameters:**
- `propertyName` *(string, default "")* — Optional. The name of a specific property that has changed. A list of valid properties for each object can be found on that object's help page. For example, Fixed Resource Properties.

This method will only have an effect if an object is a

template instance or has
template instances. It should be called after you have directly changed an object's
properties and want that change to be applied to template instances and overrides.

The method call affects the named property you pass in, or, if you pass nothing in,
it applies to all of the object's properties. It will do the following for the
respective properties:

Add Override(s) - If the object is a template instance, it will
check if the property value of the object is different than the value of the
object's template. If so, it will assert that property as an override.

Apply to Inheriting Instances - If the object has template
instances, it will set the same property value on all template instances that do not override
that property.

Note that you do NOT need to call this method when you set an object's property using
setProperty(). That method will automatically call
applyProperties(). You only need to use this method if you have changed an object's
properties through some other mechanism.

queue.size.x = 5;
queue.applyProperties("Size");

The example above demonstrates a time when you would need to call applyProperties(). You
have changed an object's size using Object.size,
instead of by calling setProperty(). Thus, you
must call applyProperties() to apply that new property value to inheriting template instances
and overrides.

### event

```flexscript
Event Object.event(string eventName, Variant req1 = nullvar, Variant req2 = nullvar, Variant req3 = nullvar)
```

Returns an Event of the object based on the given parameters

**Parameters:**
- `eventName` *(string)* — The name of the event.
- `req1` *(Variant, default nullvar)*
- `req2` *(Variant, default nullvar)*
- `req3` *(Variant, default nullvar)*

**Returns:** The resulting Event.

await current.event("OnEntry");

### getLocation

```flexscript
Vec3 Object.getLocation(double xFactor, double yFactor, double zFactor)
```

Gets the object's location.

**Parameters:**
- `xFactor` *(double)* — The x value of the point on the object where the location will be measured from. Measured as a percent of the object's size (0-1).
- `yFactor` *(double)* — The y value of the point on the object where the location will be measured from. Measured as a percent of the object's size (0-1).
- `zFactor` *(double)* — The z value of the point on the object where the location will be measured from. Measured as a percent of the object's size (0-1).

**Returns:** A vector holding the object's location.

This code gets the object's location measured at its center.
Vec3 centerLoc = obj.getLocation(0.5,0.5,0.5);
Vec3 centerLoc = obj.getLocation(Vec3(0.5,0.5,0.5));

### getProperty

```flexscript
Variant Object.getProperty(string propertyName, int flags = 0)
```

Gets the value of an object's property.

**Parameters:**
- `propertyName` *(string)* — The name of the property. A list of valid properties for each object can be found on that object's help page. For example, Fixed Resource Properties.
- `flags` *(int, default 0)* — A set of bitwise flags that modify the returned value. GET_PROPERTY_FLAG_UNITS - for a Unit property it will return a 2-length array with the display value in the first element and the display units in the second. GET_PROPERTY_FLAG_STRING - for an Options property it will return the name of the option instead of its value.

**Returns:** The current value of the property.

This gets the value of the object's MaxContent property.
int maxContent = queue.getProperty("MaxContent");
This gets the display value and units of the object's Speed property.
Array speed = conveyor.getProperty("Speed", GET_PROPERTY_FLAG_UNITS); // [5, "m/min"]

### getVariable

```flexscript
Variant Object.getVariable(string variableName)
```

Deprecated. Use getProperty(). Gets the value of an object's variable.

**Parameters:**
- `variableName` *(string)* — The name of the variable

**Returns:** The value of the specified variable.

This gets the value of the object's maxspeed variable.
double speed = obj.getVariable("maxspeed");

### message

```flexscript
Variant Object.message(double delay = 0, treenode fromObject = 0, Variant msgparam1 = nullvar, Variant msgparam2 = nullvar, Variant msgparam3 = nullvar)
```

Send a message to the object.

**Parameters:**
- `delay` *(double, default 0)* — Optional. Specifies the amount of time to delay before firing the message. If -1, the message will be sent synchronously. If 0 or greater, an event will be created in the defined delay time, and the message will be fired after that delay time.
- `fromObject` *(treenode, default 0)* — Optional. The object that is sending the message. This can be accessed from the message trigger.
- `msgparam1` *(Variant, default nullvar)* — Optional. Defines the first parameter of the message. This can be accessed from the message trigger as msgparam1.
- `msgparam2` *(Variant, default nullvar)* — Optional. Defines the second parameter of the message. This can be accessed from the message trigger as msgparam2.
- `msgparam3` *(Variant, default nullvar)* — Optional. Defines the third parameter of the message. This can be accessed from the message trigger as msgparam3.

**Returns:** If the message is sent synchronously (delay = -1), it will return whatever is returned by the object's message trigger. If the message is sent asynchronously (delay >= 0), the call will return a reference to the event node that was created in the event list.

In the example below, a message is sent to current, with a 0 second delay, and with item as the fromObject
of the message.
current.message(0, item);

### resume

```flexscript
void Object.resume(int id, int stateProfile)
```

Return the object to its state prior to stop().

**Parameters:**
- `id` *(int)* — Optional. For matching stop requests with resume requests.
- `stateProfile` *(int)* — Optional. The state profile on which to record the state.

Tells the object that it may resume whatever it was doing before stop() was called on it. Refer to stop()
for more information.
current.resume(1);

### setLocation

```flexscript
Object Object.setLocation(double x, double y, double z)
```

Sets the object's location.

**Parameters:**
- `x` *(double)* — The desired x position.
- `y` *(double)* — The desired y position.
- `z` *(double)* — The desired z position.

**Returns:** The translated object.

This code sets the object's center to the position (0,0,0).
obj.setLocation(0,0,0,0.5,0.5,0.5);
obj.setLocation(Vec3(0,0,0), Vec3(0.5,0.5,0.5));

### setProperty

```flexscript
void Object.setProperty(string propertyName, Variant value)
```

Sets the value of an object's property.

**Parameters:**
- `propertyName` *(string)* — The name of the property. A list of valid properties for each object can be found on that object's help page. For example, Fixed Resource Properties.
- `value` *(Variant)* — The desired property value.

If this method is called as part of the model processing an event the object will
store the original value of the property
and then that value will be restored when the model is reset.

After setting the property value, the method will call
applyProperties() for the relevant property,
applying the change to template instances and overrides.
queue.setProperty("MaxContent", 5);

### setRotation

```flexscript
Object Object.setRotation(double rx, double ry, double rz)
```

Sets the object's rotation.

**Parameters:**
- `rx` *(double)* — The desired rotation about the x axis.
- `ry` *(double)* — The desired rotation about the y axis.
- `rz` *(double)* — The desired rotation about the z axis.

**Returns:** The rotated object.

obj.setRotation(0,0,0);
obj.setRotation(Vec3(0,0,0));

### setSize

```flexscript
Object Object.setSize(double x, double y, double z)
```

Sets the object's size.

**Parameters:**
- `x` *(double)* — The desired x size.
- `y` *(double)* — The desired y size.
- `z` *(double)* — The desired z size.

**Returns:** The sized object.

obj.setSize(1,1,1);

### setState

```flexscript
void Object.setState(int state, int stateProfile)
```

Sets the object's state.

**Parameters:**
- `state` *(int)* — The desired state.
- `stateProfile` *(int)* — The desired state profile.

Sets the state of the object to a state number. The statistics regarding state will be
automatically updated when this is called. There are 50 predefined states. The
predefined values for state and their associated macros are:
1 - STATE_IDLE
2 - STATE_PROCESSING 3 - STATE_BUSY 4 - STATE_BLOCKED 5 -
STATE_GENERATING 6 - STATE_EMPTY 7 - STATE_COLLECTING 8 -
STATE_RELEASING 9 - STATE_WAITING_FOR_OPERATOR 10 -
STATE_WAITING_FOR_TRANSPORT 11 - STATE_BREAKDOWN 12 -
STATE_SCHEDULED_DOWN 13 - STATE_CONVEYING 14 - STATE_TRAVEL_EMPTY 15 -
STATE_TRAVEL_LOADED 16 - STATE_OFFSET_TRAVEL_EMPTY 17 -
STATE_OFFSET_TRAVEL_LOADED 18 - STATE_LOADING 19 - STATE_UNLOADING 20 -
STATE_DOWN 21 - STATE_SETUP 22 - STATE_UTILIZE 23 - STATE_FULL 24 -
STATE_NOT_EMPTY 25 - STATE_FILLING 26 - STATE_STARVED 27 -
STATE_MIXING 28 - STATE_FLOWING 29 - STATE_ALLOCATED_IDLE 30 -
STATE_OFF_SHIFT 31 - STATE_CHANGE_OVER 32 - STATE_REPAIR 33 -
STATE_MAINTENANCE 34 - STATE_LUNCH 35 - STATE_ON_BREAK 36 -
STATE_SUSPEND 37 - STATE_AVAILABLE 38 - STATE_PREPROCESSING 39 -
STATE_POSTPROCESSING 40 - STATE_INSPECTING 41 - STATE_OPERATING 42 -
STATE_STANDBY 43 - STATE_PURGING 44 - STATE_CLEANING 45 -
STATE_ACCELERATING 46 - STATE_MAXSPEED 47 - STATE_DECELERATING 48 -
STATE_STOPPED 49 - STATE_WAITING 50 - STATE_ACCUMULATING

if (current.subnodes.length <= 1)
current.setState(STATE_EMPTY);

This sets the state of the object referenced by current to "empty" if the content of current is less than or equal to 1.

### setVariable

```flexscript
void Object.setVariable(string variableName, Variant value)
```

Deprecated. Use setProperty(). Sets the value of an object's variable.

**Parameters:**
- `variableName` *(string)* — The name of the variable.
- `value` *(Variant)* — The desired variable value.

If this method is called as part of the model processing an event the object will store the original value of the variable
and then that value will be restored when the model is reset.
obj.setVariable("maxspeed", 5);

### stop

```flexscript
void Object.stop(int state, int id, int priority, int stateProfile)
```

Stops the object, sets its state, and waits for resume().

**Parameters:**
- `state` *(int)* — The state the object will go into.
- `id` *(int)* — Optional. For matching stop requests with resume requests.
- `priority` *(int)* — Optional. For breaking ties between stop() calls.
- `stateProfile` *(int)* — Optional. The state profile on which to record the state.

Tells the object to stop whatever it is doing, go into the state specified (in the state profile if specified), and waits for
resume() to be called. Stop() calls are accumulated, meaning if stop()) is called twice on the same object, the object will
not resume its operations until resume()) has been called twice as well. Stopping an object depends on the type of object
that is being stopped.
For FixedResources, generally events are delayed indefinitely, input and output is stopped, and all operations going into and
out of the object are stopped. This means that TaskExecuters trying to load/unload to or from the object will have to wait
until the object has been resumed.
For TaskExecuters, events are not delayed, but rather a preempting task sequence with priority of 100,000 is created for the
TaskExecuter, with one TE_STOP task in it.
Be aware that, if there are several stop requests for the same object, the state for each stop request is not remembered. If
an object is requested to stop by entity A with state 12, and then is later requested to stop by entity B for state 14, it
will go into state 14 and forget state 12. Even if entity B resumes the object before entity A, the object will remain in
state 14 until all stop requests have resumed.
The id and priority parameters are optional and were added to fix the problem in the previous
paragraph. If you specify these parameters, then it will store a record of the stop request, instead of just incrementing
a value for the number of stops requested. The id is like a key for that stop request. The priority allows the object to
prioritize its stop requests. For example, if you have a stop request for the scheduled down state, and another for the
breakdown state at the same time, technically the object should be in two states at once, but since a FlexSim object can only
be in one state at a time, the priority value breaks the tie and goes into the state of the highest priority stop request.
The id value should match an id value that is added to a resume() call later on. This is for matching stop requests with
their appropriate resume requests. For example, if you stop an object for scheduled maintenance with and id of 1, once
the scheduled maintenance is finished, you will need to resume the object with the same id of 1.
current.stop(STATE_CLEANING, 1);

## Properties

### animations

```flexscript
Object.animations (readonly)
```

Accesses an object's animations.

Gives access to the animations of an object as Animation interfaces.
// Start the "Walk" animation on Operator1
Object obj = Model.find("Operator1");
obj.animations["Walk"].start();

### attrs

```flexscript
Object.attrs (readonly)
```

Accesses object attribute nodes.

Object obj = Model.find("Processor1");
obj.attrs.spatialx.value = 5;

### centerObjects

```flexscript
Object.centerObjects (readonly)
```

Accesses objects connected through center port connections.

Accessing the Number of Center Port Connections
You can access the number of center port connections on the object with the following code.
obj.centerObjects.length
Accessing Objects by Rank
You can access an individual object connected through a center port connection by rank with the following code.

obj.centerObjects[1] // first object
obj.centerObjects[obj.centerObjects.length] // last object
obj.centerObjects[i] // i-th object

Getting an Array of Objects
You can get an Array of all objects connected through a center port connection with the following code.
Array centerObjects = obj.centerObjects.toArray();

### color

```flexscript
Color Object.color (readonly)
```

Get and set the Color of the object.

Object obj = Model.find("Processor1");
obj.color = Color.red;

### flags

```flexscript
Object.flags (readonly)
```

Get and set flags of the object.

isProtected
Get or set the "Protected" flag of the object.
Protecting an object keeps it from being moved, rotated, or resized with the mouse, but not from being double-clicked to
access the Parameters window for the object. View windows have an attribute named viewignoreobjects which can be used to
ignore ALL the objects in their view which means the objects can not be manipulated in any way with the mouse including
double-clicking. Protected objects will have a gray highlight in the treeview to indicate their protected status.
current.flags.isProtected = 1;
isSelected
Get or set the "Selected" flag of the object.
Selecting an object puts a red box around it in both the 3D and tree view. Dragging one selected object in the 3D view will
drag all other selected objects by the same amount. The Edit Selected Objects tool allows you to quickly modify all selected
objects in your model.
if(current.flags.isSelected) return 1;
showConnections
Get or set the "Show Connections" flag of the object.
Hiding the connections will turn off the display of ports and their connections both to and from the object. Note that view
windows have an attribute named hideallconnectors that controls the display of connectors for ALL objects in their view.
obj.flags.showConnections = 0;
showContent
Get or set the "Show Contents" flag of the object.
Hiding the contents of an object will stop the display of objects (e.g. flowitems) contained within it.
current.flags.showContent = 0;
showName
Get or set the "Show Name" flag of the object.
A value of 1 will hide the object's name and stats from being displayed on the screen, a value of 0 will show the object's
name and possibly stats depending on the viewhidealllabels attribute of the view window. The viewhidealllabels attribute of
the view window has the following possible values which will apply to the label display of all objects in the window: 0=show
name and stats, 1=hide name and stats, 2=show just names.
obj.flags.showName = 1;
showShape
Get or set the "Show Shape" flag of the object.
Hiding the object's shape will stop its 3D shape from drawing. Note that view windows also have an attribute called
hidealldrawcontent that controls the display of 3D shapes for ALL objects in their view.
current.flags.showShape = 0;

### inObjects

```flexscript
Object.inObjects (readonly)
```

Accesses objects connected through input ports connections.

Same as the centerObjects property, but for input port connections.

### input

```flexscript
Object.input (readonly)
```

Access methods on the object's input ports connections.

close
Blocks the input of the object. Only the input ports themselves, or the input in general needs to be closed to block entry.
When port connections are displayed with sufficient size in the view window, you will see a small red bar drawn across all
the input ports (squares) for the object, signifying the input ports are blocked. Individual ports may still be open and
drawn in green, but entry to the object is still blocked because of the red bar. Compare with stop().
current.input.close();
open
Unblocks the input of the object after it has been previously blocked with the close() method. It is necessary that both
the input ports themselves, and the input in general, are open before entry will be allowed. It is advised that modelers
never use this method in any field other than the OnMessage field which has been triggered with the senddelayedmessage()
command, because commands which open ports often spawn several other activities that shouldn't be performed during
transitional events. Compare with resume().
current.input.open();
resume
Opens the input of the object just like open() does, but because it keeps track of previous stop() calls on the object, it
will only open input after all stops have been resumed. Refer to open() for additional information.
current.input.resume();
stop
Closes the input of the object just like close() does, but it also keeps track of consecutive stop() calls on the object, and
will only open input after all stops have been resumed. Refer to close() for additional information.
current.input.stop();

### location

```flexscript
Vec3 Object.location (readonly)
```

Get and set the position of the object.

Object obj = Model.find("Processor1");
obj.location = Vec3(0,0,0);

### outObjects

```flexscript
Object.outObjects (readonly)
```

Accesses objects connected through output ports connections.

Same as the centerObjects property, but for output port connections.

### output

```flexscript
Object.output (readonly)
```

Access methods on the object's output ports connections.

Same as the input property, but for output port connections.

### resetPosition

```flexscript
Object.resetPosition (readonly)
```

Accesses an object's reset position.

Allows you to access the reset position of an object.
The property can be read directly to determine if an object's reset position has been set or not.
This property provides the following properties and methods:

location - returns an object's reset position location
rotation - returns an object's reset position rotation
size - returns an object's reset position size
container - returns an object's reset position container
set() - sets an object's reset position to its current location, rotation, size, and container.
clear() - clears an object's reset position.

Object obj = Model.find("Processor1");
if (obj.resetPosition) //Checks to see if its reset positon has been set
return obj.resetPosition.location;// gets the location value of the reset position
else{
obj.resetPosition.set();// set the reset postion on the object
return obj.resetPosition.location; // gets the location value of the reset position
}

### rotation

```flexscript
Vec3 Object.rotation (readonly)
```

Get and set the rotation of the object.

Object obj = Model.find("Processor1");
obj.rotation = Vec3(0,0,0);

### size

```flexscript
Vec3 Object.size (readonly)
```

Get and set the size of the object.

Object obj = Model.find("Processor1");
int xSize = obj.size.x;

### stats

```flexscript
Object.stats (readonly)
```

Allows access to an object's statistics.

Allows you to access the statistics of an object as TrackedVariable objects.
This property provides the following properties and methods:

input - returns the input tracked variable
output - returns the output tracked variable
content - returns the content tracked variable
staytime - returns the staytime tracked variable
totalTravelDistance - returns the total travel distance of a Task Executer
state(Variant& profile = nullvar) - returns a state tracked variable. If no profile argument is given, or if it is 0, then the default state profile is returned. Other
state profiles can be returned by providing the profile number or name.

// Get the input of a processor
Object obj = Model.find("Processor1");
double input = obj.stats.input.value;

// Get the state of a multiprocessor,
// using the multiprocessor state profile
Object obj = Model.find("MultiProcessor1");
int curState = obj.stats.state(1).value;

### stopCount

```flexscript
int Object.stopCount (readonly)
```

Gets the number of stops for the object.

This number increases with calls to the stop method,
and decreases with calls to the resume method.
int numStops = current.stopCount;


---
id: token
name: "Token"
kind: class
module: ""
signature: "Token"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class that represents a Process Flow token."
seeAlso: []
source: "help\\FlexScriptAPIReference\\ProcessFlow\\Token.xml"
---

# Token

A class that represents a Process Flow token.

## Methods

### create

```flexscript
Token (static) Token.create(treenode ownerNode, Variant activity, int start = 0, Token parent = NULL, int labelAcess = 1)
```

Creates a token and places it inside the activity for the given owner object (instance).

**Parameters:**
- `ownerNode` *(treenode)* — The owner object of the process flow. For a General Process Flow this is the process flow object. For a Fixed Resource, Task Executer or Sub Flow, this is the attached object.
- `activity` *(Variant)* — This can be the activity node or the name of the activity. If this is the activity node in a General Process Flow, ownerNode may be NULL.
- `start` *(int, default 0)* — If start is 1, the token will immediately call the activity's start and begin moving through the process flow. If start is 0, the default, the token will sit in the activity until it is manually released. Calling the release method on the token will fire the activity's finish causing the activity's start to never be called. This may affect how the token responds to the activity it was created in. For example, if a token is created in an Assign Labels activity with a start of 1, the created token will evaluate the Assign Labels as if entering the activity and receive whatever labels are assigned to it. If a token is created in the same Assign Labels with a start of 0 and then later it is released, the token will never evaluate the Assign Labels and no labels will be assigned.
- `parent` *(Token, default NULL)* — If another token is specified here, the new token will be created as a child of this one.
- `labelAcess` *(int, default 1)*

Token aToken = Token.create(NULL, getactivity("ProcessFlow", "Start"), 0); //General Process Flow
Token newToken = Token.create(Model.find("Processor1"), "Start", 0, aToken, TOKEN_LABEL_ACCESS_READ);
newToken.Type = 1;
newToken.release(0);

### byID

```flexscript
Token Token.byID(int id, Variant processFlow = NULL)
```

Gets the token associated with the id.

**Parameters:**
- `id` *(int)* — The unique id number of the token.
- `processFlow` *(Variant, default NULL)* — If no process flow is defined, this will search through every ProcessFlow object in the model. When there are multiple process flow objects you can pass in the process flow object (or name) where the token resides for a faster search.

Token's have their own unique id among all instances of all ProcessFlow objects.

Token token = Token.byID(25);
Token token = Token.byID(4, "ProcessFlow1");

### release

```flexscript
Token.release(Variant connector)
```

Releases the token from its current activity.

**Parameters:**
- `connector` *(Variant)* — If this is a number or a string, it identifies the connector to be released to. If it is an object, it directly references the new activity to "jump" to.

token.release("Failed");

### resume

```flexscript
Token.resume()
```

Removes a stop from the token.

Should only be called on stopped tokens. If this call removes the
last stop, the token will re-enter its current activity, and proceed normally.
Stops are added through calls to the stop
method.

### setColor

```flexscript
Token.setColor(int red, int green, int blue, int alpha)
```

Deprecated. Use the color property. Sets the color of the token.

**Parameters:**
- `red` *(int)* — The red color component [0 - 255].
- `green` *(int)* — The green color component [0 - 255].
- `blue` *(int)* — The blue color component [0 - 255].
- `alpha` *(int)* — The alpha color component [0 - 255].

This overrides the Token Colors functionality defined in the visual properties of the
process flow object.
token.setColor(255, 0, 0);

### setShape

```flexscript
Token.setShape(int shapeIndex)
```

Sets the shape of the token.

**Parameters:**
- `shapeIndex` *(int)* — The index of the shape.

This overrides the Token Shapes functionality defined in the visual properties of the
process flow object.
token.setShape(1);

### stop

```flexscript
Token.stop()
```

Adds a stop to the token

Usually, this method is called when the token is blocked in an
activity, such as a Delay. In this case, the token will not leave the activity.
The following table describes the effect of stopping the token in various activities,
if the token was not previously stopped:

ActivityEffect of stop()

Delay
The token's exit time is postponed.

Push To List
The token's Push Value is removed from the list.

Pull From List
The token's back order is removed from the list.

Enter Zone
The token's request to enter the zone is destroyed.

Acquire
The token's request to acquire the resource is destroyed.

Batch
The token is remove from its batch. If it was the only token
in the batch, the batch is destroyed.

Wait for Event
The token's listener for the specified event is destroyed.

Synchronize/Join
The token is removed from its wave. If it was the only token
in the wave, the wave is destroyed.

Travel
The task sequence created by this activity is preempted. This is
true for all Task Sequence activities.

If the token is not currently blocked, it will stop as soon as it becomes blocked.
The token will not continue until all stops have been removed from the token,
through calls to the resume method.
Stopped tokens are marked with a red octogon.

## Properties

### activity

```flexscript
treenode Token.activity (readonly)
```

Gets a reference to the token's current activity.

treenode curActivity = token.activity;

### childRank

```flexscript
int Token.childRank (readonly)
```

Gets the rank of the child token.

Returns 0 if the token has no parent.
int childRank = token.childRank;

### children

```flexscript
Token.children (readonly)
```

Accesses the token's children.

Accessing the Number of Children
You can access the number of children on the token with the following code.
token.children.length
Accessing Children by Rank
You can access an individual child token by rank with the following code.

token.children[1] // first child
token.children[token.children.length] // last child
token.children[i] // i-th child

Getting an Array of Objects
You can get an Array of all child tokens with the following code.
Array childTokens = token.children.toArray();

### color

```flexscript
Color Token.color
```

Gets or sets the color of the token.

The color set through this property overrides
the default color defined in the Visualization settings.
To restore a token to the default color settings,
set this property to transparent black:

token.color = Color();

### entryTime

```flexscript
double Token.entryTime (readonly)
```

Gets the model time of when the token entered its current activity.

double entryTime = token.entryTime;

### id

```flexscript
int Token.id (readonly)
```

Gets the token's unique ID.

int id = token.id;

### instance

```flexscript
treenode Token.instance (readonly)
```

Gets the token's Process Flow instance.

treenode instance = token.instance;

### labelProperties

```flexscript
Variant Token.labelProperties
```

Use your own named properties to get/set label values.

Works just like labelProperties on treenodes.

### labels

```flexscript
Token.labels (readonly)
```

Accesses a node's labels as nodes.

Works just like labels on treenodes.

### name

```flexscript
string Token.name
```

Gets and sets the token's name.

string name = token.name;
token.name = "Bottle";

### parent

```flexscript
Token Token.parent (readonly)
```

Gets the token's parent.

Token parentToken = token.parent;

### processFlow

```flexscript
treenode Token.processFlow (readonly)
```

Gets the token's Process Flow object.

treenode flow = token.processFlow;

### stopCount

```flexscript
int Token.stopCount (readonly)
```

Gets the number of stops for the token.

This number increases with calls to the
stop method,
and decreases with calls to the
resume method.


---
id: treenode
name: "treenode"
kind: class
module: ""
signature: "treenode"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class that represents a reference to a node in FlexSim's tree."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Tree\\treenode.xml"
---

# treenode

A class that represents a reference to a node in FlexSim's tree.

The treenode class is the base class for all objects and nodes accessed in FlexSim's tree. While
you can access more data and functionality specific to nodes with object data by using the Object
class, treenode is the most basic interface. First, it gives access to data and functionality associated with the
tree structure itself. Second, it gives access to basic data stored on the nodes themselves, such as node names
and data values. Third, since so much modeling logic uses labels, the treenode class provides a mechanism for
easy manipulation of labels on objects, tokens, and task sequences.
The treenode class can be automatically down-cast to a subclass such as Object.
treenode src = model().find("Source1");
Object srcAsObject = src; // auto-down-cast to object
You can also explicitly down-cast using the as() keyword
src.as(Object).location.x += 2;
When FlexScript performs a down-cast, if the node is not a valid instance of the target class type, an invalid cast exception
will be thrown.
Object src = model().find("Tools");// throws invalid cast exception
model().find("Tools").as(Object).location.x += 2; // throws invalid cast exception
You can check if a node is a given subclass type with the is() keyword.
if (model().find("Source1").is(Object)) {
...
}

Null Values

In general, to check if a treenode is null just compare the treenode with NULL or put it directly in an if statement. Note that the keyword nullvar represents a null
Variant and is not the same as a null treenode.

treenode fred = model().find("doesnotexist");
return fred == NULL; // true
return fred == nullvar; // false
return tonum(fred); // 0
if (fred) return 1; else return 0; // 0
if (objectexists(fred)) return 1; else return 0; // 0

However, there are a few special cases:

1. A treenode returned by an old global command, such as node(). The return value could be the SAFEREF node, which is a valid memory address,
but it doesn't exist in the main or view trees. To check for whether the treenode is a valid reference, you need to use objectexists(). For example:

treenode fred = node("doesnotexist", model());
return fred == NULL; // false
return fred == nullvar; // false
return tonum(fred); // a non-zero number
if (fred) return 1; else return 0; // 1
if (objectexists(fred)) return 1; else return 0; // 0

2. A treenode stored in a Variant, where the treenode pointer is pointing at 0x0, returned by a command that returns a Variant or by dynamic label access. In the
the following example Processor1 has a label called myPointerLabel that has pointer data, but is not pointing at anything (or 0x0):

Object processor = model().find("Processor1");
Variant fred = processor.myPointerLabel;
return fred == NULL; // true
return fred == nullvar; // false
return tonum(fred); // 0
if (fred) return 1; else return 0; // 0
if (objectexists(fred)) return 1; else return 0; // 0

3. A Variant that is nullvar, where the type of the Variant was unknown. For example, reading the .value of a node where you don't know whether the node has no
data or treenode data:

treenode Tools = model().find("Tools");
Variant fred = Tools.value;
return fred == NULL; // false
return fred == nullvar; // true
return tonum(fred); // 0
if (fred) return 1; else return 0; // 0
if (objectexists(fred)) return 1; else return 0; // 0

For more information on checking what type of data a treenode holds or checking if it has data at all see the
dataType property.

## Methods

### coevaluate

```flexscript
Variant treenode.coevaluate()
```

Evaluates the node, potentially returning a reference to a Coroutine.

**Returns:** The result of the node's evaluation.

Calling coevaluate() is the same as calling
evaluate(), with one important difference. If the
node's code is a coroutine,
and the evaluation of the code results in a suspension of the coroutine, coevaluate() will
return a reference to the coroutine. A reference to a coroutine is a Variant that references
a node that has Coroutine data. Calling evaluate(), on the other hand,
will merely return nullvar in the same scenario.
Generally you should coevaluate() instead of evaluate() only if you plan on actually
awaiting the result, meaning you want to suspend the execution of the calling function
(the caller) until the called function (the callee) is also finished, and get the
final result of the callee's evaluation.

### copy

```flexscript
treenode treenode.copy(treenode destination = 0, int flags = 0)
```

Creates a copy of the node.

**Parameters:**
- `destination` *(treenode, default 0)* — If defined, the copied node will be placed under this node. If not the copied node will be placed in the model.
- `flags` *(int, default 0)* — A set of bitwise flags that control the copy behavior. If you pass COPY_FLAG_REPLACE, the copied node will replace the destination node rather than being placed under it. If you pass COPY_FLAG_SUPPRESS_ON_CREATE, then the newly created object(s) will not fire their OnCreate event. In this case, you can subsequently configure the object and call firecreateevent() to call the OnCreate later.

**Returns:** A copy of the node.

treenode copy = Model.find("Processor1").copy();

### destroy

```flexscript
void treenode.destroy()
```

Destroys the node

Once you call this method on a node, it will have been destroyed and you should not reference the node anymore.
The following example destroyes all but one subnode of current.
while (current.subnodes.length > 1)
current.last.destroy();

### evaluate

```flexscript
Variant treenode.evaluate()
```

Evaluates the node.

**Returns:** The result of the execution of the node's code, if it is code-toggled. Otherwise, it will return the node's value.

Calling evaluate() will return the same value as treenode.value if the node
has number, non-code text, or array data. If the node has text data and is toggled as code (either dll, c++, or FlexScript) then evaluate() will
execute that code and return the resulting value.

### find

```flexscript
treenode treenode.find(string path)
```

Finds a node in the node's subtree by its path.

**Parameters:**
- `path` *(string)* — The path, starting at the node, that addresses the target node.

**Returns:** If the addressed node exists, the method returns that node, otherwise NULL.

This method is generally used to access nodes in the tree for which there are no other specific commands available.
This command uses a path
syntax not unlike unix/windows file paths. There are several path symbols you can use to traverse the tree, as listed below:

/ This symbol tells the traversal to go into the current node's sub-tree.
> This symbol tells the traversal to go into an object's attribute tree.
.. This symbol tells the traversal to go to the current node's parent tree, or up one level.
~ This symbol may have two meanings depending on context. If the symbol is encountered directly after another
special symbol (such as >objectfocus+/~), then it tells the traversal to go to the owning object of the current node.
For example, if the current node is within the attributes tree of an object, ~ will go up to the object itself. If, however,
the symbol is encountered after a node name, it will expect a number after the symbol, defining the "nth" object
with the given name. For example, a path of MODEL:/Processor~5 will return the 5th object named Processor in the model.
@ This symbol tells the traversal to go to the owner view of the current node.
+ This symbol tells the traversal to read the current node's text as a path to an object (or if the node has
pointer data, it reads the value of the pointer), and go to that object.
" Put quotes around an object name if the name contains special path characters. For example, the path
MODEL:/"Is Distance > Max Distance?" will return the object named Is Distance > Max Distance? in
the model.
? This symbol tells the traversal to perform a recursive tree search for the subsequent name. For
example, model().find("/?FlowItemBin") will search for a node with the name FlowItemBin in the model tree.
This returns the same node reference as an explicit path definition would: model().find("/Tools/FlowItemBin")
$
Surround a macro or flexscript with $ characters to dynamically define ranking or node references. For example,
if you have defined a global macro MY_RANK as 8, then the path MODEL:/$MY_RANK$ will get the 8th ranked object in the model.
If you define flexscript here, use i to get the current node. For example, the path
MODEL:/Processor1/$outobject(i, 3)$ will return the object connected to the 3rd output port of the object named
Processor1 in the model.

For the most part, you will only use the / and > symbols, and occasionally the .. symbol. The @ and + symbols are usually
only used when you are building a custom GUI.

There is also a feature that lets you specify a node by its rank instead of by its name. Just put a number in
the place where an object's name usually goes, and the ranked node will be accessed instead.

You can also define one of several optional starting point keywords in the path, in place of startnode. This will override
the defined node (the node left of the dot) as the path starting point. They are:MAIN:/
Starts at the main project treeMODEL:/
Starts at the model treeVIEW:/
Starts at the view tree

Example 1:

treenode curnode = model().find("/Conveyor3/Textured Colored Box");

This sets the variable curnode to a reference to an object named "Textured Colored Box", which is found in the object
called "Conveyor3", which is found in the model.

Example 2:

treenode curnode = model().find("/Conveyor3/3");

This example sets curnode to the 3rd ranked object in the object named "Conveyor3" in the model. This provides an
advantage over the first example since often flowitems will all have the same name, so the path
"/Conveyor3/Textured Colored Box" will only give you access to the first flowitem named "Textured Colored Box"
in Conveyor3. If several flowitems are in Conveyor3 and all have the name "Textured Colored Box", then you can
use the path "/Conveyor3/2" to get the 2nd ranked flowitem, the path "/Conveyor3/3" to get the 3rd ranked
flowitem, and so forth.

Example 3:

treenode exittriggernode = model().find("/Conveyor3>variables/exittrigger");

This example sets exittriggernode to reference the exittrigger variable on Conveyor3 in the model. Note that
this uses the > symbol to go into the Conveyor's object attributes tree instead of the Conveyor's sub-tree
like the first example did. Note that the object attribute tree holds things like labels, variables, size,
location, etc., whereas the object sub-tree holds other objects like flowitems.

Example 4:

treenode focus = c.find("@>objectfocus+");

This example would typically be used in a GUI. It gets access to the object that a parameters/properties window
is "pointing to". For more information, refer to the topic on Graphical User Interfaces in the user manual.

### getPath

```flexscript
string treenode.getPath(treenode relativeTo = 0, int byName = 1)
```

Gets the node's path, optionally relative to some other node.

**Parameters:**
- `relativeTo` *(treenode, default 0)* — If defined, the resulting path will be relative to that node. If left out, the path will be a globally-defined path that includes a root node prefix, such as MODEL:/.
- `byName` *(int, default 1)* — If defined, 0 means the resulting path will be a sequence of numbered ranks in the tree. 1 means the resulting path will be a sequence of named nodes.

**Returns:** The node's path.

## Properties

### dataType

```flexscript
int treenode.dataType
```

Gets and sets the node's data type.

If you set dataType to a different value than the node's current data type, it will delete the node's
current data and add the new data type. Usually you do not need to use this property since you can always
just use treenode.value to get and set the
value on a treenode.
The following are the values and macros available for dataType.
None = 0
DATATYPE_NUMBER = 1
DATATYPE_STRING = 2
DATATYPE_COUPLING = 3
DATATYPE_OBJECT = 4
DATATYPE_PARTICLE = 5
DATATYPE_BUNDLE = 6
DATATYPE_SIMPLE = 7
The following example assigns string data to current's label named "MyLabel" and returns that data type.
current.labels["MyLabel"].dataType = DATATYPE_STRING;
return current.labels["MyLabel"].dataType;
You can check if a node has data by comparing the datatype to NULL.
node.dataType == NULL

### first

```flexscript
treenode treenode.first (readonly)
```

Gets the node's first subnode.

Returns a reference to the first subnode contained in the node. This is a shortcut for using
node.subnodes[1], except that if it doesn't exist,
node.first will simply return NULL, whereas node.subnodes[1]
will throw an array out of bounds exception.

The following example sets the name of the first subnode of current to "FirstObj".
current.first.name = "FirstObj";

### labelProperties

```flexscript
Variant treenode.labelProperties
```

Use your own named properties to get/set label values.

The treenode class allows you to get and set custom named properties that will apply to labels on an object. For example,
if you want to set a label named "MyLabel" on current to a value of 5, you can write the following code:
current.MyLabel = 5;
This will set the label's value to 5. If a label of that name does not exist, then it will be added to the object's labels.
If you want to return the value stored in current's "MyLabel" label, you can execute the following code:
return current.MyLabel;
This label access mechanism is only available on nodes with object data, or on Process Flow tokens,
Storage.Slots, or task sequences. If you
use this mechanism on a node that is not one of these types of node, FlexSim will throw an exception.
Using dynamic label property access gives you shorter syntax for what you could otherwise do with a bit longer expression.
Short SyntaxLonger Equivalent Syntax
current.MyLabelcurrent.labels["MyLabel"].evaluate()
current.MyLabel = 5current.labels.assert("MyLabel").value = 5 *
* Note that if you use the shorter syntax to set the code of a flexscript-toggled label, it will also automatically
build the code's flexscript.
? Operator
There are, however, some caveats associated with this. Since FlexScript allows you to get/set any label names on a node, this
does introduce a potential problem if you don't spell your labels correctly. For this reason, if you get a label that does not exist,
FlexScript will throw an exception, pointing you immediately to the problem. If you intentionally want to access a label that may
not exist (just return a null Variant, or nullvar, if it doesn't), then you can put a ? after the label name to
signify that you don't want to throw an exception on an undefined label
The following code will simply return nullvar if the "MyLabel" label does not exist, instead of throwing an exception.
return current.MyLabel?;
# Operator
Because your object already has its own properties there are certain label names that won't work as a dynamic property. For example:
current.next = 5;
This will throw an exceptions since next is already a property on the treenode class and it is read-only. However, this
doesn't mean it's impossible to have an object with a "next" label (for example you could add it to an object through the
object's properties window). However, you can't access this label through dynamic properties in the same way as other
labels without a name collision. In order to reference this label (or add it in the first place) with dynamic properties
you need to add a # character before the label name:
current.#next = 5;
This will correctly find the object's "next" label and not use its next property.
It is also possible that, as the software is updated, new properties will be added to objects that collide with label names
used by your object in your model. In this case, your code will be automatically updated to use the # character so your
code continues to work the same way. For example, say you are using a "position" label on one of your objects:
current.position = 5;
Now, in the next update that object is updated and a new position property is added. Your code will be automatically updated to look like the following code snippet and will continue to work as before.
current.#position = 5;

### labels

```flexscript
treenode.labels (readonly)
```

Accesses the node's labels as nodes.

This property allows you to access the node containing a label value.
Usually you will not need to use this property, as you can use labelProperties.
The exceptions to this rule are:
1. Your label names have spaces in them. - If your label names contain spaces, you will need to access them using
the labels property:
current.labels["My Label Name"].value = 5;
Since this code is more tedious than using
labelProperties, we suggest not naming your labels with spaces.
2. You specifically want to access label nodes - In some cases you actually want to access label nodes, such as
when the label is a table of values. In this
case, you would use the labels property.Table myTable = current.labels["myTable"];
3. You want to assert the label if it doesn't exist. - Just like subnodes
property, you can assert that a label exists. Optionally, you can pass in a default value to give the label if it does not exist:

// Set myValue to the value of MyLabel
// If MyLabel doesn't exist, create it
Variant myValue = current.labels.assert("MyLabel").value;
// Increment the value of MyLabel by 10
// If MyLabel doesn't exist, create it and set its value to 1
current.labels.assert("MyLabel", 1).value += 10;

### last

```flexscript
treenode treenode.last (readonly)
```

Gets the node's last subnode.

Returns a reference to the last subnode contained in the node. This is a shortcut for using
node.subnodes[node.subnodes.length], except that if it doesn't exist,
node.last will simply return NULL, whereas node.subnodes[node.subnodes.length]
will throw an array out of bounds exception.

The following example sets the name of the last subnode of current to "FirstObj".
current.last.name = "FirstObj";

### name

```flexscript
string treenode.name
```

Gets and sets the node's name.

treenode source = Model.find("Source1");
source.name = "MySource"; // sets the node's name to "MySource"
return source.name; // returns "MySource"

### next

```flexscript
treenode treenode.next (readonly)
```

Gets the next node after the node in the tree.

Gets the node in the FlexSim tree that is in the same level and immediately after this node.
If this node is the last node in the tree at that level, then this command returns NULL which means that the requested node does not exist.

The following example sets a label called "tally" to 0 on every item in the current.
The variable item is used to store the reference to the item that is currently being worked on.
treenode item = current.first; while (item) { item.tally = 0; item = item.next; }

### prev

```flexscript
treenode treenode.prev (readonly)
```

Gets the node before the node in the tree.

Gets the node in the FlexSim tree that is in the same level and immediately before this node.
If this node is the first node in the tree at that level, then this command returns NULL which means that the requested
node does not exist.

The following example sets a label called "tally" to 0 on every item in the current by traversing backwards through
its subnodes. The variable item is used to store the reference to the item that is currently being worked on.
treenode item = current.last; while (item) { item.tally = 0; item = item.prev; }

### rank

```flexscript
int treenode.rank
```

Gets/sets the rank of the node in its parent's sub-tree

The rank is the position in the tree layer that this
node is in. The first node in a layer is rank 1, the second is rank 2, and so forth.
The following example sends a delayed message if item is the last subnode contained in current.
if (item.rank == current.subnodes.length) senddelayedmessage(current,10,current,1,2,tonum(item));

### subnodes

```flexscript
treenode.subnodes (readonly)
```

Accesses the node's subnodes

Accessing the Number of SubnodesYou can access the number of subnodes on current with the following code.
current.subnodes.length
Accessing Subnodes by Rank
You can access an individual subnode by rank with the following code.
current.subnodes[1] // first subnode
current.subnodes[current.subnodes.length] // last subnode
current.subnodes[i] // i-th subnode
Accessing Subnodes by Name
You can access an individual subnode by name with the following code.
current.subnodes["Box1"] // subnode named "Box1"
Asserting Subnodes by Name
You can assert a subnode by name with the following code.
// assert a label named "My Label"
current.attrs.labels.subnodes.assert("My Label")
This will see if a node of that name already exists, and if it doesn't, it will add a subnode with that name. It returns
the resulting subnode.
Asserting Subnodes with a Default Value
You can also assert a subnode by name and include a default start value with the following code.
// assert a label named "My Label" with a default value of 5
current.attrs.labels.subnodes.assert("My Label", 5)
This is the same as the previous assert() call, except that if it needs to add the label, it will give the
added label a value of 5.
Adding Subnodes
You can add a subnode with the following code.
// add a new node under current
current.subnodes.add()
This will add a blank subnode under current. It returns the resulting subnode.
Clearing Subnodes
You can delete current's subnodes with the following code.
current.subnodes.clear();
Getting an Array of subnodes
You can create an Array of all the subnodes with the following code.
Array subnodes = current.subnodes.toArray();

### up

```flexscript
treenode treenode.up
```

Gets and sets the node "above" the node in the tree, or in other words, its parent node.

treenode box = Model.find("Source1/Box");
box.up; // gives Source1
box.up.up; // gives the model
Setting the up property causes the node to be transfered into the target node.
treenode box = Model.find("Queue1/Box");
box.up = Model.find("Queue2");

### value

```flexscript
Variant treenode.value
```

Gets and sets the value on the node.

When getting a node's value, a node with number data will return a number, a node with text data will
return text, and a node with coupling pointer data will return the target node pointed to. Nodes that hold array
data will return the associated Array. Nodes that have object data return themselves since they
store attribute nodes on themselves. Nodes without data will return a null Variant, or nullvar.
Additionally, some nodes may hold "custom" data, using a customization class called SimpleDataType, or SDT. For these
types, the customization class can define what is returned when value is accessed or set. For example, for a node that
contains tracked variable data, such as current.attrs.state_current or current.attrs.stats_content, the
value property will return the tracked variable's current value (same as
getstat(current.attrs.state_current, "", STAT_CURRENT), etc.), and
setting the value will update the tracked variable's current value.
Examples
Get the shape path associated with current's 3D shape file:
string shapePath = current.attrs.shape.value;
Get current's shape index
int shapeIndex = current.attrs.shapeindex.value;
Set the value of current's "My Label" label to 5. (Note that you can do this easier using
labelProperties if you don't put spaces in your label names).
current.labels["My Label"].value = 5;
Assert and set the value of current's "Loading Operator" label to point to Operator1 in the model.
current.labels.assert("Loading Operator").value = Model.find("Operator1");
Assert and set the value of current's "Loading Operators" label to an array pointing to 3 operators in the model.
current.labels.assert("Loading Operators").value =
[Model.find("Operator1"), Model.find("Operator2"), Model.find("Operator3")];
Assert a subnode named "OperatorName" into the model tools folder and give it the value "Operator1"
Model.find("Tools").subnodes.assert("OperatorName").value = "Operator1";


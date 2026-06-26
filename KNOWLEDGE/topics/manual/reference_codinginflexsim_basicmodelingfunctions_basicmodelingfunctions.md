---
id: reference_codinginflexsim_basicmodelingfunctions_basicmodelingfunctions
name: "Basic Modeling Functions"
kind: manual
breadcrumb: ["Reference","CodingInFlexSim","BasicModelingFunctions","BasicModelingFunctions"]
tags: ["manual","reference","codinginflexsim","basicmodelingfunctions"]
source: "manual/Reference/CodingInFlexSim/BasicModelingFunctions/BasicModelingFunctions.html"
---

# Basic Modeling Functions

## Current and Item

 

The following expressions are used frequently throughout FlexScript:

 

- **current** - the current variable is a reference to the current resource object. It is often an access variable in pick lists.
- **item** - the item variable is a reference to the involved item for a trigger or function. It is often an access variable in pick lists.

 

 

 

## Referencing Objects

 

The following table explains how to reference objects in FlexScript:

 

| Command | Explanation | Example |
| --- | --- | --- |
| node.first | This returns a reference to the first ranked object/node inside of node | current.first |
| node.last | This returns a reference to last ranked object/node inside of node | current.last |
| node.subnodes[ranknum] | This returns a reference to the object at a given rank inside of node | current.subnodes[3] |
| object.inObjects[portnum] | This returns a reference to the object connected to the input port number of object | current.inObjects[1] |
| object.outObjects[portnum] | This returns a reference to the object connected to the output port number of object | current.outObjects[i] |
| object.centerObjects[portnum] | This returns a reference to the object connected to the center port number of object | current.centerObjects[1] |
| node.next | This returns a reference to the next ranked object of node | item.next |
| node.prev | This returns a reference to the previous ranked object of node | item.prev |
| node.find(path) | This returns the object found at path in the tree beginning from node | current.find("Box1") |

 

 

 

## Object Attributes

 

The following table explains object attributes commands in FlexScript:

 

| Command | Explanation |
| --- | --- |
| object.name | This returns the name of the object |
| object.name = newName; | This sets the name of the object to newName |
| object.color = Color.color | This sets the color of the object (where color is red, green, blue, random(), etc.) |
| object.color = Color(red, green, blue) | This sets the color of the object to an rgb color |
| setobjectshapeindex(object, indexnum) | This sets the 3D shape of the object |
| setobjecttextureindex(object, indexnum) | This sets the 3D texture of the object |

 

 

 

## Object Spatial Attributes

 

The following table explains object spatial attribute commands in FlexScript:

 

| Command | Explanation |
| --- | --- |
| object.location.x object.location.y object.location.z | Gets the x, y, and z locations of the object respectively |
| object.setLocation(xnum, ynum, znum) | This sets the x, y, and z location of the object |
| object.size.x object.size.y object.size.z | Gets the x, y, and z size of the object respectively |
| object.setSize(xnum, ynum, znum) | This sets the x, y, and z size of the object |
| object.rotation.x object.rotation.y object.rotation.z | Gets the x, y, and z rotation of the object respectively |
| object.setRotation(xdeg, ydeg, zdeg) | This sets the x, y, and z rotation of the object |

 

 

 

## Object Statistics

 

The following table explains object statistics commands in FlexScript:

 

| Command(parameter list) | Explanation |
| --- | --- |
| object.subnodes.length | This returns the current content of the object |
| object.stats.input | This returns the input statistic of the object |
| object.stats.output | This returns the output statistic of the object |
| obj.stats.state().value = statenum | This sets the current state of the object |
| obj.stats.state().value | This returns the current state value of the object |
| obj.stats.state().valueString | This returns the current state of the object as a string |
| object.rank | This returns the rank of the object |
| object.rank = ranknum | This sets the rank of the object |
| getentrytime(item) | This returns the time the flow item entered the object it is currently in |
| getcreationtime(item) | This returns the time the flow item was created |

 

 

 

## Object Labels

 

The following table explains object label commands in FlexScript:

 

| Command(parameter list) | Explanation |
| --- | --- |
| object.labelName object.attrs.labels.subnodes[labelRank].value | This returns the value of the object's label. |
| object.labelName = value object.attrs.labels.subnodes[labelRank].value = value | This sets the value of the object's label. |
| object.labels["labelname"] object.attrs.labels.subnodes[labelRank] | This returns a reference to the label as a node. Often used if you have a label that is used as a table. |

 

 

 

## Object Control

 

The following table explains object control commands in FlexScript:

 

| Command(parameter list) | Explanation |
| --- | --- |
| obj.input.close() | This closes the input of the object |
| obj.input.open() | This re-opens the input of the object |
| obj.output.close() | This closes the output of the object |
| obj.input.open() | This re-opens the output of the object |
| obj.message(delayTime, fromobject, parameter1, parameter2, parameter3) | This causes the message trigger of the object to fire, optionally after a certain delay time. |
| object.stop(downstate) | This tells the object to stop whatever its operation is and go into the given state |
| object.resume() | This allows the object to resume whatever its operation is |
| obj.output.stop() | This closes the output of the object, and accumulates stopoutput requests |
| obj.output.resume() | This opens the output of the object once all stopoutput requests have been resumed |
| obj.input.stop() | This closes the input of the object, and accumulates stopinput requests |
| obj.input.resume() | This opens the input of the object once all stopinput requests have been resumed |
| insertcopy(originalobject, containerobject) | This inserts a new copy of the object into the container |
| moveobject(object, containerobject) | This moves the object out of its current container into its new container |

 

 

 

## Object Variables

 

The following table explains object variable commands in FlexScript:

 

| command(parameter list) | Explanation |
| --- | --- |
| getvarnum(object, "variablename") | This returns the number value of the variable with the given name |
| setvarnum(object, "variablename", value) | This sets the number value of the variable with the given name |
| getvarstr(object, "variablename") | This returns the string value of the variable with the given name |
| setvarstr(object, "variablename", string) | This sets the string value of the variable with the given name |
| getvarnode(object, "variablename") | This returns a reference to the variable with the given name as a node |

 

For more information about the Object class see the FlexScript Class Reference.

 

 

 

## Tables

 

In order to get access to the set of table methods and properties, you'll need to create a Table variable.

 

```
		Table table = Table("GlobalTable1"); //Global tables
		Table labelTable = current.labels["TableData"]; //Label table
```

 

The following methods and explanations assume you have a Table variable like the ones listed above.

 

| command | Explanation |
| --- | --- |
| table[row num/name][col num/name] | This returns the value in the specified row and column of the table |
| table[row num/name][col num/name] = value; | This sets the value in the specified row and column of the table |
| table.setSize(rows, columns) | This sets the size of the table in rows and columns |
| table.numRows | This returns the number of rows in the table |
| table.numCols | This returns the number of columns in the table |
| table.clear() | Sets all number values in the table to 0 |

 

For more information about the Table class see the FlexScript Class Reference.

 

 

 

## Task Executer Control

 

For more information on controlling task executers, refer to the task sequence reference.

 

 

 

## Prompts and Printouts

 

The following table explains prompt and printout commands in FlexScript:

 

| Command(parameter list) | Explanation |
| --- | --- |
| print(var value) | Prints variant values to the output console |
| msg("title", "caption") | Opens a simple Yes, No, Cancel dialog |
| userinput(targetnode, "prompt") | Opens a dialog box where you can set the value of a node in the model |
| string1 + string2 | This returns the string concatenation of two strings |

 

 

 

## Advanced Functions

 

For more advanced functions and for a more complete set of command documentation, refer to the Command Summary and the FlexScript Class Reference.

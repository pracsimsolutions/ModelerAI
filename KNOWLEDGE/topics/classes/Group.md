---
id: group
name: "Group"
kind: class
module: ""
signature: "Group"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class for grouping objects."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\Group.xml"
---

# Group

A class for grouping objects.

## Methods

### addMember

```flexscript
void Group.addMember(treenode object)
```

Adds an object to the group.

**Parameters:**
- `object` *(treenode)* — The object to add to the group.

group.addMember(Model.find("Processor1"));

### Group

```flexscript
Group Group.Group(string name)
```

Creates a reference to a group.

**Parameters:**
- `name` *(string)* — The name of the group.

Group parts = Group("Packers");

### indexOf

```flexscript
int Group.indexOf(treenode object)
```

Returns the index of the object in the group.

**Parameters:**
- `object` *(treenode)* — The object to find the index of.

**Returns:** The index of the object in the group, 0 if it is not a member.

int index = group.indexOf(Model.find("Processor1"));

### isMember

```flexscript
int Group.isMember(treenode object, int recursive)
```

Checks if the object is a member of the group.

**Parameters:**
- `object` *(treenode)* — The object to look for in the group.
- `recursive` *(int)* — Should groups contained by this group also be searched.

**Returns:** A 1 if the object is a member of the group, 0 otherwise.

This code checks if Processor1 is a direct member of the group:
group.isMember(Model.find("Processor1"));
This code checks if Processor1 is a member of the group or the member of any of the groups contained by the group:
group.isMember(Model.find("Processor1"), 1);

### removeMember

```flexscript
void Group.removeMember(treenode object)
```

Removes an object from the group.

**Parameters:**
- `object` *(treenode)* — The object to remove from the group.

group.removeMember(Model.find("Processor1"));

### toFlatArray

```flexscript
Array Group.toFlatArray()
```

Converts the group and any nested groups into a single Array of group members.

**Returns:** A single, flat Array of all the group members of the group and the members of any nested groups.

Array members = group.toFlatArray();

## Properties

### length

```flexscript
int Group.length
```

The number of members in the group.

int numMembers = Group("Staff").length;


---
id: reference_3dobjects_generalpropertylist_generalpropertylist
name: "General Property List"
kind: manual
breadcrumb: ["Reference","3DObjects","GeneralPropertyList","GeneralPropertyList"]
tags: ["manual","reference","3dobjects","generalpropertylist"]
source: "manual/Reference/3DObjects/GeneralPropertyList/GeneralPropertyList.html"
---

# General Property List

## Overview

 

Each 3D object in FlexSim supports a set of properties that allow you to configure the behavior of that object. These properties, uniquely identified by their name, can be used in object property tables, for template inheritance, as well as by using the Object.setProperty() and Object.getProperty() methods.

 

While different classes of objects support their own class-specific set of properties, this topic lists the set of valid general purpose FlexSim object property names, and their meanings.

 

For information on setting individual object properties through FlexSim's interface, see Using Properties.

 

 

 

## General

 

| Property | Type | Explanation |
| --- | --- | --- |
| AllGroups | Array of strings (read only) | The set of groups of which the object is a member, either indirectly (through subgroups) or directly. See also Property Tables: AllGroups. |
| CenterObjects | Array of strings | The array of paths, relative to the owning object, to the objects that are connected to the object's center ports. |
| Classes | Array of strings | The set of class names of which the object is an instance. See also Property Tables: Classes. |
| Class | String | The name of the class of which the object is an instance. |
| Container | Node | The reference to the object's container. |
| Groups | Array of strings | The set of groups of which the object is a *direct* member. This does not include the groups that the object is an indirect member of (through groups with subgroups). |
| InObjects | Array of strings | The array of paths, relative to the owning object, to the objects that are connected to the object's input ports. |
| IsSelected | Boolean | 1 if the object is selected (red-framed), 0 otherwise. See also Property Tables: IsSelected. |
| Name | String | The name of the object. See also Property Tables: Name. |
| Object | Node | The reference to the object. |
| OutObjects | Array of strings | The array of paths, relative to the owning object, to the objects that are connected to the object's output ports. |
| Shape | String | The path of the object's shape file. |
| Texture | String | The path of the object's 3D texture file. |

 

 

 

## Visual

 

| Color | Array(4) | The red/green/blue/alpha color of the object. Each element is in the range [0, 1]. See also Property Tables: Color. |
| --- | --- | --- |
| Color.Red | Number | The red component of the object's color, in the range [0, 1]. |
| Color.Green | Number | The green component of the object's color, in the range [0, 1]. |
| Color.Blue | Number | The blue component of the object's color, in the range [0, 1]. |
| Color.Alpha | Number | The alpha component of the object's color, in the range [0, 1]. |
| Location | Array(3) | The x/y/z location of the object. |
| Location.X | Number | The x location of the object. |
| Location.Y | Number | The y location of the object. |
| Location.Z | Number | The z location of the object. |
| NoSelect | Boolean | 1 if the object should not be clickable in 3D views, 0 otherwise. |
| Protected | Boolean | 1 if the object should be "protected" from deletion or manipulation, 0 otherwise. |
| Rotation | Array(3) | The x/y/z rotation of the object. |
| Rotation.X | Number | The x rotation of the object. |
| Rotation.Y | Number | The y rotation of the object. |
| Rotation.Z | Number | The z rotation of the object. |
| ShowContents | Boolean | 1 if the object's contents are to be shown in 3D views, 0 otherwise. |
| ShowName | Boolean | 1 if the object's name is to be shown in 3D views, 0 otherwise. |
| ShowPorts | Boolean | 1 if the object's ports are to be shown in 3D views, 0 otherwise. |
| ShowShape | Boolean | 1 if the object's shape is to be shown in 3D views, 0 otherwise. |
| Size | Array(3) | The x/y/z size of the object. |
| Size.X | Number | The x size of the object. |
| Size.Y | Number | The y size of the object. |
| Size.Z | Number | The z size of the object. |

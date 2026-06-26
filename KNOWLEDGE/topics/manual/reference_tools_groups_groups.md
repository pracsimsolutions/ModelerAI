---
id: reference_tools_groups_groups
name: "The Groups Tool"
kind: manual
breadcrumb: ["Reference","Tools","Groups","Groups"]
tags: ["manual","reference","tools","groups"]
source: "manual/Reference/Tools/Groups/Groups.html"
---

# The Groups Tool

## Overview and Key Concepts

 

The groups utility lets you create and edit different groups in your model.

 

Group information is stored both in the /Tools/Groups folder and on each of the individual objects. A node is added to the object attributes called Groups with a coupling to the associated group. Objects may be included in multiple groups. Groups can also be added as a member of another Group.

 

 

 

## Properties

 

The Groups tool has the following properties:

 

 

### Members List

 

The Members List displays the objects and groups that are currently members of the group.

 

The following table explains the buttons in the Members List section:

 

| Button | Explanation |
| --- | --- |
|  | Opens a menu allowing you to add currently selected items to the group, set the group to match the current model selection, or add another group. |
|  | Click to enter "Sample" mode then select an object in the 3D View or a group in the Toolbox to add it to the group. |
|  | Duplicates the selected object(s) and adds the copies to the group. Does not duplicate groups. |
|  | Removes the selected(s) object from the group. |
|  | Reorder members of the group. |
|  | Centers the 3D view on the selected object or for a group it opens its properties window. |
|  | Selects the selected object in the 3D View (puts a red box around it). |

 

### Parent Groups List

 

The Parent Groups List displays the parent groups of this group. The current group is a member of each of these groups. Opening the properties of any of these groups will show this group in its members.

 

The following table explains the buttons in the Parent Groups List section:

 

| Button | Explanation |
| --- | --- |
|  | Opens a menu allowing you to add this group as a member of another group chosen from the list in the menu. |
|  | Click to enter "Sample" mode then select a group in the Toolbox to add this group as a member of the sampled group. |
|  | Removes this group from the members of the selected parent group. |
|  | Opens the properties window of the selected group. |

 

 

 

## Additional Concepts

 

### 3D View Right Click Menu

 

You may also add objects to groups through the right-click menu of the 3D View.

 

 

The current list of groups will appear at the top of the menu. A checkmark appears to left of each group the object is a member of. Selecting a group in this menu will toggle the object's membership in that group.

 

- **Add to New Group** - Creates a new group and adds the object to the group. If multiple objects are selected (red box) this will add those objects to the group as well.

 

### Quick Properties Panel

 

 

If a highlighted object is the member of at least one group, the Groups panel will display in Quick Properties. The dropdown displays the names of all parent groups of the object.

 

The following table explains the buttons in Quick Properties panel:

 

| Button | Explanation |
| --- | --- |
|  | Opens a menu allowing you to add this object as a member of a group chosen from the list. |
|  | Click to enter "Sample" mode then select a group in the Toolbox to add this object as a member of the sampled group. |
|  | Opens the properties window of the selected group. |
|  | Removes this object from the members of the selected parent group. |
| Duplicate Object | Duplicates the object and adds the copy to the group. |

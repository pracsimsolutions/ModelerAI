---
id: reference_3dobjects_fluid_general_general
name: "The General Tab"
kind: manual
breadcrumb: ["Reference","3DObjects","Fluid","General","General"]
tags: ["manual","reference","3dobjects","fluid"]
source: "manual/Reference/3DObjects/Fluid/General/General.html"
---

# The General Tab

## About the General Tab

 

The General tab affects the overall appearance and display of 3D objects.

 

 

 

 

## Appearance

 

The Appearance group has the following properties:

 

### 3D Shapes

 

This option specifies the 3D shape(s) for the object. The drop down contains all of the shape frames for the object. The number (0 -) is the shape frame index for use in the setframe() command.

 

| Icon | Description |
| --- | --- |
|  | Adds a Shape Frame to the object. |
|  | Removes the current Shape Frame. |
|  | Moves the selected Shape Frame up or down in the list. |

 

3D shapes can either be referenced through the or loaded 3D shapes will display in the down arrow menu. You can also select Browse... from this menu to load a new shape. FlexSim can load many types of 3D files, including:

 

- .fbx, .jt, .skp, .3ds, .wrl, .stl, .stp, .igs, .dae, .obj, .ac, .x, .blend

 

### Shape Factors

 

Click on this button to display the object's shape factors popup.

 

### 3D Texture

 

This field specifies the object's 3D texture. If the 3D shape does not already have a texture defined within its 3D file, then this texture will be drawn on the face of the 3D shape. Note that if the object's 3D shape already has a texture defined, then this texture will not be used.

 

### Color

 

This field specifies the color of the object. Note that if the object already has materials defined in its 3D shape's file, then this color will not show. This color shows through only if no materials are defined in the 3D file.

 

 

 

## Customize Shape

 

The button opens a window to customize how this object draws the meshes in its 3D shape. Other objects using the same shape will not be affected.

 

 

The mesh drop down contains a list of meshes imported from the 3D shape file. Select a mesh from the drop down and check the Override box to customize the mesh.

 

**Hide Mesh** - Toggles displaying the mesh in the 3D view.

 

**Use Object Color** - Toggles whether the mesh should use the main object's color as its diffuse color.

 

**Diffuse Color** - Use a particular diffuse color for the mesh.

 

**Use Object Texture** - Toggles whether the mesh should use the main object's texture as its texture.

 

**3D Texture** - Use a particular texture for the mesh.

 

 

 

## Visuals/Animations

 

These buttons allow you to save/load visual information for objects. You can also access an object's animations through the 3D views right click menu.

 

### Load/Save

 

These buttons load/save all of the Appearance settings of an object, allowing you to save shape, texture, color, and animations in a .t file to load into other objects you want to look the same.

 

### Edit

 

This button opens the Animation Creator, which allows you to create animations for FlexSim Objects.

 

 

 

## Flags

 

Here you can check different boxes to show/hide different parts of the object, such as the contents of the object, the name, the ports, etc.

 

These flags can also be toggled through the Edit Selected Objects view.

 

**Show Name** - Toggles displaying the object's name in the 3D view.

 

**Show Ports** - Toggles displaying port connections in the 3D view.

 

**Show 3D Shape** - Toggles displaying the object's 3D shape.

 

**Show Contents** - Toggles displaying the object's contents.

 

**Scale Contents** - If checked, any objects within the content of this object will be scaled according to the size of this object.

 

**Protected** - If checked, this object will not allow the user to move, size, rotate, or delete the object.

 

**No Select** - If checked, this object will not be able to be clicked on the 3D view. To gain access to an object with the No Select flag checked, find it in the Tree Window.

 

 

 

## Position, Rotation, and Size

 

Here you can set the position, rotation, and size of the object based on X, Y, and Z values.

 

| Icon | Description |
| --- | --- |
|  | Change the position of the object. |
|  | Change the rotation of the object. |
|  | Change the size of the object. |

 

 

 

## Ports

 

This area lets you edit the object's connections. Select either Input Ports, Central Ports, or Output Ports from the combobox on the left. The list on the right shows the appropriate connections. Once you have finished editing an object's connections, you will need to reset the model before running it again.

 

- **Rank ^** - This button will move the selected connection up in the list.
- **Rank v** - This button will move the selected connection down in the list.
- **Delete** - This button will delete the selected connection.
- **Properties** - This button will open a new Properties window for the selected object.

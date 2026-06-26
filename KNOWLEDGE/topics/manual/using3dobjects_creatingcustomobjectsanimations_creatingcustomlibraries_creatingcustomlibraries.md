---
id: using3dobjects_creatingcustomobjectsanimations_creatingcustomlibraries_creatingcustomlibraries
name: "Creating Custom Object Libraries"
kind: manual
breadcrumb: ["Using3DObjects","CreatingCustomObjectsAnimations","CreatingCustomLibraries","CreatingCustomLibraries"]
tags: ["manual","using3dobjects","creatingcustomobjectsanimations","creatingcustomlibraries"]
source: "manual/Using3DObjects/CreatingCustomObjectsAnimations/CreatingCustomLibraries/CreatingCustomLibraries.html"
---

# Creating Custom Object Libraries

## Introduction to Creating Custom Object Libraries

 

Consider using a custom object library if you have customized the properties of a particular object and you'd like to be able to easily add copies of this custom object directly from the Library. For example, if you've made several complex changes to the properties of a specific processor, you can add this processor to your custom object library. Then you can simply drag and drop the custom processor from the library into your model and the new processors will have identical properties.

 

You can also export custom libraries to share custom objects with other users.

 

 

 

## Creating a Custom Object Library

 

To create a custom object library:

 

1. Right-click the object you want to add to the library to open a menu. Point to **Edit**, then **Add User Library**, then select **New User Library**.
1. The new library and object will appear at the top of the Library in the left pane. From here you have a few options: - To rename the library or the object, right-click them and select **Rename**. - To edit the object's properties, right-click the object and select **Properties**. - To save a custom library for use in other models, right-click the library title and select **Save Library**.

 

To add the custom object to the model, simply drag and drop the way you add any other object from the Library.

 

 

 

## Changing and Deleting Objects in a Custom Library

 

If you need to modify a library object once it has been added to a user library:

 

1. Depending on whether your custom object is a 3D objects or a process flow object, drag and drop the object from the library into a model view or process flow view.
1. Make the desired modifications and then add it to the user library.
1. Delete the old object from the library by clicking on the down arrow to the left of the user library name and clicking **Remove Object from Library**.

 

 

 

## Adding Containers to a Custom Library

 

You can do the same thing with containers to create copies of multiple custom objects. For example, if you have a workstation with several 3D objects that you will copy throughout your model, it might be helpful to put the entire workstation in a container and add that container to your custom library. See Using Containers as a Model Management Tool for more information.

 

 

 

## Adding Process Flow Activities and Display Objects

 

Activities and display objects, Process Flow objects and 3D objects attached to Process Flow objects can be added to user libraries.

  

#### External References

 

When you add a Process Flow object or a set of activities to a user library, keep in mind that references that are external to your object or activities will not be saved in your library object. For example, if you have a user library object that includes a Create Object activity that references Processor4 in your model as its destination, when the library is distributed to other users, that reference will be null and be displayed as None.

 

One way to solve this issue is to reference the external object by name using FlexScript. For example, `node("Processor4", model())`. If you are creating a Process Flow user library object you can set the destination property to a Process Flow Variable. When the library is distributed to other users, they only need to set up the user accessible Process Flow Variables for the object to function.

  

To add activities and display objects to a custom library:

 

1. Ctrl+click all the activities and display objects you want to add to the library.
1. Right click and point to **Add To User Library**, then select **New User Library**.

 

 

A User Library will appear in the Library window. This library can now be saved and distributed to other users.

 

 

 

 

## Adding a Process Flow Object

 

To add a project flow object to a custom library:

 

1. In the Quick Properties for the Process Flow Properties, click the User Library button button next to the **Process Flow Name** field.
1. Select the user library to add to, or create a new user library.
1. Select **As Draggable Icon** or **As Auto-Install Component**.

 

When a Process Flow object is added to a user library, any instances, or attached objects, will be removed. The exception is for the General Process Flow which has one instance: itself.

 

Once added, a Process Flow object can be updated by adding it to the user library again. This will overwrite the previous Process Flow with the new one.

 

 

 

## Adding Attached Objects (Instances)

 

When you add a 3D object like a task executer or fixed resource object to a user library that is attached to a Process Flow, the Process Flow object will automatically be added to the user library. By default, the Process Flow object is added as a draggable icon along side the 3D object that was added. The Process Flow object can be moved into an auto-install component if desired.

 

When the 3D object is dragged into the model, the library will check to see if attached Process Flow objects are currently in the model. If the Process Flow isn't in the model, the Process Flow object will be added. If it does exist, the object will be connected to the pre-existing Process Flow object, but the Process Flow will not be updated. To updated the Process Flow, manually drag the Process Flow object from the user library into the model. Doing this will maintain all of the Process Flow instances.

  

#### User Libraries Compare Object Names

 

When saving to a user library and when creating an object from a user library, the Process Flow object's name is used to compare with what already exists in the model or user library. If a Process Flow object is saved to a user library with another Process Flow that has the same name, that Process Flow will be overwritten. When a 3D object is dragged into the model, it will check for a Process Flow object in the model with the same name as the attached Process Flow.

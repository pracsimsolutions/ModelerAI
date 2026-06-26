---
id: bestpractices_whilebuilding_containers_containers
name: "Using Containers as a Model Management Tool"
kind: manual
breadcrumb: ["BestPractices","WhileBuilding","Containers","Containers"]
tags: ["manual","bestpractices","whilebuilding","containers"]
source: "manual/BestPractices/WhileBuilding/Containers/Containers.html"
---

# Using Containers as a Model Management Tool

## Introduction to Using Containers

 

The Container is a 3D object in the FlexSim library whose primary purpose is to contain objects. This makes them helpful for managing complex models. The following sections explain different ways you can use containers to make your model more efficient and well-organized.

 

### Organizing Objects Visually

 

Perhaps you have a group of objects that are related in some way, such as a workstation that contains various 3D objects. One way that containers can help you better manage groups of 3D objects is by keeping these objects organized visually, as demonstrated in the following image:

 

 

### Moving Groups of Objects

 

When you add a container to your model and populate it with sub-objects, you can move it as a single movable object in your model:

 

 

See Moving, Rotating, Resizing 3D Objects for more information.

 

### Duplicating Groups of Objects

 

Copying and pasting a container will duplicate all of the objects in the container:

 

 

See Copying and Pasting Objects and Properties for instructions on duplicating objects.

 

### Creating Custom Libraries

 

Custom libraries make it so that you can easily add copies of an object with custom properties. Using custom libraries, you can drag and drop a custom object directly from the library into your model. For example, if you've made several complex changes to the properties of a specific processor, you can add this processor to your custom object library. Then you can simply drag and drop the custom processor from the library into your model and the new processors will have identical properties.

 

You can do the same thing with containers to create copies of multiple custom objects. For example, if you have a workstation with several 3D objects that you will copy throughout your model, it might be helpful to put the entire workstation in a container and add it to your custom library.

 

 

See Creating Custom Object Libraries for instructions on creating custom libraries.

 

### Merging Models Created by Other Modelers

 

If you are working with a team of modelers to create different portions of a complex simulation model, using containers can make the process of merging models much simpler. Not only do containers make it easy to move large sections of a model at the same time, they also make it so that you can quickly change the names of objects inside that container if needed for model functionality.

 

For example, two FlexSim users were working on different portions of the same model and didn't realize they gave their objects the same names. Identical names would have caused problems in the model's behavior because their model's logic frequently referenced specific objects by name. Fortunately, all of their objects were in separate containers. Rather than rename every single object, one modeler simply changed the name of the container and easily updated the model's logic.

 

### Creating Additional Floors and Model Layouts

 

If you need to model a facility that has multiple floors, you can use containers to create those floors:

 

  

#### Use the Model Layout Tool to View Floors Side By Side

 

It's not always convenient to view multiple floors stacked on top of each other while building a model. Using containers, you can use the Model Layout tool to quickly change between viewing the floors side by side or stacked on top of each other. See Creating Model Layouts for more information.

  

 

 

## Adding Objects to a Container

 

When you add an empty container to your model, it will open a view into the contents of the container in a separate pane. You can then work in that pane to add and connect objects in the container.

 

 

Once you've finished adding the objects you need, you can close the container's pane and work with the container as a single object in the model's 3D view. To get back to the pane, right-click on the container and choose **View / Edit Contents**.

 

You can also edit the contents of the container in place by clicking on the container, and in properties, selectin **Edit In Place**. This will let you edit the contained objects directly in the model view. Uncheck **Edit In Place** once you are finished.

 

### Moving Existing Objects into a Container

 

If you want to put existing objects into a container, you will need to use the Edit Selected Object tool:

 

1. Add a **Container** to your model (from the Library under **Visual**).
1. Position the container near the objects you want to contain.
1. Hold the Shift key and draw a box over all the objects to select them. A red box will appear around the selected objects.
1. If you selected the container in red as well, hold the Ctrl key and click the container to deselect it.
1. Click on the container so it will have a yellow border around it to indicate that it is the currently highlighted object.
1. On the main menu, click **View**, then **Edit Selected Objects**. This tool will open in the right pane (where you normally see **Quick Properties**).
1. Under the **Edit** group, click the **Move into Highlighted** button.
1. If needed, click the **X** in the corner of the **Edit Selected Objects** pane to close this tool and return to Quick Properties.
1. Hold the Shift key and click in a blank area of the model to deselect the objects.
1. Test to ensure that you successfully moved the objects into the plane by clicking the plane and moving it around. If you were successful, all the objects should move together when you move the plane.

  

#### Surrounding Contained Objects

 

To resize the container to fit its contents within its size, click on the container and press **Fit Size To Contents** in the properties pane.

  

 

 

## Moving Objects out of a Container

 

If you want to remove objects from a container, you will use the Edit Selected Objects tool:

 

1. Click on the container and in its properties select **Edit In Place**.
1. Use Ctrl+click to select the objects you want to remove from the container one by one.
1. On the main menu, click **View**, then **Edit Selected Objects**. This tool will open in the right pane (where you normally see **Quick Properties**).
1. Under the **Edit** group, click the **Move into Model** button.
1. If needed, click the **X** in the corner of the **Edit Selected Objects** pane to close this tool and return to Quick Properties.
1. Hold the Shift key and click in a blank area of the model to deselect the objects.
1. Test to ensure that you successfully moved the objects out of the plane by clicking the plane and moving it around. If you were successful, all the objects should remain stationery when you move the plane.
1. Once finished, uncheck **Edit In Place** to return to manipulating the container as a single object.

 

 

 

## Creating Port Connections with Contained Objects

 

You can connect from outside objects to the objects contained by a container. By default, when you attempt to connect with a container, the contained objects themselved with be selectable, so that when you connect, you will connect with those individual contained objects.

 

 

 

## Hiding the Content of Containers

 

To hide the content of containers, right-click the container, and select **Edit** then select **Hide Content**.

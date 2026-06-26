---
id: using3dobjects_creatingcustomobjectsanimations_creatingcustomanimations_creatingcustomanimations
name: "Creating Custom Animations"
kind: manual
breadcrumb: ["Using3DObjects","CreatingCustomObjectsAnimations","CreatingCustomAnimations","CreatingCustomAnimations"]
tags: ["manual","using3dobjects","creatingcustomobjectsanimations","creatingcustomanimations"]
source: "manual/Using3DObjects/CreatingCustomObjectsAnimations/CreatingCustomAnimations/CreatingCustomAnimations.html"
---

# Creating Custom Animations

## Introduction to Animations

 

Many of the 3D objects in FlexSim Library come pre-programmed with some basic animations. While those animations are generally enough for most simulations, you can also create your own custom animations for FlexSim objects and your custom 3D objects.

 

You can use the Animation Creator tool to create these custom animations. Using the Animation Creator you can:

 

- **Import and edit animation files created in other animation software applications.** You can import custom objects with bone animation files from third-party software applications such as Blender, Poser, Mixamo, or 3ds Max.
- **Export animations created in FlexSim to other FlexSim users.** Any animation files you create using the Animation Creator can be easily shared with other FlexSim users if they want to use that animation in a different simulation model.
- **Create variables and triggers that can dynamically change how an object is animated during a simulation run or get information from an animation-related object.** Using the Animation Creator, you can create variables that change an object's animation under certain conditions during a simulation run. For example, you could create a variable that slows down or speeds up an animation based on certain conditions. You could also use variables to change the position, rotation, or color of an object's animated components under certain conditions.
- **Customize how flow items are animated.** You can use the animation creator to create surrogate objects. Surrogate objects are objects that can control the way flow items will be animated when they are being handled by a task executer or a fixed resource. For example, you could animate a surrogate object so that it will rotate when it is inside a processor.

 

 

 

## Key Terms and Definitions

 

The following are some important terms you'll need to know in order to use the Animation Creator:

 

### Animations

 

An animation is a named sequence of object movements that are triggered at various points in a simulation run. You can animate any object that is capable of moving throughout the model or interacting with flow items, such as task executers or fixed resources. Each object may have multiple named animations, and you can dynamically run specific animations as part of your model logic. Each animation contains a set of keyframes, which in turn contain information for animating the individual components of the object.

 

### Components

 

Components are 3D shapes that comprise movable sub-parts of an object. These shapes can be hierarchically ordered, and their position can be defined and changed in keyframes, so that they move as part of an animation run. Most standard FlexSim objects do not contain components by default. They are only comprised of the main object shape. However, some objects do contain pre-assigned components. The Robot, for example, includes components for each articulating joint. You can also add custom components to any FlexSim object.

 

The following image shows a custom component that has been added to the Operator's Walk Loaded animation:

 

 

### Keyframes, Component Keyframes, and Keyframe Datapoints

 

A keyframe is a location in the timeline of an animation that marks the beginning or end of a transition. Each keyframe may contain multiple *datapoints*, which store data like the location, size, rotation, color, and/or shape of one or more of the object's components.

 

A *component keyframe* often refers to a keyframe that only contains data for a single component.

 

A *keyframe datapoint* refers to a single piece of data stored in a keyframe. For example, a keyframe datapoint may be a specific component's x location.

 

Usually, you will put two or more keyframes on a timeline to define the beginning and end of an animation segment. Although keyframes typically define the beginning and end of an animation, you should also put keyframes any place that marks a major movement between components. See the example in the following section about tweening for more information.

 

### Tweening

 

Tweening is when FlexSim automatically interpolates between two keyframes. FlexSim can tween an object's position, rotation, size, and color, but not shape. For example, imagine you want to animate a cube component that will change from blue to red in about 5 seconds, as illustrated in the following image:

 

 

The timeline for this kind of animation would look approximately like the following image:

 

 

Notice that in this timeline, the cube had a keyframe at time 0. This keyframe includes a color datapoint, where the color is blue. There is a second keyframe at time 5, containing a color datapoint of red. While the animation is running, FlexSim will automatically tween the colors, which means that it will create all the transitional colors between time 0 and 5. In other words, you don't have to manually create a purple color halfway through the animation because FlexSim will do it for you when it tweens the two keyframes.

 

As a general rule, you should try to put keyframes in places that mark the beginning and end of a major movement. For example, let's say you wanted to animate the rotation of a box. The first keyframe should be the rotation the box starts from, and the next keyframe should be the rotation you want the box to end up at. FlexSim will then automatically generate a smooth transition between the keyframes as the animation runs, showing the box in motion.

  

#### Timing Animations

 

The amount of time between the keyframes on the timeline will determine how quick or slow the transition will occur.

  

### Animation Variables

 

Animation variables can change the way objects are animated based on conditions that might change during a simulation run. They are useful when some aspect of an animation needs to be different each time it is run. For example, a variable on a component keyframe can change a component's position, rotation, size, color, shape, etc. based on dynamic conditions in the simulation model. You could also use a variable to determine how fast a particular animation should run based on certain conditions.

 

Keep in mind that an animation variable is basically a reference point to an animation-related object, component, or keyframe. Animation variables can point to:

 

- Components
- Surrogates
- Keyframe times
- Time gaps
- Component keyframes (such as the component's position, size, rotation, color, or shape frame)

 

Once you've created an animation variable, you can reference that variable in a FlexScript command. (See Animation Commands for more information.) FlexScript commands can either get information about a variable or it can set the value of a variable. For example, if you want to dynamically change the length of your animation based on parameters in your model at run time, you can create an animation variable that points to a time gap, and then set that animation variable through a keyframe trigger or from a trigger in your model.

  

#### Component Variables are Read-Only

 

Animation variables pointing to a component are static variables. They are merely available as a reference point to a component through the `getanimationvar` command. Calling `setanimationvar` to a variable pointing to a component will have no effect. If you want to dynamically set a component's property, use a variable on a component keyframe instead.

  

### Keyframe Triggers

 

Triggers can be added to animations that will fire when the animation gets to a specific point in the animation's timeline. They are added through the timeline. Triggers allow you to dynamically change or update animation variables, stop the animation, change some parameter in your model, or execute any other FlexScript code.

 

 

 

## Opening the Animation Creator

 

To open the animation creator:

 

1. You first need to add the object you want to animate to the 3D simulation model.
1. Then, right-click the object and select **Edit**, then **Animations** from the menu. The animation creator for this object will open in a separate window (tab) in the center pane.

 

 

 

## Overview of the Animation Creator

 

This section will provide an overview of the Animation Creator UI. The most important elements of the animation creator are labeled in the following image:

 

 

Note that in the preceding image, the toolbar is slightly truncated. See Toolbar for an explanation of the full toolbar accompanied by complete images.

 

Each element will be described in more detail in the following sections.

 

### Preview Windows

 

Located in the upper portion of the animation creator, the preview windows allow you to see how your objects will appear visually during the animation. There are four different panes in this window that allow you to see the animation from various angles.

 

You'll be able to see the object's animation in action when you run an animation using the animation control bar. You can also scrub the timeline (which means to put the cursor at a particular place in the animation timeline) to see how the object will look at specific points in the animation.

 

If you add any components to the animation, you can also use the preview windows to manually resize, reposition, or rotate the component object. You can also zoom in and out or change the angle the same way you would in the 3D model.

 

In the top right corner of each preview pane, you can click the *Maximize* button to hide the other 3D windows and expand the selected 3D window to take up the entire preview window. Click the button a second time to return to the default view.

 

If you right-click anywhere inside a blank spot of the preview window, a menu will pop up with one option: *Flip Axis*. This option flips the axis of the view. For example, if you flip the axis that is currently displaying the top view, it will display the bottom view. This option is only available in the side, front, and top 3D views.

 

### Library

 

When the animation creator is open, the Library will change to display some basic objects that can be added as components in the animation.

 

### Properties

 

You can use the settings in Properties to change the basic functionality of the animation, such as its repeat type and time base. You can also use Properties to create new animation variables. See Managing Animations for more information.

 

### Animation Control Panel

 

Located in the lower portion of the animation creator, the animation control panel is the primary workspace you'll use while working inside the animation creator, as shown in the following image:

 

 

### Toolbar

 

The toolbar contains the various buttons you will use in managing components, animations, and keyframes.

 

### Timeline List

 

The timeline list is kind of like a header for the timeline. It displays any animation clips, triggers, and/or components that have been added to this object's animation.

 

### Timeline

 

The timeline is a chronological display of the animation over time. You can also scrub the timeline (which means to put the cursor at a particular place in the animation timeline) to see how the object will look at specific points in the animation.

 

You can also drag keyframes along the timeline to make them longer. As the space increases between keyframes on the timeline, the tweening animation will take longer.

 

If you want to zoom in or out of the timeline, you can use either one of the following methods:

 

- Click the *Zoom* button on the toolbar to zoom in or out on the timeline to make all the keyframes fit on the screen.
- Position the mouse above the timeline and press the Ctrl key while scrolling the mouse wheel.
- Click the right end of the scroll bar underneath the timeline and drag it to zoom in and out:

 

 

 

## Managing Components

 

 

When defining animations, generally you first define the set of components to be animated. To add a component, press the button in the **Components** section of the toolbar. This will add a new component that can be seen both in the preview pane as well as in the timeline list. Alternately, you can drag a shape from the library into a preview pane.

 

Once you've added a component, click on it in the timeline list and press the and buttons to reorder it in the timeline list. Press the button to move the component into the component above it, and the button to move it out of its containing component.

 

If you click on the arrow next to a component, you can expand the list to show the component's individual keyframe datapoints:

 

 

### Component Properties

 

 

When you click on a component in the preview pane or in the timeline, the Component Properties pane will appear in the properties window. This pane lets you configure various properties of the component.

 

### Draw Surrogate

 

A surrogate is a special kind of component that will be replaced visually by another object (such as a flow item) when an animation is running in the simulation model. It acts like a placeholder for an object that may not yet exist, or cannot be referenced, at the time you create the animation. Whereas components are part of an animation visually (meaning it will show up in animation), a surrogate can be visually replaced by another object.

 

The most common reason for turning a component into a surrogate is to change the animation of a flow item while it is being handled by one of the fixed resources or task executers during a simulation run. Once a component has been converted to a surrogate in the animation creator, it can be used to change a flow item's position, size, or rotation in an animation while a fixed resource or task executer is handling it. When the fixed resource or task executer begins handling the flow item, the flow item's shape will be replaced by the shape indicated in the *Draw Surrogate* menu in the surrogate's Properties.

 

Surrogates are used in conjunction with animation variables. To set up a surrogate:

 

1. Add the component you want as a surrogate, and click on it in the preview pane.
1. In **Component Properties** under ** Draw Surrogate**, select a flow item that generally looks like the object that will be put in place of the surrogate when the simulation runs.
1. In the Animation Properties pane, add an animation variable and give it an appropriate name indicating the surrogate.
1. Below the animation variable name, press the button. Then click on that component in the preview pane. This will associate that animation variable with that surrogate. The control beside the button should now say *Surrogate for *.
1. Add triggers to your model that fire just before running an animation, or when an item enters an object. These triggers may be coded directly, or in process flow. They should set the animation variable you have defined to the dynamically determined flow item (or some other object) that you want to put in place of the surrogate (using setanimationvar()). Once you've set the animation variable and run the animation, the flow item will become part of the animation.
1. Add triggers to your model that will fire after an animation is finished, or when a flow item exits the object, resetting the animation variable to point to nothing, so that the flow item will no longer be part of the animation.

 

When you set up animation surrogates, you may get the flow items drawn twice in the 3D view: once in their standard location and once as they are drawn in place of the surrogate. To fix this, you can hide the contents of the containing object. This will cause the flow item to be shown only as part of the animation.

 

### Rotational Centroid

 

The rotational centroid control lets you define the location of the axes around which the component will rotate. The values should be between 0 and 1, defining a ratio of the size of the object.

 

### Scale X/Y/Z With

 

You can also define how a component's size will be scaled when the main object's size is scaled. Generally, a component's size axes will scale with the same axes of the main object, but these options are provided for exception cases.

 

To use this properly, you should first save base positions, so FlexSim will know how to scale the components.

 

### Parent to Bone

 

See Animating Bones Directly for more information.

 

 

 

## Saving Base Positions

 

 

Once you've defined your set of components, it is a good idea to save your "base positions". Press the to do this. This will save off the default positions for the components that you have defined. Saving base positions is useful for two main reasons:

 

- After manipulating components while creating animations, it is good to be able to go back to a "base configuration".
- With base positions saved, FlexSim will know how to scale components and animations when you resize the main object differently than the size that the animations were created with.

 

 

 

## Managing Animations

 

 

Once you've defined an object's components and saved base positions, you can start managing animations. The controls in the Animations section of the toolbar let you select the animation you want to edit, as well as add, remove, duplicate, and rename animations. Additionally, as you are building the animation, you can press the and buttons to reset and run the animation.

 

### Animation Properties

 

The Animation Properties pane lets you define various properties of an animation.

 

 

#### Animation Variables

 

Here you can define the set of animation variables. To add an animation variable:

 

1. Press the button.
1. Enter the variable's name.
1. Press the button and click on a component, a keyframe, a keyframe datapoint, etc. to define what that animation variable is bound to.

 

#### Repeat Type/Value

 

Here you can define if and how an animation should repeat by default. Choose the desired option, then, if needed, define a repeat value.

 

#### Time Basis

 

This defines how a given animation should "progress":

 

- **Time Based**: The animation will progress by time, meaning the time that the animation is started becomes the "0" point, and the animation will progress evenly as the simulation time progresses.
- **Travel-Distance Based**: The animation will progress based on the location of the main object at any point in time. This is most useful for traveling animations like walking or the rolling of wheels. You want the animation to progress based on the movement of the main object. The faster the object moves, the faster the animation should progress. If the main object is stopped, then the animation should be stopped, etc.

  

 

 

## Managing Keyframes

 

 

As described above, a keyframe is a location in the timeline of an animation that marks the beginning or end of a transition. In the user interface, keyframes a rectangles surrounding small black diamonds that turn red when selected. The black diamond will be shown on each component for which the keyframe contains data. You can also expand a component to see the individual datapoints stored in a keyframe. Each datapoint of a keyframe is represented by a smaller black diamond. See Keyframes and Tweening for more information.

 

### Adding a Keyframe

 

To add a keyframe that stores spatial datapoints on all components, do the following:

 

1. Scrub the time cursor in the timeline to the time where you want the keyframe to be added.
1. Set the location, size, and/or rotation of all of the components to their desired values by moving/sizing/rotating those components in the preview pane.
1. Press the button to add a keyframe.

 

This will add a keyframe that includes datapoints for location, size, and rotation on all components. Note that it does not add datapoints for color or shape. You can add datapoints by adding a component keyframe.

 

### Adding a Component Keyframe

 

Alternately, you can add a keyframe that stores data only for a specific component, or for a specific datapoint of a component. To do this:

 

1. Scrub the time cursor in the timeline to the time where you want the keyframe to be added.
1. Set the location, size, rotation, and/or color of the target component.
1. Select the specific component, or datapoint, in the timeline, by clicking on its associated row.
1. Press the button to add a component keyframe.

 

This will add a keyframe that stores data only associated with the row you have selected in the timeline list.

 

### Adjusting Existing Keyframes

 

You can also change existing keyframes in an animation. To update an existing keyframe:

 

1. Click on the keyframe in the timeline. It should change to red, signifying it is selected. If you only want to update the datapoint(s) of a single component in the keyframe, click on that specific component or datapoint row, so that only that component/datapoint's diamond is red.
1. Set the location, size, rotation, and/or color of the target component(s).
1. Right-click on the keyframe and select **Update Keyframe**.

 

This will update the keyframe's data to store the component(s)' current associated values.

 

### Adding Datapoints to an Existing Keyframe

 

By default, adding a keyframe will only store spatial datapoints, not color or shape. If you want a given keyframe to store datapoints that weren't originally present when the keyframe was added, do the following:

 

1. Click on the keyframe in the timeline, on the specific row associated with the datapoint you want to add. For example, click on the **color** row of the keyframe. Since the keyframe doesn't yet have that datapoint, the row will be blank, but the keyframe should be outlined in red.
1. Set the desired color, shape, or other target aspect of the target component(s).
1. Press the button to add a datapoint to the keyframe.

 

This will update the keyframe's data to store the component(s)' current datapoint value.

 

### Deleting Keyframe Data

 

In addition to deleting an entire keyframe by clicking on it and pressing the Delete key, you can remove specific datapoints or components from a keyframe's data. Just click on the specific black diamond you would like to remove, and press Delete.

 

 

 

## Importing Custom Animations

 

You can create your own custom 3D objects and animations using third-party software applications such as Blender, Poser, Mixamo, or 3DMax. To import a custom object and animation:

 

1. First you need to create a custom object to act as your basic template. Drag either the **Basic FR** object or the **Basic TE** from the Advanced section of the Library into your simulation model. (Use the Basic FR object if you want to create a fixed resource or the Basic TE if you want to create a task executer.)
1. Click the **Basic FR** object or the **Basic TE** object to select it.
1. In Properties under **General Properties**, click the arrow next to the **Shape** box to open a menu.
1. Select **Browse** from the menu and navigate to the location of your custom 3D object's file. Select the file that you want to import. The **Basic FR** object or the **Basic TE** will be updated to the new shape that you imported.

 

When you import an object, it will also import any animations that are associated with it. In order to create custom animations, you might need to create animation clips for the object. See the next section about Creating Animation Clips for more information.

 

 

 

## Creating Animation Clips

 

When you create a custom object using third party software, you might typically want to create more than one animation for it. For example, the default Operator object in FlexSim has three different animations: walking, walking while carrying a load, and standing. So, if you wanted to make your own custom Operator object, it should probably have at least these three same animations.

 

However, some animation software programs only allow you to export all these animations as a single animation file. Consequently, when you import these files into FlexSim, you'll need to cut the animation file into individual clips using the clip editor in the animation creator.

 

To create animation clips:

 

1. With the animation editor open for the object you want to animated, click the **Bone Animations** button on the animation toolbar and select Edit Animation Clips from the menu. This will open the clip editor, as shown in the following image:
1. In the Clips group, click the **Add** button to add a new clip to the list of clips.
1. Give the clip an appropriate name by typing a new name in the **Name** box.
1. If needed, press the **Play** button to preview the full animation.
1. Now you'll need to choose which section of the animation should be included in this clip. Drag the sliders on the timeline to the beginning and end of the section you want to include in this clip. Alternatively, you could manually set the exact frames and times that should be included using the **Frame** and **Time** boxes below the timeline.
1. If needed, you can also change the rate of frames per second using the **Rate** box.
1. Click the **Apply** button to save the changes to the clip.
1. Make any other necessary changes and then close the window.
1. Now when you click the **Animation** menu in the animation control panel, the new clip you created will appear in the list of available animations.

 

You can also add additional animations from other external files using the *Add* button beside the Animation dropdown. This process works correctly if the other animation file has the same skeletal bone structure and bone names as the imported shape file.

 

Use the animation creator's timeline if you want to combine one or more clips together.

 

 

 

## Animating Bones Directly

 

You can directly animate a shape with bones using the **Bone Animations** button and selecting Animate Bones Directly.

 

The bones in the shape will be shown in the Animations and Components hierarchy list and displayed in the 3D views. With this option enabled, you can edit the rotations, positions, and sizes of the bones directly using keyframes as you would any other component.

 

Any keyframe animation of the bones will be applied as an additional transformation after any animation clips are applied, so you can use animation clips, direct bone animations, or a combination of both.

 

The Reset Bone Positions options will set all of the bones back to their unchanged positions. You may want to use this option to add a keyframe at the beginning and end of any animations that animate the bones directly, especially when combining direct bone animations with animation clips.

 

Each component's Properties panel has a **Parent to Bone** option that you can use to animate a component along with a bone when animating bones directly. This can be used for adding additional shapes that animate with the object, such as a hat or a clipboard. The component's position will be relative to the bone instead of the main object when using this option, so you will probably want to set the component's position close to [0, 0, 0] to be near the bone.

 

 

 

## Animation Commands

 

The following commands deal with animations:

 

**startanimation(object, animation)** - Starts an animation on the object. Animation can be either a string value that is the name of the animation, or it can be a number which is the animation rank.

 

**stopanimation(object, animation)** - Stops an animation on the object. Animation can be either a string value that is the name of the animation, or it can be a number which is the animation rank.

 

**resumeanimation(object, animation)** - Resumes an animation on the object that was previously stopped. Animation can be either a string value that is the name of the animation, or it can be a number which is the animation rank.

 

**getanimationvar(object, animation, "varname")** - Returns the value of an animation variable.

 

**setanimationvar(object, animation, "varname", value)** - Sets the value of an animation variable. Value can be a number or object and is based upon what the animation variable is linked to.

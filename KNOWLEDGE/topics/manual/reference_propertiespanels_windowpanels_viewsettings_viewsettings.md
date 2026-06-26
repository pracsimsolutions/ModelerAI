---
id: reference_propertiespanels_windowpanels_viewsettings_viewsettings
name: "View Settings"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","WindowPanels","ViewSettings","ViewSettings"]
tags: ["manual","reference","propertiespanels","windowpanels"]
source: "manual/Reference/PropertiesPanels/WindowPanels/ViewSettings/ViewSettings.html"
---

# View Settings

The View Settings panel provides options for customizing the 3D view.

 

 

The following properties are on the View Settings panel:

 

### Mode

 

This option gives you two view setting presets. Switching to Presentation mode turns off Connections, the Grid, and Object Names.

 

### Perspective Projection

 

This option toggles the 3D view from being a Perspective View, to displaying as an Orthographic View. The Perspective View looks more real-world.

 

 

Where as the Orthographic view has no depth.

 

 

### Show Connections

 

If this box is checked, the ports and port connection lines will be displayed in the view window. Hiding these connections often makes it easier to see what is happening in the model. If a model is slowing down, often un-checking this box can help speed it up.

 

### Show Grid

 

If this box is checked, the grid will be drawn in the view window.

 

### Snap to Grid

 

If this box is checked, objects will automatically move to the nearest grid line when they are moved in the model. This is useful for placing objects in precise locations. Resizing of objects will also snap to the grid if this is checked.

 

### Snap to Background

 

If this box is checked, objects will automatically move to the nearest background snap point of the first model background object when they are moved or resized in the model.

 

### 1st Person

 

If this box is checked, the view window's mouse controls will be in first person mode. This means that the view will rotate around the user's view point, and not around a point in the middle of the screen. This mode is most useful when navigating in fly-through mode.

 

### Ignore Objects

 

If this box is checked, the user will not be able to click on any objects in the view window. This is useful for navigating around a model that is completed, as the user will not be able to accidentally move any objects.

 

### VR Mode

 

Checking this box will cause the view to be rendered on a connected virtual reality headset. FlexSim works with Oculus Rift and HTC Vive VR headsets. The view will also render and handle inputs from VR motion controllers (Oculus Touch or HTC Vive controllers) if this box is checked. If you are using an Oculus Rift, ensure that Unknown Sources is checked in your Oculus Settings.

 

### RTX Mode

 

Checking this box will cause the view to be rendered using Nvidia's OptiX ray tracing engine. This rendering mode is only available using 64-bit FlexSim on compatible Nvidia graphics cards and drivers.

 

Nvidia RTX graphics cards (Turing architecture) have RT cores that improve the performance of RTX Mode. Some older Nvidia graphics cards (such as Maxwell, Pascal, and Volta architectures) can work with RTX Mode with lower performance.

  

#### Stability

 

RTX Mode uses graphics technology that is still being developed, tested, and improved. Save your work before enabling RTX Mode to avoiding losing progress in the event of a crash.

   

#### Performance

 

RTX Mode performance is primarily determined by the resolution of the 3D view. If you are using a large monitor resolution, you may want to make the 3D view smaller in order to improve the framerate of the view while you are testing your model in RTX Mode. Once your model looks good, you can use the Capture View or Video Recorder features to capture screenshots or video of your model at high resolutions in RTX Mode.

  

#### RTX Benefits

 

- **Improved shadows.** All of the scene's light sources (including point source lights) can cast shadows, and the resolution of the shadows is not determined by rendering the scene multiple times into cascading shadow map textures. Each light can cast realistic hard or soft shadows depending on its settings.
- **Unlimited light sources.** RTX Mode can use any number of light sources, although performance will decrease as the number of lights increases.
- **Order-independent transparency.** Transparent meshes do not have to be drawn after other meshes when using RTX Mode, and the order of the triangles within a translucent mesh does not affect how it is rendered.
- **Global illumination.** In RTX Mode, indirect lighting is calculated based on light bouncing off surfaces rather than estimated with a global ambient light color. This provides for more realistic looking shadows on surfaces that are not directly lit by light sources.
- **Reflections.** Meshes can use reflectivity to mirror the environment around them.

 

#### RTX Limitations

 

- **Only triangles.** RTX Mode currently only renders meshes drawn with GL_TRIANGLES. Other draw modes, such as lines and points, are not supported.
- **Slow bone animations.** In RTX Mode, bone animations are currently being calculated on the CPU rather than in a vertex shader on the GPU. This causes models with bone animations to run slower in RTX Mode than with the default shader rendering.

 

#### RTX Future Improvements

 

- RTX Mode is currently using a material shading technique that is similar to FlexSim's default shaders. Future improvements could include physically based materials, refraction, and other ray tracing techniques.

 

### Sync Views

 

If this box is checked, all open view windows will be updated at the same time. If it is not checked, some windows may not be updated until an action is completed in a different window. Checking this box may cause the program to run a little slower.

 

### Grid Z

 

Changes the z position of the grid to more easily create multi-floor models. Objects added to the model will be placed at this z position.

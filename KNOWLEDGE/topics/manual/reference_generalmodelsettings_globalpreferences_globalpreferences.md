---
id: reference_generalmodelsettings_globalpreferences_globalpreferences
name: "Global Preferences"
kind: manual
breadcrumb: ["Reference","GeneralModelSettings","GlobalPreferences","GlobalPreferences"]
tags: ["manual","reference","generalmodelsettings","globalpreferences"]
source: "manual/Reference/GeneralModelSettings/GlobalPreferences/GlobalPreferences.html"
---

# Global Preferences

## Overview and Key Concepts

 

The global preferences dialog window specifies default user preferences for FlexSim. These preferences are saved across several models and remembered when FlexSim is closed and reopened. The dialog window is split into seven tabs.

 

These settings are accessed from the File menu.

 

 

 

## The Fonts and Colors Tab

 

The Fonts and Colors tab specifies syntax highlighting and other settings used in FlexSim's implementation of the Scintilla code editor. You can also specify settings and colors for the template editor, as well as for unformatted text (which is used in multi-line unformatted text controls like the user description portion of the Properties window).

 

 

For more information on the Scintilla code editor, go to [www.scintilla.org](http://www.scintilla.org).

 

The Scintilla text editor is under copyright 1998-2003 by Neil Hodgson . All Rights Reserved.

 

 

 

## The Environment Tab

 

 

In this tab page you can specify various settings such as whether you want code to be C++ or FlexScript by default, various grid settings in the ortho and perspective view, excel DDE settings, etc.

 

You can also change the application language. FlexSim must be restarted for a language change to take effect.

 

AutoSave will automatically save a backup model of your currently open model. This backup model will be named [modelname]_autosave.fsm and will be saved in the same directory as your model. AutoSave will only save your model if it is reset and not running. You can disable AutoSave for a specific model in the Model Settings window.

 

For a list of valid time and date format options, see Model Settings.

 

### Cloud Computing

 

You can use this area to specify a list of remote computers for the Experimenter. When the Experimenter runs a job using remote computers, it will send an HTTP request to the specified **IP Address** and **Port**. It assumes that the FlexSim Webserver will respond to those requests. The Experimenter will launch the number of FlexSim instances specified in the **CPUs** field on each remote machine. You can use the **Test Connections** button to ping each remote computers, and report whether the ping was recieved or not.

 

For more information on using remote computers for Experiment Jobs, see Running Jobs on the Cloud for more information.

 

 

 

## The Libraries Tab

 

 

This tab page lets you control what libraries are displayed in the Drag-Drop Library.

 

### User Libraries to Open on Startup

 

This section lets you list user libraries that will be automatically loaded when FlexSim is opened. The paths specified in the list are relative to your project/libraries folder (for example, C:\Users\User1\Documents\FlexSim 2022 Projects\libraries) or the libraries folder of the install directory (for example, C:/Program Files/FlexSim/libraries). 

### Enabled Libraries

 Enabled Libraries allow you to specify which libraries are visible and which order they appear in. Once a User Library is loaded, it will be displayed in this list.

 

 

 

## The Dynamic Content Tab

 

 

In this tab page you can specify various settings that send or pull content from the FlexSim server in order to give you a more dynamic experience or to help FlexSim better understand how to improve the software.

 

 

 

## The Customize Toolbar Tab

 

 

In this tab page you can customize which menu commands are accessible easily through the customizable section of the top toolbar.

 

 

 

## The Graphics Tab

 

This tab is used for customizing 3D graphics settings so that FlexSim will run the best on your hardware.

 

Several of these options provide suggestions or hints to the graphics card as to the quality you would like. If the graphics card does not support the feature requested, such as Quad Buffered Stereo 3D or a 32 Bit Depth Buffer, it will fall back to using the next best option that it supports.

 

If no graphics acceleration is available with your system configuration, FlexSim will automatically fall back to using a generic software OpenGL implementation and ignore the settings defined here.

 

 

### OpenGL Context

 

This option controls which type of OpenGL rendering context FlexSim will request from the graphics driver on your computer.

 

- **Recommended** - A hardware-accelerated OpenGL rendering context that supports almost all features of OpenGL, including deprecated features. This setting will request the best OpenGL Compatibility Profile that the graphics driver supports.
- **Core Profile (3.3)** - A hardware-accelerated OpenGL rendering context that supports only OpenGL 3.3 features. Deprecated features have been removed. Some graphics driver implementations and system configurations (such as VMware virtual machines) only perform hardware graphics acceleration with an OpenGL Core Profile.
- **Generic (No GPU Acceleration)** - A generic software renderer by Microsoft that supports OpenGL 1.1 on any hardware. Graphics processing will be done on the CPU and not the graphics card (GPU). This type of rendering context is slow, and certain graphics features, such as bone animations and shadows, will not work with this context. This option was previously known as Compatibility Mode.

  

#### Graphics Cards

 

If drag-dropping objects, making connections, or navigating the 3D view is not working properly with the Recommended context, your graphics card may not be compatible with FlexSim.

 

You should first try updating your graphics card driver from your computer's manufacturer. If that does not help, try updating the driver from the graphics card vendor (typically Nvidia, AMD, or Intel). If that doesn't work, you should try switching to the Core Profile (3.3) OpenGL Context. As a last resort, select the Generic OpenGL Context to use your CPU for graphics processing.

  

### Show FPS Counter

 

If checked, FlexSim will display a counter showing how quickly the 3D view is rendering in frames per second.

 

### 3D Text

 

Choose the font and size for the object names and stats rendered in the 3D view. The font choice will also affect the default font for rendered 3D text.

 

### Use Shadows

 

This option controls whether shadows will be rendered in the 3D environment.

 

### Shadow Type

 

This specifies whether shadows should be rendered with hard or soft edges.

 

### Shadow Texture Size

 

This specifies the resolution of the shadow map. A higher resolution will improve shadow quality, but will require more graphics processing.

 

### Cascade Splits

 

This specifies the number of cascaded shadow maps to use. More cascade splits will improve shadow quality, but will require more graphics processing.

 

### Softness

 

This affects how much shadows are blurred around the edges when using soft shadows.

 

### Build Mipmaps

 

If checked, FlexSim will generate multiple resized "thumbnails" of each texture it loads. This allows for faster and better texture rendering when zoomed out, but requires approximately double the graphics card memory to store the texture.

 

### Texture Magnification

 

Select how the graphics card should resolve the color of each pixel when you are zoomed in close to a texture.

 

- **GL_LINEAR** - This will do a linear interpolation of the color values of multiple texture elements surrounding the pixel. While this looks much better when zoomed in, the linear interpolation calculation can be slower than the GL_NEAREST option.
- **GL_NEAREST** - This will just take the color value of the texture element closest to the pixel. While faster, this doesn't look as good when zoomed in.

 

### Texture Minification

 

Select how the graphics card should resolve the color of each pixel when you are zoomed out from a texture.

 

- **GL_LINEAR** - This will do a linear interpolation of the color values of multiple texture elements near the pixel. This allows for a more smooth look when zooming out, but the linear interpolation calculation can be slower than the GL_NEAREST option.
- GL_NEAREST - This will just take the color value of the texture element closest to the pixel. While faster, can cause weird "flicker" artifacts when zooming out.
- The other options are more or less similar to the above options, but allow more flexibility in defining which mipmap "thumbnail" to use as well.

 

### Color Bit Depth

 

Choose the format in which you want the graphics to store color data.

 

### Depth Buffer Bit Depth

 

Choose the format in which you want the graphics to store depth buffer data. Depth buffer values span from a 3D view's near plane to its far plane, so the depth buffer bit depth defines the granularity by which the graphics card can distinguish which objects are in front of or behind other objects.

 

### Stereoscopic 3D

 

This option controls how stereoscopic 3D should be rendered.

 

- **No Stereoscopic 3D** - Do not attempt to do any stereoscopic 3D rendering.
- **Quad Buffered Stereo 3D (Frame-Sequential)** - Certain system configurations can use this mode to sync the 3D display with active-shutter glasses using four OpenGL render buffers to display every other frame to each eye. This mode requires specialized hardware and software designed for 3D, such as Nvidia 3D Vision. Once your computer is properly configured with a 3D display, this mode will automatically render 3D views in stereoscopic 3D. If you are using an Nvidia Geforce graphics card, the view must be full-screen (F11) to trigger the effect. On Nvidia Quadro or AMD FirePro graphics cards, the effect will work with all 3D views. This mode only works with certain configurations of hardware and software. The graphics card, 3D display monitor, cable/port (DVI-D, DisplayPort, HDMI 1.4a), graphics driver, and operating system must all be compatible for this mode to work correctly. If any of those pieces are not compatible, then FlexSim will automatically fall back to rendering without stereoscopic 3D.
- **Side-by-Side** - This option will render each eye to one side of the screen. This can be used to record 3D screenshots or video to be later played with a stereoscopic 3D picture viewer or video player. It can also be used in full-screen mode with certain 3D TVs or monitors that support side-by-side stereo 3D input.
- **Top-Bottom** - This option works similarly to side-by-side, but one eye will be rendered at the top of the screen and the other will be rendered at the bottom.

 

### Separation

 

This controls the difference in horizontal position between the right and left images, changing the depth of the stereo effect.

 

### Convergence

 

This controls the focal point of the stereo effect. Increasing the convergence makes near objects appear to pop out of the screen.

 

### RTX Mode

 

These settings are applied when enabling RTX Mode. After modifying these settings, restart RTX Mode to see the changes.

 

- **Denoiser** - The denoiser is an AI-accelerated post processing step that blurs noise in the scene caused by heuristics in the ray tracer. If the denoiser is installed, a checkbox will be available for enabling or disabling it. If the denoiser is not installed, then a button will be available for attempting to automatically download and install the denoiser. If the automatic install isn't working, then the denoiser can be manually downloaded and installed.
- **Samples Per Pixel** - This option controls the number of rays that are traced per pixel. The final pixel color will be the average color of the samples. Using more samples per pixel can improve the quality of certain ray tracing effects, such as soft shadows, but it requires more processing, which lowers the frame rate.

 

 

 

## Code

 

This tab is used for configuring how FlexSim interacts with dlls or modules made by other languages:

 

 

### Python Version

 

Used when executing a node configured to call a python module. The version you specify should match the major and minor version of python found on your path. You can check which version of python is installed by running the following command in a command prompt window:

 

```
python --version
```

 

Note that in FlexSim, you only need to specify the major and minor version numbers. For example, if python 3.12.6 is installed, you should specify 3.12 in this field.

 

See Connecting to Python for instructions on configuring a node to call a python function.

 

### .NET Runtime Config

 

Used when executing a node configured to call a .NET dll function. If left blank, FlexSim will load the runtime found in FlexSim's directory install directory:

 <!-- Use the language-markup class to avoid regex/property highlighting --> 

```markup
program/CSharp/default.runtimeconfig.json
```

 

This runtime should work for most cases. If your dll needs a different runtime configuration, you can specify an absolute path to a different runtime file in this field.

 

See Connecting to a C# DLL for instructions on configuring a node to call a .NET dll function.

 

---

 

The remaining properties on this tab deal with compiling C++ code found in the tree, such as the code found in `MAIN:/project/exec/globals/nodefunctions/byteblocktofile`. In general, only the FlexSim developers need to modify and compile that code.

 

If you want to write custom C++ code, you should follow the instructions for adding a dll to a module or building a dll with the DLL Maker. These properties have no effect on module DLLs or DLL Maker projects.

 

---

 

### Enable C++ Debugging

 

If checked, code in the tree will be compiled with debug symbols to allow debugging with Visual Studio.

 

### Visual C++ Version

 

Specifies the version of Visual Studio to use when compiling the tree. Choosing this property sets the **Visual Studio Install Path** and **Platform SDK Install Path** properties.

 

### Visual Studio Install Path

 

Specifies the path to Visual Studio.

 

### Platform SDK Install Path

 

Specifies the path to the current platform SDK.

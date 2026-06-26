---
id: reference_tools_codeeditor_codeeditor
name: "Code Editor"
kind: manual
breadcrumb: ["Reference","Tools","CodeEditor","CodeEditor"]
tags: ["manual","reference","tools","codeeditor"]
source: "manual/Reference/Tools/CodeEditor/CodeEditor.html"
---

# Code Editor

#### FlexScript API Reference

 

For information on FlexScript classes, see the FlexScript Class Reference.

  

 

 

## Overview and Key Concepts

 

The Code Editor window allows you to edit code for picklists and triggers throughout FlexSim. The window can be used as a floating window (default), or it can be docked into the main FlexSim window in any configuration. Just click the tab or the title bar and drag it over the Dock Windows icon.

 

Throughout FlexSim you will see buttons. Clicking this icon will open the Code Edit window and allow you to edit that picklist or trigger's code. Alternatively, many right click menus (like in the Tree Window) have the option to select Explore, then As Code that will also open a Code Edit window.

 

When you open up the Code Editor, you'll likely see some header statements that will look something like this:

 

```
		treenode current = ownerobject(c);
		treenode item = parnode(1);
```

 

Within the code window, you can specify whether you want your code to be interpreted as FlexScript or externally linked. You can also check the FlexScript syntax by pressing the button.

 

### Code Builders

 

When you are editing code in the Code Editor, or entering values in a picklist field, the Library (left pane) changes to display a list of Code Builders.

 

 

These Code Builders can be dragged into your Code Editor or picklist field to give you the correctly formatted command. Use the tab key to select each commented section of the template code. i.e., /*"title"*/

 

 

You can also filter the Code Builder list by typing in the field.

 

 

### Externally Linked Code

 

You can also specify the given field as accessing a function from externally linked code, such as a C++ DLL or Python. In this case you would not provide the code as the text, but would provide the path to the external file as well as the name of the function to call. To create a C++ DLL, you use a special DLL Maker Visual C++ project that is available to download from the FlexSim website.

 

See Connecting to External Code for more information.

 

 

 

## Properties

 

The Code Editor has the following properties:

 

The tab bar displays the current object and trigger/picklist being edited.

 

| Icon | Description |
| --- | --- |
|  | Displays this help page. |
|  | Removes all template code. Template code may be found in picklists and triggers and takes the form of: /***popup:Conditional*/ and /** \nCondition: *//***tag:expression*//**/ |
|  | Checks syntax for compile errors. |
|  | Toggles the current code as Flexscript. |
|  | Toggles the current code as externally linked. |
|  | The Sampler allows you to insert code into your code editor to reference objects, set labels, get values, etc. |

---
id: reference_tools_scriptconsole_scriptconsole
name: "Script Console"
kind: manual
breadcrumb: ["Reference","Tools","ScriptConsole","ScriptConsole"]
tags: ["manual","reference","tools","scriptconsole"]
source: "manual/Reference/Tools/ScriptConsole/ScriptConsole.html"
---

# Script Console

#### FlexScript API Reference

 

For information on FlexScript classes, see the FlexScript Class Reference.

  

 

 

## Overview and Key Concepts

 

 

The script console allows you to execute Flexscript commands on the fly without needing to run your model. This can be useful for getting information from your model as well as configuring your model. Type the Flexscript code in the main field at the bottom of the window and press the . If your code has a return value, this value will be displayed in the results field. If you are executing a command with a return value like, `Model.find("Processor1")` and your code consists of only one line, you can omit the return and the semi-colon at the end of the line to see the return value in the results field.

 

You can use the Sampler button to reference objects and paths in your model.

 

 

A Script Console can be accessed through either the Debug menu on the main toolbar.

 

 

 

## Debugging Code

 

If you need to debug your code, you can enter debugging mode by clicking the of the but and select Debug. This will place a debug point at line 1 of your code, open it in a Code Editor and execute the code.

 

 

 

## Saving Scripts

 

Code entered into the Script Console can be saved as either a Model Script or a User Script by pressing .

 

| Icon | Description |
| --- | --- |
|  | Saves the current script. If the script has not yet been saved, opens the Save As popup. |
|  | Opens the Save As popup. |

 

The following image shows the Save As popup. If needed, press the arrow to display a list of previously saved scripts.

 

 

### Save As User Script

 

These scripts are saved in the user preferences folder (VIEW:/environment). These scripts are available for all models while FlexSim is open under your Window's user account.

 

### Save As Model Script

 

These scripts are saved in the model's /Tools/Scripts folder. They are only available for the current model.

 

 

 

## Loading Scripts

 

Saved scripts can be loaded by pressing thebutton. A menu will appear with a list of all Model Scripts, separated by a line, and then all your User Scripts.

 

 

### Empty Script

 

Closes any currently open script and clears the code field.

 

 

 

## Managing Saved Scripts

 

As seen in the above image, Change Variables contains a sub-menu of further Model Scripts. You can organize your scripts through any number of sub-menus. To manage your scripts, click the button and select Manage model scripts... or Manage user scripts... A Tree Window will appear.

 

 

From here, you can organize your scripts by creating sub-nodes and renaming any of your scripts.

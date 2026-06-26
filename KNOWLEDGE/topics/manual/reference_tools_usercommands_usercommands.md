---
id: reference_tools_usercommands_usercommands
name: "User Commands"
kind: manual
breadcrumb: ["Reference","Tools","UserCommands","UserCommands"]
tags: ["manual","reference","tools","usercommands"]
source: "manual/Reference/Tools/UserCommands/UserCommands.html"
---

# User Commands

## Overview and Key Concepts

 

 

The User Commands tool lets you add, delete, and edit custom commands in your model.

 

Once you have created your command you can call it like any other FlexSim command. It will appear in blue when typing code and it will appear in the Command Summary. You can also hover the mouse over the command and press F1 to display the Command Helper.

 

 

User Commands are accessed from the Toolbox.

  

#### Calling User Commands from C++

 

When calling a user command from C++, all parameters passed will need to be converted to a number, or else you may get compiler errors. To convert a parameter to a number, use the tonum() command.

  

 

 

## Parameters

 

 

### Parameters

 

Each parameter should be one of the following types:

 

`Object` `obj` `treenode` `node` `Array` `array` `string` `str` `num` `int` `double` `var`

 

The `var` parameter will accept any of the datatypes. To specify multiple datatypes but not include all datatypes you can concatenate parameters together using a forward slash:

 

`str/node`

 

In the Parameters field you can have optional parameters by enclosing the additional parameters in square brackets at the end of the list:

 

`(var object, num type [, int flags])`

 

### Return Type

 

The Return Type can be blank or one of following types:

 

`num` `str` `node` `var`

 

The `var` type indicates that the return type of the function is a Variant value.

  

#### Parameters and Return Type

 

The values specified here will be used by the FlexScript parser. The number of parameters and their datatypes will give FlexScript warnings if not passed in as documented in the Parameters section here. If the return type is incorrectly used in code, it will return a FlexScript error and not build correctly. You can specify the parameters list as (...) and leave the return type blank for the FlexScript parser to ignore them. For examples of parameter lists and return types, view the Command Documentation.

  

 

 

## Accessing Parameters

 

If your command specifies any of the above data types as parameters that will be passed in to the command, you can access those parameters through the param command:

 

`param(num)`

 

Where num is the rank from the parameter list. For example, if the parameters was specified as:

 

`(treenode dataNode, num maxNumDataPoints, num incrementValue)`

 

You would access those values in your code by:

 

```
		treenode dataNode = param(1);
		double maxNumDataPoints = param(2);
		int incrementValue = param(3);
```

 

 

 

## Description

 

When you open the command documentation from the menu Help > Commands, the user commands will be shown in the command documentation in addition to all of the built-in FlexScript commands. The description field shown above will be displayed as html so you can use html markup for formatting, such as  and <\b> for bold. Because the description allows for html markup, some standard characters do not work properly, such as <, >, &, and new line characters. To display these types of characters, you can use special markup codes, such as ">" (without the quotes) for a > symbol and   for a line break. If any of the commands' descriptions container invalid html markup, FlexSim may throw an exception when you try to open the Users Manual and your commands will not be properly documented.

 

 

 

## Properties

 

User Commands has the following properties:

 

 

| Icon | Description |
| --- | --- |
|  | Adds a new blank User Command. |
|  | Removes the selected User Command. |
|  | Reorders User Commands Up or Down in the list. |
|  | Adds the User Command or All User Commands to a User Library as either a Draggable Icon or an Auto-Install Component. |

 

### User Commands List

 

Displays a list of all User Commands. Select a command from the list to edit it.

 

### Name

 

The name of the User Command. In writing code, this name is the name of the command, ie myUserCommand().

 

### Parameters

 

Specifies the parameters that can be passed into the command. Each parameter should be separated by a comma. To access a parameter in code, use the `param(paramNum)` command.

 

### Return Type

 

Specifies the value the command will return. This can be values like, num, treenode, string, etc.

 

### Short Description

 

Specifies the short description of the command. The short description displays in the tooltip when the command is typed or you hover the mouse over the command in a code edit window.

 

### Description

 

The description displays in the Command Summary.

 

### Example

 

The example displays in the Command Summary. 

### Code

 Here is where you specify the Flexscript, C++ or DLL code for the command.

 

### Apply

 

Saves all changes to the User Command.

 

### Close

 

Closes the User Command window without saving changes.

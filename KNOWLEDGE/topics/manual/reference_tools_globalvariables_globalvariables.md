---
id: reference_tools_globalvariables_globalvariables
name: "Global Variables and Macros"
kind: manual
breadcrumb: ["Reference","Tools","GlobalVariables","GlobalVariables"]
tags: ["manual","reference","tools","globalvariables"]
source: "manual/Reference/Tools/GlobalVariables/GlobalVariables.html"
---

# Global Variables and Macros

## Global Variables

 

The Global Variables window lets you create global variables and macro definitions that are accessible in FlexScript and C++. Once a variable has been created, you can get and set the value of that variable in a Code Editor window or Script Console.

 

  

#### Values Shown are Initial Values

 

The value shown is the *initial* value of the variable. It is not the current value of the variable. The current value of the variable is stored in memory and can be seen by returning it in a script window or printing it from somewhere in code. The current value is not stored in the model tree anywhere. Global variable values are reset when you open the model, reset or compile.

  

There are 6 variable types you can use: integer, double, treenode, string, Array, and Map. For the Array type, you can specify the size of the array and the initial value of each array element.

  

#### Using C++

 

If you access global variables in C++, you must make sure that the variables' names are globally unique names, meaning you do not use those names anywhere else in your C++ code except for when you are accessing the global variables themselves. FlexSim uses a macro definition to define these variables, so any other occurrences of the variable name may cause model malfunction and compile errors.

  

Global Variables are accessed from the Toolbox.

 

### Properties

 

- Adds a new Global Variable.

 

- Removes the selected Global Variable.

 

- Reorders the selected Global Variables Up or Down in the list.

 

**Variable List** - Displays all the model's Global Variables. Click to edit.

 

**Variable Name** - The name of the Global Variable. This is the name that will be used when writing code, for example `setlabel(current, "wIP", maxWIP)`.

 

**Type** - Specify the Global Variable's type.

 

- Only available for Integer and Double types. Pins the global variable to a Dashboard as either the current value, bar chart or line graph.

 

**Value** - The initial value of the Global Variable.

 

- Only available for Tree Node type. Opens a Tree Browse Dialog allowing you to select a node from the tree. Any node or object attribute may be selected.

 

- Only available for Tree Node type. Opens a popup allowing you to select an object in the model.

 

- Only available for Tree Node type. Click to sample an object, node or attribute in your model. If the Array type is chosen, the Global Variables window will display the following:

 

 

**Add** - Adds an empty value to the end of the array.

 

**Remove** - Removes the selected value.

 

**Browse** - Opens a Tree Browse Dialog allowing you to select a node from the tree. Any node or object attribute may be selected.

 

- Opens a popup allowing you to select an object(s) in the model. If a value is selected in the list, this sets that value, otherwise, it adds new values to the end of the array.

 

- Click to sample an object, node or attribute in your model. If a value is selected in the list, this sets that value, otherwise, it adds a new value to the end of the array.

 

  

## Global Macros

 

 

The global macros page lets you make macro definitions.

 

You can define macros using #define statements, as follows:

 

```
		#define MAX_WAIT_TIME 30
		#define PROCESS_TIME_TABLE Table("ProcessingTimesGT")
		#define PAINTER_COLUMN 1
```

 

Once you have made these definitions, you can use them in your code:

 

`gettablenum(PROCESS_TIME_TABLE, 1, PAINTER_COLUMN)`

  

#### Macro Definition Syntax

 

Macro definitions do not end with a semicolon. If you put a semicolon in the macro definition, it may do things you don't expect it to do. Macros essentially replace the given text with the following specified value/text throughout your code. If you have a semicolon at the end of the statement, you may end up with semicolons in incorrect places in your code.

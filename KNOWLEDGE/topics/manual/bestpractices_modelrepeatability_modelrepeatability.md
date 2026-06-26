---
id: bestpractices_modelrepeatability_modelrepeatability
name: "Model Repeatability"
kind: manual
breadcrumb: ["BestPractices","ModelRepeatability","ModelRepeatability"]
tags: ["manual","bestpractices","modelrepeatability","modelrepeatability"]
source: "manual/BestPractices/ModelRepeatability/ModelRepeatability.html"
---

# Model Repeatability

## Overview

 

FlexSim allows you to have variability in your simulation runs through the use of statistical distributions. These randomly generated values give you more accurate results than a static set of data. Using the Experimenter you can run multiple replications of various scenarios, each of which can produce different results. You can compare and analyze those results for a more thorough understanding of your simulated system, or for the creation of confidence intervals.

 

However, when you are building a model and want to validate your results, or you are trying to fix an error in your model, these varying results may be undesired. In these cases it can be useful to have your model be repeatable.

 

This topic lists all the ways to make your model repeatable.

 

 

 

### Repeat Random Streams

 

This option can be found under the Statistics Menu. If checked, every time you reset and run the model the random number generator will produce the same sequence of numbers. This makes it possible to have the results of your model be the same for each simulation run. If repeatable results are desired, this must always be checked.

 

To truly have a repeatable model, the conditions below must be satisfied.

 

 

 

### Task Executer Reset Position

 

If your Task Executers do not have a reset position, then when you reset and run your model, those Task Executers will start from wherever they were at the end of your last simulation run. This means that the Task Executers will have different travel times from run to run, which will affect your results.

 

In order to set a reset position, right click on the Task Executer and select Edit > Set Object Reset Position. Or to set the reset position for multiple Task Executers at once, shift-select or Ctrl-click a group of Task Executers, then highlight one of the selected objects and click the "Set" Position on Reset button in the View > Edit Selected Objects section of Quick Properties.

 

 

 

### Custom Draw Code

 

If any of your objects have code in the "Custom Draw" trigger, make sure that that code does not include any calls to functions that use the random number generator, such as statistical distributions.

 

 

 

### Item Locations

 

If any of your code queries an item's location, you must call the function updatelocations() on the item or the Fixed Resource object containing the item before making your query. If updatelocations() is not called, the location returned will be where the item was when it was last drawn. Drawing rates vary with the run speed, so if updatelocations() is not called, model results will vary between running the model slowly and running it quickly, or running on different hardware.

 

Keep in mind that loading/unloading an item while it is moving (on a conveyor, for example), requires that its location be queried. As such, updatelocations() should be called immediately before loading/unloading items to/from a moving location.

 

 

 

### Initializing Variables

 

Declared variables in custom code should always be initialized to some default value. Failure to do so can result in variables containing random data. If there are any circumstances under which a variable might be accessed before it has explicitly been set, random data in that variable may cause non-repeatable behavior in your model.

 

```
			int myNumber;		//the myNumber variable will have random data
			int myValidData = 0;	//by initializing to a value that I can account for in my code, I guarantee that no random behavior will be introduced by uninitialized variables
			treenode myObject;	//because I did not initialize this value, there is no telling what it is referencing. It is not NULL by default!
			treenode myNode = NULL;	//much better to specify a NULL treenode, or some other treenode, as the default value, rather than assume (or hope) that an uninitialized variable will be NULL.
```

 

 

 

### Resetting Data

 

If at any time during your model run values of persistent data are changed, that data will not change back to its original state on model reset. Examples of persistent data include Global Tables and object labels. Be sure to add code into OnReset triggers that will change the data back to its original state.

 

For object labels, one way to do this is to check Automatically Reset Labels on the object's Label page.

 

 

 

### Non-Repeatable Data

 

If any of your logic is based on data that will always vary, your model will never be repeatable. You should re-architect those portions of your logic to avoid basing decisions or conditions on non-repeatable data.

 

Examples of non-repeatable data include memory addresses (using tonum() to get the memory address of a treenode, note that tonum() is deprecated and should no longer be used) and real time (using the realtime() function to get real-world time stamps).

 

 

 

### Eliminate all Modeling Errors

 

Track down and fix any issues that are causing **System Console** or **Compiler Console** errors. Do not ignore these errors! These errors tell you that something in your model is wrong and should be fixed.

 

Among other issues, errors in your model can cause unpredictable behavior and may cause your model to not be repeatable.

 

 

 

### Debugging Non-Repeatable Models

 

If a model is still not giving repeatable results and the above suggestions have already been applied, the best way to debug the model is by looking at the Event Log, which can be found under the **Debug** menu.

  

1. Set up the model exactly how you want to test it: - Event Log open, "Enable Logging" checkbox ticked - Start Time and End Time values set - Various interface windows open or closed (3D views, Global Tables, etc.) - etc.
1. Save the model.
1. Close FlexSim.
1. Double-click on the model to open FlexSim with the model open.
1. Press Reset once and Run once.
1. Once the run is complete, or has passed your desired Stop Time in the Event Log window, press Export to save the log as a CSV file.
1. Press Reset once and Run once.
1. When the second run is complete, Export the new Event Log using a different file name.
1. Now that you have the two event logs, you can compare them using a text comparison tool such as WinMerge.
1. Find the first event where the time differs between the two logs.
1. Inspect the objects involved in that event, or the objects that created that event, to see what could be different between the two runs.
1. Rerun the model to just before that point, then step through events leading up to the diverging event. Use the debug tools to step through your code where necessary.

 

For some models, it helps to click the Event header on the Event Log and filter out particular events. For instance, if you are sending messages that include a memory address as one of the message parameters, those addresses will be different for every simulation run and will make finding other differences in your exported log file hard to find. Sometimes this is expected and can be accounted for by removing these events from your log file.

 

You can close and reopen FlexSim each time just in case the model is doing something to the application's state or variables in the MAIN or VIEW trees that aren't reset when you just open a model. This helps ensure that even if subsequent runs aren't the same, the first run in each test will match the first run of each subsequent test. Steps 3-6 should produce identical runs even if the model isn't repeatable after a simple Reset/Run without closing FlexSim.

 

 

 

### Additional memory addressing considerations

 

If memory addresses of objects (retrieved with the tonum() command, note that tonum() is deprecated and should no longer be used) are used in your modeling logic, they should be stored as doubles. You will have issues if you cast them into integers. int is a signed datatype, but memory addresses are unsigned. Therefore, if you try to call tonode() on a memory address that has been stored as an integer, it might be a negative number and return NULL instead of actually returning the node.

 

64-bit systems compound the problem. Integers are 32-bit and, on a 64-bit system, memory addresses are 64-bit. This means you may have a memory address that is larger than an int, and it will get truncated when you cast it into an int. When you try to reference the node with tonode(), some other location in memory will be referenced. Always store memory addresses as doubles!

 

Or, better yet, you should try to use the reference directly or cast it into a treenode as soon as possible. Avoid storing the memory address as a declared variable whenever possible. For example, if msgparam(3) returns a memory address: 

```
			treenode qtde = tonode(msgparam(3));	//convert to a treenode as soon as possible
			int qtde = msgparam(3);			//don't do this! always convert to treenode as soon as possible or, if necessary, store the memory address as a double
```

 

In summary: 

- Always convert a memory address to a treenode using tonode() as soon as possible.
- If you must pass memory addresses through numbers, use a double datatype, not an int.
- A double stores 64 bits and will properly reconvert back to a treenode when you call tonode() on its value.

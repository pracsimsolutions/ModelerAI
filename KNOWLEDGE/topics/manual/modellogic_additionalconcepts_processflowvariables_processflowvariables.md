---
id: modellogic_additionalconcepts_processflowvariables_processflowvariables
name: "Process Flow Variables"
kind: manual
breadcrumb: ["ModelLogic","AdditionalConcepts","ProcessFlowVariables","ProcessFlowVariables"]
tags: ["manual","modellogic","additionalconcepts","processflowvariables"]
source: "manual/ModelLogic/AdditionalConcepts/ProcessFlowVariables/ProcessFlowVariables.html"
---

# Process Flow Variables

## What Are Process Flow Variables?

 

In computer programming, a variable is a value that can change depending on different conditions. Process flow variables work the same way. You can create a variable on a process flow to give you better control over aspects of your simulation model that might change regularly under varying conditions.

  

#### Process Flow Instances

 

Process flow variables are most commonly used when you are running more than one instance of a process flow. For that reason, you might want to ensure you are familiar with the concepts discussed in the Process Flow Instances topic.

  

Process flow variables can be used:

 

- **As a centralized location for elements in your simulation model that you change frequently.** Similar to dashboards, process flow variables can provide a global repository where you can quickly update process flow settings that might need to change frequently from one simulation run to another. Many process flow templates use process flow variables for this purpose.
- **To help a model scale well when you've attached several objects to a process flow.** If you attach several different objects to a process flow (when you're running several different instances of a process flow), you can use process flow variables to quickly set variables that might change for each object in the process flow.

 

Since process flow variables are a somewhat abstract concept, it's helpful to illustrate a few different examples in which they can be useful.

 

### Using Process Flows as a Control Panel

 

Sometimes you might need to reference a certain setting in multiple properties on various activities in a process flow. In that case, it might be helpful to use process flow variables as a common reference point for all of these properties. A process flow variable gives you the ability to edit the value of a setting in one central location without having to update several properties at a time.

 

For example, imagine you wanted to use the AGV template process flow to control a simulation model that uses automatic guided vehicles (AGVs). Different brands or models of AGVs might have varying battery lifespans. The time they take to drop off or retrieve flow items might depend on factors that are unique to a specific model. For that reason, the AGV template provides three different process flow variables to easily control these settings for all AGVs that are connected to the process flow:

 

 

### Using Process Flow Variables With Multiple Instances

 

Imagine you were creating a simulation with multiple processors that all will run the same basic logic from the same object process flow. However, each processor needs to have a single operator that is dedicated to that processor and can't be shared with other processors in the model. And imagine that each processor needs its own unique process time:

 

 

You could add local process flow variables to the object process flow that will calculate the process time and designate the specific operator that should be assigned to each processor:

 

 

After attaching each processor to the object process flow, you could then set the default values for these variables in the processor's Quick Properties. For example, you could set the Welding Station to use a process time of 60 and to use the Welding Operator whenever a task executer is needed:

 

 

The Assembly Station would have different settings for the same process flow variables:

 

 

Lastly, you'll set up any relevant process flow activities to refer to the process time variable and any task sequences will be assigned to the operator variable. When the simulation model runs, each attached processor will run as a separate instance of the process flow. Each processor will reference the value of the process flow variables in its Quick Properties.

 

 

 

## Adding a Variable to a Process Flow

 

Process flow variables can be added to any type of process flow, including general process flows. To add a process flow variable:

 

1. Open a process flow and make sure nothing in the process flow is selected.
1. In Quick Properties, click the **More Properties** button.
1. In the **Variables** tab, click the **Add** button . A new variable will appear in the tab.
1. In the **Name** box, type a descriptive name for this variable. Try to choose a name that is intuitive and easy to understand in the context of the simulation model.
1. In the **Type** menu, select the variable's data type. For example, if the variable will reference an object in the 3D model, choose *object*. If the variable should return a number such as a process time, choose *number*.
1. By default the **User Accessible** checkbox is checked. Uncheck this box if you don't want people viewing the simulation model to edit the variables.
1. From the **Defined** menu, determine whether you want the variable to be global or local. Your selection on this menu will affect where and how you set the value of the variable. If the variable is global, you will set the variable to an absolute value on the process flow itself. If the variable is local, you will set the variable to a relative value that is defined on each object that is attached to the process flow.
1. In the **Default Value** box, type or select the default value of the variable if desired.
1. Press **OK** to save the changes and close the window.
1. If you need to change the variable's settings, click the **More Properties** button in the process flow's Quick Properties and make any necessary changes on the **Variables** tab.

 

When you're finished, your new variable will show up in the Process Flow Variables group in the general properties for the process flow in Quick Properties. It will generally be near the bottom of the Quick Properties pane (after the Labels group). For example, if you had just created a process flow variable called *MyProcessTime* with a default value of 15, it would look similar to the following image:

 

 

 

 

## Linking a Process Flow Activity to a Variable

 

After you've added a process flow variable to a process flow, you can link that variable to any property on a process flow activity. It will use the value of the process flow variable for that property during a simulation run. For example, you could set a Delay activity to use the value of a process flow variable to set its delay time.

 

To link a process flow activity to a variable:

 

1. Click the process flow activity to select it.
1. In Quick Properties, find the property that you want to link to a process flow variable.
1. Click the arrow next to the process flow to open a menu. Point to **ProcessFlow Variable** to open a list of the available variables. Select the appropriate variable.

 

When you're finished, the activity will display the process flow variable's name, as shown in the following example:

 

 

 

 

## Setting the Value of a Variable

 

Setting the value of the variable will depend on whether the variable is globally or locally defined. (See step 7 in Adding a Variable to a Process Flow for more information.)

 

### Setting Globally-Defined Variables

 

To change a globally-defined variable:

 

1. Open a process flow and make sure nothing in the process flow is selected.
1. In Quick Properties, under the **Process Flow Variables**, look for the specific variable you want to set.
1. Edit the variable so that it is set to whatever value you want it to have. The variable's data type will determine the kinds of values and related picklist options you can use.

 

### Setting Locally-Defined Variables

 

To change a locally-defined variable:

 

1. Open a process flow and make sure nothing in the process flow is selected.
1. In Quick Properties under the **Process Flow Instances** group, check to make sure the object(s) that will define the variable have been attached to the process flow. If not, use the **Sampler** button to add the object(s) to the process flow.
1. In the 3D model, click one of the attached objects to select it.
1. In Quick Properties, look for the **Process Flow Variables** group, which is generally the last group near the bottom of the pane.
1. Edit the process flow variable as needed.
1. Repeat the previous steps for each object that is attached to the process flow.

---
id: modellogic_processflowbasics_editingproperties_editingproperties
name: "Editing Activity Properties"
kind: manual
breadcrumb: ["ModelLogic","ProcessFlowBasics","EditingProperties","EditingProperties"]
tags: ["manual","modellogic","processflowbasics","editingproperties"]
source: "manual/ModelLogic/ProcessFlowBasics/EditingProperties/EditingProperties.html"
---

# Editing Activity Properties

## Introduction to Editing Activity Properties

 

Editing activity properties shares many things in common with editing 3D object properties. See Editing a Property for an explanation of some of the common components of a property.

 

Activity properties can be edited two different ways in process flow:

 

1. Directly on the activity in the process flow
1. In the Quick Properties pane

 

This topic will discuss how to edit an activity's properties using those two methods. It will also discuss how to edit properties that have the Universal Edit feature.

  

#### Information About Specific Properties

 

Each process flow activity has a reference page that describes its properties in greater detail. See Overview of Activities and Shared Assets for links to each activity's reference page.

  

 

 

## Editing Properties Directly in a Process Flow

 

To edit an activity's properties directly in a process flow:

 

1. Click the activity's icon, as labeled in the example in the following image:
1. Clicking the icon will open the activity's Quick Properties, as shown in the following example:
1. Make changes to the activity's properties as needed and then click outside of Quick Properties to save the changes.

  

#### Changing the Activity Name

 

You can change the name of the activity by double-clicking the name to select it and then typing a new name. See Name for more information about this property.

  

 

 

## Editing Properties in Quick Properties

 

To edit an activity's properties in the Quick Properties pane, simply click the activity to select it. When it is selected, a yellow box will appear around the activity and the properties will appear in the Quick Properties pane (on the left of the FlexSim window), as shown in the following example:

 

 

Be aware that if you have joined activities together into a stacked block, you will select all the activities at the same time when you click anywhere on the stacked block. You should also be aware that the Quick Properties pane will display the properties for all the activities in the stacked block. The properties will appear in the order they appear in the block, as shown in the example in the following image:

 

 

You can click the **Collapse** button in the top left corner of an activity's group of properties to hide those properties from view.

 

 

 

## About the Universal Edit Feature

 

Many of the activities in process flow have special properties that use the *Universal Edit* feature. The Universal Edit feature is designed to make it easy to add complex functionality to certain properties without needing to know FlexScript.

 

You can tell when a property has the Universal Edit feature because it will have a blue border around the property box, as shown in the following image:

 

 

Properties that use the Universal Edit feature can accept the following possible types of input:

 

- Keywords - Keywords can act as a user-friendly substitute for FlexScript commands. Using keywords, you can set a property can access data that could possibly change dynamically during a simulation run such as labels, groups, variables, and more.
- Static Data - Universal Edit boxes can accept fixed, static data that won't change during a simulation run. It includes standard data types such as numbers, strings, and references to specific objects in the simulation model.
- Commands - If you are comfortable with FlexScript commands, you can still use FlexScript commands in Universal Edit boxes. These boxes can also sometimes accept SQL commands.

 

Each of these types of inputs will be explained in more detail in the following sections.

  

#### Not All Properties Support All Input Types

 

Even though Universal Edit boxes can accept a wide variety of types of input, some properties may not be able to use certain types of inputs. If the property doesn't support a particular type of data, you'll get an error message from the system console when you try to use that type of input. You could possibly avoid this problem by clicking the arrow next to the Universal Edit box to open a menu. This menu will show possible options of input types that are valid for the given property.

  

### Keywords

 

Keywords can act as a user-friendly substitute for Flexscript commands. Using keywords, you can set a property can access data that could possibly change dynamically during a simulation run such as labels, groups, variables, and more.

 

When you begin typing a particular keyword, the FlexSim system will automatically generate a menu with possible options that could auto-complete the keyword phrase. For example, if you want a property to use the value from a label on a token, you can type the keyword `token`. After typing that keyword, the system will search for all the available labels that you've created in the process flow so far and list these labels in a menu, as shown in the example in the following image:

 

 

You can then select the appropriate label from the list or continue typing the label name manually. During the simulation run, the property will look for that label on the entering token. The property will then use the value listed for that label.

 

The following table lists the different keywords that Universal Edit boxes can recognize:

 

| Keyword | While Editing the Property | During a Simulation Run |
| --- | --- | --- |
| `**Variable:**` | Typing this keyword will create a menu listing all the process flow variables that have been created so far. Select the appropriate variable from the menu. | During a simulation run, this property can reference the value of a particular value. See Process Flow Variables for more information about how variables work. |
| `**SubFlow:**` | This keyword only works correctly with two process flow activities: the Run Sub Flow and Create Tokens activities. You would typically use this keyword with the Destination property to determine which sub flow will be triggered or which sub flow should receive the newly-created tokens. Typing this keyword will create a menu listing the names of any sub flows that you have created so far. Select the appropriate sub flow from the menu. NOTE: Most of the time you will use the sampler button to select the sub flow, rather than typing in a keyword. | During a simulation run, this property will trigger the sub flow listed in the Destination property. |
| `**ProcessFlow:**` | Similar to SubFlow, some activities can reference activities or shared assets in other Process Flow objects. The text takes the form `ProcessFlowName: ActivityName`. The referenced activity or shared asset must be in a general process flow or sub flow type. NOTE: This is the only keyword that does not display as bold. | During a simulation run, the property will reference the specific activity listed in the process flow. |

 

### Static Data

 

Universal Edit boxes can accept fixed, static data that won't change during a simulation run. It includes the following standard data types:

 

- **Numbers** In the property can accept a number value, you can type a number directly in the field. If the property requires a whole number, any decimal values will be removed automatically.
- **Strings** You can enter strings into a universal edit by placing the text in quotes. For example, `"Token Name"`.
- **Objects** You can reference a specific object in the simulation model or an activity in the process flow. To select an object, you can use the sampler button to select the object in the simulation model or process flow. If the type of object you select is not valid for this particular property, you will get an error message.

 

### Commands

 

If you are comfortable with FlexScript commands, you can still use FlexScript commands in Universal Edit boxes:

 

- **Code** If the Universal Edit property cannot resolve the entered text into any of the keywords or standard data types, the text will be assumed to be FlexScript code.
- Example - `duniform(1, 10)`

  **SQL** Some properties associated with resources, lists, and zones allow you to enter SQL queries. These SQL queries may be entered directly into the universal edit or they may be stored on a label or access through a Process Flow Variable.  

- Example - `WHERE label < 10`

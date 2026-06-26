---
id: reference_tools_modelparametertables_modelparametertables
name: "Model Parameter Tables"
kind: manual
breadcrumb: ["Reference","Tools","ModelParameterTables","ModelParameterTables"]
tags: ["manual","reference","tools","modelparametertables"]
source: "manual/Reference/Tools/ModelParameterTables/ModelParameterTables.html"
---

# Model Parameter Tables

## Overview and Key Concepts

 

Model Parameter Tables are used to define a model's parameters. A parameter is a global value that controls some aspect of the model's behavior.

 

 

Model Parameter Tables are accessed from the Toolbox. For more information on how to use parameters, see the Model Parameters topic.

 

 

 

## Model Parameter Table Properties

 

| Icon | Description |
| --- | --- |
|  | Duplicate the selected parameter(s). |
|  | Remove the selected parameter(s). |
|  | Move the selected parameter(s) up or down in the list. |

 

### Parameters Row Edit

 

You can use this edit field or the spinner to adjust the number of parameters in the table.

 

### Table Rows

 

Each row of the table allows you to edit the following attributes of each parameter: 

- Name
- Value
- Display Units
- Description

 The name of each parameter must be unique. In addition, we recommend using a name that doesn't begin with a number, and doesn't contain any spaces. This enables access using dynamic properties.

 

You can change the current value of the parameter by editing the Value cell directly. To edit other attributes of the value, you can use the Value Properties panel, discussed in the following section.

 

The Display Units column can contain arbitrary text. However, that text should be fairly short, and should indicate the units of the parameter. This may be used in future features, such as on the axis of a chart. For now, it is simply a hint to the model user about the units of the parameter.

 

The Description column can contain arbitrary text. This text is usually a short phrase that indicates what the parameter controls.

 

 

 

## Value Properties

 

When you select a cell in the second column of the Parameters table, a button will appear. Clicking that button will open the Value Properties for the selected value.

 

 

### Type

 

The Type property allows you to change the type of the value. For more information on the meaning of each type, see the Model Parameters topic.

 

### Lower Bound

 

Visible for Continuous, Integer and Discrete types. The value cannot be less than the specified lower bound.

 

### Upper Bound

 

Visible for Continuous, Integer, and Discrete types. The value cannot exceed the specified upper bound.

 

### Step Size

 

Visible for Discrete types. The value can only be increased or decreased by the step size.

 

### Options

 

 

Visible for Option types. Option type values require you to enumerate the set of possible values for this value. Each possiblity is called an option. Each option has a name and a value. The value can be a number, text, or a FlexScript expression. The name is arbitrary.

 

### Sequence Length

 

Visible only for Sequence types. The value in this case is an array of numbers, exactly as long as the sequence length.

 

### Reference

 

Specifies an optional node associated with this value. The reference is passed in to the On Set trigger. If the parameter is a Pass-through parameter, then the reference specifies the node that stores the value for this parameter.

 

### On Set

 

Specifies code that fires when the value of the parameter is changed, or if the model is reset. The reference, new value, old value, and reset flag are all passed in to this trigger. The reset flag is only true if the model is currently being reset. Note that the On Set trigger fires before the Model Reset trigger. Pass-through parameters do not fire the On Set trigger.

 

### Apply to All

 

Visible only if multiple values are selected. Applies any changes to all selected values, rather than just the first selected value.

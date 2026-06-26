---
id: reference_tools_processflowvariables_processflowvariables
name: "Process Flow Variables"
kind: manual
breadcrumb: ["Reference","Tools","ProcessFlowVariables","ProcessFlowVariables"]
tags: ["manual","reference","tools","processflowvariables"]
source: "manual/Reference/Tools/ProcessFlowVariables/ProcessFlowVariables.html"
---

# Process Flow Variables

## Overview and Key Concepts

 

Process Flow objects can have variables defined on them that can be referenced from any activity through the Universal Edit fields or through the `getprocessflowvar()` command.

 

### Why Use Process Flow Variables?

 

Process Flow Variables allow you to present certain inputs in the Process Flow in an easy-to-use front-end. Let's say there is a certain input that may change across different instances, or perhaps across different scenarios that you want to test. Instead of requiring you or another person to dive into the internal workings of the Process Flow, you can place it in a Process Flow variable, then turn off Edit Mode so that when users want to change it, they can change it through an easy front-end interface.

 

 

 

## Properties

 

The following image shows properties for Process Flow Variables:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

You can set the name of the variable here.

 

### Type

 

This specifies the return value of this variable, which can be Any, Number, Object, Node, or String.

 

### User Accessible

 

If checked, this variable will be visible and editable to anyone. If unchecked, this variable will remain internal to this Process Flow.

 

### Defined

 

This defines the instancing behavior for the Process Flow Variable. If set to **Global**, there will be one variable for every instance of this Process Flow. Since there is only ever one instance of a General Process Flow, this option is always set to **Global** for General Process Flows. If set to **Per Instance**, each instance will have its own version of this variable.

 

### Default Value

 

This is where you define the code to calculate this variable.

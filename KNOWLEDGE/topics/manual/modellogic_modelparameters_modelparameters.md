---
id: modellogic_modelparameters_modelparameters
name: "Model Parameters"
kind: manual
breadcrumb: ["ModelLogic","ModelParameters","ModelParameters"]
tags: ["manual","modellogic","modelparameters","modelparameters"]
source: "manual/ModelLogic/ModelParameters/ModelParameters.html"
---

# Model Parameters

## What are Model Parameters?

 

Model parameters are input variables for your model. You can use model parameters to make some part of your model easily adjustable. For example, you may want to be able to vary the number of workers and the location of equipment easily. You can build your model so that those aspects of the model are controlled by a parameter. When you change the parameter's value, you will see that change reflected in your model.

 

Common model parameters include: 

- The number of workers present
- The time required for a certain part of a process
- The sequence in which jobs should be completed
- Whether or not certain equipment is active
- The arrival rate of customers

 

 

 

## Why Should I Make Model Parameters?

 

There are many good reasons to make model parameters. Several of the best reasons are explained in the following subsections.

 

### Model Parameters are Easy to Change

 

Making a model usually requires a lot of iteration. Requirements change, understanding improves, and new needs are discovered. It can be hard to predict if something will change in the future, and whether that change can be represented as a parameter. However, it is common for a model to require process times that are currently unknown. If you make that process time a parameter, then when better data becomes available, you can easily update your model to use the new data.

 

### Model Parameters are Strict

 

You can configure model parameters to only accept certain values. For example, you can set an upper and lower bound. If anyone tries to set the value of a parameter to something invalid, the parameter will instead be set to something valid. This can help you prevent mistakes. It can also help you communicate the intent of your parameters.

 

### Model Parameters are Global

 

Model parameters are global, meaning you can define them in a single place and use them everywhere in your model. For example, if 100 machines all use the same process time, you can make a parameter for that process time, and have all of those machines reference that parameter. Then, if you need to change the process time, you can change the value of the parameter, and all of those machines will use the new value.

 

### Model Parameters are Exposed

 

Model parameters allow you to expose values. This can help you and anyone who uses your model. It is usually much easier to change the value of a parameter than it is to find all the places in a model that need to change if a parameter changes. Even if the parameter is only used in a single place, it can still be much easier to find the parameter than remember where the parameter is used.

 

### The Experimenter Depends on Model Parameters

 

FlexSim provides an Experimenter tool. This tool works by setting the model parameters to different values to understand their impact on the model's performance.

 

 

 

## Model Parameter Tables

 

All model parameters are defined in Model Parameter Tables. By default, new models have a single parameter table called Parameters. You can find it in the toolbox:

 

 

You can also add additional parameter tables from the Add button in the toolbox.

 

Each row of the table represents a single parameter. To add or remove parameter, you just add or remove rows from this table. To edit a parameter, you can edit one of the four columns on that row.

 

The Name column allows you to define the name of the parameter. All parameters in a model must have a unique name, even if there are multiple parameter tables. In addition, we recommend that you not begin a name with a number, or use spaces in the name.

 

The Value column allows you to define the current value of the parameter. In addition, the Value column allows you to define the type of the parameter, and any additional constraints on the parameter value. To edit the type, you can click on the cell you want to edit, and click the button that appears:

 

 

The Display Units and Description columns can have arbitrary text. They allow you to provide hints about the parameter to whoever uses the model.

 

 

 

## Parameter Value Types

 

The value of a parameter has a type. The following subsections describe each type in detail.

 

### Continuous

 

The Continuous type requires a lower bound and an upper bound. The value can be set to any number in the inclusive range defined by the lower and upper bounds.

 

This type is well suited for percentage-based parameters. For example, you may have a parameter rate that controls the percentage of jobs that need to be reworked, or the percent of customers that fall within a certain category.

 

### Integer

 

The Integer type requires a lower bound and an upper bound. The value can be set to any integer value in the inclusive range defined by the lower and upper bounds.

 

This type is well suited for values that can only be integers, such as the number of workers in a model, or the number of tools available.

 

### Discrete

 

The Discrete type requires a lower bound, an upper bound, and a step size. The value can be set to any value that is *n* steps from the lower bound, where *n* is a non-negative integer. The value cannot be greater than the upper bound.

 

This type is well suited for values that increment in steps other than 1, or that begin incrementing from a non-integer value.

 

### Binary

 

The Binary type does not require any additional information. Its only possible values are 0 or 1. This type is well suited for values that are on or off, true or false. For example, you could use this type of parameter to turn areas of your model on or off.

 

### Option

 

The Option type allows you to specify one or more expressions as possible values for this parameter. You can then set the value to an integer between one and the number of options you specify.

 

The Option type is useful when choosing between alternatives, where the alternatives aren't ordered. For example, you may use this option to choose between two floor layouts. The first layout isn't "less" than the second layout; they are simply different options.

 

This distinction is important especially for the Optimizer. Usually, the Optimizer attempts to correlate increases or decreases in parameter values with increases or decreases in performance measures. The Optimizer won't do that with Option parameters.

 

### Sequence

 

The Sequence type requires a sequence length, which should be an integer that is greater than zero. The value can only be set to an array of numbers. That array must contain all the values from 1 to *n*, where *n* is the sequence length. In addition, each value can only be contained once, so that the array is *n* long.

 

The Sequence type is well suited to ordering a sequence of jobs. This type of value is designed for use with the optimizer, where you may be interested in the optimal sequence of jobs to avoid changeover times.

 

### Expression

 

The Expression type can be set to any valid FlexScript expression. This type is well suited for distributions, such as a customer arrival rate, or a processing time.

  

#### Expression parameters are ignored by the Optimizer

 

Expression types are so open-ended that the Optimizer is unable to propose meaningful possibilities for this type. If you need the Optimizer to choose from a set of expressions, use the Option type.

  

### Pass-through

 

The Pass-through parameter allows any node in the model to become a parameter. When you set the value of this type of parameter, it sets the value of the specified node. When you get the value of this type of parameter, it gets the value from the specified node. A common use case for this kind of parameter is to reference a cell in a Global Table.

  

#### Pass-through parameters are ignored by the Optimizer

 

Like Expression parameters, Pass-through parameters are ignored by the optimizer. Since the value of a node is unrestricted, the value of a Pass-through parameter is also unrestricted. This means that the Optimizer is unable to propose meaningful possibilities for this type.

  

### Custom

 

A Custom parameter can bet set to any of the following kinds of values:

 

- A number
- A string
- A null value
- An Array
- A Map (with strings as key values)

 

A custom **cannot** be set to the following kinds of values:

 

- A node value
- A Map or Array containing a node value
- A Map with non-string keys

  

#### The Parameter Table uses JSON to Handle Custom Parameters

 

A custom parameter's value can be any Variant that meets the above rules. However, the parameter table view uses json to display and edit the value of a custom parameter. To set a custom parameter's value in the parameter table view, enter any valid JSON text in the cell. The value will be set to whatever the result of JSON.parse() would be. The value is displayed in the table by using JSON.stringify(). JSON is the easiest way to display and specify Array or Map values.

  

In addition, a Custom parameter allows you to define the On Constraint trigger, which fires when the value is about to be set. You can use this trigger to validate the new value. If the value is correct, the trigger should return the new value. If the new value is not correct, this trigger should attempt to repair the value, or provide a valid default if a repair is not possible. Whatever value this trigger returns will be used as the new value for the parameter.

  

#### Custom parameters are ignored by the Optimizer

 

Like Expression and Pass-through parameters, Custom parameters are ignored by the optimizer.

  

 

 

## Using Model Parameters

 

There are two standard ways to use model parameters. The first is to access the value through FlexScript. The second is to use the OnSet trigger of the parameter values.

 

### Accessing Parameters through FlexScript

 

If you have a parameter named "MyParameter", you can access the value like this:

 

```
Model.parameters.MyParameter
```

 

You can also use this alternate form:

 

```
Model.parameters["My Parameter"].evaluate()
```

 

The above two statements are identical in behavior. However, for Option and Expression type parameters, you can use the `evaluate()` method to pass up to three parameters to your expressions. The parameters you pass in are available in the expression as `param(1)`, `param(2)`, and `param(3)`.

 

A common use case is to define an Expression type parameter with a distribution as a value, such as:

 

```
uniform(10, 20, param(1))
```

 

You can then pass the needed parameter using `evaluate()`:

 

```
Model.parameters["MyParameter"].evaluate(2)
```

 

### Using the OnSet Trigger

 

Each parameter value has an OnSet trigger. This trigger fires whenever the value of the parameter is changed. It is also fired when you reset the model using the Reset button.

 

When the OnSet trigger is fired, you have access to four values: 

- The value of the Reference field
- The new value of the parameter
- The old value of the parameter
- A flag, indicating if this OnSet is fired because the model is being reset.

  

#### Pass-through Parameters Ignore the OnSet Trigger

 

Pass-through parameters don't fire the OnSet trigger when the value changes.

  

The sampler button on the Reference field can be used to set the reference and pickoption simultaneously. For example, if you sample a Processor, point to Variables, then choose maxcontent, the Reference is set to the Processor, and the OnSet will set the maxcontent variable on reset.

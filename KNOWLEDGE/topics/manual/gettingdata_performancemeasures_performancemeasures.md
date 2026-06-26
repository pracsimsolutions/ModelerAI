---
id: gettingdata_performancemeasures_performancemeasures
name: "Performance Measures"
kind: manual
breadcrumb: ["GettingData","PerformanceMeasures","PerformanceMeasures"]
tags: ["manual","gettingdata","performancemeasures","performancemeasures"]
source: "manual/GettingData/PerformanceMeasures/PerformanceMeasures.html"
---

# Performance Measures

## What are Performance Measures?

 

Performance measures are output values for a model. Common performance measures include: 

- Total throughput
- Average utilization
- Average wait time
- Maximum queue length

  

#### Number Outputs vs Table Outputs

 

Each performance measure reports a solitary value. To report an entire table as an output of the model, you can use tools like the Statistics Collector or the Calculated Table.

  

Performance measures are the opposite of a Model Parameter. As the user, you determine the value of a parameter, and parameters influence model behavior. Performance measures, on the other hand, only observe model behavior, and do not influence it. As the user, you don't set the value directly. Rather, you'll set an expression that determines the value based on the current state of the model.

  

#### Experiments and Optimizations

 

Performance measures are automatically included in any experiment or optimization.

  

 

 

## Performance Measure Tables

 

All performance measures are defined in a Performance Measure Table. By default, new models have a single performance measure table called PerformanceMeasures. You can find it in the toolbox:

 

 

You can also add additional performance measure tables from the Add button in the toolbox.

 

Each row of the table represents a single performance measure. To add or remove performance measure, you just add or remove rows from this table. You can edit the Name, Display Units, and Description of the performance measure directly in the table.

 

Like Model Parameters, the name of all performance measures should be unique, even if they are defined in separate tables.

 

The Value of a performance measure is defined by a function. Whenever you get the value of a performance measure, that function is evaluated. To edit that function, you can click on a cell in the Value column, and click the button that appears:

 

 

The Value field of the performance measure value allows you to define an expression. The expression is evaluated whenever the value of the performance measure is required, such as at the end of a replication, or whenever the performance measure table paints.

 

The Reference field of the performance measure value allows you to point to a node in the model. That node is passed in when the Value field is evaluated.

 

You can use the Sampler button to to sample an object. You can also use it to sample an object and a statistic related to that object. For example, if you sample a processor, point to Statistics, and choose Output, the Reference field will be set to the Processor, and the Value field will be configured to get the output statistic from the reference. You can sample: 

- Most 3D Objects
- Most Activities and Shared Assets
- Groups (in the toolbox)
- Charts

  

#### Performance Measures Shouldn't Reference Themselves

 

The expression you provide as the value of your performance measure should not try to include the performance measure itself, or its previous values. If you need a previous value to calculate a performance measure, you should use a Tracked Variable to store current and previous values, and use a performance measure to display the value of that Tracked Variable.

   

#### Performance Measures and Run Speed

 

In some cases, it can be computationally expensive to calculate a performance measure. If closing the Performance Measure Table's window improves run speed, consider keeping it closed by default, and opening it at the end of a model run.

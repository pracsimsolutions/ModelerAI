---
id: gettingdata_advanceddatagathering_keyconceptsexperiments_keyconceptsexperiments
name: "Key Concepts About Experiments"
kind: manual
breadcrumb: ["GettingData","AdvancedDataGathering","KeyConceptsExperiments","KeyConceptsExperiments"]
tags: ["manual","gettingdata","advanceddatagathering","keyconceptsexperiments"]
source: "manual/GettingData/AdvancedDataGathering/KeyConceptsExperiments/KeyConceptsExperiments.html"
---

# Key Concepts About Experiments

## About the Experimenter

 

The Experimenter is a tool that automates running a model with different inputs and collecting the results. The experimenter can help you answer the following kinds of questions:

 

- How sensitive are the Performance Measures to random variation?
- What are the tradeoffs between the Parameter values and the Performance Measure values?
- What are the Parameter values that produce the best results?
- Does the model behave correctly with different combinations of Parameter values?

 

These questions can only be answered by running the model many times, with different input values each time, and then gathering and comparing the results. The Experimenter is designed to make this process simple and easy, and less tedious. In addition, the Experimenter can run multiple configurations of the model simultaneously, which saves significant time.

 

 

 

## Key Terms

 

This section defines and describes the key terms associated with the Experimenter. This section provides a brief introduction, with more in-depth discussion in the following sections.

 

When you use the Experimenter, you'll follow this process:

 

1. Configure a **Job** - A Job defines a strategy for generating **Scenarios**, and usually requires multiple **Replications** of each Scenario. Running a Replication of a Scenario is considered a **Task**.
1. Run a Job - When you run a Job, the Job creates a list of **Tasks** to run. The Experimenter will run the model once for each Task and store the result in the **Results Database File**.
1. Explore the results - After running a job, you can see the results produced by each Scenario, and also compare Scenarios.

 

### Jobs

 

The Experimenter allows you to define one or more Jobs. A Job defines a strategy for generating **Scenarios**, as well as for determining how many **Replications** of each Scenario to run.

 

There are three types of Job:

 

- Experiment
- Range-Based
- Optimization

 

Experiment jobs allow you to define Scenarios directly, and specify how many replications of each Scenario you want to run. Range-Based jobs allow you to specify a range of values for one or more parameters. This job type will generate one scenario for every value in the range for every parameter. Optimization jobs allow you to specify an objective function (e.g. maximize throughput). This type of job generates scenarios automatically, searching for scenarios that optimize the objective function.

  

#### Optimization Jobs Require a License

 

The Optimization Job uses a third-party software program named OptQuest to search for optimal Scenarios, and requires an additional license. This license can be purchased from FlexSim, but isn't usually included with FlexSim. To purchase an OptQuest license or to ask about pricing, contact FlexSim Customer Support.

  

### Scenarios

 

A Scenario is a set of input values that can be applied to a model. It includes the following information:

 

- All Model Parameter values
- The model Stop Time
- The model Warmup Time

 

You can think of a Scenario as a tuple or vector composed of these values:

 

| P1 | P2 | ... | PN | Stop Time | Warmup Time |
| --- | --- | --- | --- | --- | --- |
| 3 | 7 | ... | 5.3 | 10000 | 500 |

 

As the Experimenter runs, it applies a Scenario to a model by setting the Model Parameter values, Stop Time, and Warmup Time. Then it runs the model and records the results. While the Stop Time and Warmup Time are both part of the Scenario, you'll likely be focusing on the effect different Model Parameter values have on the performance of your model. For more information on configuring Model Parameters, see the Model Parameters topic.

 

Each Scenario can be identified by the values it contains. The Experimenter will generate a unique identifier called a hash for each Scenario. The hash is made up of numbers and letters. If two scenarios compute the same hash, they are considered identical. In many places on the interface, you may see the last 8 characters of the hash.

 

For convenience, you can also specify a name for a Scenario. The name can be arbitrary, and is only used for display purposes. It is not included in the Scenario's hash.

 

### Replications

 

FlexSim models use random numbers, generated from probability distributions, to introduce variability into the simulation. That variability influences the behavior of the system; for example, a process time might take longer than usual, causing a temporary bottleneck. Including randomness makes a model far more realistic and trustworthy. However, it also means that the results generated by the model depend on the random numbers used. In other words, model results are also variable.

 

Because model results are variable, it is important to run Replications of each Scenario. Some Scenarios will be more sensitive to random variability than others. The only way to understand the distribution of results for a given Scenario is to run Replications.

 

The random numbers produced by probability functions depend on the Replication number. For Replication 1, FlexSim will generate certain set of random values. For Replication 2, FlexSim will generate a different set of random values, and so on.

 

### Tasks

 

Each Job generates a list of Tasks. Each Task is made up of a Scenario and a Replication number. The Experimenter will run the model once for each Task, and record the results. For example, a Job that defines three Scenarios and requests 10 Replications of each Scenario will generate 30 tasks.

 

Some larger and more detailed models can take minutes or even hours to complete a single Task. Fortunately, The Experimenter can run Tasks simultaneously. By default, the number of tasks that run simultaneously will match the number of CPU cores on the machine you use. If you have network or web access to a machine with additional CPUs, you can configure the Experimenter to use the CPUs on the remote machine to run models. For more information, see Running Jobs on the Cloud.

  

#### Tasks are Run in the Background

 

When you run an experiment, FlexSim will run Tasks in the background and record their results. It won't close the instance of FlexSim you are using to run the Experimenter.

  

### Results Database File

 

The Experimenter collects results for each Task that it runs. Those results include:

 

- All Performance Measures values
- All data for Dashboard charts
- All tables from Statistics Collectors and Calculated Tables
- All messages in the System Console and Output Console

 

Usually, the most important results are the Performance Measure values. To learn more about configuring helpful Performance Measures, see the Performance Measures topic.

 

The results are stored in the Results Database File. This is an extra file that the Experimenter creates when you run the first Job. By default, it will be in the same directory as the model file, and it will have the same name. The results are stored in the file at each Task completes, so the file will grow as a Job runs. The Results Database File persists between Job runs, and all Jobs share the same file.

 

The Results Database File provides several advantages:

 

- Before running any Task, the Experimenter checks if results for that Task are in the file already. If so, the Experimenter won't run that Task again, which avoids unnecessary computation.
- You can run additional Replications of any Scenario, or append a new Scenario to the results at any time.
- Jobs can quickly generate copious amounts of data. Storing that data in a separate file lowers RAM requirements, and won't affect your original model's file size.

  

#### Never Mix Results from Different Models

 

It is possible to run a Job, change the model, and then run additional Jobs. This will incorrectly combine data from two different models, and present results from both. This is statistically invalid.

 

To avoid this, rename your model using Auto Increment Save when you make changes. If you are using the default file name for your Results Database File, then the file name will update with your model's name. This will preserve the previous model file as well as the previous results file generated by that model.

 

You can also delete the result file before running a new Job, if you are unsure.

  

 

 

## Best Practices

 

The following sections explain a few best practices to keep in mind when designing experiments:

 

### Make Sure Your Model Was Built With Good Data Inputs

 

You need to ensure that you put good data *into* your model (inputs) in order to get good data out of your model (outputs). Make sure that your simulation model relies on accurate data and random number distributions that accurately represent your business system's requirements. (See Data Gathering Strategies for more information.)

 

### Validate Your Model to Reduce Errors

 

Try running your simulation model at least one time from start to finish before you run an experiment. Once your simulation model can be run without any errors, you are ready to use the Experimenter.

 

### Know Your Computer's Memory Limits

 

The more experiments or optimizations you run, the more memory it will require from your computer. The Experimenter and Optimizer will use all your cores unless you limit the number of cores on the Advanced tab. You might begin to experience problems if you are using lots of cores. Each core requires as much memory as a single model run. For example, using 8 cores means that it would require 8 times as much memory as a normal model run.

 

For that reason, make sure you are familiar with your computer's capabilities. Design your experiments that your computer can handle without causing an overload or taking longer than you wanted it to take. Running more experiments and optimizations can increase your confidence in the results you are getting, but you need to weigh the need for good data against your computer's capabilities.

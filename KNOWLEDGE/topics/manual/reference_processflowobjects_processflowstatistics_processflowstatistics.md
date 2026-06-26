---
id: reference_processflowobjects_processflowstatistics_processflowstatistics
name: "Process Flow Statistics"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","ProcessFlowStatistics","ProcessFlowStatistics"]
tags: ["manual","reference","processflowobjects","processflowstatistics"]
source: "manual/Reference/ProcessFlowObjects/ProcessFlowStatistics/ProcessFlowStatistics.html"
---

# Process Flow Statistics

## Introduction to Process Flow Statistics

 

The following topic is intended to be a reference page for all of the different possible statistics that are available in process flows. If you are unclear about what kind of information is collected by a particular statistic, you can reference it in this topic. It will also provide information about statistical values, different uses for statistics, and commands related to statistics.

 

 

 

## Activity Statistics

 

All activities in process flow keep the following two statistics:

 

- **Input** - The number of tokens that have entered an activity
- **Output** - The number of tokens that have exited an activity

 

In addition, any activity that might cause a token to wait keeps the following two statistics:

 

- **Content** - The number of tokens currently in an activity
- **Staytime** - The amount of time each token spends in the activity

 

The Batch activity keeps many additional statistics. You can read about these statistics on the Batch Statistics page.

 

 

 

## Shared Asset Statistics

 

Shared assets (the Resource, List, and Zone objects) keep a minimum of 8 statistics, as described below:

 

- **Input** - The number of tokens that have begun using the asset
- **Output** - The number of tokens that have finished using the asset
- **Content** - The number of tokens currently using the asset
- **Staytime** - The amount of time each token spends using the asset
- **RequestInput** - The number of tokens that requested to use the asset
- **RequestOutput** - The number of tokens that requested to use the asset, and have since either used the asset or failed
- **RequestContent** - The number of tokens waiting to use the asset
- **RequestStaytime** - The amount of time a token spends waiting to use the asset

  

#### Using Shared Assets

 

What it means to "use the asset" might vary depending on the type of shared asset it is.

  

You can find more details about these statistics, along with information about any additional statistics, on each Shared Asset's reference page.

 

 

 

## About Statistics Values and Types

 

Each statistic provides access to four values:

 

- **Current** - The current value of the statistic
- **Minimum** - The lowest recorded value of the statistic
- **Maximum** - The highest recorded value of the statistic
- **Average** - The average value of the statistic

 

In addition, each statistic can be classified as one of three types:

 

- **Incremental** - These statistics always increase. Any statistic with *Input* or *Output* in its name is an incremental statistic. Because incremental statistics only increase, the minimum, maximum, and average values are meaningless. The current value can also be called the *total* value.
- **Level** - These statistics increase and decrease over time, like the level of fluid in a tank. Any statistic with *Content* in its name is a level statistic. Level statistics provide meaningful current, minimum, maximum, and average values; the average is time-weighted.
- **Stream** - These statistics represent values obtained from individual objects. Staytime, for example, is updated whenever a token leaves an activity. The current staytime is simply the last token's staytime, and so is not usually helpful. Stream statistics do provide helpful minimum, maximum, and time-weighted average values.

 

 

 

## Events

 

For information on events, see the Event Listening page.

 

Process Flow activities have the following events:

 

### On Entry

 

On Entry fires when a token arrives at the activity.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Instance | Object | The Process flow instance the token belongs to |
| Activity | treenode | The activity the token arrived at |
| Token | Token | The token that is entering the activity |

 

### On Exit

 

On Exit fires when a token leaves the activity.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Instance | Object | The Process flow instance the token belongs to |
| Activity | treenode | The activity the token is leaving |
| Token | Token | The token that is leaving the activity |

 

### On Manual Release

 

On Manual Release fires when a token is forced to leave an activity early usually by using the Token class's release method or a Release Token activity.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Instance | Object | The Process flow instance the token belongs to |
| Activity | treenode | The activity the token is leaving |
| Token | Token | The token that is leaving the activity |
| Destination | Variant | The connection the token will be leaving out of |

 

### On Content Change

 

This event fires anytime the activity's content changes. This occurs when a token enters or exits the activity. Note that not all activities have this event.

 

This event has no parameters.

 

### On Input Change

 

This event fires when a token enters the activity.

 

This event has no parameters.

 

### On Output Change

 

This event fires when a token exits the activity.

 

This event has no parameters.

 

### On Staytime Change

 

This event fires when a token exits the activity, immediately after the On Output Change event. Note that not all activities have this event.

 

This event has no parameters.

 

 

 

## Statistics as Events

 

In process flow, you can listen to statistics in addition to listening to events. These events are called Value Change Events, and are fired whenever the value of the statistic changes.

 

### Commands

 

You can use the `getstat` command to get data from an activity or shared asset in FlexScript. This command requires an object, a statistic name, and a value flag. The following example shows how to use this command:

 

```
// This is the code header given by most process flow objects.
// The comments are added here for clarification.
treenode current = param(1); // This is the instance, meaning the object that owns the 
Process Flow
treenode activity = param(2); // This is the activity or shared asset
treenode token = param(3);
treenode processFlow = ownerobject(activity);

// Here is where you would put your code. The following code gets the average content
double avgContent = getstat(
					// getstat requires between 4 and 6 parameters:
	activity,       // 1. the object (shared asset or activity) that has the statistic
	"Content",      // 2. the name of the statistic
	STAT_AVERAGE,   // 3. the desired value from the statistic (could also be STAT_CURRENT, STAT_MIN, or STAT_MAX)
					// 4. an optional parameter the instance object (usually current)
					// 5. an additional parameter to help resolve which data to get
					// 6. an additional parameter to help resolve which data to get
);
```

 

For example, to get the current content of a Delay activity, you could use the following command:

 

`getstat(delayActivity, "Content", STAT_CURRENT)`

 

To get the output of any Process Flow activity, you could use the following:

 

`getstat(activity, "Input", STAT_CURRENT)`

 

All statistics accessible through the `getstat` command use Tracked Variables, so you can access the current, minimum, maximum, and average values for any of the statistics. You specify which value you want using the third parameter, the flag. To get the average staytime of an activity, for example, you could use the following:

 

`getstat(activity, "Staytime", STAT_AVERAGE)`

 

Notice that the statistic name is very important. The name must match the name of a statistic kept by the object. Source activities, for example, do not keep a "Content" statistic; the `getstat` command will return 0 in this case.

 

 

 

## Statistics for Instanced Process Flows

 

If you want to get statistics for an activity in a Task Executor or Fixed Resource Process Flow, then you have to give a fourth parameter to the `getstat` command. This fourth parameter is the name of the instance object. For example, if Processor3 is attached to a Process Flow, and you want to get a statistic from that flow, then you would need something like the following:

 

`getstat(activity, "Output", STAT_CURRENT, "Processor3")`

 

For general process flows, or for process flows with only one instance, this parameter is ignored.

 

### List Statistics

 

The List activity keeps statistics for the list as a whole:

 

- **Input** - the total number of entries that have been pushed on the list
- **Output** - the total number of entries that have been pulled from the list
- **Content** - the total number of entries currently on the list
- **Staytime** - the staytime for entries on the list
- **BackOrderInput** - the total number of back orders that have been created by the list
- **BackOrderOutput** - the total number of back orders that have been fulfilled
- **BackOrderContent** - the current number of back orders on the list
- **BackOrderStaytime** - the amount of time each back order stays on the list

 

These same statistics are kept for each partition on the list. Simply prefix the word "Partition" to the statistic name.

 

For example, these two lines of code get the number of entries on a list, and on a specific partition:

 

```
		getstat(globallist("list"), "Content", STAT_CURRENT);
		getstat(globallist("list"), "PartitionContent", STAT_CURRENT, 0, 1);
```

 

The fourth parameter, the 0, is the instance object parameter, ingored for the general process flow case. The fifth is the partition ID. The value returned is the statistic for that specific partition.

 

Please note that when a partition is empty, the list destroys it by default. This also clears the statistics for that partition. You can avoid this by checking the "Keep Dead Partitions" box in the List Properties window. Be aware that for large numbers of partitions, using this option can use a lot of memory.

 

### Zone Statistics

 

The Zone keeps the statics kept by the other shared assets. However, it also keeps those statistics for each subset. For example, if you have a subset within a zone called "SmallItems", you can get the content of that subset as follows:

 

`getstat(zone, "SubsetContent", STAT_CURRENT, 0, "SmallItems")`

 

Again, the fourth parameter is the instance parameter, which is ignored in general Process Flows. The fifth is the name of the subset for which you want the statistic.

 

You can also get the data of a subset calculation. Simply provide the name of the desired calculation as the sixth parameter:

 

`getstat(zone, "CalculationInput", STAT_CURRENT, 0, "SmallItems", "TotalWeight")`

---
id: bestpractices_beforebuilding_datagathering_datagathering
name: "Data Gathering Strategies"
kind: manual
breadcrumb: ["BestPractices","BeforeBuilding","DataGathering","DataGathering"]
tags: ["manual","bestpractices","beforebuilding","datagathering"]
source: "manual/BestPractices/BeforeBuilding/DataGathering/DataGathering.html"
---

# Data Gathering Strategies

## Putting Good Data Into Your Model

 

You need to ensure that you put good data *into* your model (inputs) in order to get good data out of your model (outputs). For example, you'll need to know things like how long it takes to perform a particular process or how frequently customers arrive, etc.

 

 

In order to input that kind of data into FlexSim, you'll have to observe your business system or use other data-gathering methods to get kind of information FlexSim needs to create an accurate model of your business system. This topic will discuss various methods for gathering useful data.

  

#### What If the Inputs Vary Over Time?

 

If your business system is like most, there are aspects of your business that seem random or that fluctuate over time. For example, you might know that customers arrive at heavier rates during certain peak hours than they do during slower hours. FlexSim can fortunately simulate this kind of variance. Aspects of your business that might appear to be purely random at first might actually follow statistically predictable patterns if you observe it closely.

 

After you've observed your business system using the data gathering methods described in the following sections, you can input that data into FlexSim and it will suggest an appropriate statistical distribution to model this kind of variance over time.

  

 

 

## Use Historical Data

 

You might already have all the useful data you need right at your fingertips. If your facility uses automated tracking for customers, work orders, etc., you could possibly pull that data from the computers that track it. You could then use that data to determine an appropriate statistical distribution for a particular process or set of processes. Talk to your facility's IT managers about pulling statistical data from these computers for a specific period of time. Remember that you'll want to gather enough data to be representative of what is normal for your business system.

  

#### Simulating the Exact Historical Record

 

You don't necessarily need to translate the historical data into a statistical distribution; FlexSim is capable of simulating the exact historical record if needed. However, using the exact historical record in your simulation system has a few advantages and disadvantages.

 

One advantage is that historical re-runs can better replicate exactly what happened in your actual business system, which is great if you are only building a current-state simulation. However, historical re-runs don't always scale well when you need to create a future-state simulation. For example, you won't have any historical data on the 10 new products that you plan to introduce next year. (See Current State vs. Future State Models for more information.)

 

Another disadvantage is that you won't be able to explore the inherent randomness of a system. You also have to re-import the historical data every time you want to simulate different data for a different historical time period.

  

 

 

## Conduct a Time Study

 

A time study involves direct and continuous observation of a particular task or process to determine how much time the process takes. The observer often uses a timekeeping device (such as a stopwatch or video camera) to record the time taken to accomplish a task. The observer will observe the task multiple times over a long period of time, recording the amount of time each process takes every time. There are many free guides on the Internet for conducting time studies if you would like to conduct a time study yourself. There are also consulting firms that are willing to conduct time study projects if needed.

 

 

 

## Observe Your Business First Hand

 

Don't just ask yourself: is this what is happening in my business system today? Make sure you actually go and see first-hand for yourself. While you're there observing your business system first hand, try walking along the actual pathways of the customer, material, and/or information flow yourself. Start with a quick walkthrough of the entire door-to-door business system to get an overall sense of the flow of materials and information. Ultimately the goal is to make real observations in real time talking to real people.

 

 

 

## Interview Employees

 

You could interview all your staff members and get a rough estimate about how long particular processes take and use that data to get an approximate wait and processing times. Although employee interviews are subjective by nature, this kind of information would at least be useful for making an educated guess.

 

 

 

## Make an Educated Estimate

 

There are actually some decent ways to make an educated guess within a 90% confidence range. Consider reading Douglas Hubbard's *How to Measure Anything: Finding the Value of "Intangibles" in Business*, in its 3rd edition at the time of this writing. This book discusses useful methods for estimating measurements that are potentially too costly or difficult to observe directly.

 

 

 

## Conduct a Sensitivity Analysis

 

Each of the data-gathering methods described in the previous sections will involve some investment of time and money. Some methods are more resource-intensive than others. For example, time studies are the most costly, whereas interviewing employees or making an educated guess are not very costly at all. Before engaging in a costly time study, you should ensure that the information you would gain from this study would be valuable enough to justify the expense. You don't want to waste time and money on data that isn't going to matter to your simulation project in the end.

 

One effective way to justify this expense is to conduct a *sensitivity analysis*. To conduct sensitivity analysis, begin building your simulation model with the least costly data-gathering methods (such as making an educated guess). Then, once you've built your first prototype of your simulation model, try testing your model inputs (such as customer arrival rates or processing times, etc.) by changing them and monitoring their impact on your key metrics. After performing this analysis, you'll know which model inputs have the strongest impact on the key metrics. You can then determine which model inputs are most valuable to your simulation project. In other words, you will know which model inputs might justify a more expensive data-gathering technique to get higher quality data.

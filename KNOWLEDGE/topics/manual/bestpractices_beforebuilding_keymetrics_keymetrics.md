---
id: bestpractices_beforebuilding_keymetrics_keymetrics
name: "Determine Your Key Metrics"
kind: manual
breadcrumb: ["BestPractices","BeforeBuilding","KeyMetrics","KeyMetrics"]
tags: ["manual","bestpractices","beforebuilding","keymetrics"]
source: "manual/BestPractices/BeforeBuilding/KeyMetrics/KeyMetrics.html"
---

# Determine Your Key Metrics

## Why Do I Need to Think About Key Metrics?

 

If your primary goal in creating a simulation model is to solve a problem, you're going to need to identify the key metrics that your business should use to determine whether it is running as effectively as it possibly can. FlexSim can give you almost any kind of statistic about your business system that you are interested in. FlexSim also gives you the ability to run experiments, seeing how various changes to your business system can impact the key metrics you are most interested in.

 

For example, perhaps you want to determine whether leasing a $400,000 machine will justify the expense by sufficiently reducing costs on labor, maintenance, and/or raw materials. Which statistics would you want to look at in your simulation model? In that case, you'd want your simulation model to give you the minimum, maximum, and average statistics for:

 

- The production level of the old machine as opposed to the new machine
- The reduction in wasted raw material per unit on the old machine as opposed to the new machine
- The amount of time employees spend per unit operating the old machine versus the new machine
- The amount of time the old machine breaks down compared to the new machine

 

You would then use these ranges of statistics to calculate the costs of the raw material, labor, and breakdowns per unit. The difference in costs between the old machine and the new machine would give you a range of possible savings per unit. You would then calculate the annual savings by adding the savings together and multiplying it by the annual production level. These statistics would help you determine whether those savings were high enough to justify the $400,000 expense.

 

FlexSim empowers you to solve these kind of problems by giving you access to this information. However, it's ultimately up to you to decide which information has the most value to you. With that in mind, there are a few different ways you can possibly determine which metrics are the most important to your business system, as discussed in the following sections.

 

 

 

## Determine What Decision You Want to Support

 

You probably started using FlexSim because you need to make some important decisions. And we all know we make better decisions when we have better data. In his best-selling book *How to Measure Anything*, Douglas Hubbard recommends asking these questions to determine what you want to measure:

 

1. What is the decision this measurement is supposed to support?
1. What is the definition of the thing being measured in terms of observable consequences?
1. How, exactly, does this thing matter to the decision being asked?
1. How much do you know about it now (i.e., what is your current level of uncertainty)?
1. What is the value of additional information?

 

Thinking seriously about these questions might help you to get more clear and specific about what metrics will most help you make better decisions for your business system.

 

 

 

## Use the Clarification Chain

 

If you're still having difficulty identifying what you should measure. Consider using what Douglas Hubbard calls "the clarification chain." The goal of the clarification chain is to move from vague and intangible goals to specific and tangible things that can be measured. Many measurements start by deconstructing an uncertain variable into its constituent parts and then identifying directly observable things that are easier to measure.

 

Hubbard writes: "First, we recognize that if X is something that we care about, then X, by definition, must be detectable in some way. How could we care about things like *quality, risk, security,* or *public image* if these things were totally undetectable, in any way, directly or indirectly? If we have reason to care about some unknown quantity, it is because we think it corresponds to desirable or undesirable results in some way. Second, if this thing is detectable, then it must be detectable in some amount. If you can observe a thing at all, you can observe more of it or less of it. Once we accept that much, the final step is perhaps the easiest. If we can observe it in some amount, then it must be measurable."

 

As you work through the clarification chain, you should also try to get clear about *why* you want to measure something in order to understand what is really being measured. The purpose of the measurement is often the key to identifying what the key metric should be.

 

 

 

## Consider Building First and Identifying Metrics Later

 

The statistician David Moore, the 1998 president of the American Statistical Association, once said: "If you don't know what to measure, measure anyway. You'll learn what to measure." It's helpful to think of measurement as an iterative process. At this stage of the simulation project, don't worry about identifying too many concrete metrics yet. Build your model and begin measuring what you think will be most valuable. Keep in mind that you can always adjust the method based on these initial findings.

 

 

 

## For Deeper Reading

 

Consider reading Douglas Hubbard's *How to Measure Anything: Finding the Value of "Intangibles" in Business*, in its 3rd edition at the time of this writing. Not only does this book provide useful guides for determining which metrics are the most valuable, but it is also a good introduction into the core principles behind simulation modeling.

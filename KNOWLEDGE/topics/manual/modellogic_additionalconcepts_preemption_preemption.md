---
id: modellogic_additionalconcepts_preemption_preemption
name: "Preemption"
kind: manual
breadcrumb: ["ModelLogic","AdditionalConcepts","Preemption","Preemption"]
tags: ["manual","modellogic","additionalconcepts","preemption"]
source: "manual/ModelLogic/AdditionalConcepts/Preemption/Preemption.html"
---

# Preemption

## What is Preemption?

 

In Process Flow, *preemption* is when you stop a token's current activity and redirect it to a sequence of activities that take precedence over other activities. After the token completes the preempting activity sequence, it can possibly resume its previous activities.

 

Preemption has several possible applications, such as:

 

- **Machine Availability or Scheduled Maintenance** - It's possible that not every section of your business system is available 24 hours a day, 7 days a week. Some sectors of the business might close down at regularly scheduled times. For example, some sets of machines might only operate during regular business hours. You could use preemption to simulate these scheduled closures. At closing time, the process flow could trigger the preempting sequence of activities. Then when the machines reopen the next day, they could resume their work from the previous day.
- **Routine or Periodic Tasks** - It's possible that your employees have to perform tasks at regularly scheduled periods of time. For example, perhaps your employees have to attend a weekly staff meeting or take a break every four hours. It's also possible that if you have employees who work in shifts, you might have a certain set of tasks that employees need to do during each shift change. You can use preemption to begin the activities that need to be performed at periodic intervals, then have the employees resume their previous tasks after they finish the preempting activities.
- **High Priority Situations** - Perhaps your business system receives an expedited order or an urgent event occurs that needs immediate attention. You can use preemption to divert business resources to handle the situation. When the situation has been resolved, the resources that were diverted can resume normal operations.
- **Machine Breakdowns** - If you wanted to simulate the occasional breakdown of machines in your business system, you could use a random statistical distribution to trigger the preempting activities that would need to occur during a breakdown. After the breakdown has been resolved, the machine can resume normal business operations.

  

#### Machine Breakdown Logic

 

You can use the standard MTBF/MTTR objects in the toolbox for generating the events that simulate machine breakdowns. Then you could set up your Process Flow to either listen for events on the MTBF/MTTR or for the onStop and onResume events on objects. See MTBF/MTTR for more information.

  

 

 

## Process Flow Activities Use in Preemption

 

Process Flow preemption involves three key actions. Each corresponds to a Process Flow activity, which you will likely use in this exact order when building a preemption process flow:

 

1. **Save Token Context** - Before preempting, you need to save the token's current context in the process flow so that it can possibly return to this context after it finishes the preempting activities.
1. **Release Token** - This action aborts the token's current activity and releases it to a new activity, either to do something else, or to just wait.
1. **Restore Token Context**: After completing the operation that required the preemption, the token can be restored back to its saved context.

 

Each of these activities has a Token(s) property that defines the target tokens, meaning the tokens that will be preempted. When you build your preempting process flow, the Token(s) property on each of these three activities need to refer to the same tokens. The recommended method is to use an Assign Labels activity to create a common label that can be referenced by the Token(s) property on each preemption activity. The following sections about How Preemption Works and Connecting Preempting Process Flows will explain this in more detail.

 

 

 

## How Preemption Works

 

Typically in any process flow that uses preemption, you would have these three elements:

 

- **Core System Process Flow** - This process flow represents how your business system operates under normal working conditions.
- **Preempting Event Process Flow** - This process flow triggers the preempting sequence of activities. The event can be triggered at random, by an event in the simulation model, or it can happen at regular intervals using any of the Token Creation (Source) activities. The Preempting Event Process Flow will run parallel to the Core System Process Flow.
- **Preempted Token Processing** - You need at least one activity that temporarily stores the tokens that have been preempted until they can be restored to the Core System Process Flow.

 

The following image shows an example of a very simple preemption system with each of these three key elements:

 

 

### How the Core System Works

 

The Core System works like any normal process flow. In the specific example illustrated in the preceding image:

 

1. An **Inter-Arrival Source** creates a token every 20 seconds.
1. A **Delay** activity (Delay 1) then delays the token for 20 seconds, representing the processing time for the token.
1. A **Sink** activity removes all tokens that have been successfully processed in Delay 1.

 

### How the Preempting Event Works

 

The preempting process will control when the preempting event occurs. It will also include any activities that are required as part of the preemption process. In the specific example illustrated in the preceding image:

 

1. An **Inter-Arrival Source** creates a token randomly using a statistical distribution within a range of 90-100 seconds. In other words, a preempting event will occur randomly every 90-100 seconds.
1. An **Assign Labels** activity assigns all incoming tokens a label called *preemptedTokens* that refers to all tokens in the **Delay 1** activity. See Connecting Preempting Process Flows for more information.
1. A **Save Token Context** activity saves the context of all tokens referenced by the *preemptedTokens* label. In this case, it will save the context of any tokens that are in the Delay 1 activity. The activity creates a label on the token called *savePoint* which will store the token's context. See Key Concepts About Save Points for more information.
1. A **Release Token** activity sends any tokens referenced by the *preemptedTokens* label to the Preemption Token Processing. In this case, it means that any tokens in the Delay 1 activity will be sent to the Delay 3 activity.
1. A **Delay** activity delays entering tokens for 40 seconds. This represents the amount of time it will take for the preempting process to finish. You could possibly add additional activities (such as task sequences) instead to control the logic of the simulation when the preempting event occurs.
1. A **Restore Context** activity restores the preempted tokens in Delay 3 to the exact state at which they were saved in the Delay 1 activity by the Save Token Context activity (in step 3). It uses the savePoint label to restore the context.
1. A **Sink** activity removes the tokens that triggered the preempting event.

 

### How the Preempted Token Processing Works

 

The Preempted Token Processing simply stores or holds the tokens that have been preempted from the Core System. In the specific example illustrated in the preceding image:

 

1. During a simulation run, when a token enters the **Release Token** activity in the Preempting Event process flow, the token in the Core System process flow will disappear from the **Delay 1** activity.
1. That same token will appear in the **Delay 3** activity in the Preempted Token Processing process flow.
1. The token will remain in the **Delay 3** activity until a token enters the **Restore Context** activity in the Preempting Event process flow.
1. The token will then return to the **Delay 1** activity in the exact state it was in before. For example, if it had been delayed for 10 seconds (50% of the delay time), the delay would resume at 10 seconds.

 

Be aware that you could possibly make your Preempted Token Processing process flow more elaborate by adding additional activities to it.

 

### The Preemption System in Action

 

The following image shows this example preemption system in action during a simulation run:

 

  

#### Preventing New Tokens from Entering

 

Notice that in the preceding example, new tokens can still enter the Core System process flow even while tokens are preempted away from the target activity. If you want to prevent new tokens from entering the target activity during the preempting event, consider using a Zone shared asset to restrict access or a Decide activity that checks for certain conditions before diverting tokens to an activity.

  

 

 

## Key Concepts About Save Points

 

The Save Token Context and Restore Token Context activities both use a label called *savePoint* on the target tokens:

 

- The **Save Token Context** activity creates the *savePoint* label on the preempted token and saves which activity it was in when it was saved. If the activity is time-sensitive (such as a Delay or any activity that uses a Max Waiting timer or Max Idle timer), it will also save how long the token was in that activity.
- The **Restore Token Context** activity references the *savePoint* label on the preempted token and restores it to the activity that it was in when the label was created. It will also restore it to the specific time it was in the activity if it was a time-sensitive activity. For example, if a token had been in a Delay activity for 10 seconds when its context was saved, it will resume the Delay at 10 seconds when it is restored.

  

#### The *savePoint* Label Does Not Save Token Relationships

 

The *savePoint* label does not contain any information about the relationship between preempted tokens. For example, if 5 tokens were all preempted at the same time, the *savePoint* label does not record that these tokens are connected in any way. It doesn't save them to a special group or collection or list of tokens that were preempted together. The *savePoint* label only contains information about each individual token itself.

  

 

 

## Connecting Preempting Process Flows

 

The Assign Labels method described in this section is the recommended method for connecting preempting process flows to the core process flow. This section will also briefly mention alternative methods.

 

### Assign Labels (recommended)

 

The recommended method is to use an Assign Labels activity to create a common label that can be referenced by the Token(s) property on each preemption activity:

 

1. Add an **Assign Labels** activity before the preempting activities in the Preempting Event process flow.
1. With the **Assign Labels** activity selected, in Quick Properties, click the **Add** button to add a new label.
1. Give the new label a name in the **Name** box, such as *preemptedTokens*.
1. Click the arrow next to the **Value** box to open a menu. Point to **Token** then select **Tokens in an Activity** to open a picklist, as shown in the following image:
1. Use the **Sampler** button to select the target activity in the Core System process flow, meaning the activity from which tokens will be preempted away from the Core System.
1. In the Quick Properties for the **Save Token Context** activity, you'll need to point to the new label you just created. Click the arrow next to the **Token(s)** box to open a menu. Point to **Token Label** and select the name of the label you added in step 2. The following image gives an example:
1. Repeat steps 4-6 for the **Token(s)** property on the **Release Token** and the **Restore Context** activities.

 

You could modify this method to preempt all the tokens in a shared asset, such as a Zone, List, or Resource. This modification is ideal if you have an entire section of activities and you want to save all tokens anywhere in that Zone, List, or Resource. To preempt tokens in a shared asset:

 

- For step 4, select **Tokens in a Shared Asset** from the menu instead. This will open a similar picklist, as shown in the following image:
- For step 5, use the **Sampler** button to select the target shared asset, meaning the Zone, List, or Resource from which tokens will be preempted away. After that point, you can follow the rest of the steps as listed.

 

See the next section for an additional method for connecting activities.

 

### Alternative Methods

 

Although the Assign Labels method is the recommended method for connecting preempting process flows, you could possibly connect the preempting activities directly to the specific activity or shared asset from which you want to preempt tokens by sampling it:

 

1. In the Quick Properties for the **Save Token Context** activity, click the arrow next to the **Token(s)** box to open a menu. Point to **Tokens in Activity** or **Tokens in Shared Asset** to open a picklist.
1. Use the **Sampler** button to select the target activity or shared asset in the Core System process flow.
1. Repeat steps 1 and 2 for the **Release Token** activity.
1. You could possibly repeat steps 1 and 2 for the **Restore Token Context** activity or you could sample one of the activity or shared assets that is being used in your Preempted Token Processing process flow.

 

The following image shows an example of the properties for the preempting activities using this method:

 

 

One of the reasons this method is not recommended is that you have to be very sure that the target tokens are the same for all activities, or else you could get error messages when you try to restore the tokens to their previous context. These error messages can be caused when new tokens enter the Core System process flow while other tokens are currently preempted. For example, if your Restore Token Context activity points directly to a Zone and it tries to restore all the tokens in that Zone, any new tokens that have entered the Zone will create an error (because they don't have a *savePoint* label).

 

That being said, you might have valid reasons for using this method. If you do, just keep the following guidelines in mind:

 

- Make sure you clearly understand what the *savePoint* label does and does not do. In particular, this label does not store any information regarding what groups of tokens were preempted at the same time. See Key Concepts About Save Points for more information.
- Make sure that the set of tokens that you restore is the exact same as the set of tokens that were saved and released.

 

 

 

## Using Save Token Context Activities as Checkpoints

 

It's possible that when you restore a token to the Core System, you don't want it to resume in the exact place where it was preempted. Instead, you might want it to repeat a series of activities after certain checkpoints (or milestones). You could use Save Token Context activities to create these checkpoints.

 

The following image shows an example of a preemption system that uses checkpoints:

 

 

This system is similar to the system illustrated in the How Preemption Works example with a few key differences:

 

- The Core System process flow uses a **Zone** shared asset (named *Welding Zone* here), as well as **Enter Zone** and **Exit Zone** activities.
- The Core System process flow has three **Saved Token Context** activities, each one followed by a **Delay** activity (which could represent different stages at the Welding Station). The **Saved Token Context** activity will save the context for all entering tokens.
- The **Assign Labels** activity in the Preempting Event process flow refers to the tokens in the Welding Zone.

 

The following image shows this example checkpoint preemption system in action during a simulation run (the Zone is highlighted to illustrate the token relationships):

 

 

 

 

## Using Preempting Activities with Time Tables

 

Preempting activities can be very useful when dealing with Time Tables and MTBF/MTTR objects. These tools allow you to put an object or set of objects into a down state for maintenance, repairs or operational/non-operational times. By listening to events in these tools and performing the appropriate logic, you can cause your process flow to pause what its doing and even start an additional process during the down time.

 

 

Process Flow preemption involves at least two tokens, but may include more. One or more *core tokens* will perform the core activities of the process. Additionally another token, the *preemption token*, will trigger and manage the preemption. The preemption token waits for, or is created by, some event that triggers preemption. When this event happens, the preemption token saves the context of all the core tokens. This saved context includes the core token's current activity, as well as any activity-related data. For example, if a core token is in a delay activity, it will save off the total delay time as well as how far into the delay the token is currently. The context is saved on a user-defined label on the core token (not on the preemption token).

 

Once the preemption token has saved the context on all the core tokens, it releases those tokens to a different activity. If the preemption is meant to just freeze the process, core tokens would be released to an activity that does nothing, such as an 0 delay activity with no successor activities. Core tokens will just sit in that activity until the preemption token restores them back. This is the implementation shown in the above image, where the preemption token releases the token to a *Preemption Silo*. On the other hand, if the preemption needs to do an "ordered take down", core tokens could be released to an activity where they perform the ordered take down, such as releasing resources, exiting zones, etc.

 

The following image shows the preempted core token in the *Preemption Silo*.

 

 

Once the preempting operation is completed, core tokens can be restored back to their saved contexts. They will continue from the exact point where they were saved.

---
id: reference_processflowobjects_basic_batch_batch
name: "Batch"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","Basic","Batch","Batch"]
tags: ["manual","reference","processflowobjects","basic"]
source: "manual/Reference/ProcessFlowObjects/Basic/Batch/Batch.html"
---

# Batch

## Overview and Key Concepts

 

In the Process Flow module, the Batch activity collects incoming tokens and sorts them into groups of tokens (batches). When a batch is ready, the Batch activity will release it to the next activity.

 

The Batch activity basically functions similarly to Combiners and Separators in the 3D model in that it is capable of both combining and separating tokens.

 

 

Using the Batch activity for simple grouping and releasing is relatively straightforward. However, the Batch activity has some additional features that allow you to collect, sort, and release batches in fairly complex ways. If you want to learn more about what the Batch activity is really capable of, consider reading the following topics:

 

The rest of this topic is a reference page for the Batch activity and its properties.

 

 

 

## Organizing Batches

 

The Batch activity can collect entering tokens and organize them into batches based on the criteria you specify. The following properties on the Batch activity are directly involved in how batches organize tokens:

 

 

- **Batch Quantity** - This property sets the target value for the batch. The batch will continue collecting tokens until this value is reached, after which the batch will be released.
- **Quantifier** - This property sets the criteria that will determine how the batch quantity is calculated.
- **Group By** - This property sets the criteria by which entering tokens will be organized and grouped into batches.
- **Overflow** - This property determines what will happen if an entering token will cause the batch to exceed its Batch Quantity.

 

With that in mind, the Quantifier has two different methods for collecting and organizing tokens:

 

- Batch by number of tokens - If you select this option (which is the default), the Batch Quantity will be based upon the number of tokens that arrive at the Batch activity. Each entering token will increase the current quantity of the batch by one. When the number of tokens reaches the value specified in the Batch Quantity property, the batch will be released.
- Batch by a label on the tokens - If you select this option, the Batch Quantity will be based upon the value of a label on the entering tokens. In this case, a token can increase the batched quantity by any numeric value stored on the label.

 

Each of these methods will be explained in the following sections.

 

### Batch by the Number of Tokens

 

The simplest (and default) method of batching is to collect by the number of tokens. This means that entering tokens each take up one space in their batch with respect to the Batch Quantity. When the number of tokens reaches the value specified in the Batch Quantity property, the batch will be released.

 

To set the Batch activity to collect by number of tokens:

 

1. Click the **Batch** activity to select it.
1. In Quick Properties, check that the **Quantifier** box says *Number of Tokens*.
1. Click in the **Batch Quantity** box and type the number of tokens that should be collected in each batch.

 

### Batch by a Label on the Tokens

 

If you select this option, the Batch Quantity will be based upon the value of a label on the entering tokens. In this case, a token can increase the batched quantity by any numeric value listed in the label. For example, if the Quantifier is set to `token.Weight` and the entering token has a label named *Weight* with a quantity of 200, the token will increase the current batch's quantity by 200 when it is added to the batch.

 

To set the Batch to collect by label values:

 

1. Make sure an Assign Labels activity or some other activity earlier in the process flow will create the label that will be used for batching.
1. Click on the **Batch** activity in your process flow to select it.
1. In Quick Properties, click the arrow next to the **Quantifier** box to open a menu. Point to **Token Label**, which will open up a submenu listing all the labels that currently exist in the process flow. Select the desired label from the menu by double-clicking it or using the arrow and Enter keys.
1. The **Quantifier** box will now show the command `token.YourLabelName`. For example, if the label is named *Weight*, the box would read: `token.Weight`. Alternatively, you could type `token.Weight` into the box. You can also enter a string into this box using quotation marks: `"Weight"`.
1. Click in the **Batch Quantity** box and type the target batch quantity. For example, if you wanted the Batch activity to collect tokens until it reaches a collective weight of 300 grams, you would type `300` here.

 

Now each time a token enters the batch, the value of the specified label on each token will be added to the current batch quantity. When the batch quantity reaches the specified limit, the batch will be released. For example, imagine a Batch activity is using a label named *Weight* to collect tokens and it has a specified Batch Quantity of 300, meaning it needs to collect a total weight of 300 in order to get released. If the first token that enters the Batch activity has a *Weight* label with a value of 200 and the second token's *Weight* label has a value of 100, the batch would get released after the second token is added to the batch. That's because the total sum of the *Weight* label for both tokens would be 300, which was the target weight for each batch.

  

#### What if the Label Value Exceeds the Batch Quantity?

 

When batching by a label, it is possible that the batched quantity may be greater than the specified Batch Quantity, which will cause the batch to overflow. See Batch Overflow Options in the next section for more information about how to control overflows.

  

 

 

## Batch Overflow Options

 

When batching by a label, it is possible that the batched quantity may be greater than the specified Batch Quantity, which will cause the batch to overflow. When that happens, it will trigger the Batch activity's overflow logic.

  

#### Batching by Number of Tokens Won't Cause Overflows

 

If you are batch by number of tokens, your batch will never exceed the specified Batch Quantity and therefore won't trigger the overflow logic.

  

To illustrate what happens when a batch is going to overflow, let's return to the same example that was used in the previous section. As a reminder, in this example, the Batch activity is set to release the batch when the total sum of the *Weight* label is 300. If the first token's *Weight* label has a value of 200 and the second token's *Weight* label has a value of 150, the total sum of the *Weight* label for both tokens would be 350. 350 would exceed the target weight for that batch and it would therefore trigger the Batch's overflow logic.

 

When overflow occurs the Batch activity's behavior is governed by the Overflow property. This property has a menu with the following options:

 

- **Do Not Divide, Release Excess** - The batch is released without modification. If this option is selected, the batch would be released including its excess quantity. For example, if the Batch Quantity is set to 300 and an entering token would cause the batch to overflow by 50, the batch would be released with a quantity of 350. No token labels will be modified by the batch when using this option. Use the Label Aggregation method for modifying labels.
- **Do Not Divide, Release Partial** - Rather than release an overflowed batch, the Batch activity will release a batch that is only partially full. The entering token that would have caused the batch to overflow is put in the next batch. This logic can be illustrated using the same example in which the Batch Quantity is set to 300 and the current batch has a quantity of 200. An entering token has a value of 150, which would cause the batch to overflow by 50. When this happens, the current batch would be released with a quantity of 200 and the entering token would be placed in a new batch with a quantity of 150. No token labels will be modified by the batch when using this option. Use the Label Aggregation method for modifying labels.
- **Hold Excess Quantity** - The current batch will be released with the exact required quantity. A new token is then created and placed in a new batch with a label containing the excess quantity. All labels and label values are copied from the last token (the one that caused the overflow) to the new token. For example, if the Batch Quantity is set to 300 and an entering token would cause the batch to overflow by 50, the batch would be released with a quantity of 300. Then, a new batch would be started with a new, nearly identical token containing a label with a quantity of 50. In this option, the batch will modify the quantifier label on tokens to move excess quantity into additional batches. Tokens that were already waiting in the first batch will not have any of their labels modified. Use the Label Aggregation method for modifying labels.
- **Destroy Excess Quantity** - The label value of the last token is reduced until the batch contains the exact amount specified by the Batch Quantity property. This essentially destroys the overflowed quantity. In this option, the batch will modify the quantifier label on the last token only. No other labels will be modified. Use the Label Aggregation method for modifying labels.

  

#### New Batches Created from Excess Quantities

 

If you choose the Do Not Divide, Release Partial or Hold Excess Quantity options, you have the possibility of creating new batches with the entering token. Be aware that if the next batch created from the entering token meets the batch quantity, it will be immediately released. If the new batch exceeds the batch quantity, it will trigger the overflow logic again. This will continue until the quantity is less than the Batch Quantity.

  

The overflow logic only affects the label value that is specified by the Quantifier property. Other labels on the tokens will not be affected. Label Aggregation can also be affected if of a token is held over to be released in a new batch. See Assigning Labels to Outgoing Batches for more information.

 

 

 

## Grouping

 

When a new token arrives at the Batch activity, it will evaluate the Group By property and place the token into the matching batch. If no matching batch is found, a new batch will be created. By default, Group By is set to *None* and only one batch will be created at a time. But you can use the Group By property if you want to sort your tokens based upon criteria you specify. For example, you could sort tokens into batches based on the token's label values, names, or the object to which the token is linked. Generally this will be a label on the entering token, but it may reference a global table or other value.

 

The Group By can be numbers, strings, objects, or arrays, as explained in the following table:

 

| Type | Description | Example |
| --- | --- | --- |
| Number | The tokens can be grouped into batches based on a number, such as a numerical value in a label on the token. The number can be an integer or floating point number and it may be negative or positive. |  |
| Text | The tokens can be grouped into batches based on a string (which is another word for text). For example, you could have an Assign Labels activity earlier in the process flow that assigns a new label to each token called *Batch* and it could assign the strings "First Batch" or "Second Batch" to a certain percentage of the tokens. |  |
| Object | The tokens can be grouped into batches based on an object. For example, the tokens might have a label called *ProcessBy* that references a processor in the 3D model that needs to process that particular batch of tokens (Processor1, Processor2, or Processor3). |  |
| Array | The tokens can be grouped into batches based on an array, which may contain multiple labels of various data types. Tokens being grouped by an array will only group together if all elements in the array match. For example, if items have a *Size* label of either 8 or 12 oz (number), and a *Color* label of either “Red” or “Blue” (string), you can group by the array [token.Size, token.Color]. This will create four groups, one for each possible outcome, as shown in this example. |  |

 

Batches displayed on the Batch activity will be displayed oldest to newest from top to bottom and from right to left.

 

 

 

## Releasing More than One Token and Destroying Tokens

 

The Batch activity has a variety of different options you can use to control how batches are released. The following properties on the Batch activity are used when a batch is released:

 

 

The Batch activity allows any number of tokens to be released when a batch is finished. This can be useful to simulate a process that collects materials, combines them together, and then divides the combined materials equally into a certain number of portions. For example, a Batch activity could be set to collect 5 tokens and release 3 tokens, almost as though it had mixed the original tokens together and divided them up into different portions.

 

You can use the # Tokens to Release property to change how many tokens get released. This number can be the same number of tokens collected in the batch, greater than the number of tokens collected, or less than the number of tokens collected. It's important to understand what is happening inside the batch for each of these three scenarios.

  

#### Release Order

 

There are a number of references below to the order in which tokens are released from the batch. Though all of the tokens are released at the same model time (in 0 model time), the order in which they are released may impact the flow of your process flow. For instance, the first token released from the batch will enter an Acquire activity first and may acquire a resource before any other tokens from the same batch arrive.

  

### If # Tokens to Release is Equal to the # of Collected Tokens

 

This is the simplest of the three scenarios. If three tokens are collected in the batch and the # Tokens to Release is set to three, then all three of the original tokens added to the batch will be released in the same order in which they arrived. This can be useful as a method of syncing a set of tokens.

 

If the Quantifier is set to a label value, the number of tokens in each batch may vary. In order to release all of the tokens that were collected in the batch, you can select the option *Number of Tokens Batched* or return any number <= 0 in the # Tokens to Release box.

 

### If # Tokens to Release is Greater than the # of Collected Tokens

 

If the number specified in the # Tokens to Release box is greater than the number of tokens collected in the batch, the Batch activity will create and release new tokens. These new tokens will have no initial labels. Labels may be created on these new tokens by using the Label Aggregation. See Assigning Labels to Outgoing Batches for more information. These new tokens will be the last to leave the Batch activity.

 

### If # Tokens to Release is Less than the # of Collected Tokens

 

If the # Tokens to Release is less than the number of tokens collected in the batch, tokens will be destroyed until the correct number of tokens remains. In deciding which tokens to destroy and which to preserve, the Batch activity prioritizes tokens using the following rules, in order:

 

1. Tokens that have children in the same batch should be preserved over their children. (See Sub Process Flows for more information about tokens with parent-child relationships.)
1. Tokens with more children should be preserved over tokens with fewer children.
1. Tokens with more allocated shared assets should be preserved over tokens with fewer allocated shared assets.

 

Once the Batch activity has prioritized the tokens, it will destroy tokens with the lowest resulting priority.

 

Be aware that when a Batch activity destroys tokens, it functions in a way that is similar to Finish activity:

 

- If the Batch activity destroys a token that has a parent that is in a Run Sub Flow activity, the parent activity will be notified that the child finished.
- Any shared assets that have been allocated by the token will be deallocated.
- If the token has children, the token will not be destroyed until all its children are destroyed (it will remain in the activity but will not be part of any subsequent batch).

 

Extra tokens will be destroyed before any tokens are released from the Batch activity.

 

 

 

## Assigning Labels to Outgoing Batches

 

By default, tokens that enter and leave the Batch activity will retain their labels and no new labels will be added to them. Newly created tokens will have no labels assigned to them. However, using the Label Aggregation, the Batch activity can assign new labels or aggregate the labels of the tokens that were included in the batch to all of the outgoing tokens. Label aggregation does not occur until the batch has collected all of its tokens as has reached the Batch Quantity value.

 

One possible use of the label aggregation would be to calculate the total weight of all of the parts that are combined into a single part. If each token represented a single part and they each had a label named *Weight*, the label aggregation can be used to sum all of the weights of every token added to the batch and create a new label like *BatchWeight* that will be assigned to all of the outgoing tokens.

 

 

To assign labels to outgoing batches:

 

1. Click the **Batch** activity to select it.
1. In Quick Properties, under the **Label Aggregation** group, click the **Add** button to add a new label. A new row will appear in the table.
1. In the **From Label** column, double-click on the cell with the text that says *labelName*. Type the name of the label on the tokens in the batch that will be used as the source of the data. For example, if the incoming tokens all have a label named *Weight*, you would type that label name here.
1. In the **To Label** column, double-click on the cell with the text that says *labelName*. Type the new name of the label that will be assigned to outgoing tokens. For example, the new name could be *BatchWeight*.
1. In the **Aggregation** column, you'll select how the value of the new label will be calculated or assigned. Click on the cell with the text that says *Batch ID* to open a menu. Select the appropriate option from the menu. For example, if you wanted to calculate the total sum of all the batched token's *Weight* labels, you could select **SUM** from this menu.

 

For your reference:

 

**From Label** - The name of a label on the batched tokens that will be used as a source of data. The value of this label from each token will be used to calculate the aggregated value. If the label does not exist on a token, the returned value will be 0. NOTE: This entry is not used for the *Batch ID* and *Token Count* aggregation types.

 

**To Label** - The name of the label that the new value will be written to. Each token in the outgoing batch will get a label with this name, and with the assigned value.

 

**Aggregation** - The value that will be assigned to the To Label on all tokens released for the batch. The possible values are as follows:

 

- **Batch ID** - Starting at 1, each batch that is released from the Batch activity will increment the Batch ID by 1. The Batch ID is only guaranteed to be unique among batches produced by a given Batch activity.
- **Token Count** - The number of tokens collected in the batch. If the Quantifier property is set to *Number of Tokens*, this value will be equal to the Batch Quantity value.
- **Last Value** - This will get the label value of the From Label on the last token to enter the batch.
- **First Value** - This will get the label value of the From Label on the first token to enter the batch.
- **Make Array** - This will get the label value of every token collected in the batch and create an array of those values. If the label does not exist on a token, a NULL value will be set on the array.
- **SUM ** - Sums the From Label values on all tokens in the batch.
- **AVG ** - Gets the average of the From Label values on all tokens in the batch.
- **MIN ** - Gets the minimum From Label value on all tokens in the batch.
- **MAX ** - Gets the maximum From Label value on all tokens in the batch.
- **STD ** - Gets the standard deviation of the From Label values on all tokens in the batch.
- **VAR ** - Gets the variance of the From Label values on all tokens in the batch.
- **COUNT ** - Gets the number of tokens in the batch that have a label named From Label.
- **INDEX** - Assigns the index of the token released from the batch activity per batch.

  

#### Batch Modifying Labels

 

In general, token labels are not modified by the Batch. There are some exceptions that can occur based upon the overflow type. See Batch Overflow Options for specific information.

   

#### Aggregating the Quantifier Label when using Overflow

 

If you are batching by a label on the tokens and your Overflow is set to *Hold Excess Quantity* or *Destroy Excess Quantity* then the quantifier label may be modified prior to performing the aggregation. For example, if you collect by the label *Weight* and if the Batch Quantity is set to 300 and the final batched quantity is 350, the SUM of the *Weight* label will be equal to 300, not 350.

  

 

 

## Releasing a Batch Early

 

The Batch activity has both a Max Wait Time and Max Idle Time property. These properties allow you to define start criteria and the amount of time to wait before firing a trigger. If the batch reaches one of these times and calls either the OnWaitTimerFired or OnIdleTimerFired trigger, you have the option to release the batch early. However, this is not a requirement. If the batch is not released, the batch will continue to collect tokens until it reaches its Batch Quantity, after which the batch will be released as normal.

 

The Max Wait Time and Max Idle Time properties give you a reference to the batch. The `releasebatch()` command can be called to release a batch prematurely from within the OnWaitTimerFired or OnIdleTimerFired triggers or from anywhere else in the model. If failed is 1, the batch will immediately release all tokens in the batch and not perform any label aggregation or evaluate the # of Tokens to Release property. If failed is 0, the batch will release normally and evaluate the # of Tokens to Release and perform all of the label aggregations on the tokens in the batch. The `releasebatch()` command also gives you the ability to send the released tokens out any connector of the Batch activity. The connector can be referenced by index or name. The `releasebatch()` command even allows you to release tokens to an activity that is not directly connected to the Batch activity by referencing the activity using `getactivity()`.

 

See Max Wait Time and Max Idle Time for more information about these properties.

 

 

 

## Connectors

 

The Batch activity allows for any number of connectors out. However, batches that complete will always exit out the first connector. Only manually released batches have the opportunity to exit out a different connector. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Batch activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

This button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

This button opens the activity's statistics window. See Statistics for more information about this property.

 

### Batch Quantity

 

The Batch Quantity box sets the target value for the batch. The batch will continue collecting tokens until this value is reached, after which the batch will be released. See Organizing Batches for more information.

 

### Quantifier

 

The Quantifier box sets the criteria that will determine how the batch quantity is calculated. You can either collect by *Number of Tokens* or by a label on the incoming tokens. See Organizing Batches for more information.

  

#### Valid Quantifier Values

 

The Quantifier property must be either *Number of Tokens*, a string which will be interpreted as the label name on the entering tokens, or a pointer to the token label (using Label: aLabel or getlabel(token, "aLabel")).

  

### # Tokens to Release

 

Specifies the quantity of tokens that the batch will release once it has finished collecting.

 

### Group By

 

The Group By box sets the criteria by which incoming tokens will be grouped into batches.

 

### Overflow

 

The Overflow setting controls what happens to values that exceed the Batch Quantity while collecting. This option is not available when collecting by the *Number of Tokens* Batch Overflow Options for more information.

 

### Label Aggregation

 

Using this set of properties, you can create and modify labels on tokens when a batch is released. The operation performed, as set in the Aggregation column, will be done on every token that was added to the batch, even if the token is destroyed when the batch is released. Once the operation is performed, the resulting value will be set on every token being released by the batch. See Assigning Labels to Outgoing Batches for more information.

 

### Use Max Wait Timer

 

The Max Wait Timer will be evaluated when a batch is first created. If a timer is not created because the Start Criteria evaluates to false, the max wait timer will be evaluated each time a token is added to the batch until the Start Criteria does evaluate to true. See Releasing a Batch Early for more information.

 

### Use Max Idle Timer

 

The Max Idle Timer will be evaluated each time a token is added to a batch. See Releasing a Batch Early for more information.

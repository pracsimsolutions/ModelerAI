---
id: reference_processflowobjects_batchstatistics_batchstatistics
name: "Batch Statistics"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","BatchStatistics","BatchStatistics"]
tags: ["manual","reference","processflowobjects","batchstatistics"]
source: "manual/Reference/ProcessFlowObjects/BatchStatistics/BatchStatistics.html"
---

# Batch Statistics

## Introduction to Batch Statistics

 

You can use statistics to get useful data from your Batch activities if needed. This topic will discuss how the statistics behave differently on Batch activities (as opposed to the statistics on other process flow objects).

 

 

 

## Batch Dialog Box

 

 

The **Batch dialog box** can be opened by clicking on the batch displayed on the Batch activity. This dialog box gives information on the selected batch and lets you see which tokens are currently in the batch.

 

- **Batch** - This is the **Group By** value of the batch. If the **Group By** was set to None, this field will display 0.
- **Collected** - Displays the progress of the batch either as the # of tokens collected or the summed label value collected out of the total Batch Quantity.
- **Start Time** - The time the batch was created in model time units.
- **Active Tokens** - Displays all the tokens currently in the batch.
- **Token Info** - Displays the labels of the selected token and allows you to select and visually trace the token.

 

 

 

## Standard Statistics

 

The standard statistics kept by the Batch activity (see Process Flow Statistics) are unique because the Batch can create or destroy tokens. This makes the statistics a little less intuitive. The following list discusses the meaning of each statistic:

 

- **Input** - All tokens that arrive at a Batch activity increment the input. Tokens created by the Batch do not affect input.
- **Output** - All tokens that move on to another activity from the Batch increment the output. Tokens destroyed by the Batch do not affect output.
- **Content** - All tokens that arrive at the Batch activity increment the content. All tokens that leave the Batch activity decrement the content, including tokens that are destroyed by the Batch. Tokens created by the Batch do not affect content, unless they were created as part of overflow logic. The content is always equal to the number of tokens currently being collected.
- **Staytime** - All tokens that arrive at the Batch activity, or are created as part of overflow logic, will affect the staytime when they leave or when they are destroyed. The average staytime is the average time that tokens spend collecting on the Batch activity.

 

 

 

## Batch Statistics

 

The Batch activity can also record statistics dealing with batches, rather than tokens. The following list describes these statistics; type of each statistic (Incremental, Level, or Stream) is listed with the title.

 

- **Number of Batches (Level)** - The number of batches presently being collected on the batch.
- **Batches Created (Incremental)** - The number of batches created by the Batch activity.
- **Batches Released (Incremental)** - The number of batches released by the Batch activity.
- **Batch Staytime (Stream)** - The amount of time each batch spent being collected. The Batch activity updates this statistic every time a batch is released (including manually releasing a batch early).
- **Batch Quantity (Stream)** - The quantity within a batch. The Batch activity updates this statistic every time the quantity of any batch changes, setting it to the quantity of the batch. This statistic is probably most useful as an event; for example, you could listen for any time a batch quantity exceeds a certain value.
- **Tokens per Batch (Stream)** - The number of tokens in each batch. The Batch activity updates this statistic every time a batch is released, setting it to the number of tokens in the most recently released batch.
- **Quantity per Batch (Stream)** - The quantity in each batch. If the Batch is collecting by Number of Tokens, this statistic will not be updated, since it would be identical to the Tokens per Batch statistic.

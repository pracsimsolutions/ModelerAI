---
id: reference_propertiespanels_fixedresourcepanels_queue_queue
name: "Queue"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","FixedResourcePanels","Queue","Queue"]
tags: ["manual","reference","propertiespanels","fixedresourcepanels"]
source: "manual/Reference/PropertiesPanels/FixedResourcePanels/Queue/Queue.html"
---

# Queue

The Queue panel defines how items are stored in the queue.

 

 

The following properties are on the Queue panel:

 

### Maximum Content

 

This is the maximum number of flow items the queue can hold at once.

 

### Item Placement

 

This menu defines how the flow items are placed in the queue visually. It has the following options:

 

- **Stack Vertically** - The flow items are stacked on top of each other. The flow item at the bottom of the pile is the one that has been in the queue the longest.
- **Horizontal Line** - The flow items are lined up horizontally. The one closest to the output ports of the queue is the one that has been in the queue the longest.
- **Stack inside Queue** - The flow items are stacked in rows inside the queue. The flow items will move if a product ranked before them is taken out of the queue. If you would like the product positions to stay the same once they are in the queue, then have the queue be LIFO by having downstream objects pull only the last product in the queue.
- **Do Nothing** - Flow items are all placed at the same point in the queue. This may make it appear as if the queue is only holding one flow item.

 

### Stack Base Z

 

This number defines the height where the queue begins placing flow items that are being stacked vertically or inside the queue.

 

### LIFO

 

If this box is checked the queue will act as a "last in first out" (LIFO) queue, otherwise it will act as a *first in first out* (FIFO) queue.

 

### Perform Batching

 

If this box is checked, the queue will accumulate flow items into a batch before releasing them downstream. Accumulation continues until either the target batch size is met or the max wait time expires. If this box is not checked, no batching will occur, and flow items may leave as soon as downstream objects are available.

 

### Target Batch Size

 

This number defines the size of the batches that the queue will gather before sending the flow items downstream. Flow items are sent downstream individually.

 

### Max Wait Time

 

This number is the maximum length of time that the queue will wait before sending the flow items downstream. If this time expires and the batch size has not been met, the currently collected batch will be released anyway. If 0 is specified in this field, then there is no maximum wait time, or in other words the queue will wait indefinitely.

 

### Flush contents between batches

 

If this box is checked the queue will not allow new flow items to enter until the entire current batch has left.

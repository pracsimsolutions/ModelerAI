---
id: workingwithtasks_agvnetworks_troubleshooting_troubleshooting
name: "Troubleshooting AGVs"
kind: manual
breadcrumb: ["WorkingWithTasks","AGVNetworks","Troubleshooting","Troubleshooting"]
tags: ["manual","workingwithtasks","agvnetworks","troubleshooting"]
source: "manual/WorkingWithTasks/AGVNetworks/Troubleshooting/Troubleshooting.html"
---

# Troubleshooting AGVs

This topic provides suggestions for troubleshooting common AGV problems.

 

 

## Navigation Errors

 

Sometimes when you are working with AGVs you will get a navigation error that looks like the following:

 

 

This error is caused because the task executer (TaskExecuter1 in the screenshot above) is at a location on the network that cannot reach the target destination. This may be caused by problems in the way you have defined your paths (one-way paths that should be two-way paths, etc.). However, sometimes, even when you've supposedly defined your paths properly, this error may still show up.

 

 

 

## Showing AGV Routing Accessibility

 

When you get navigations errors, you can show routing accessibility information for control points in the AGV system. This lets you easily troubleshoot navigation errors. To do this:

 

1. Find the destination control point that the task executer cannot find a path to. In the example screenshot above, this would be the control point connected to *Source1*.
1. Right-Click on that control point and choose *Show AGV Routing Accessibility*.

 

Various path transfers will be highlighted in blue. These are points on the network that CAN reach the target control point.

 

 

If a task executer cannot reach the control point, then the path transfers around it will not be blue. You can then follow the assumed path from that task executer to the destination. At the point where transfers change from black to blue, that is the point where there is some break in the path links such that the task executer cannot reach the destination.

 

### Getting Routing Table Information

 

You can also hover the cursor over various control points and path transfers. This will show route cost information for traveling from the hovered point to the destination control point.

 

 

Each line of the displayed text represents an entry in a routing table. The AGV system must calculate multiple of these routing tables because different situations may call for different routes. For example, an AGV's speed profile may be different when it is loaded versus when it is empty. Accordingly, if you have configured the AGV network to find the fastest route instead of the shortest route, then an AGV may travel a different route when it is empty versus when it is loaded. Each unique route must be calculated and stored independently.

 

The AGV system will dynamically calculate, and cache, routes as they are needed in the simulation run. This means the number of route cost entries you see when you hover over a path point may grow as the simulation progresses.

 

Each routing table entry displays a tuple key for that table, the calculated cost to travel to the destination, and a "next rule", which is a rule defining where to go next from this point to get to the destination.

 

 

When you hover over a path transfer, it will also show, for each entry, the Path that the routing table entry is associated with. A path transfer actually has two "sides", so it will have two entries in the routing table: one determining where to go when an AGV is traveling on one path, and another for when the AGV is traveling on the other path. This is most important in looking at the *rule* information. A *next* rule means that, when on the given path, the AGV should continue on the path to get to the destination. When the rule is *tnext*, it means the AGV should transfer onto the other path to get to the destination.

 

 

 

## Refreshing Path Links

 

In the course of building your model, sometimes AGV paths will lose, or not properly create, links to their neighbors. This can cause the navigation errors mentioned above. You can manually refresh these links through a path's right-click menu. Once you've found the point where navigation is blocked, you can right click on the path and refresh its path links.

 

 

This will make the network re-analyze the geometry of that, and potentially other, paths, and will re-link the path with its neighbor paths, allowing the AGV system to properly routing AGVs to their destinations.

 

 

 

### Troubleshooting AGV Accumulation Behavior

 

Many users will see the option for accumulation and think this is the perfect solution for what they want to do. The truth, however, is that using accumulation, while making many simulation scenarios easier, can also introduce new complexities and potential problems. This doesn't mean you shouldn't use accumulation. Most encountered problems are easily fixed with the right know-how. Rather, you should just make sure you have a good understanding of how accumulation works before deciding to use accumulation.

 

First, generally you should not use accumulation for spur paths. These are paths where the AGV will enter, load or unload, then switch directions to exit on the same path that it entered. Accumulation doesn't really make sense in a spur because, if two AGVs must avoid each other inside a spur, you've probably already done something wrong by letting them both in. Instead of accumulation, you should implement other mutual exclusion mechanisms, e.g. using control areas or process flow, to only allow one AGV into the spur at a time. If you've implemented this one-at-a-time mutual exclusion, then accumulation is not needed anyway. Also, for two-way through paths, you must be careful in using accumulation. Again you must implement some additional method, either through control areas or process flow, to prevent AGVs going in opposite directions from getting onto the same path.

 

Another complexity with using accumulation is that it creates a new allocation scheme, one that is different than, but will operate in tandem with, the standard allocation logic used for control points and control areas. Consequently, in some situations the two allocations schemes will "fight" with each other, and cause deadlock in the system. This is especially more probable in close-quarter intersection areas that use control areas to mutually exclude traffic.

 

An example problem situation is shown in the following figure.

 

 

In this situation, AGV_7 has allocated the intersection *after* the control area, but has yet to allocate the control area itself. This creates a serious problem because AGV_9, the other AGV at the bottom center of the screen, will allocate the control area first. Then the two AGVs will get into a deadlock because AGV_7 can't allocate the control area owned by AGV_9, and AGV_9 can't allocate the intersection owned by AGV_7.

 

As mentioned before, this is caused because two different allocation schemes are in play here, namely the allocation of control areas vs. the allocation of intersections on accumulating paths. When allocating control areas, the AGV allocates when it pre-arrives at the previous control point. It will drive up to the closest control point, then allocate (in an all-or-nothing fashion) the next control point on its path, as well as all control areas on its path to that next control point. Thus the timing of when the AGV allocates the control area is based on the position of the control point leading up to the control area.

 

Accumulating intersection allocation, on the other hand, is determined based on the position of the intersection and the settings of the associated Accumulation Type, defined in AGV network properties. In the situation shown above, AGV_7 would use the "On Path Long" setting, whose default says the AGV should be able to stop with its leading edge at least one meter short of the intersection if it can't allocate that intersection. This determines the point at which the AGV will attempt to allocate the intersection.

 

In the example situation, AGV_7 allocates the intersection first because the control point is pretty close to the control area, so it reaches the point where it needs to allocate the intersection *before* it reaches the point where it needs to allocate the control area. On the other hand, AGV_9's control point is situated a little farther away from the control area, so it will allocate the control area before it allocates the intersection. This causes the deadlock.

 

### Solutions

 

When encountering issues like this, there are two primary solutions you can try. First, you can create a special accumulation type that is specifically designed for close-quarter intersections. This accumulation type would ensure that control areas are always allocated *before* intersections. Here you would simply create a new accumulation type in the AGV network properties. Then for each intersection stop point, base it on the center of the AGV, with a distance of 0, meaning the AGV's center must stop 0 meters short of the intersection if the AGV can't allocate it. Once you've done that, make all the paths in that small area use that accumulation type.

 

The second solution to try is to simply not use accumulation in those close-quarter areas. Just go to each path in the intersection and make its accumulation setting "No Accumulation." This will make it so the control area is the sole mutually excluding element in the area. Then you don't have to worry about the right order of allocation. Here you'll want to make sure that the deallocation of the control area is not too early. You want the AGV to at least allocate the first intersection on an accumulating path ahead of it *before* it deallocates the control area. That shouldn't be too difficult. If it doesn't do it already, you can either lengthen the deallocation distance for that control area's deallocation type, or you can put down a control point at the beginning of the next accumulating path, then make the control area's deallocation type "Deallocate at Next Control Point."

 

As a general rule of thumb, accumulation works very well in areas of a model where there are long sections of path with relatively few intersections. On the other hand, in areas where there are lots of close-quarter intersections, it can be easier and less problematic to turn off accumulation, bound the areas with control points, and then limit traffic inside them with control areas.

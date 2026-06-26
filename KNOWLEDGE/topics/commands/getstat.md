---
id: getstat
name: "getstat"
kind: command
module: ""
signature: "getstat(node object, str name, int flag [, var p1, var p2, var p3]) -> var"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: "Object.stats"
description: "Returns the statistic value for the given object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getstat

```flexscript
getstat(node object, str name, int flag [, var p1, var p2, var p3]) -> var
```

Returns the statistic value for the given object

It is preferred to use Object.stats instead. Gets the specified statistic from the given object.
name - The name of the statistic. Valid names can be enumerated using
function_s(object, "enumerateStatistics", targetNode). Documentation for the
meaning of the various statistics that an object provides should be found in each object's
documentation. The documentation may include spaces in the names of the statistics, for
readability. In passing these as the name parameter to getstat(), you should remove all spaces.
flag - Can be one of the following values

STAT_CURRENT - returns the current value of this statistic
STAT_MIN - returns the minimum value of this statistic
STAT_MAX - returns the maximum value of this statistic
STAT_AVERAGE - returns the average value of this statistic
STAT_TIME_AT_VALUE - returns the total simulation time the stat spent
at a given value (requires an additional p1/p2/p3 parameter designating the desired value). This
is only valid if the stat is tracking its history or profile data, such as in objects' standard "State"
statistics. See below for more information on this.

p1, p2, and p3 are for providing "requirements" to resolve the correct statistic. For
example, a list has several per-partition statistics. In order to get a partition's statistic,
you need to pass the desired partition ID as p1. Requirements depend on the statistic that is being retrieved.

There are also several special stat names that you can use. They are: "State", "Output", "Input", "Content", "Staytime".
If you use one of these names, the command will return the corresponding standard statistic of the object. Examples:
getstat(object, "Output", STAT_CURRENT) - returns the current (total) output of the object (same as object.stats.output)
getstat(object, "Content", STAT_AVERAGE) - returns the average content of the object
getstat(object, "State", STAT_CURRENT, 0) - returns the current state of the object (same as object.stats.state().value)
getstat(object, "State", STAT_TIME_AT_VALUE, 0, STATE_IDLE) - returns the total idle time of the object (0 designates the
state profile, i.e. the default state profile)
getstat(object, "Staytime", STAT_MAX) - returns the maximum recorded stay time of the object
Note that some stats that do not make sense will not return valid data, such as average state, or average output.

STAT_TIME_AT_VALUE
If you want to get the STAT_TIME_AT_VALUE access, the statistic needs to track either historical or profile data. If it is
an object's standard "State" statistic, FlexSim will automatically make it track its profile data, so you don't need
to do any extra work for that. For other statistics, however, you will need to explicitly "subscribe" to the statistic
to make it track its profile or history data. Historical
data records each change of value and the corresponding simulation time of that value change. Profile data records the total
time the variable was at each value. Only non-negative integer values are valid for profile data.

To subscribe to a tracked variable,
first find where the tracked variable statistic is in the tree. For example if it's
the "Content" statistic, you'll likely find it at object>stats/stats_content. Then call the following command on the node:
function_s(statNode, "addSubscriber", needsHistory, needsProfile, persist). The needsHistory and needsProfile parameters
are booleans telling which data you want to track. The persist is a boolean that tells if you want it to track these statistics
indefinitely going forward. The tracked variable tries not to continue tracking extra data if it is not needed. If nobody
subscribes to its data "persistently", then it will discontinue recording the data if it doesn't get subscribers for a while
(I think after 5 or so model resets with no subscribers).

Recording historical data will allow you to get the STAT_TIME_AT_VALUE access. However, using historical data for getting
STAT_TIME_AT_VALUE is slower than if you track profile data. With historical data, it must traverse the entire history to
calculate STAT_TIME_AT_VALUE, whereas if you track profile data, it stores the total time at the value directly in the
profile data.


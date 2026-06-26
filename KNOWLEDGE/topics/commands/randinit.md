---
id: randinit
name: "randinit"
kind: command
module: ""
signature: "randinit(num stream, num seedvalue[, num seedvalue2, num seedvalue3, num seedvalue4, num seedvalue5, num seedvalue 6])"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: "the"
description: "Initializes a random stream"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# randinit

```flexscript
randinit(num stream, num seedvalue[, num seedvalue2, num seedvalue3, num seedvalue4, num seedvalue5, num seedvalue 6])
```

Initializes a random stream

Initializes the random number stream by setting its first value to seedvalue. Although there are an unlimited number of random number streams available to the user, only the first one hundred streams numbered 0 through 99 have been initialized when the FlexSim application is opened. To create a new stream, simply execute the randinit() command for the new stream. For instance, executing randinit(105, 7628912) will create a new stream number 105 if it does not currently exist, and set its initial seed value to 7628912.

If "Repeat Random Streams" has been selected in the Statistics menu, then streams 0 through 99 will be automatically initialized with a default seed value for that stream whenever the model is reset. For streams 100 and above, you must use the randinit() command to set their initial seed values if you wish to repeat their random streams. Any positive integer may be passed in as the seedvalue for a random number stream. If you pass in a seedvalue of 0 for random streams 0 through 99, it is a special case that will initialize them to their default seed values.

seedvalues 2 through 6 are only used if you are using FlexSim's Combined MRG random number generator. To enable this generator, call maintenance(49,1) when your model opens.

## Example

```flexscript
Example 1:

randinit(5, 73241);

Initialize FlexSim's random stream number 5 with a sead value of 73241.

Example 2:

randinit(2,0);
for(int i = 100; i <= 199; i++)
{
randinit(i, realtime(1) * uniform(0, 1, 2));
}

First initialize standard stream number 2 to its default seed value, then execute a loop for 100 iterations to create (if necessary) streams 100 through 199 and set their initial seed value to the product of a random number pulled from stream 2 and the current number of real clock seconds.
```


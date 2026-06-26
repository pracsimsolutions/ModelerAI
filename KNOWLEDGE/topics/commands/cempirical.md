---
id: cempirical
name: "cempirical"
kind: command
module: ""
signature: "cempirical(str/node/num table[, num stream]) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: "with"
description: "Returns a random sample from a continuous empirical distribution"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# cempirical

```flexscript
cempirical(str/node/num table[, num stream]) -> num
```

Returns a random sample from a continuous empirical distribution

A continuous empirical distribution with percentages and values defined in the table specified.

Empirical distributions reference tables created by the user that contain a list of values and probability percentages associated with each value. Probability percentages must be entered in column 1 starting with row 1 of the table, and the associated values are entered in column 2. The table may have as many rows as needed to define as many values as desired. The percents are entered as numbers between 0 and 100, and should add up to a total of 100 percent; otherwise any values defined after a cumulative percentage of 100 is reached will never be returned.

There are three commands in FlexSim that can be used to generate random variates from the empirical distributions defined in tables. The three commands are dempirical(), empirical() and cempirical(). The first one is a discrete distribution and will return the explicit values listed in the table. The next two are bounded continuous distributions, meaning they return a continous number within a range. The values in column 2 for the continuous distributions must be in ascending order because they will interpolate values between two adjacent values in the table.

Let's assume we have a table with 4 rows and 2 columns. In column one are the percentages 10, 20, 30 and 40 adding up to 100 percent. In column two are the values 0.1, 0.2, 0.3 and 0.4. Now let's see the differences between the discrete empirical command and the two continuous empirical commands. The discrete command will only generate random variates that match exactly the values that were entered into column two of the table. The continuous commands will generate random variates that are real numbers uniformly distributed between two adjacent numbers in column two of the table. The difference between the two continuous commands is in how the bounds of the uniform ranges are defined.

The command dempirical("mytable") will return the number 0.1 for ten percent of the samples, the number 0.2 for twenty percent of the samples, the number 0.3 for thirty percent of the samples, and the number 0.4 for forty percent of the samples assuming a very large sample set.

The command empirical("mytable") will return a number uniformly distributed between 0.1 and 0.2 for ten percent of the samples, a number uniformly distributed between 0.2 and 0.3 for twenty percent of the samples, a number uniformly distributed between 0.3 and 0.4 for thirty percent of the samples and a number uniformly distributed between 0.4 and 0.4 for forty percent of the samples.
If you want the last forty percent to be between 0.4 and 0.5 instead of 0.4, add a 5th row with column one being a value of 0 and column two being 0.5 or the desired upper bound.

The command cempirical("mytable") will return a number uniformly distributed between 0.0 and 0.1 for ten percent of the samples, a number uniformly distributed between 0.1 and 0.2 for twenty percent of the samples, a number uniformly distributed between 0.2 and 0.3 for thirty percent of the samples and a number uniformly distributed between 0.3 and 0.4 for forty percent of the samples.
You can define a lower bound for the first ten percent to be a value different than 0.0 by adding a row to the beginning of the table (row 1) where column one is a value of 0 and column two is 0.05 or the desired lower bound.

Here is a summary in tabular form showing the possible return values ("x") for each of the three distribution functions:

Here is an example of adding a "dummy" first row so that cempirical() starts at 0.05 instead of 0.0 (notice that row 1 has a probability of 0 percent):

Here is an example of adding a "dummy" last row so that empirical() has a range between each value including the last (any number can be entered for the percent of the last row because the percents already add up to 100% with the previous row, so it doesn't matter what it is):

When using ExpertFit to determine an empirical distribution that matches your data, you need to be aware that if your data has been defined as integers, then ExpertFit will fit it for use with dempirical(), and if your data has been defined as real numbers, ExpertFit will fit it for use with empirical(). When your data set is composed of real numbers, ExpertFit will show the same percentage for the last value as for the previous to last value, but you'll notice that the percents add up to 100% with the second to last entry.

## Example

```flexscript
cempirical("mytable", 5);

Returns a random variate from a continuous empirical distribution defined in a global table named "mytable" using random number stream 5.

cempirical(current.labels["ptime"] 5);

Returns a random variate from a continuous empirical distribution defined in a node table defined on a label named "ptime" of the current object, and using random number stream 5.
```


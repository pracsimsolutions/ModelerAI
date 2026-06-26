---
id: reference_peopleobjects_peopletables_personlabels_personlabels
name: "Person Labels"
kind: manual
breadcrumb: ["Reference","PeopleObjects","PeopleTables","PersonLabels","PersonLabels"]
tags: ["manual","reference","peopleobjects","peopletables"]
source: "manual/Reference/PeopleObjects/PeopleTables/PersonLabels/PersonLabels.html"
---

# Person Labels

## Overview

 

 

The Person Labels table records the label values of each flowitem created with the Create Person activity. The labels that appear in the table are the labels specified with the People Settings object.

 

For more general information on People Statistics Tables, see the People Statistics Tables topic.

  

#### Label Column Ordering

 

The order of columns in this table may not exactly match the order specified in the People Settings object. This table orders those columns first by type (number, string, or pointer), and then by the specified order.

  

 

 

## Rows

 

Rows are added whenever a new flowitem is created with the Create Person activity.

 

 

 

## Columns

 

- **PID** - This column records the id of the flowitem that was created with Create Person.
- **Number Columns** - These columns appear if there are labels in the People Settings object that store number data. The column name matches the name of the label.
- **String Columns** - These columns appear if there are labels in the People Settings object that store string data. The column name matches the name of the label.
- **Pointer Columns** - These columns appear if there are labels in the People Settings object that store pointer (node) data. The column name matches the name of the label.

 

 

 

## Value Changes

 

The PID column does not change once added. All other columns will reflect the current value of the flowitem's label, by listening for changes to the label value. When the flowitem leaves the model, these columns will contain the final value of each label.

 

 

 

## Warmup Behavior

 

At the warmup time, this table will remove the rows corresponding to flowitems that are no longer in the model. The data in the remaining rows will remain unaffected.

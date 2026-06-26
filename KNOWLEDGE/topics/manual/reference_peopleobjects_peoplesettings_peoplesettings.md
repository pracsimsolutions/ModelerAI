---
id: reference_peopleobjects_peoplesettings_peoplesettings
name: "People Settings"
kind: manual
breadcrumb: ["Reference","PeopleObjects","PeopleSettings","PeopleSettings"]
tags: ["manual","reference","peopleobjects","peoplesettings"]
source: "manual/Reference/PeopleObjects/PeopleSettings/PeopleSettings.html"
---

# People Settings

## Overview and Key Concepts

 

The People Settings object allows you to specify the labels and states that are statistically meaningful in your model.

 

 

### Labels and Label Sets

 

The labels you specify will be added to each object created with any Create Person activity. In addition, the label values for each object will be recorded in the Person Labels table. Specifying labels is optional; there are no required labels.

 

The People Settings object also allows you to specify multiple Label Sets. A Label Set is a set of initial values for the labels you have specified. The Create Person activity can specify which label set you wish to use, which will determine the initial values of the labels on the created object.

 

### States

 

The states enumerated in the People Settings object fall into two categories:

 

- Default states, and
- Additional states.

 

FlexSim will set objects to the default states automatically. However, the Process activity, as well as the People Down Behaviors, can set the objects into any state. This allows you to have custom states specific to your process. For example, a restaurant model might have use a state called "OrderingFood", while a healthcare model might have a state called "GettingXRay". The various state tables (for example, the Person States table) will include one column for each state listed in the People Settings object.

  

#### Enumerate any state you use

 

If a Process activity sets an object in a state that is not listed in the People Settings object, then no statistics for that state will be recorded. Therefore, it is important to enumerate any state that you want to use.

  

 

 

## Person Labels

 

The Person Labels tab has the following properties:

 

 

- **Labels** - The Labels property allows you to add, remove, or reorder columns of the table. The name of each column specifies the name of the label, and each row of the table specifies an initial value of the label.
- **Label Sets** - The Label Sets property allows you to add, remove, and reorder rows in the labels. Row defines a label set. The name of the row specifies the name of the label set, and each column specifies the initial value of the label for that set.
- **Label Sets** - The Label Sets property allows you to add, remove, and reorder rows in the labels. Row defines a label set. The name of the row specifies the name of the label set, and each column specifies the initial value of the label for that set.
- **Label Type** - The Label Type property shows the type of label in a given column. You can use this edit field to set the type of a label after you have added it.
- **Code** - The code button toggles whether the set of selected cells are FlexScript. If a cell is FlexScript, then that code is evaluated when the person is created. Note that the type of the label remains unchanged. If a number label is FlexScript, then the code should return a number. If a text label is FlexScript, then the code should return text. If a pointer label is FlexScript, then the code should return a pointer.
- **Default States** - The Default States interface allows you to set the Utilization value for the default states, as well as choose a display name for them. The Utilization setting here is used by the PersonStates table to calculate the utilization of each person.
- **Additional States** - The Additional States interface allows you to add new states to the list of states that are statistically tracked. The interface also allows you to choose how those states affect utilization.

 

 

 

## Staff States

 

The Staff States tab has the following properties:

 

 

- **Default States** - The Default States interface allows you to set the Utilization value for the default states, as well as choose a display name for them. The Utilization setting here is used by the StaffStates table to calculate the utilization of each Staff object.
- **Additional States** - The Additional States interface allows you to add new states to the list of states that are statistically tracked. The interface also allows you to choose how those states affect utilization.

 

 

 

## Equipment States

 

The Equipment States tab has the following properties:

 

 

- **Default States** - The Default States interface allows you to set the Utilization value for the default states, as well as choose a display name for them. The Utilization setting here is used by the EquipmentStates table to calculate the utilization of each Equipment object.
- **Additional States** - The Additional States interface allows you to add new states to the list of states that are statistically tracked. The interface also allows you to choose how those states affect utilization.

 

 

 

## Transport States

 

The Transport States tab has the following properties:

 

 

- **Default States** - The Default States interface allows you to set the Utilization value for the default states, as well as choose a display name for them. The Utilization setting here is used by the TransportStates table to calculate the utilization of each Transport object.
- **Additional States** - The Additional States interface allows you to add new states to the list of states that are statistically tracked. The interface also allows you to choose how those states affect utilization.

 

 

 

## Location States

 

The Location States tab has the following properties:

 

 

- **Default States** - The Default States interface allows you to set the Utilization value for the default states, as well as choose a display name for them. The Utilization setting here is used by the LocationStates table to calculate the utilization of each Location object.
- **Additional States** - The Additional States interface allows you to add new states to the list of states that are statistically tracked. The interface also allows you to choose how those states affect utilization.

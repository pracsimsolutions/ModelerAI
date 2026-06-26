---
id: reference_propertiespanels_peoplepanels_enterexitinstructions_enterexitinstructions
name: "Enter/Exit Instructions"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","PeoplePanels","EnterExitInstructions","EnterExitInstructions"]
tags: ["manual","reference","propertiespanels","peoplepanels"]
source: "manual/Reference/PropertiesPanels/PeoplePanels/EnterExitInstructions/EnterExitInstructions.html"
---

# Enter/Exit Instructions

The Enter/Exit Instructions panel defines what people do when they arrive at or leave this object.

 

 

The following properties are on the Enter/Exit Instructions panel:

 

### Edit Enter Instructions

 

Opens the Enter Instructions window where you define what people do when arriving at this location.

 

### Edit Exit Instructions

 

Opens the Exit Instructions window where you define what people do when leaving this location.

 

 

 

## The Instructions Window

 

This window is used for both Enter and Exit intructions.

 

 

### Template

 

This dropdown specifies the set of instructions to be used for the object. All but the last item are pre-defined instruction sets for various location types. The last item is the Custom option which allows you to define custom instructions sets.

 

### Role

 

This and all other options below become available if Template is set to Custom. The Role listbox defines the different roles an entering person may assume. For example, a person flowitem entering a bed could use a Patient role which details a list of instructions to make the person lay in the bed. Whereas a staff object could use a Staff role with a list of instructions to walk to the side of the bed and start a stethoscope animation. The list defines all the possible roles, with their associated instruction set, used by this object. When a person enters the location they will assume the first role from the top that they satisfy the condition for. Sub roles can also be defined inside a role. This allows a set of instructions to be executed for a role, and then the person will also assume the first sub role within that role to assume.

 

### Name

 

The name of the role or subrole.

 

### Condition

 

The condition the entering person must meet to assume this role or subrole. For example, a Doctor role might require the entering person to be a member of the MDs Group. The entering person will check the condition for each role from the top. The first condition that returns true is the role they will assume.

 

### Instructions

 

Here you define the list of instructions for the currently selected role. New instructions can be added from the add button.

 

### Planner Person

 

Adds a planner person visual inside the location. This can be used by some of the instruction panels with the Apply Planner Position button to more easily set position and rotation values of the entering person.

---
id: modellogic_submodels_keyconcepts_keyconcepts
name: "Key Concepts About Submodels"
kind: manual
breadcrumb: ["ModelLogic","Submodels","KeyConcepts","KeyConcepts"]
tags: ["manual","modellogic","submodels","keyconcepts"]
source: "manual/ModelLogic/Submodels/KeyConcepts/KeyConcepts.html"
---

# Key Concepts About Submodels

## Submodels Overview

 

Submodels allow you to create and maintain reusable components that can be shared across multiple simulation models. Submodels also enable more collaboration among team members working on different parts of a model; one team member can work on a submodel while another works on the main model or a different submodel.

 

A **Submodel** is a Container object (including its contents) and all the **dependencies** that it needs to function. In general, dependencies are the objects found in the toolbox that the container or any objects within the container require to function properly. Examples of dependencies include: 

- Groups
- Global Tables
- Global Lists
- General or Object Process Flows
- The Storage System
- The AStar Navigator
- The AGV Navigator

 By creating a Submodel, you can use the container in another model. When you do, any dependencies it requires will be added to the new model automatically.

 

If you want to improve a submodel, perhaps by adding a feature or fixing a bug, you can make those changes and save the submodel. Any models that use the submodel can easily update to use the latest version of the submodel.

 

### Creating a Submodel

 

A submodel is a regular FlexSim model that contains: 

1. A Container (its contents and dependencies), and
1. A Submodel Export tool that points at that container.

 

Any model that has a Submodel Export tool that points at a container can be used as a Submodel.

 

### Using a Submodel

 

Any model can use a Submodel by using the Submodel Import tool. To use a Submodel Import tool, you need to specify two things: 

1. The model file that contains the Submodel Export tool, and
1. The name of the Submodel Export tool within that model.

 By default, the file path to the exporting model is relative to the importing model. This means that if you move both models to a different folder, the import will still work. It also means that you are required to save the importing model before you can use the Submodel Import tool.

 

Once you have created a Submodel Import tool, you can drag in one or more instances of the submodel into your main model. Each instance will create a copy of the container. Any dependencies that are not present in the main model will be added automatically.

 

### Updating a Submodel

 

To update a Submodel, you simply need to make your changes in the original model file that contains the Submodel Export tool. Once you save the exporting model, you can open any model that imports the submodel. Right-click on the Submodel Import tool in the toolbox and select "Update Instances from Source".

  

### Warning: Version and Units Compatibility

 

When importing, exporting, or updating submodels, ensure that both the source model and the importing model are fully compatible in terms of **FlexSim version** and **unit settings**.

 

Submodels should not be mixed between different FlexSim versions. Always ensure that both the source model and the importing model are created with the same FlexSim version.

 

When importing or exporting a submodel, ensure that the source model and the importing model use the same units (distance, time, flow rate, etc.). Always verify that both models are configured with matching unit settings before importing or updating a submodel.

  

### Submodels and Templates

 

When you import a submodel, the container you import acts like a Template Instance of the original container. This means you can choose to override properties in the imported container. For example, you might want to change the position or rotation of a component within the submodel, or you might want to add extra labels. If you update the submodel, FlexSim will save all the current properties, apply the new version of the submodel, and then re-apply any overridden properties.

 

### The Import Algorithm

 

The import algorithm runs whenever you update the instances of a submodel from the source model. Dragging in a new instance actually creates a placeholder container and then updates all the instances from the source model.

 

The import algorithm works as follows: 

1. Load the entire model containing the submodel.
1. Find the Submodel Export tool specified in the Submodel Import tool.
1. Find the container associated with the Submodel Export tool.
1. For each dependency:
1. If the dependency is not present in the importing model, **add** it.
1. If the dependency is present, check if the existing dependency is compatible with the one in the submodel. If so, **use** it.
1. If the dependency is not compatible, **replace** it with the one from the submodel.

  For each instance container: 

1. Check if this instance is out of date with the exported container. If not, skip to the next instance. Otherwise, continue with the following steps.
1. Save any overridden properties.
1. Replace the contents of the container with a copy of the container from the submodel.
1. Re-apply any overridden properties, if an object with the same relative path still exists.

 

If any objects will be replaced by the import, FlexSim will prompt you to confirm all replacements. This gives you a chance to cancel the import if you do not want to replace those objects. When importing, you must either accept all replacements or cancel the import; you cannot selectively accept or reject replacements. However, some dependencies are handled more granularly. For example, if a Storage System already exists in the importing model, but it doesn't have all the required slot labels, the import algorithm will simply import the missing labels. It will only replace a label if that label already exists but is incompatible.

 

Most dependencies require a strong match between the imported and existing objects for the existing dependency to be considered compatible. The definition of a "strong match" depends on the type of dependency. For example, a Process Flow must have identical activities and connections, but different zoom/pan settings are not considered a difference. For some dependencies (like Global Tables), the export author can choose between a strong match or a more relaxed compatibility requirement. Still others (like Groups) will always use the existing dependency, as they are always compatible.

 

 

 

## Best Practices

 

When creating a submodel, consider the following best practices:

 

### Only Export What you Need

 

When choosing which dependencies to export, be sure to export as few dependencies as possible. For example, if you don't need custom flowitems, don't export the Flowitem Bin. Exporting fewer dependencies makes the submodel easier to use and reduces the chance of conflicts when importing the submodel into another model.

 

### Use Specific Names for Dependencies

 

For dependencies, give them as specific a name as possible to avoid false conflicts. For example, if you export a table named GlobalTable1, that is likely to conflict with an existing table. Instead, use a more specific name.

 

### Creation over Promotion

 

It is generally better to create new submodels rather than promoting existing objects to submodels. The main reason is that not all dependency types are currently supported. It is possible and even likely that an existing model, built without thought for export, will use dependencies that are not supported by the submodel feature. Instead, it is better to iteratively build a submodel from scratch, testing it as you go, to ensure that the features you need are supported.

 

Another reason to create a new submodel rather than to promote an existing model is naming. When you create a new submodel, you can choose a name that is less likely to conflict with existing objects. When you promote an existing object, you are forced to use the existing name, which may conflict with other objects, or to change the existing model which can lead to bugs.

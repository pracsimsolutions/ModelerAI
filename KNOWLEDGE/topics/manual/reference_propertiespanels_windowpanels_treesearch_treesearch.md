---
id: reference_propertiespanels_windowpanels_treesearch_treesearch
name: "Tree Search"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","WindowPanels","TreeSearch","TreeSearch"]
tags: ["manual","reference","propertiespanels","windowpanels"]
source: "manual/Reference/PropertiesPanels/WindowPanels/TreeSearch/TreeSearch.html"
---

# Tree Search

The Tree Search panel allows you to search through text and node names in the FlexSim Tree. The search will begin at the highlighted node in the active tree window. If no node is highlighted, the search will begin at the top of the active tree window. The search recursively searches through all subnodes and object attribute nodes. Replace allows you to replace all occurrences of the found text. This can be applied to text and/or node names.

 

 

The following properties are on the Tree Search panel:

 

### Find

 

Finds the specified text (or press the Enter key in the field).

 

### Replace

 

Finds the above text and replaces it with the specified text (or press the Enter key in the field).

 

### Search text

 

Searches through all text under the selected node.

 

### Match Whole Word

 

If checked, only finds whole word matches of the text (meaning characters immediately before and after the search text must be non-alphabetic characters).

 

### Search names

 

Searches through all node names under the selected node.

 

### Match Whole Name

 

If unchecked, the search will return any node name that contains the searched text.

 

### Match Case

 

If unchecked, the search will find all text/names containing the search text, regardless of capitalization. For example, if you search for "myvariablename", the results will still return all nodes containing "MyVariableName".

 

### The Find Results Window

 

Once a search is begun, the Find Results window will appear:

 

 

Double-click on a result to view the text in a Code Editor window. You may also right click the result to Edit Code or explore the tree.

 

Nodes toggled as FlexScript will have an S to the left of the node path. Nodes toggled as C++ will display a C and nodes toggled as DLL linked will display a D.

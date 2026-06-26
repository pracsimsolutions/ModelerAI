---
id: xml__nodeplacement
name: "XML::NodePlacement"
kind: class
module: "XML"
signature: "XML::NodePlacement"
aliases: []
tags: ["class", "xml-module"]
deprecated: false
replacedBy: null
description: "An enumeration of integer values associated with XML node placement. Primarily used with the XML.Node's create functions. XML.NodePlacement. Last is default behavior."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\XML.NodePlacement.xml"
---

# XML::NodePlacement

An enumeration of integer values associated with XML node placement. Primarily used with the XML.Node's create functions. XML.NodePlacement. Last is default behavior.

## Properties

### After

```flexscript
int XML::NodePlacement.After (readonly)
```

Place this new node after another node.

XML.Document doc;
XML.Element root = doc.createElement("root");
var element = root.createElement("firstChild");
root.createElement("lastChild");
root.createElement("middleChild", XML.NodePlacement.After, element);

### First

```flexscript
int XML::NodePlacement.First (readonly)
```

Add the new node as the first child of the current node.

XML.Document doc = XML.loadFile("test.xml");
doc.createComment("my comment", XML.NodePlacement.First);

### Last

```flexscript
int XML::NodePlacement.Last (readonly)
```

Add the new node as the last child of the current node. This is default behavior.

XML.Document doc = XML.loadFile("test.xml");
doc.createComment("my comment", XML.NodePlacement.Last);
// Adding XML.NodePlacement.Last is redundant, you can leave it out


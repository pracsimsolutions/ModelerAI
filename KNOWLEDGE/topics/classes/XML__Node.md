---
id: xml__node
name: "XML::Node"
kind: class
module: "XML"
signature: "XML::Node"
aliases: []
tags: ["class", "xml-module"]
deprecated: false
replacedBy: null
description: "A base class for other XML Nodes."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\XML.Node.xml"
---

# XML::Node

A base class for other XML Nodes.

## Methods

### createComment

```flexscript
XML.Comment XML::Node.createComment(string comment, int placement = XML.NodePlacement.Last, XML.Node afterThis = nullvar)
```

Create an XML.Comment child node.

**Parameters:**
- `comment` *(string)* — The comment text.
- `placement` *(int, default XML.NodePlacement.Last)* — Where the comment will be placed.
- `afterThis` *(XML.Node, default nullvar)* — The child the new comment will be placed after.

**Returns:** An XML.Comment object.

This example code will place the comment "my comment" at the beginning of the file.

XML.Document doc = XML.loadFile("test.xml");
doc.createComment("my comment", XML.NodePlacement.First);

### createDeclaration

```flexscript
XML.Declaration XML::Node.createDeclaration(string declaration = 0, int placement = XML.Placement.Last, XML.Node afterThis = nullvar)
```

Create an XML.Declaration child node.

**Parameters:**
- `declaration` *(string, default 0)* — Replace the default declaration with your own.
- `placement` *(int, default XML.Placement.Last)* — Where the declaration will be placed.
- `afterThis` *(XML.Node, default nullvar)* — The child the new declaration will be placed after.

**Returns:** An XML.Declaration object.

This example code will create a new document and set the first child to be the default declaration.
The default declaration is: xml version="1.0" encoding="UTF-8".

XML.Document doc;
doc.createDeclaration();

### createElement

```flexscript
XML.Element XML::Node.createElement(string name, int placement = XML.NodePlacement.Last, XML.Node afterThis = nullvar)
```

Create an XML.Element child node.

**Parameters:**
- `name` *(string)* — The name of the tag of the new element.
- `placement` *(int, default XML.NodePlacement.Last)* — Where the element will be placed.
- `afterThis` *(XML.Node, default nullvar)* — The child the new element will be placed after.

**Returns:** An XML.Element object.

This example code creates a blank document with a root element and subnode elements.
The last line demonstrates how you might use the additional parameters.

XML.Document doc;
XML.Element root = doc.createElement("root");
var element = root.createElement("firstChild");
root.createElement("lastChild");
root.createElement("middleChild", XML.NodePlacement.After, element);

### createText

```flexscript
XML.Text XML::Node.createText(string text, int placement = XML.NodePlacement.Last, XML.Node afterThis = nullvar)
```

Create an XML.Text child node.

**Parameters:**
- `text` *(string)* — The text string.
- `placement` *(int, default XML.NodePlacement.Last)* — Where the text will be placed.
- `afterThis` *(XML.Node, default nullvar)* — The child the new text will be placed after.

**Returns:** An XML.Text object.

XML.Document doc;
var students = doc.createElement("students");
var student = root.createElement("student");
child.attrs.name = "John Smith";
var gradesNode = child.createElement("grades");

Array classes = ["English", "Math", "Science", "Art"];
Array grades = [52, 98, 97.3, 12];
for (int i = 1; i <= classes.length; i++) {
var class = gradesNode.createElement(classes[i]);
class.createText(string.fromNum(grades[i]));
}

doc.saveAs("grades.xml");

### createUnknown

```flexscript
XML.Unknown XML::Node.createUnknown(string text, int placement = XML.NodePlacement.Last, XML.Node afterThis = nullvar)
```

Create an XML.Unknown child node.

**Parameters:**
- `text` *(string)* — The tag contents.
- `placement` *(int, default XML.NodePlacement.Last)* — Where the unknown node will be placed.
- `afterThis` *(XML.Node, default nullvar)* — The child the new unknown node will be placed after.

**Returns:** An XML.Unknown object.

XML.Document doc;
doc.createUnknown("testing this unknown thing 153");
// results in the following tag
// <!testing this unknown thing 153>

### deleteChild

```flexscript
XML::Node.deleteChild(XML.Node child)
```

Delete a child from this node.

**Parameters:**
- `child` *(XML.Node)* — The child node to be deleted.

This example code deletes the first child from the root node of the document.

XML.Document doc = XML.loadFile("test.xml");
var root = doc.root;
var first = root.first;
root.deleteChild(first);
doc.saveAs("test1.xml");

### deleteChildren

```flexscript
XML::Node.deleteChildren()
```

Deletes all children from this node.

This example code deletes all the children from the root node of the document.

XML.Document doc = XML.loadFile("test.xml");
var root = doc.root;
root.deleteChildren();
doc.saveAs("test1.xml");

## Properties

### first

```flexscript
XML.Node XML::Node.first (readonly)
```

Get the first child of the node, null if there are no children.

XML.Document doc = XML.loadFile("test.xml");
XML.Declaration dec = doc.first;

### last

```flexscript
XML.Node XML::Node.last (readonly)
```

Get the last child of the node, null if there are no children.

XML.Document doc = XML.loadFile("test.xml");
XML.Element element = doc.last;

### next

```flexscript
XML.Node XML::Node.next (readonly)
```

Get the next sibling of the node, null if there is no next sibling.

This example code loops through each child of the root node and prints its value.

XML.Document doc = XML.loadFile("test.xml");
XML.Element root = doc.root;
XML.Element element = root.first;
while (element) {
print("value:", element.value);
element = element.next;
}

### prev

```flexscript
XML.Node XML::Node.prev (readonly)
```

Get the previous silbing of the node, null if there is no previous sibling.

XML.Document doc = XML.loadFile("test.xml");
XML.Element root = doc.root;
XML.Element element = root.last;
XML.Element sibling = element.prev;

### up

```flexscript
XML.Node XML::Node.up (readonly)
```

Get the parent of the node, null if the current node is a Document.

XML.Document doc = XML.loadFile("test.xml");
XML.Element root = doc.root;
XML.Element element = root.first;
...
XML.Element parent = element.up; // parent == root

### value

```flexscript
string XML::Node.value (readonly)
```

Returns the value of the node. The meaning of 'value' changes for the specific type.

Document: Empty
Element: Name of the element
Comment: The comment text
Unknown: The tag contents
Text: The text string


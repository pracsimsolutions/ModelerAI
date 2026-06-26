---
id: xml__element
name: "XML::Element"
kind: class
module: "XML"
signature: "XML::Element"
aliases: []
tags: ["class", "xml-module"]
deprecated: false
replacedBy: null
description: "A class that represents an XML Element."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\XML.Element.xml"
---

# XML::Element

A class that represents an XML Element.

## Methods

### getText

```flexscript
string XML::Element.getText()
```

Returns text (if any) found in the first child of the element.

**Returns:** The text string value.

XML.Document doc = XML.loadFile("test.xml");
var root = doc.root;
var first = root.first;
string text = first.getText();

## Properties

### attrs

```flexscript
XML::Element.attrs (readonly)
```

An AttributeMap which manages element attributes.

attrs is a dynamic property similar to the Process Flow token.labels.
That means you can add new attributes, reassign them, and retrieve them like this:

XML.Document doc = XML.loadFile("test.xml");
XML.Element root = doc.root;
root.newAttribute = 5; // create and assign new attribute
root.newAttribute = "house"; // reassign attribute
string val = root.newAttribute; // get attribute

Additionally, there are several functions for explicitly getting, setting, and deleting.

XML.Document doc = XML.loadFile("test.xml");
XML.Element root = doc.root;
root.set("newAttribute", 5); // create and assign new attribute
root.set("newAttribute", "house"); // reassign attribute
string val = root.get("newAttribute"); // get attribute
root.deleteAttr("newAttribute"); // delete attribute

If you try to retrieve an attribute that hasn't been assign yet, it will throw an error.
However, you can query whether an attribute exists using the "has" function.

XML.Document doc = XML.loadFile("test.xml");
XML.Element root = doc.root;
Variant v = 0;
if (root.has("oldAttribute")) {
print("I've got it!");
v = root.oldAttribute;
} else {
print("I don't have it!");
v = "new value";
}


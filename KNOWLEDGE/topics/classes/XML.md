---
id: xml
name: "XML"
kind: class
module: ""
signature: "XML"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "Namespace for the XML functions. Contains a few static functions."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\XML.xml"
---

# XML

Namespace for the XML functions. Contains a few static functions.

## Methods

### load

```flexscript
XML.Document (static) XML.load(string xml)
```

Load a string of XML into an XML document.

**Parameters:**
- `xml` *(string)* — The string of text to be parsed as XML and loaded into a document.

**Returns:** An XML.Document object.

string xml = "<MyElement>Some text</MyElement>";
XML.Document doc = XML.load(xml);
...

### loadFile

```flexscript
XML.Document (static) XML.loadFile(string filePath)
```

Load a file into an XML document.

**Parameters:**
- `filePath` *(string)* — The file name to be loaded into a document.

**Returns:** An XML.Document object.

string fileName = "test.xml";
XML.Document doc = XML.loadFile(fileName);
...


---
id: xml__document
name: "XML::Document"
kind: class
module: "XML"
signature: "XML::Document"
aliases: []
tags: ["class", "xml-module"]
deprecated: false
replacedBy: null
description: "A class that represents an XML Document."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\XML.Document.xml"
---

# XML::Document

A class that represents an XML Document.

## Methods

### saveAs

```flexscript
XML::Document.saveAs(string filePath, int compact = 0)
```

Saves the XML file to disk. Returns XML_NO_ERROR on success, or an errorID (see loadError above).

**Parameters:**
- `filePath` *(string)* — The file path to save your document to.
- `compact` *(int, default 0)* — If 1, will only keep required whitespace and newlines. Otherwise, leave it as is.

You can provide a relative path or an absolute path.

XML.Document doc;
...
doc.saveAs("test1.xml"); // equivalent to: doc.saveAs(modeldir() + "test1.xml");

## Properties

### loadError

```flexscript
int XML::Document.loadError (readonly)
```

The integer value of the error encountered while loading a document.

XML.Document doc = XML.loadFile("test.xml");
if (!doc) {
return doc.loadError;
}

### loadErrorName

```flexscript
string XML::Document.loadErrorName (readonly)
```

The string name of the error encountered while loading a document.

Here's a list of error names:

XML_NO_ERROR
XML_NO_ATTRIBUTE
XML_WRONG_ATTRIBUTE_TYPE
XML_ERROR_FILE_NOT_FOUND
XML_ERROR_FILE_COULD_NOT_BE_OPENED
XML_ERROR_FILE_READ_ERROR
XML_ERROR_ELEMENT_MISMATCH
XML_ERROR_PARSING_ELEMENT
XML_ERROR_PARSING_ATTRIBUTE
XML_ERROR_IDENTIFYING_TAG
XML_ERROR_PARSING_TEXT
XML_ERROR_PARSING_CDATA
XML_ERROR_PARSING_COMMENT
XML_ERROR_PARSING_DECLARATION
XML_ERROR_PARSING_UNKNOWN
XML_ERROR_EMPTY_DOCUMENT
XML_ERROR_MISMATCHED_ELEMENT
XML_ERROR_PARSING
XML_CAN_NOT_CONVERT_TEXT
XML_NO_TEXT_NODE

XML.Document doc = XML.loadFile("test.xml");
if (!doc) {
print("Load Error:", doc.loadErrorName);
return doc.loadError;
}

### root

```flexscript
XML.Element XML::Document.root (readonly)
```

Return the root element of the document.

XML.Document doc = XML.loadFile("test.xml");
XML.Element root = doc.root;
...


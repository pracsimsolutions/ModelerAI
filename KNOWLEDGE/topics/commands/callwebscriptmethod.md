---
id: callwebscriptmethod
name: "callwebscriptmethod"
kind: command
module: ""
signature: "callwebscriptmethod(obj view, str domid, str methodname, int type [, num/str data ...])"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: callwebscriptmethod."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# callwebscriptmethod

```flexscript
callwebscriptmethod(obj view, str domid, str methodname, int type [, num/str data ...])
```

Call a method on a DOM element in the HTML document of an HTML view. You must define the type of data of any parameters that are to be passed
into the method. The following types are available:
CWSM_DOUBLE
CWSM_STRING
CWSM_NODE
CWSM_BUNDLE
CWSM_BUNDLE_HEADER
CWSM_BUNDLE_DATA
CWSM_JSON
The last parameter passed into the method must be CWSM_END.
When using the CWSM_JSON type, the json string must start with json:. For example, json:{"title":"My Title"}

## Example

```flexscript
callwebscriptmethod(view, "chart", "updateData", CWSM_DOUBLE, time(), CWSM_NODE, getvarnode(current, "data"), CWSM_STRING, "My Title", CWSM_END);
This examples calls the method named updateData on the element with id "chart".
```


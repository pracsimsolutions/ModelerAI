---
id: agv__request
name: "AGV::Request"
kind: class
module: "AGV"
signature: "AGV::Request"
aliases: []
tags: ["class", "agv-module"]
deprecated: false
replacedBy: null
description: "An enumeration of values to be returned by an AGV.AllocatableObject's OnRequest trigger."
seeAlso: []
source: "help\\FlexScriptAPIReference\\AGV\\AGV.Request.xml"
---

# AGV::Request

An enumeration of values to be returned by an AGV.AllocatableObject's OnRequest trigger.

If you return one of
these values from the trigger, then the object (a Control Point or Control Area) will override
its default behavior and either allow or block the allocation request.

## Properties

### Allow

```flexscript
int AGV::Request.Allow (readonly)
```

Allow the request to allocate the object.

### Block

```flexscript
int AGV::Request.Block (readonly)
```

Block the request from allocating the object.


---
id: http__method
name: "Http::Method"
kind: class
module: "Http"
signature: "Http::Method"
aliases: []
tags: ["class", "http-module"]
deprecated: false
replacedBy: null
description: "An enumeration of integer values representing Http Request Methods."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Http\\Http.Method.xml"
---

# Http::Method

An enumeration of integer values representing Http Request Methods.

The Http.Method values represent the method used in Http.Request.method.

## Properties

### Get

```flexscript
int Http::Method.Get (readonly)
```

Method to request data.

### Post

```flexscript
int Http::Method.Post (readonly)
```

Method to create or replace a target resource with specified data. Is not idempotent.

### Put

```flexscript
int Http::Method.Put (readonly)
```

Method to create or replace a target resource with specified data. Is idempotent.


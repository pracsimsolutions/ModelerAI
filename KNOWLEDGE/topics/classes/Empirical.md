---
id: empirical
name: "Empirical"
kind: class
module: ""
signature: "Empirical"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class that represents an Empirical Distribution."
seeAlso: []
source: "help\\FlexScriptAPIReference\\EmpiricalDistribution\\EmpiricalDistribution.xml"
---

# Empirical

A class that represents an Empirical Distribution.

## Methods

### Empirical

```flexscript
Agent Empirical.Empirical(string name)
```

Creates a reference to a defined Empirical Distribution tool.

**Parameters:**
- `name` *(string)* — The name of a defined Empirical Distribution tool.

Empirical distribution1 = Empirical("Distribution1");

### get

```flexscript
Variant Empirical.get(int stream = 0)
```

Returns a random variate from a defined Empirical Distribution tool.

**Parameters:**
- `stream` *(int, default 0)* — the stream your distribution will use.

return Empirical("Distribution1").get(getstream(current));


---
id: licenseactivation
name: "licenseactivation"
kind: command
module: ""
signature: "licenseactivation(num operation[, str buffer, var p1, node callback]) -> num"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: licenseactivation."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# licenseactivation

```flexscript
licenseactivation(num operation[, str buffer, var p1, node callback]) -> num
```

For developer use. Do various licensing operations.

## Example

```flexscript
licenseactivation(LA_SET_ACTIVATION_SERVER_PARAMS,"@localhost",0);
```


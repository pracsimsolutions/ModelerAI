---
id: getdrawtextmetric
name: "getdrawtextmetric"
kind: command
module: ""
signature: "getdrawtextmetric(str text, num fontsize, num metric[, num a]) -> num"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "FlexScript command: getdrawtextmetric."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getdrawtextmetric

```flexscript
getdrawtextmetric(str text, num fontsize, num metric[, num a]) -> num
```

Returns information about the text. If metric is 1, it will return the width of the text. If metric is 2, it will return the height of the text. If metric is 3, it will return the max fitting length of the text using 'a' as the max extent.

## Example

```flexscript
getdrawtextmetric("Hello",12,1,0);
```


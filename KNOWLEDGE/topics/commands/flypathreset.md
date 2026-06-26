---
id: flypathreset
name: "flypathreset"
kind: command
module: ""
signature: "flypathreset(obj view, num pathno)"
aliases: []
tags: ["command", "group-media"]
deprecated: false
replacedBy: null
description: "FlexScript command: flypathreset."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# flypathreset

```flexscript
flypathreset(obj view, num pathno)
```

For developer use. This command clears the points from the flypath numbered pathno in the view specified as view. Once the points have been cleared, a new set of flypath points can be generated.

## Example

```flexscript
flypathreset(ownerview(c),1);
This clears the flypath points out of flypath 1 in the view containing an event's code.
```


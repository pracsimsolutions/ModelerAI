---
id: flypathadd
name: "flypathadd"
kind: command
module: ""
signature: "flypathadd(obj view, num pathno)"
aliases: []
tags: ["command", "group-media"]
deprecated: false
replacedBy: null
description: "FlexScript command: flypathadd."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# flypathadd

```flexscript
flypathadd(obj view, num pathno)
```

For developer use. This command adds a flypath to the flypath numbered pathno in the view designated specified as view. Any view window can be passed as view.

## Example

```flexscript
flypathadd(ownerview(c),1);
This will add a flypath to flypath number 1 in the view window containing an event's code.
```


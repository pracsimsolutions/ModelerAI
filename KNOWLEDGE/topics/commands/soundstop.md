---
id: soundstop
name: "soundstop"
kind: command
module: ""
signature: "soundstop(obj view, num sound)"
aliases: []
tags: ["command", "group-media"]
deprecated: false
replacedBy: null
description: "Stop a sound playing"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# soundstop

```flexscript
soundstop(obj view, num sound)
```

Stop a sound playing

Stop a sound playing. Sounds are attached to a window by the operating system, so the view parameter is important.

## Example

```flexscript
soundstop(sv(),1)
```


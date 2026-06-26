---
id: soundplay
name: "soundplay"
kind: command
module: ""
signature: "soundplay(obj view, num soundindex, num volume [, num tempo, num repeat y/n, num balance])"
aliases: []
tags: ["command", "group-media"]
deprecated: false
replacedBy: "getsoundindex"
description: "Play a loaded sound"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# soundplay

```flexscript
soundplay(obj view, num soundindex, num volume [, num tempo, num repeat y/n, num balance])
```

Play a loaded sound

Play a loaded sound. Sounds are attached to a window by the operating system, so the view parameter is important. To get the soundindex use getsoundindex(). The volume, tempo and balance are numbers between 0 and 1. To continuosly repeat the sound, use a repeat of 1 else 0.

## Example

```flexscript
soundplay(sv(),3,.75,.25,1,.5)
Continuously play sound index 3 in the selected view with 3/4 volume, and 1/4 tempo and balanced evenly across both speakers.
```


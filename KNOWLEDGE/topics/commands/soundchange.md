---
id: soundchange
name: "soundchange"
kind: command
module: ""
signature: "soundchange(obj view, num sound, num volume, num tempo, num repeat, num balance)"
aliases: []
tags: ["command", "group-media"]
deprecated: false
replacedBy: "getsoundindex"
description: "Change a currently playing sond"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# soundchange

```flexscript
soundchange(obj view, num sound, num volume, num tempo, num repeat, num balance)
```

Change a currently playing sond

Change a currently playing sound. Sounds are attached to a window by the operating system, so the view parameter is important. To get the soundindex use getsoundindex(). The volume, tempo and balance are numbers between 0 and 1. To continuosly repeat the sound, use a repeat of 1 else 0. Interesting affects are achieved when the volume and balance are changed based on the viewpoint relative to an object making the object appear to be emitting the sound.

## Example

```flexscript
soundchange(sv(),1,1,1,1,.5)
```


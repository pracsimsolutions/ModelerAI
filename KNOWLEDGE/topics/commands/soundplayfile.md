---
id: soundplayfile
name: "soundplayfile"
kind: command
module: ""
signature: "soundplayfile(obj view, str path+filename, num volume [, num tempo, num repeat, num balance])"
aliases: []
tags: ["command", "group-media"]
deprecated: false
replacedBy: "a"
description: "Play a sound directly from a file"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# soundplayfile

```flexscript
soundplayfile(obj view, str path+filename, num volume [, num tempo, num repeat, num balance])
```

Play a sound directly from a file

Play a sound directly from a file. Sounds are attached to a window by the operating system, so the view parameter is important. The volume, tempo and balance are numbers between 0 and 1. To continuosly repeat the sound, use a repeat of 1 else 0.

## Example

```flexscript
soundplayfile(sv(),"C:/My Sounds/APPLAUSE.WAV",1,1,0,.5);
```


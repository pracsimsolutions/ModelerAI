---
id: soundload
name: "soundload"
kind: command
module: ""
signature: "soundload(str path+filename)"
aliases: []
tags: ["command", "group-media"]
deprecated: false
replacedBy: null
description: "Load a sound into FlexSim"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# soundload

```flexscript
soundload(str path+filename)
```

Load a sound into FlexSim

A sound file on disk will be loaded into FlexSim and recorded in the Media List. The media list of FlexSim will keep a list of all loaded sounds. Each sound has a corresponding name and an index. This command will return the index corresponding to the loaded sound. Microsoft Wave Format files (.wav) can be loaded and played. Sounds can have their volume, balance and frequency adjusted dynamically. Multiple channels are managed through the Microsoft DirectSound Interface.

## Example

```flexscript
int applauseindex = soundload("c:/My Music/APPLAUSE.WAV");
```


---
id: loadsound
name: "loadsound"
kind: command
module: ""
signature: "loadsound(str path+filename) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "soundload"
description: "Deprecated, use soundload()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# loadsound

> ⚠ **DEPRECATED.** Use `soundload` instead.

```flexscript
loadsound(str path+filename) -> num
```

Deprecated, use soundload()

This command is deprecated. Use soundload() instead.

A sound file on disk will be loaded into FlexSim and recorded in the Media List. The media list of FlexSim will keep a list of all loaded sounds. Each sound has a corresponding name and an index. This command will return the index corresponding to the loaded sound. The sound must be a .wav file.

## Example

```flexscript
int applauseindex = loadsound("c:/My Music/APPLAUSE.WAV");
```


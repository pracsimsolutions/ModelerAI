---
id: resumeobject
name: "resumeobject"
kind: command
module: ""
signature: "resumeobject(obj object [, num id, num stateprofile ] )"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Object.resume"
description: "Deprecated, use Object.resume()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# resumeobject

> ⚠ **DEPRECATED.** Use `Object.resume` instead.

```flexscript
resumeobject(obj object [, num id, num stateprofile ] )
```

Deprecated, use Object.resume()

This command is deprecated. Use Object.resume() instead.

Tells the object that it may resume whatever it was doing before stopobject() was called on it. The id value is optional and is for matching stop requests with resume requests. Refer to the stopobject()documentation for more information on this.

## Example

```flexscript
resumeobject(current.centerObjects[1]);
```


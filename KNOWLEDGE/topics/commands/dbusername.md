---
id: dbusername
name: "dbusername"
kind: command
module: ""
signature: "dbusername(str username, str password)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Database.Connection"
description: "Deprecated. Use Database.Connection"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# dbusername

> ⚠ **DEPRECATED.** Use `Database.Connection` instead.

```flexscript
dbusername(str username, str password)
```

Deprecated. Use Database.Connection

This command is deprecated. Use Database.Connection instead.

## Example

```flexscript
dbusername("user1","abc123XYZ");
This sets the username to "user1" and the password to "abc123XYZ". These values will be used to access a database the next time dbopen() is called.
dbusername("","");
This clears out the last set username and password. The next time dbopen() is called, the database will be accessed without a username or password.
```


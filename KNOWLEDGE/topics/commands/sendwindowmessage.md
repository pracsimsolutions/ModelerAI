---
id: sendwindowmessage
name: "sendwindowmessage"
kind: command
module: ""
signature: "sendwindowmessage(HWND window, num msg[, num/str/node wParam, num/str/node lParam])"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: "windowfromnode"
description: "FlexScript command: sendwindowmessage."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# sendwindowmessage

```flexscript
sendwindowmessage(HWND window, num msg[, num/str/node wParam, num/str/node lParam])
```

For developer use. This gives direct access to the Windows API SendMessage() command, sending a message to a window. Use windowfromnode() or getviewhwnd() to get access to the window handle. Refer to Windows API documentation for more information. Not that you cannot use a message that uses lParam or wParam as an [out] parameter, or one that you pass in as a pointer to a struct. You should be able to pass strings into lParam or wParam, but again thay cannot be [out] parameters. FlexSim does not include all possible WM_ messages in its flexscript macros, so if you want to use one in flexscript, you will need to find the definition in the windows header files and add the definition to your flexscript code.

## Example

```flexscript
sendwindowmessage(windowfromnode(view), WM_COPY, 0,0);
```


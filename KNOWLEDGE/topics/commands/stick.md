---
id: stick
name: "stick"
kind: command
module: ""
signature: "stick(num stickno, num command [, HWND/num win, var/num p]) -> var"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: "the"
description: "FlexScript command: stick."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# stick

```flexscript
stick(num stickno, num command [, HWND/num win, var/num p]) -> var
```

For developer use. This command queries the joystick or controller state or sets functionality for a joystick or controller.

stickno defines an id for the joystick or gamepad you want to query. This is a number between 0 and 32.
Usually you will only have one joystick connected to your computer, so in this case you should use the value 1 as stickno.
If you are using a gamepad, you might need to use stickno 2.
For Xbox controllers, stickno will be a value between 0 and 3.
For VR controllers, stickno will be either 0 or 1.
For Oculus touch controllers, stickno 0 is the left hand and stickno 1 is the right hand.
Use stickno -1 to get the status, position, or rotation of a connected VR headset.

command defines the operation that the command should perform. This should be one of the macros below:

STICK_JOY_STATUS - Queries the connectivity of the joystick. If the joystick is connected properly,
then this command will return 0. If the return value is non-zero, then there is some error with the joystick's connectivity,
and you can view the system console to see what the error is. The following errors apply:
MMSYSERR_NODRIVER - The joystick driver is not present.
MMSYSERR_INVALPARAM - Invalid joystick ID or hwnd is NULL.
JOYERR_NOCANDO - Cannot capture joystick input because a required service (such as a Windows timer) is unavailable.
JOYERR_UNPLUGGED - The specified joystick is not connected to the system.
JOYERR_PARMS - Invalid joystick ID or hwnd is NULL.

STICK_JOY_XPOS - Current X-coordinate of the joystick. This is a number between 0 and 65535. 0 means the joystick is pushed all the way to the left, 65535 means it is pushed all the way to the right, and 32768 is exactly in the center. If you are using a gamepad, then this is the x position of the left side joystick.

STICK_JOY_YPOS - Current Y-coordinate of the joystick. Again this is a number between 0 and 65535. 0 is up, 65535 is down. If you are using a gamepad, this is the position of the gamepad's left joystick.

STICK_JOY_ZPOS - Current Z-coordinate of the joystick. If you are using a joystick, this will usually be the current setting of the joystick's throttle. Again it is a value between 0 and 65535. If you are using a gamepad, this represents the y position of the right side joystick.

STICK_JOY_BUTTONS - Current state of the 32 joystick buttons. This is bitwise integer with each bit representing the state of a given button. For a joystick, the main trigger button is usually represented by the 0x00000001 bit, other buttons on the stick are 0x00000002 and 0x00000004, and buttons on the base of the joystick are then higher order bits. You will need to test these out on your own as each joystick is a little different.

STICK_JOY_SET_CAPTURE - This command sets the joystick capture to a FlexSim window that you specify. This command requires the win and p parameters to be passed in. By executing this command, joystick messages will be sent to the OnStick attribute of the view. For example, if you have an ortho view that you would like to listen to joystick events, you would call this command, passing the ortho view's HWND handle (use windowfromnode()), and a polling period. The period specifies a time interval in milliseconds to repeatedly call the OnStick event. If p is 0, then the OnStick event will only be called when the joystick's state changes. If p is greater than 0, then the OnStick event will simply be called at the end of each sample period interval. Example: stick(1,5, windowfromnode(sv()), 200); This will cause the OnStick event to be called every 200 milliseconds. In order for this to work, you need to add an OnStick attribute to the view (you may also need to rebind the view's attributes after adding it: right-click->Edit|Rebind Object Attributes).

STICK_JOY_RELEASE_CAPTURE - This command releases the joystick capture for the window that was previously captured with the 5 command. You need to pass in the win parameter as with command 5.

STICK_JOY_RPOS - Current position of the rudder or fourth joystick axis of the joystick. For a joystick, this is usually the rotation of the joystick if your joystick can rotate around its z axis. Again it is a number between 0 and 65535. If you are using a gamepad, it is the x position of the right side joystick.

STICK_JOY_UPOS - Current fifth axis position of the joystick.

STICK_JOY_VPOS - Current sixth axis position of the joystick.

STICK_JOY_POV - Current position of the point-of-view control of the joystick. It is a number between 0 and 35900 and represents the number of degrees times 100. For a gamepad, this represents the directional pad on the left of the gamepad. Up is 0, right is 9000, down is 18000, left is 27000. If the directional pad is not pressed, the value will be 65535. If you are using a joystick, there may be a directional thumb controller on the stick itself that you can press up, down, right, or left, and this will get the setting of that controller.

STICK_JOY_CURRENT_BUTTON - Current button number that is pressed.

STICK_JOY_FLAGS - Flags indicating the valid information returned about the joystick.

STICK_XBOX_STATUS - Queries the connectivity of the Xbox controller. If the controller is connected properly, this returns 0.

STICK_XBOX_BUTTONS - Bitmask of the device digital buttons. A set bit indicates that the corresponding button is pressed.

STICK_XBOX_LEFT_THUMB_X - Left thumbstick x-axis value. Each of the thumbstick axis members is a signed value between -32768 and 32767 describing the position of the thumbstick. A value of 0 is centered. Negative values signify down or to the left. Positive values signify up or to the right.

STICK_XBOX_LEFT_THUMB_Y - Left thumbstick y-axis value. The value is between -32768 and 32767.

STICK_XBOX_RIGHT_THUMB_X - Right thumbstick x-axis value. The value is between -32768 and 32767.

STICK_XBOX_RIGHT_THUMB_Y - Right thumbstick y-axis value. The value is between -32768 and 32767.

STICK_XBOX_LEFT_TRIGGER - The current value of the left trigger analog control. The value is between 0 and 255.

STICK_XBOX_RIGHT_TRIGGER - The current value of the right trigger analog control. The value is between 0 and 255.

STICK_VR_STATUS - Returns 1 if an Oculus Rift controller is active. Returns 2 if an HTC Vive controller is active. Returns 0 if the controller is inactive. Returns nullvar if the controller is not attached.

STICK_VR_THUMB_X - Thumbstick or trackpad x-axis value between -1 and 1.

STICK_VR_THUMB_Y - Thumbstick or trackpad y-axis value between -1 and 1.

STICK_VR_INDEX_TRIGGER - Index trigger value between 0 and 1.

STICK_VR_GRIP_TRIGGER - Grip trigger value between 0 and 1. On HTC Vive, grip button is pressed.

STICK_VR_BUTTON_1 - The first button is pressed. A or X on Oculus Rift. Unused on HTC Vive.

STICK_VR_BUTTON_2 - The second button is pressed. B or Y on Oculus Rift. Unused on HTC Vive.

STICK_VR_BUTTON_3 - The third button is pressed. Enter or Home on Oculus Rift. Menu on HTC Vive.

STICK_VR_BUTTON_4 - The fourth button is pressed. Thumbstick on Oculus Rift. Trackpad on HTC Vive.

STICK_VR_TOUCH_BUTTON_1 - The first button is touched. Unused on HTC Vive.

STICK_VR_TOUCH_BUTTON_2 - The second button is touched. Unused on HTC Vive.

STICK_VR_TOUCH_BUTTON_3 - The third button is touched. Unused on Oculus Rift.

STICK_VR_TOUCH_BUTTON_4 - The fourth button is touched.

STICK_VR_TOUCH_THUMB_REST - The thumb rest sensor is touched on Oculus Rift. Unused on HTC Vive.

STICK_VR_TOUCH_INDEX_TRIGGER - The index trigger sensor is touched.

STICK_VR_TOUCH_POINTING - The hand gesture is pointing on Oculus Rift. Unused on HTC Vive.

STICK_VR_TOUCH_THUMB_UP - The hand gesture is thumbs-up on Oculus Rift. Unused on HTC Vive.

STICK_VR_HAND_POSITION - The controller position in VR space.

STICK_VR_HAND_ROTATION - The controller rotation in VR rotation coordinates.

STICK_VR_HAND_WORLD_POSITION - The controller position in FlexSim world space.

STICK_VR_HAND_WORLD_ROTATION - The controller rotation in FlexSim rotation coordinates.

STICK_VR_HAND_OFFSET_POSITION - The controller's position in FlexSim world space, but with an offset applied based on the controller's rotation. Pass an array as the offset vector in the p parameter. You can use this option to get a point relative to the controller, such as a finger position or where the controller is pointing.

STICK_VR_TARGET_FLOOR_POSITION - The target position on the floor that the controller is pointing towards. Pass a value in the p parameter for the maximum distance away from the controller.

STICK_VR_SET_VIBRATION - Causes the controller to vibrate. On Oculus Rift, pass an array for the p parameter where the first element is a value between 0 and 1 for the frequency and the second element is a value between 0 and 1 for the amplitude. On HTC Vive, pass a duration in microseconds for the p parameter.

STICK_VR_SUBMIT_VIBRATION - Causes the controller to subtly vibrate. On original Oculus Touch controllers, pass an array of amplitudes (0 - 255) for the p parameter to be queued up and executed. Unused on HTC Vive or Oculus Rift S controllers.

## Example

```flexscript
stick(1, STICK_JOY_YPOS); This gets the y position of the joystick id 1
```


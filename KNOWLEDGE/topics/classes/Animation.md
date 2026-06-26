---
id: animation
name: "Animation"
kind: class
module: ""
signature: "Animation"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class that represents an object animation."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\Animation.xml"
---

# Animation

A class that represents an object animation.

## Methods

### resume

```flexscript
void Animation.resume(int resumeAtType = 0, double resumeAtVal = 0)
```

Resumes an animation from some point in the middle of the animation.

**Parameters:**
- `resumeAtType` *(int, default 0)* — Used in conjuction with resumeAtVal to define where in the animation to resume. Can be one of the following values: RESUME_AT_SAME_POS: (Default) Resumes the animation from the same point where it was last stopped. RESUME_AT_NEXT_KEYFRAME: Advances the animation to the next keyframe from the point where it was last stopped, then resumes. RESUME_AT_TIME: Resumes the animation at a specific time in the animation.
- `resumeAtVal` *(double, default 0)* — Only used if resumeAtType is RESUME_AT_TIME, in which case this value defines the time at which to resume.

current.animations["Walk"].resume();

### start

```flexscript
void Animation.start(int speedType = 0, double speedValue = 0, int repeatType = 0, double repeatValue = 0)
```

Starts an animation.

**Parameters:**
- `speedType` *(int, default 0)* — Used in conjuction with speedValue to manipulate the speed with which the animation is run. Can be one of the following values: SPEED_TYPE_DEFAULT: The animation will run at the standard speed. SPEED_TYPE_DURATION: The animation will run such that its total duration is equal to speedValue. SPEED_TYPE_MULTIPLIER: The animation's run speed will be multiplied by speedValue. In other words, a speedValue of 2 will make the animation run at double-speed.
- `speedValue` *(double, default 0)* — See speedType.
- `repeatType` *(int, default 0)* — Used in conjunction with repeatValue to define how the animation will repeat. If not REPEAT_TYPE_DEFAULT, this will override the repeat type and value specified in the Animation Creator. Valid values are as follows: REPEAT_TYPE_DEFAULT: The animation will use its default repeat information defined in the Animation Creator REPEAT_TYPE_NONE: The animation will run just once. REPEAT_TYPE_NR_ITERATIONS: The animation will repeat (repeatValue) times before finishing. REPEAT_TYPE_USE_TIME: The animation will repeat indefinitely, but will start the animation over once it gets (repeatValue) time into the animation, which may be before the end of the animation. REPEAT_TYPE_TIME_AFTER_END: The animation will repeat indefinitely, but will finish the animation and wait (repeatValue) time units before starting the animation again. REPEAT_TYPE_LAST_FRAME: The animation will run just once, but then repeat the last frame of the animation indefinitely.
- `repeatValue` *(double, default 0)* — See repeatType.

current.animations["Walk"].start();

### stop

```flexscript
void Animation.stop()
```

Stops an animation.

current.animations["Walk"].stop();

### update

```flexscript
void Animation.update(double updateTime = -1)
```

Updates an animation.

**Parameters:**
- `updateTime` *(double, default -1)* — The simulation time on which to base the update. If not passed, it will update based on the current simulation time.

The method will update all of the positions of an object's animation components based on the update time and
the keyframes defined in the animation. Usually you do not need to call this explicitly, since FlexSim's engine automatically
updates active animations when it repaints any 3D windows. However, if your simulation logic is dependent on sub-component
positions that change during an animation, you should call update() before getting those positions, or else the retrieved data
will be based on the last time a window was repainted. This will cause different results with each simulation run.
current.animations["Walk"].update();


---
id: reference_tools_simulationcontrolbar_simulationcontrolbar
name: "Simulation Control Bar"
kind: manual
breadcrumb: ["Reference","Tools","SimulationControlBar","SimulationControlBar"]
tags: ["manual","reference","tools","simulationcontrolbar"]
source: "manual/Reference/Tools/SimulationControlBar/SimulationControlBar.html"
---

# Simulation Control Bar

## Overview and Key Concepts

 

To run a simulation, you'll use the simulation control bar (also called the *control bar* for short), located underneath the main menu and toolbar. The control bar contains most of the tools and commands that you will need to run your simulation. The control bar essentially has three different groups of tools you can use to control how your simulation runs, as illustrated in the following image:

 

 

The following sections explains the different buttons on each section of the control bar and other properties.

 

 

 

## Run Control Buttons

 

The Run Control buttons are the basic tools you will use to start and stop your simulation, as shown in the following image:

 

 

These buttons are fairly intuitive to use since they closely resemble the playback controls on a media player. However, a few of the buttons possibly work differently than you might expect. For that reason, you should read the explanation of each button in the following table:

 

| Icon | Button Name | Purpose | Keyboard Shortcut |
| --- | --- | --- | --- |
|  | Reset | - Resets the simulation to the starting position and sets the time display (clock) on the run time tool to zero or to its starting date. - It clears out any items or products that are in the map and returns all the icons to their starting states. - NOTE: You should always press the Reset button when you are starting the simulation from its beginning. | Ctrl + left arrow |
|  | Run | - Begins the simulation. - While the simulation is running, you'll notice that the time display on the Run Time tool will begin to increase. | Ctrl + space bar |
|  | Stop | - Stops the simulation. - After pressing this button, the time display on the Run Time tool will stop increasing. - NOTE: It's probably better to think of the Stop button as functioning more like a *pause* button that only temporarily suspends the map simulation rather than ending it. If you press the Run button again, it will resume running the simulation from the point in time you pressed the Stop button. | Ctrl + space bar |
|  | Fast Forward | - Runs the model at maximum speed. - The run speed is set to maximum and the program stops updating most views. - Once the next model stop time is reached or the run is manually stopped, the previous run speed is restored and the program resumes updating views. | Ctrl + Shift + up arrow |
|  | Skip | - Steps to the next model event and then skips over any and all events at that same model time. - The model will stop after the last event occurs at that model time and won't keep running. - The Skip button will skip over bunches of events that all happen at the same time, essentially allowing you to quickly skip to the next eventful model time. | Ctrl + Shift + right arrow |
|  | Step | - Steps the clock forward to the time of the next event that should occur in the simulation model. - The model will stop at the moment that event occurs and won't keep running. - The Step button essentially allows you to quickly step through the events of the simulation one event at a time. | Ctrl + right arrow |

 

 

 

## Run Time Tools

 

The Run Time tools help you to monitor how much time has passed in the simulation and to define when the simulation should stop running.

 

 

### Simulation Clock

 

The simulation clock indicates the amount of time that has elapsed during the simulation run so far. The simulation clock will start at 0 time units and will progress from there.

 

### Run Time Options

 

If you click the drop-down arrow to the right of the simulation clock, the run time options will appear:

 

- **Display Mode** - Choose whether to display the time in model time units, date and time text, or both.
- **Start Time** - Set the date and time that correspond to time 0 in the simulation run.
- **Warmup Time** - Set the date and time that correspond to the simulation's warmup time.
- **Stop Times** - Set the time when the model should automatically stop. The model will only stop at the specified time if the stop time is enabled by checking the box in front of the stop time. You can add and enable as many stop times as desired. If no stop times are enabled or the model is run after the last enabled stop time, the model will run indefinitely until you press the Stop button.

 

 

 

## Run Speed Tools

 

The Run Speed tools allow you to change how fast the simulation will move while it is running.

 

 

### Run Speed

 

You can use the slider button to manually adjust the run speed.

 

### Simulation Speedometer

 

Displays the current speed at which the simulation model is running or will run.

 

### Speed Options

 

Select which speed you would like the simulation to run. Options are:

 

- **1.00** will run the model at a speed of one displayed simulation time unit per real second.
- **Maximum** is the fastest speed.
- **Custom** allows you to type the speed you would like in the box directly below it. Click the **Set** button to apply the new custom speed settings.

 

You can also change the **Display Mode** of the simulation speedometer so that it is defined as a ratio of some other unit per real second. This can be useful if you have different model events that take place over different time spans. In order to run your model in real time you can set the Display Mode to Simulation Seconds per Real Second and then set the run speed to 1.

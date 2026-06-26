---
id: modellogic_reinforcementlearning_workingwithgym_training_training
name: "Reinforcement Learning Training"
kind: manual
breadcrumb: ["ModelLogic","ReinforcementLearning","WorkingWithGym","Training","Training"]
tags: ["manual","modellogic","reinforcementlearning","workingwithgym"]
source: "manual/ModelLogic/ReinforcementLearning/WorkingWithGym/Training/Training.html"
---

# Reinforcement Learning Training

## Training Overview

 

Before beginning to train a reinforcement learning algorithm, you should ensure that you have reviewed Key Concepts About Reinforcement Learning. As part of preparing for training, you should have downloaded the included python scripts and configured an environment to run them. Now, we will modify and use those scripts as an example of how you can train a reinforcement learning algorithm using a FlexSim model as the environment where the agent learns.

 

 

 

## Preparing a FlexSim Model

 

Before we execute any scripts to begin training, we must first configure a FlexSim model so that it is ready to respond to the functions involved in training a reinforcement learning algorithm. For this example, we will build a simple example model so that we can focus on the reinforcement learning rather than on the simulation model itself.

 

In the model, we will have items that arrive with random item types between 1 and 5. We will process those items using a processor with a setup time that varies when changing item types. We will train an algorithm to pick which item to process next in order to maximize throughput. We will let the algorithm learn to minimize changeover times rather than explicitly programming rules to govern which item to pull next.

 

 

 

## Step 1 Creating a Simple Model

 

1. Drag a **Source**, **Queue**, **Processor**, and **Sink** into a new model.
1. Connect the objects as shown below.
1. In the **Toolbox** pane, add a **Global Table**.
1. In the Global Table's **Properties** pane, change the name to *ChangeoverTimes* and set the number of rows and columns to 5.
1. Enter the following data into the table. Each cell represents the amount of setup time required when processing different item types. For example, changing from item type 1 to 4 requires 30 seconds of setup time.
1. Click on the Processor to edit its **Properties**.
1. In the **Setup Time** field, select *From/To Lookup Table* from the dropdown.
1. Select *ChangeoverTimes* for the **Table** and leave the **Label** as *"Type"*.
1. Click on the Source to edit its **Properties**.
1. Add an **On Creation** trigger and select **Data > Set Label and Color**.
1. Modify the **Value** to be `duniform(1,5,getstream(current))`
1. **Save** the model with the name *ChangeoverTimesRL.fsm*
1. **Reset** and **Run** the model. Verify that the processor includes a setup time when changing item types.

 

 

 

## Step 2 Preparing Action and Observation Parameters

 

1. In the **Toolbox** pane, add a **Statistics > Model Parameter Table**.
1. Right-click on the new Parameters1 table and select **Rename**.
1. Type *Observations* as the new name for the table.
1. In the **Toolbox** pane, add another **Statistics > Model Parameter Table**.
1. Right-click on the new Parameters1 table and select **Rename**.
1. Type *Actions* as the new name for the table.
1. In the **Observations** table view, rename *Parameter2* to *LastItemType*.
1. In the **Observations** table view, click on the cell showing the value *1*.
1. Click on the dropdown button to open a popup to modify the properties of that Parameter.
1. Change the **Type** to *Integer* and the **Upper Bound** to *5*.
1. Click on blank space in the **Observations** table view to apply and close the popup.
1. In the **Actions** table view, rename *Parameter3* to *ItemType*.
1. In the **Actions** table view, click on the cell showing the value *1*.
1. Click on the dropdown button to open a popup to modify the properties of that Parameter.
1. Change the **Type** to *Integer* and the **Upper Bound** to *5*.
1. Click on blank space in the **Actions** table view to apply and close the popup.
1. Click on the processor. In the **Properties** pane, expand the **Input** panel and check the **Pull** box.
1. In the **Pull Strategy** dropdown, select the **Pull Best Item** option.
1. Change the **Label** dropdown to *Custom Value*, and enter the following code snippet for the **Expression**.
1. **Save** the model.
1. **Reset** and **Run** the model. Verify that the processor pulls red items (Type 1) if they are available.

 

 

 

## Step 3 Configuring Reinforcement Learning

 

1. In the **Toolbox** pane, add a **Connectivity > Reinforcement Learning** tool.
1. In the **Observation Space** group, select *Discrete* from the dropdown menu.
1. In the **Parameters** expand **Observations** and check **LastItemType**.
1. In the **Action Space** group, select *Discrete* from the dropdown menu.
1. In the **Parameters** expand **Actions** and check **ItemType**.
1. Click on the **Apply** button to save your changes.
1. If you click on the processor and press the Code button on its Setup Time picklist, you can see that the code sets a variable named `"f_lastlabelval"`. We will use this variable to set the LastItemType observation parameter from within the On Observation callback of the Reinforcement Learning tool.
1. In the **Reinforcement Learning** properties window, press the Add button by the **On Observation** trigger and select **Code Snippet**.
1. Change the description text from *Code Snippet* to *Set observation parameter*.
1. Type or paste the following code snippet into the field.
1. Before we fill out the **Reward Function**, we want to configure the sink to accumulate a reward amount on a label as items leave the model.
1. Click on the sink in the 3D View. In the **Labels** pane, add a Number label and name it *LastTime*.
1. Add another Number label and name it *Reward*.
1. Check the **Automatically Reset** button, and press the save icon to save the label reset values as 0.
1. In the **Triggers** panel, add an **On Entry** trigger.
1. Add a **Data > Increment Value** option to the trigger.
1. In the **Increment** dropdown, select or type `current.labels["Reward"]`
1. In the **by** field, enter `10 / (Model.time - current.LastTime)`
1. Add a **Data > Set Label** option to the trigger.
1. Change the **Object** to *current* and the **Label** to *"LastTime"*.
1. For the **Value**, enter `Model.time`
1. Back on the **Reinforcement Learning** properties window, edit the parameters for the **Reward Function**.
1. Change the description text from *Reward Function* to *Reward based on throughput*.
1. Type or paste the following code snippet into the field.
1. In the **On Request Action** trigger, add the option to *Take a Random Action*.
1. In the **Decision Events** group, add a new event reference and sample the processor's **Pull Strategy** event.
1. Click on the **OK** button to apply your changes and close the window.
1. **Save** the model.
1. **Reset** and **Run** the model. Verify that the processor now pulls random items based on the ItemType parameter changing randomly before each pull. If the queue does not have the requested item type, the processor should pull the first item in the queue.

 

 

 

## Testing the Environment Connection

 

We will now return to the Python scripts to begin training. You should ensure that you have reviewed Key Concepts About Reinforcement Learning and that you have downloaded the included python scripts and configured a development environment to run them.

 

The flexsim_env.py script contains the definition for a FlexSimEnv class that we can use to communicate from Python to FlexSim. It has functions for launching FlexSim with a model and communicating with the model using sockets.

 

In the flexsim_env.py file, a main() function is defined that can be used to test the connection from Python to FlexSim.

 

1. Near the bottom of the **flexsim_env.py** script, modify the flexsimPath and modelPath to reference the location where FlexSim is installed and where the model is saved. For example:
1. Run the **flexsim_env.py** script by pressing the **Run Python File** button at the top.
1. In the python terminal window, press any key to close FlexSim and end the python process.

 

 

 

## Training the Algorithm

 

If the flexsim_env.py script was able to successfully launch FlexSim and run test episodes of the model, then you are ready to train a reinforcement learning algorithm.

 

The flexsim_training.py script contains an example function that uses a FlexSimEnv to train a Stable Baselines3 PPO algorithm.

 

1. In the **flexsim_training.py** script, modify the flexsimPath and modelPath to reference the location where FlexSim is installed and where the model is saved.
1. Run the **flexsim_training.py** script by pressing the **Run Python File** button at the top.
1. To fix missing packages errors, use the python terminal window to install the missing packages. For example, enter the command "pip install stable-baselines3" to install the stable-baselines3 package.
1. Press the **Run Python File** button to execute **flexsim_training.py** again once the required packages are installed.
1. In this script, because **visible** is set to *False* on the FlexSimEnv, you will not see a FlexSim window appear. Instead, you can can use the Task Manager to see that a FlexSim process is created as a background process. The process may appear under a python process.
1. As the algorithm learns, Stable Baslines3 will print information to the python window about the training status.
1. After the model is saved, the script will wait for you to push any key and then run some test episodes using the trained model. In the output below, you can see that the agent is often rewarded 1 at a given step, which indicates no changeover time in our example model.
1. If you open Windows Explorer to your python project directory, you will find a zip file containing the data for the trained AI model. To learn more about using that data, continue to Using a Trained Model.

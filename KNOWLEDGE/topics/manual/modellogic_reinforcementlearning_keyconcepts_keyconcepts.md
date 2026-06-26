---
id: modellogic_reinforcementlearning_keyconcepts_keyconcepts
name: "Key Concepts About Reinforcement Learning"
kind: manual
breadcrumb: ["ModelLogic","ReinforcementLearning","KeyConcepts","KeyConcepts"]
tags: ["manual","modellogic","reinforcementlearning","keyconcepts"]
source: "manual/ModelLogic/ReinforcementLearning/KeyConcepts/KeyConcepts.html"
---

# Key Concepts About Reinforcement Learning

## Reinforcement Learning Overview

 

A FlexSim model can be used as an environment for training and evaluating reinforcement learning algorithms.

 

 

A typical reinforcement learning episode consists of an agent making an observation of the environment, taking an action in response to the observation, receiving a reward, and then continuing to observe, take actions, and receive rewards in a cycle until the episode is done.

 

The agent usually begins by taking random actions (exploration). As the agent receives rewards, it learns to maximize those rewards by taking more optimal actions based on what it has learned (exploitation). The reward for a given action may not be immediate. For example, if you take a wrong turn in a maze, you don't know until you hit a dead end. Different reinforcement learning algorithms account for this delayed reward in different ways, but they each follow the same basic flow even when a reward may not be directly related to the most recent action.

 

Many different reinforcement learning algorithms are available, but the particulars of how the agent learns are slightly different in each. Some examples of different algorithms include DQN (Deep Q Network), DDPG (Deep Deterministic Policy Gradient), PPO (Proximal Policy Optimization), and [many more](https://en.wikipedia.org/wiki/Reinforcement_learning#Comparison_of_reinforcement_learning_algorithms).

 

FlexSim does not provide implementations of specific reinforcement learning algorithms. Instead, it provides features and examples to demonstrate how you can communicate with a FlexSim model as a simulated environment for training these algorithms.

 

 

 

## Workflow for Reinforcement Learning with FlexSim

 

The goal of Reinforcement Learning is to produce an agent that can make intelligent decisions, given the current set of observations. To reach that goal, you can follow these steps:

 

- Choose a Reinforcement Learning library or service.
- Create a FlexSim model that uses the Reinforcement Learning tool.
- Wrap the FlexSim model as an environment.
- Train an agent, using that environment.
- Deploy the trained agent.

 

The details for accomplishing each of these steps depends on the Reinforcement Learning library you choose. However, the same basic steps apply to any choice. We provide details here about working with OpenAI Gym.

 

 

 

## OpenAI Gym

 

OpenAI Gym is a python library for Reinforcement Learning. It provides the following advantages:

 

- Widely used
- Available for free (under the MIT License)
- Well documented

 

To get started with OpenAI Gym, refer to the following topics:

 

- Getting Started with OpenAI Gym
- Reinforcement Learning Training
- Using a Trained Model

 

 

 

## Other Options

 

There are many other Reinforcement Learning libraries or services available. Even if you want to use FlexSim as an environment in something besides OpenAI Gym, we recommend going through the above topics related to OpenAI Gym. In particular, the python files included in that walkthrough show how to wrap a FlexSim model as an arbitrary environment, compatible with whatever platform you choose.

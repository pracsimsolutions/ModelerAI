---
title: Navigators auto-create — never create a navigator directly
status: v1 — captured 2026-06-14
tags: navigator, connections, network, astar, agv, gis, task-executer, connect_task_executer_to_navigator, connect_fixed_resource_to_navigator
---

# Navigators are auto-created, not created directly

A navigator (DefaultNetworkNavigator, AStarNavigator, AGVNetwork,
DefaultNavigator, GISNavigator) is **never** created with
`modelerai_create_object`. FlexSim creates it **automatically** with its
**default name** the moment you add the object that requires it. The
navigator-connect tools then reference that default name.

> Do NOT `create_object` a navigator and give it a custom name. There is no
> "make a navigator" step — there is only "add the object, and the navigator
> appears."

## Which object auto-creates which navigator

| Add this object | Auto-creates (default name) |
|---|---|
| a **Network Node** | `DefaultNetworkNavigator` |
| any **A\*** object (AStar barrier, etc.) | `AStarNavigator` |
| any **AGV** object (AGV path, etc.) | `AGVNetwork` |
| a **TaskExecuter** (Operator / Transporter / etc.) | `DefaultNavigator` (auto-placed on it) |
| the **Map**, then a **Point** | `GISNavigator` |

## Implications for the connect tools

- To use `modelerai_connect_task_executer_to_navigator` or
  `modelerai_connect_fixed_resource_to_navigator`, the navigator must already
  exist — i.e. the constituent object above must have been added first. Pass the
  navigator's **default name** in the `navigator` argument.
- A freshly created Operator/Transporter is **already** on `DefaultNavigator`.
  Connecting it to a specific system (A\*, Network, AGV) requires that system's
  object to exist first.
- Build order is therefore: add the system object (→ navigator auto-appears) →
  then call the connect tool with the default navigator name.

## Anti-pattern (caught 2026-06-13)

```
# WRONG — navigators can't be created this way
modelerai_create_object {class:"NetworkNavigator", name:"NavNet"}

# RIGHT — add a Network Node; DefaultNetworkNavigator appears automatically
modelerai_create_object {class:"NetworkNode", name:"NN1"}
# ... then reference "DefaultNetworkNavigator" in the connect tool
```

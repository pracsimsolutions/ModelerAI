/**Custom Code*/
// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// commands/commands.h — forward decls for every modelerai_* command.
//
// Each declared function is a C++ DLL export bound to a FlexScript-callable
// name via a tree node under ModelerAI.fsx (near the existing bridge
// bindings). Invoked from FlexScript via applicationcommand("modelerai_<x>",
// args...) or by the AI through the modelerai_call bridge tool.
//
// Implementations live next to this header in commands.cpp. The library is
// intentionally collected into one .cpp; resist the urge to split each
// command into its own file — most bodies are short and shared helpers
// (string-param reading, JSON result building, fsEscape) want a single home.

#pragma once

#include "module.h"

modelerai_export Variant ModelerAi_ping                    (FLEXSIMINTERFACE);

// Object lifecycle (Tools 1-3 of the 2026-06-02 redesign)
modelerai_export Variant ModelerAi_createObject            (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_deleteObject            (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_cloneObject             (FLEXSIMINTERFACE);

// Properties: setProperty / getProperty / listProperties are schema-driven
// (KNOWLEDGE/schemas/<Class>.json). Labels: set/get/list/remove form the
// full per-object metadata surface.
modelerai_export Variant ModelerAi_setProperty             (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_getProperty             (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_listProperties          (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_setLabel                (FLEXSIMINTERFACE);

// Groups — Tools.create("Group") + Group("name").addMember/removeMember.
modelerai_export Variant ModelerAi_createGroup             (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_createUserCommand       (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_createDashboard         (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_listChartTemplates      (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_groupAddMember          (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_groupRemoveMember       (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_listGroups              (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_listGroupMembers        (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_deleteGroup             (FLEXSIMINTERFACE);

// Global Tables — Tools.create("GlobalTable") + Table("name")[r][c].
modelerai_export Variant ModelerAi_createGlobalTable       (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_resizeGlobalTable       (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_setGlobalTableCell      (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_getGlobalTableCell      (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_listGlobalTables        (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_deleteGlobalTable       (FLEXSIMINTERFACE);

// Parameter reads + general discovery.
modelerai_export Variant ModelerAi_getParameter            (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_listParameters          (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_listClasses             (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_getObjectInfo           (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_getLabel                (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_listLabels              (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_removeLabel             (FLEXSIMINTERFACE);

// Schema extraction — walks function_s(class, "enumerateProperties") and
// emits per-class property catalog (name + property class + value kind +
// origin + combo options static/dynamic). Used to populate
// KNOWLEDGE/schemas/<Class>.json.
modelerai_export Variant ModelerAi_extractClassSchema      (FLEXSIMINTERFACE);

// Connections (Tools 4-8 of the redesign — semantic split by class-pair role)
modelerai_export Variant ModelerAi_connectFixedResources                (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_connectTaskExecuterToNavigator       (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_connectFixedResourceToNavigator      (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_connectDispatcherToTaskExecuter      (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_connectCenterport                    (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_disconnect                           (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_inspectConnections                   (FLEXSIMINTERFACE);

// Parameters (existing — Tool 18/19 of the redesign)
modelerai_export Variant ModelerAi_addParameter            (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_setParameter            (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_removeParameter         (FLEXSIMINTERFACE);

// Execution domain — model run state control + run-stop event listening.
// The trigger callback (notifyRunState) is invoked from FlexScript inside
// our installed OnRunStart / OnRunStop / OnModelReset hook blocks; it
// records the new state + sim time and wakes any thread waiting on a
// run-stop signal.
modelerai_export Variant ModelerAi_notifyRunState      (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_resetModel          (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_runToTime           (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_runToEnd            (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_runUntil            (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_run                 (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_stopModel           (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_stepModel           (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_getRunState         (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_addStopTime         (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_removeStopTime      (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_setWarmupTime       (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_setRunSpeed         (FLEXSIMINTERFACE);

// ProcessFlow lifecycle — create/list/delete the ProcessFlow tools that live
// under Tools/Toolbox/ProcessFlow in the model tree.
modelerai_export Variant ModelerAi_createProcessFlow       (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_openProcessFlowView     (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_listProcessFlows        (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_deleteProcessFlow       (FLEXSIMINTERFACE);

// ProcessFlow activities — add/delete/connect activities within a ProcessFlow,
// and set/read variables on an existing activity.  Activities are created
// through the ProcessFlowView internal callbacks (openprocessflowview /
// createActivity) so the connector wiring and placement logic that lives in
// the view is reused.
// NOTE: ProcessFlows live at /Tools/ProcessFlow/<name> — a FLAT list under
// one node, NOT at the model root and NOT at /Tools/Toolbox/ProcessFlow/...
// (the toolbox subtree is the UI coupling shortcut, not storage).
// Model.find("<name>") alone returns null. All tools below walk the storage
// location internally so callers should NEVER need run_script for PF work.
modelerai_export Variant ModelerAi_addActivity             (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_connectActivities       (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_deleteActivity          (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_setActivityVariable     (FLEXSIMINTERFACE);
// Read-side mirrors — added so the agent never needs run_script to inspect
// PF state (where it has historically hallucinated wrong tree paths).
modelerai_export Variant ModelerAi_listActivities          (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_getActivityInfo         (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_getActivityVariable     (FLEXSIMINTERFACE);

// Activity-table tools — for table-shaped variables on PF activities
// (ScheduleSource.arrivals, AssignLabels.>labels, and similar). Variable
// names starting with ">" resolve as hidden subnodes (activity.find);
// regular names use getvarnode.
modelerai_export Variant ModelerAi_setActivityTableCell         (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_getActivityTableCell         (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_getActivityTableSize         (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_resizeActivityTable          (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_setActivityTableColumnHeader (FLEXSIMINTERFACE);

// CreateObject specialized: copy/replace + stringValue + assignType.
// Handles the token-label-mode SDT structure for the assignTo variable.
modelerai_export Variant ModelerAi_setCreateObjectTargetLabel   (FLEXSIMINTERFACE);

// Output domain — performance measures, object stats, model summary.
// PerformanceMeasures live inside PerformanceMeasureTables. Each PM has a
// Value/valueNode script that's invoked when you read it via
// Model.performanceMeasures.Name — the script gets the bound `reference`
// treenode as param(1) and returns whatever you want (number, string,
// array, anything FlexScript can compute).
modelerai_export Variant ModelerAi_createPerformanceMeasure(FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_deletePerformanceMeasure(FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_listPerformanceMeasures (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_getPerformanceMeasure   (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_getPerformanceMeasures  (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_getObjectStats          (FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_getModelSummary         (FLEXSIMINTERFACE);

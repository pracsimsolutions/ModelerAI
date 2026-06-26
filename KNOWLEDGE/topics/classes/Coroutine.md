---
id: coroutine
name: "Coroutine"
kind: class
module: ""
signature: "Coroutine"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "An opaque class representing a paused coroutine."
seeAlso: []
source: "help\\FlexScriptAPIReference\\SpecialParameters\\Coroutine.xml"
---

# Coroutine

An opaque class representing a paused coroutine.

A Coroutine does not have any member methods or properties.
It is only used to allow you to check whether a Variant reference to a node
is a coroutine.

Variant val = myNode.coevaluate();
if (val.is(Coroutine)) {
await val;
}


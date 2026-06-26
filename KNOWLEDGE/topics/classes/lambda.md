---
id: lambda
name: "lambda"
kind: class
module: ""
signature: "lambda"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "Allows an expression to be passed in as a parameter without immediatly being evaulated."
seeAlso: []
source: "help\\FlexScriptAPIReference\\SpecialParameters\\lambda.xml"
---

# lambda

Allows an expression to be passed in as a parameter without immediatly being evaulated.

This class represents a FlexScript expression. Functions that accept
these values store the expression passed in as a parameter, rather than the result of the expression.
The function may evaluate the expression one or more times. During evaulation, the
keyword count may be used to determine how many times the expression
has been called. One method that accepts lambdas as parameters is the
Table.query() method.


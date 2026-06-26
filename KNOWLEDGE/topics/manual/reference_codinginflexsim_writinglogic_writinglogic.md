---
id: reference_codinginflexsim_writinglogic_writinglogic
name: "Writing Logic in FlexSim"
kind: manual
breadcrumb: ["Reference","CodingInFlexSim","WritingLogic","WritingLogic"]
tags: ["manual","reference","codinginflexsim","writinglogic"]
source: "manual/Reference/CodingInFlexSim/WritingLogic/WritingLogic.html"
---

# Writing Logic in FlexSim

## Where to Get Help

 

Whenever you need help with what commands to use and how to use them, you can refer to the Commands documentation, which is accessible through FlexSim's Help menu. Refer to the FlexScript Class Reference for information about FlexScript classes.

 

 

 

## FlexScript and C++ General Rules

 

Here are some general rules you will need to know when creating your own logic.

 

- Language is case sensitive (A is not the same as a)
- No specific format is required (free use of spaces, tabs and line returns is encouraged)
- Text strings are usually entered between quotes. "mytext"
- Parentheses follow a function call and commas separate the arguments of the function. moveobject(object1,object2);
- The end of a line or function call will always end with a semi-colon
- Parentheses can also be used freely to make associations in your math and logic statements.
- Curly braces are used to define a block of statements.
- To comment out the rest of a line use //
- To comment out blocks of text use /* text */
- Don't use spaces or special characters in name definitions (_ is ok).
- Named variables and explicit values can be interchanged in writing expressions.

 

 

 

## Variable Types

 

FlexSim uses various variable types. Some of the more common are listed here. Refer to the FlexScript Class Reference for the complete list of FlexScript classes.

 

### Classes

 

| Type | Description |
| --- | --- |
| int | integer number type |
| double | double precision floating point number type |
| string | text string |
| treenode | reference to a FlexSim node |
| Variant | a number, string, treenode or array |
| Array | a collection of Variants |
| Object | treenode with object data |
| Color | rgba components of a color |
| Vec3 | xyz components of a vector |
| Table | a table of elements |
| List | dynamic list of things used for synchronizing tasks and logic |
| var | type inferred variable |

 

For more information on how the treenode (or FlexSim node) type works, refer to the FlexSim tree structure.

 

 

 

## Declaring and Setting Variables

 

The following are some examples of how to declare and set variables.

 

```
		int index = 1;
		double weight = 175.8;
		string category = "groceries";
		treenode nextObj = Model.find("Processor1");
		Variant cellValue = table[2][3];
		var value = 3;
```

 

 

 

## Constructing Class Objects

 

The following are examples of how to use constructors.

 

```
		Color myColor = Color(1, 0, 0);   // Makes a red color
		object.location = Vec3(0, 0, 0);  // Sets the object's location to the origin
```

 

 

 

## Math Operations

 

The following list show different math operations that can be performed on values.

 

| Operation | Floating Point Example (=solution) | Integer Example (=solution) |
| --- | --- | --- |
| + | 1.6+4.2 (=5.8) | 2+3 (=5) |
| - | 5.8-4.2 (=1.6) | 5-2 (=3) |
| * | 1.2 * 2.4 (=2.88) | 3*4 (=12) |
| / | 6.0/4.0 (=1.5) | 20/7 (=2) |
| % (integer mod) |  | 34%7(=6) |
| Math.sqrt() | Math.sqrt(5.3) (=2.3) |  |
| Math.pow() | Math.pow(3.0,2.2) (=11.2) | Math.pow(3,2) (=9) |
| Math.round() | Math.round(5.6) (=6) |  |
| Math.frac() | Math.frac(5.236) (=0.236) |  |
| Math.fabs() | Math.fabs(-2.3) (=2.3) |  |
| Math.fmod() (floating point mod) | Math.fmod(5.3,2) (=1.3) |  |

  

#### A Few Cautions About Operations

 

Be aware that by performing operations on floating point numbers, some precision may be lost.

 

Be careful in using these operations while mixing integer types with floating point types, or with using just integer types. For example, the / operator will return an integer if both operators are integers. This may not be what you want to get out of the operation, in which case you will need to use floating point types instead of integer types. The number 5 as an integer type. If you want it to interpret the number as a floating point type, enter 5.0 instead of just 5.

  

 

 

## Comparing Variables

 

The following table shows different operations for comparing two values or variables.

 

| Operation | Example (solution) |
| --- | --- |
| > (greater than) | 1.7 > 1.7 (false) |
| < (less than) | -1.7 < 1.5 (true) |
| >= (greater than or equal to) | 45 >= 45 (true) |
| <= (less than or equal to) | 45 <= 32 (false) |
| == (equal to) | 45 == 45 (true) |
| != (not equal to) | 45 != 35 (true) |
| string comparisons | current.name == "Processor5" |
| pointer comparisons (treenodes, Objects) | current == Model.find("Processor5") |

  

#### A Few Cautions About ==

 

The == operator can often cause problems if you are comparing two double precision floating point values, and one or both of those values have been calculated using math operations. When doing math operations, floating point values may lose some precision. Since the == operator will only return true if all 64 bits of each value are exactly the same, even a small precision loss will cause the == operator to return false. In such cases, you will want to instead verify that the two values are within a range of each other. For example: fabs(value1 - value2) < 0.000001, will return true if the two values are practically equal for all intents and purposes.

  

 

 

## Relating Variables

 

The following table shows different operations for relating several comparisons.

 

| Operation | Example |
| --- | --- |
| && (logical AND) | x>5 && y<10 |
| \|\| (logical OR) | x==32 \|\| y>45 |
| ! (logical NOT) | !(x==32 \|\| y>45) |
| Math.min() | Math.min(x, y) |
| Math.max() | Math.max(x, y) |

 

 

 

## Setting and Changing Variables

 

The following tables show ways of setting and changing variables.

 

| Operation | Example |
| --- | --- |
| = | x = x + 2; |
| += | x += 2; (same as x = x + 2) |
| -= | x -= 2; (same as x = x - 2) |
| *= | x *= 2; (same as x = x * 2) |
| /= | x /= 2; (same as x = x / 2) |
| ++ | x ++; (same as x = x + 1) |
| -- | x --; (same as x = x - 1) |

 

 

 

## Executing Commands

 

Executing a command in FlexSim is made of following parts. First type the command's name, followed by an open parenthesis. Then enter each parameter of the command, separating multiple parameters by commas. Each parameter can be a variable, an expression of variables, or even a command itself. Finish the command with a close parenthesis, and a semi-colon. For detailed information on the commands, their functionality and parameter lists, refer to the "Commands" documentation found through FlexSim's Help menu.

 

### Syntax

 

`commandname(parameter1,parameter2,parameter3...);`

 

### Example

 

`getstat(current, "Content", STAT_CURRENT);`

 

 

 

## Dot Syntax

 

Classes like the treenode, Object and Variant (to name a few) allow you to call methods, as well as access properties, variables, attributes and labels (if applicable) using dot syntax.

 

### Syntax

 

`object.method(parameter1,parameter2,parameter3...);`

 

### Example

 

```
		current.setLocation(1, 1, 1);
		treenode lastItem = current.last;
		treenode item3 = current.subnodes[3];
		int quantity = current.quantity; //Accesses label named 'quantity'
```

 

 

 

## Control Flow

 

FlexScript includes several [control flow](https://en.wikipedia.org/wiki/Control_flow) constructs. These are statements you can place in your code to control which of multiple possible paths should be taken in the execution of the code. Control flow statements allow for branching, looping, and even suspension and resumption of code execution.

 

 

 

## If Statement The if statement allows you to execute one piece of code if an expression is true, and another piece of code if it is false. The else portion of the construct is optional. It uses this construct: 

`if (test expression) { code block } else { code block }`

 For example: 

`if (item.subnodes.length == 2) { item.color = Color.red; } else { item.color = Color.black; }`

  The ?: Ternary Operator FlexScript supports an alernative syntax for an if-else construct which is used in many languages and is known as the ternary operator. The ternary operator can be very useful in FlexSim since it allows you to write an if-else contruct in a single line. The ternary operator uses the following syntax: `condition ? value_if_true : value_if_false` If the condition specified before the question mark is true, the value between the question mark and semicolon is returned. If the condition is false, the value after the semicolon is returned. The following two code snippets are equivalent: `return time() > 1440 ? 10 : 20` 

`if (time() > 1440) { return 10; } else { return 20; }`

    While Loop

 

The while loop will continue to loop through its code block until the test expression becomes false.

 

It uses this construct:

 

```
		while (test expression)
		{
			code block
		}
```

 

For example:

 

```
		while (current.subnodes.length > 2)
		{
			current.last.destroy();
		}
```

 

 

 

## For Loop

 

The for loop is like the while loop, except that it is used usually when you know exactly how many times to loop through the code block. The start expression is executed only once, to initialize the loop. The test expression is executed at the beginning of each loop and the loop will stop as soon as this expression is false, just like the while loop. The count expression is executed at the end of each loop, and typically increments some variable, signifying the end of one iteration.

 

It uses this construct:

 

```
		for(start expression; test expression; increment expression)
		{
			code block
		}
```

 

For example:

 

```
		for (int index = 1; index <= current.subnodes.length; index++)
		{
			current.subnodes[index].as(Object).color = Color.blue;
		}
```

 

 

 

## Switch Statement

 

The switch statement allows you to choose one piece of code to execute out of several possibilities, depending on a variable to switch on. The switch variable must be an integer type. The example below sets the color of items of type 1 to yellow, type 5 to red, and all other types to green.

 

It uses this construct:

 

```
		switch (switchvariable)
		{
			case casenum:
			{
				code block;
				break;
			}
			default:
			{
				code block;
				break;
			}
		}
```

 

For example:

 

```
		switch (item.type)
		{
			case 1:
			{
				item.color = Color.yellow;
				break;
			}
			case 5:
			{
				item.color = Color.red;
				break;
			}
			default:
			{
				item.color = Color.green;
				break;
			}
		}
```

 

### Redirection

 

Each of the flow constructs described can be redirected mid-execution with either a continue, break or return statement. The following explains how each of these statements work.

 

| Construct | Examples |
| --- | --- |
| continue; | Only valid in For and While loops. Halts the current iteration of the loop and goes on to the next iteration in the loop. In a For loop the counter is incremented before continuing. |
| break; | Only valid in For, While and Switch statements. Breaks out of the current For, While or Switch block and continues with the line immediately following this block. Nested statements only break out of the current statement and continue on in the containing statement. |
| return 0; | Returns out of the current method entirely and continues with the line following the code that called this method. A value is required after the return statement because all FlexScript commands (picklists and triggers included) return a Variant type. Typing return; is not valid. |

 

 

 

## Coroutines and the Await Statement

 

FlexScript also supports [coroutines](https://en.wikipedia.org/wiki/Coroutine) using the **await** keyword. Coroutines allow a method or function to span simulated time. The most basic example of a coroutine is to await a simple delay time.

 

```
print("time before delay: ", time());
await Delay.seconds(5);
print("time after delay: ", time());
```

 

The above code prints the current time to the output console, then waits for 5 seconds (the coroutine is suspended for 5 simulated seconds), then prints the current time again, which will have increased by 5 simulated seconds. While the coroutine is suspended, all local variables are preserved.

 

A FlexScript function becomes a coroutine if it contains a statement with the keyword **await**, with the following syntax:

 

```
await someexpression;
//or...
typename result = await someexpression;
//or...
result = await someexpression;
```

 

Some specific examples:

 

```
await current.event("OnEntry"); // wait for current's OnEntry event to fired

Variant pulled = await List("List1").pull("", 1, 1, current); // pull a value from List1 and wait for the pull operation to complete

await Delay.minutes(5); // suspend the coroutine for 5 simulated minutes
```

 

The predicate of the await statement (the expression after the **await** keyword) must be *awaitable*, meaning the predicate's type has been specially configured so that you can await instances of that type. Only a few specific types are awaitable:

 

- Delay
- Event
- List.BackOrder
- List.PullResult
- List.PushResult
- List.Entry
- Variant (see Variant - Awaitable for more information)

 

Coroutines allow you to model entire processes as a single code block. You can start the execution of a process, and suspend and resume the execution of the process in any number of ways. You can suspend its execution for a defined delay time, or until some requirements are met, such as waiting until you pull a value from a list, or waiting until an event is fired on an object. In combination, these different capabilities allow you incredible flexibility in defining your process.

  

### Coroutines vs. Process Flow

 

It's important to note that Process Flow enables you to simulate almost any time-spanning process within a flow chart, giving you much more visual explanatory power than you would get looking at a large block of code. Indeed, FlexScript's coroutine features were added *after* Process Flow, and much of its design was intended to mirror some of the functionality you could do in Process Flow, albeit using code (wait for event, pull from list, delay, etc.). So if Process Flow can do what you need, we encourage you to use it. Process Flow models are easier to support and debug, and they improve communication with stakeholders regarding how a process works. Nevertheless, some users actually prefer implementing time-spanning processes through code. Additionally, there are some situations that can benefit from a code-based time-spanning approach. For example:

 

- When speed factors are paramount, a coroutine will generally run much faster than tokens moving from activity to activity.
- In some specific cases, implementing a process in Process Flow can be cumbersome, requiring a lot of activities, synchronization, etc. One specific example is a "multi-pull" process that tries to pull different requirements from different lists, and must wait until some minimum set of requirements are met. While this is certainly possible in process flow, it can require a lot of activities and synchronization, that might be easier to perform directly in code.
- Coroutines allow you to augment your process flow with customized activities. Specifically, the Custom Code will await the result of the code that is executed. This means if you implement the custom code as a coroutine (you use an **await** statement somewhere in the code), then the activity itself will not finish until the coroutine is finished.
- In some cases you may not want to be forced to create a whole new process flow just to perform some simple time-spanning operation. Maybe you have a trigger on an object that delays for a certain time before performing some simple operation. Here coroutines allow you to do that operation easily, without requiring a whole new process flow or a complicated delayed message mechanism.
- Coroutines make some standard picklist options easier to implement. For example, picklist options that pull from a list will generally await the result of the pull, potentially spanning some time period before the pull is fulfilled.

 

Again, we encourage you to use Process Flow where it is suitable, specifically because of its visual explanatory power. Nevertheless, coroutines have specific advantages that can augment or provide alternatives to your use of Process Flow in simulating your customized processes.

  

 

 

## Optional Chaining

 

FlexScript also supports a concept called optional chaining, using the `?.` syntax. The need for this arises when there is a possibility of encountering null values in the evaluation of your code. If a given expression results in a null value, and you access a class member of the expression using dot syntax, FlexSim will throw an exception by default.

 

```
	treenode nullNode = 0;
	return nullNode.last; // exception
```

 

Avoiding such exceptions can cause your code to balloon into lots of if statements. Alternately, you can use optional chaining to access class members.

 

```
	treenode nullNode = 0;
	return nullNode?.last; // returns null (no exception)
```

 

If the owning object is null, then the `?.last` accessor will simply give back null, instead of throwing an exception. It is called optional chaining because it allows you to chain multiple accessors together very easily without the need for if statement guards. The code will simply bail out and return null when it encounters a null pointer.

 

```
	treenode nullNode = 0;
	return nullNode?.first?.first?.first; // returns null (no exception)
```

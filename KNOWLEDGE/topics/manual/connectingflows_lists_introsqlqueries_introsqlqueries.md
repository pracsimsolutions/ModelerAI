---
id: connectingflows_lists_introsqlqueries_introsqlqueries
name: "Introduction to SQL Queries"
kind: manual
breadcrumb: ["ConnectingFlows","Lists","IntroSQLQueries","IntroSQLQueries"]
tags: ["manual","connectingflows","lists","introsqlqueries"]
source: "manual/ConnectingFlows/Lists/IntroSQLQueries/IntroSQLQueries.html"
---

# Introduction to SQL Queries

## Overview of SQL Queries

 

Before reading this topic, consider reading Key Concepts About Lists and Creating List Queries.

 

List queries in FlexSim might have the following components:

 

 

For example, a valid query might be something like this:

 

`WHERE queuesize > 10`

 

In this example, the list will restrict the entries that get pulled from the list. It will only pull list entries that are in a queue containing more than 10 items.

 

The following sections will explain the purpose of each component of this query:

 

### SQL Clause

 

The SQL language uses clauses to represent some kind of logical rule that will be used to perform a search. Usually, the clause begins with a keyword that is formatted in all caps. Although SQL has many different clauses with different purposes, you will generally only need to use two clauses when creating list queries:

 

- **WHERE** - Use this clause to restrict which entries get pulled from the list.
- **ORDER BY** - Use this clause to sort and prioritize which entries will get pulled first from the list.

 

See SQL Clauses for more detailed information on these two clauses.

 

### Field on the List

 

Every list uses fields to track data about the list entries. When you created your list, you determined which fields your list would have and which data they would track. In the SQL query, you will indicate which field on the list should be used to filter or prioritize list entries.

 

### Operator (optional)

 

Operators can be used to compare two values or create a new value. You'll typically only use operators with the WHERE clause, which filters or restricts list entries. If the result of the comparison is true, then the list entry will get pulled. If the result of the comparison is false, the list entry will not get pulled.

 

Operators consist of the normal basic math operators (such as +, -, *, /, =, etc.) and some additional logical operators. In list queries, you'll typically use an operator to compare the value of a list to the value in your query. See Operators for more information about the operators you can use in your queries.

 

### Value (optional)

 

You'll use a value along with an operator to compare this value to the value of the field on the list in a WHERE clause (not an ORDER BY clause). If the result of the comparison is *true*, it could potentially be pulled from the list. If the result is *false*, it will not be pulled from the list.

 

 

 

## SQL Clauses

 

In SQL, you use what are called *clauses* to define queries. Each clause represents a rule that the query will use to perform its search. There are many different clauses available in standard SQL, but when using lists, you usually only need to know two: WHERE and ORDER BY.

 

### WHERE Clause

 

The WHERE clause essentially defines *what* you want to pull from a list. When you pull from a list using a query with a WHERE clause, you are essentially saying, "I want to pull something from the list *where* the entry meets this criterion." For example, if you have a processor that should only pull items that have an item with a label named *type* that has a value of 3, you could use the following query:

 

`WHERE type == 3`

 

This WHERE clause defines a rule that you only want to pull items whose *type* label is equal to 3. More specifically, the type field on the list must have a value that is equal to 3.

 

### ORDER BY Clause

 

The ORDER BY clause essentially defines how to prioritize what should be pulled when there are multiple potential candidates for pulling. When you pull from a list using a query with an ORDER BY clause, you are essentially saying, "I want to pull something from the list, and if there are multiple possibilities, *order* them *by* this rule." For example, if you were using the age field to pull entries that have been on the list for the shortest amount of time, you could use the following query:

 

`ORDER BY age ASC`

 

This ORDER BY clause says that you want to prioritize the items on the list with the lowest age. More specifically, the pull query should sort list entries by the age field value in ascending order, and pull the first item in the resulting sorted list. The ASC keyword is optional and tells the list to sort the entries in ascending order (default). You could alternatively use the DESC keyword to sort in descending order.

 

 

 

## Combining Multiple Clauses and Rules

 

You can combine many different rules and clauses to make more complex SQL queries if needed, as explained in the following sections.

 

### Ordering by Multiple Fields

 

The ORDER BY clause can easily prioritize list entries by more than one field at a time. You can separate multiple fields using a comma. For example:

 

`ORDER BY type ASC, age DESC, queueSize DESC`

 

In this query:

 

1. The list will first order the entries using the `type` field in ascending order (so items with a type of 1 will get pulled first, then items with a type of 2, etc.).
1. After applying that first filter, if two entries have identical types, the list will then order the entries using the `age` field in descending order (so items that have been on the list the longest will get pulled first).
1. Lastly, if two entries have identical types and ages, the list will sort the entries using the `queueSize` field in descending order (so items will get pulled from the largest queue first).

 

### Using Both WHERE and ORDER BY Clauses

 

If you define a query that uses both the WHERE and ORDER BY clauses, the WHERE should come first, and the ORDER BY can be placed immediately after it, as in the following example:

 

`WHERE type == 3 ORDER BY age ASC`

 

This query says that it will only pull items with a type of 3, and if there are multiple that meet that criterion, it should take the one with the smallest age.

 

 

 

## Operators

 

The following tables show the operators that you can use in pull queries.

 

### Comparison Operators

 

| Operator | Definition | Example |
| --- | --- | --- |
| = or == | Equals comparison | `WHERE type == 2` |
| < | Less-than comparison | `WHERE type < 2` |
| > | Greater-than comparison | `WHERE type > 2` |
| <= | Less-than-or-equal comparison | `WHERE type <= 2` |
| >= | Greater-than-or-equal comparison | `WHERE type >= 2` |
| <> or != | Not-equal comparison | `WHERE type <> 2` |
| BETWEEN | Between a defined range (inclusive) | `WHERE type BETWEEN 2 AND 5` |
| IN | Value in a defined set | `WHERE type IN (1, 3, 5)` |

 

### Logical Operators

 

| Operator | Definition | Example |
| --- | --- | --- |
| AND or && | Logical and | `WHERE type == 2 AND queueSize < 5` |
| OR or \|\| | Logical or | `WHERE type == 2 OR queueSize < 5` |

  

#### Example of Logical Operators

 

Imagine you need to have a simulation model with flow items that can have 1 of 3 item types. You may have a processor that is dedicated to more that one item type. For example, one processor only processes item types 1 and 3. You could use either of the following pull queries:

 

`WHERE type == 1 OR type == 3`

 

`WHERE type IN (1, 3)`

  

### Math Operators

 

| Operator | Definition | Example |
| --- | --- | --- |
| + | Addition | `ORDER BY distance + age` |
| - | Subtraction | `ORDER BY distance - age` |
| * | Multiplication | `ORDER BY distance * age` |
| / | Division | `ORDER BY distance / age` |

 

### Expression Grouping

 

| Operator | Definition | Example |
| --- | --- | --- |
| () | Grouping Parentheses | `WHERE (type < 2 OR type > 8) AND queueSize < 5` |

 

 

 

## FlexScript Functions

 

You can also use FlexScript function calls in your queries. This is often used to do math operations that are not in the standard set. For example, you may want to sort primarily by distance (find the item closest to where the pulling processor currently is), but only up to some threshold, and then sort by some other rule. Specifically, maybe you want to separate distance into partitions of 10 meters each. Items within the same partition would be pulled in LIFO order (age ASC). Here you would use the round() function to round the distance to every 10 meters, and then use age as the second order priority.

 

`ORDER BY round(distance / 10) ASC, age ASC`

 

 

 

## SELECT Clause

 

In some special circumstances you might also use the SELECT clause. See the Functional Reference for more information.

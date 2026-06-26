---
id: regexresult
name: "RegExResult"
kind: class
module: ""
signature: "RegExResult"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class that handles the results of a RegEx match"
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\RegExResult.xml"
---

# RegExResult

A class that handles the results of a RegEx match

The RegExResult class is returned from the
string.match() method.
The RegExResult can be used to iterate over all matches in the
string. This is usually done with as shown in the following example:

string toMatch = "Processor1 Processor200";
Array matches;
var result = toMatch.match(/([a-zA-Z]+)(\d+)/g);
while ((matches = result.findNextMatch()).length) {
pt(matches.join(", "));
pr();
}
// Text printed to Output Console:
// Processor1, Processor, 1
// Processor200, Processor, 200

The string is not actually searched until one of the methods or properties are called.
Then, the string is only searched until the requested information can be provided.
Once a match is found, it is cached, so that future calls that require that match can
look it up.

Unless you mark the RegEx as global (using the "g" flag), this object will only search
until it finds the first match. None of the methods or properties will cause it to search
beyond that point.

Creating RegExResult Objects
You can only obtain a RegExResult object through a call to string.match(), and you can
only declare a variable of this type using the type-inference mechanism (var).

var result = someString.match(/a pattern/g); // correct
// RegExResult result = someString.match(/a pattern/g); INCORRECT
// RegExResult result; INCORRECT

## Methods

### findNextMatch

```flexscript
Array RegExResult.findNextMatch()
```

Searches for the next match in the string.

**Returns:** An array containing the most recently found match.

If a match is not found, or if all matches have been found and the string is exausted
(e.g., by calling RegExResult.length), then this method returns an empty Array.
Otherwise, it returns an Array of strings. The first value in the
Array is the complete match. Subsequent values (if any) will correspond to the
capture groups marked in the RegEx. Without the global flag, this method will only search the string one time.

### getMatch

```flexscript
Array RegExResult.getMatch(int i)
```

Gets gets the nth match (the array of match and subgroups) from a string.

**Parameters:**
- `i` *(int)* — The match number to retrieve (1 is the first match)

**Returns:** An array containing the full match, and any sub-matches.

If the value given is larger than the number of matches, this method will
fail. If necessary, this method will search the string. If the pattern to match is not global,
than any value greater than 1 will always fail (1 could still fail if no match was found).

### getMatchIndex

```flexscript
Array RegExResult.getMatchIndex(int i)
```

Gets the position in the original string of the nth match.

**Parameters:**
- `i` *(int)* — The match number for which you want the position (1 is the first match).

**Returns:** The position of the nth match in the original string

If the value given is larger than the number of matches, this method will
fail. If necessary, this method will search the string. If the pattern to match is not global,
than any value greater than 1 will always fail (1 could still fail if no match was found).

## Properties

### length

```flexscript
int RegExResult.length (readonly)
```

Counts the number of matches to the RegEx in the string.

Counts (searching, if necessary), the total number of matches
in the string. This will find all the matches, exhausting the string.
Use getMatch() to get the results. Without the global flag,
this value cannot exceed 1.


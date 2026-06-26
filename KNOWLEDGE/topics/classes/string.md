---
id: string
name: "string"
kind: class
module: ""
signature: "string"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class that represents a sequence of characters."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Basic\\string.xml"
---

# string

A class that represents a sequence of characters.

Contains methods for examining, searching and replacing, and manipulating strings.
Encoding and String Indexes
Strings in FlexSim are encoded in UTF-8 format.
This means that for simple ASCII strings, each
character takes up a single byte (8-bits). However, non-ASCII characters may consist of
up to 4 bytes. For example, the greek Σ character requires two bytes to encode
in UTF-8.

Since any character in a string may be made up of multiple bytes, it is non-trivial to access
characters in the string by their character index. As such, FlexSim's string interface uses byte-based indexing
for accessing the characters inside the string. In other words, all string methods that take or
return indexes into the string (charAt(),
substr(), slice(),
indexOf(), lastIndexOf(),
search(), slice(),
length, array [] operator),
take and return byte indexes, not character indexes. Also, in FlexSim, all indexing
is 1-based, such that the first byte in a string is accessed by index 1 (not index 0 like in many other
programming languages).

Take the string "Σ=σ" for example. While the = character is technically the second character and thus would
be at "character index" 2, since the Σ character takes up two bytes, the = character is accessed with "byte index" 3.
string("Σ=σ").charAt(3) // returns "="
Note also that, given this rule, some command calls may give back the same results for different byte indexes
because the same character takes up several bytes.
string("Σ=σ").charAt(1) // returns "Σ"
string("Σ=σ").charAt(2) // returns same "Σ"

Usually this will not affect the way you manipulate strings, especially since there are many methods that
do not involve accessing characters by their index, such as using the
replace() and/or match() methods.
If you do need to manually access characters in a string, there are several methods of ensuring that your
code will work for all types of characters.

The easiest way to manipulate individual characters without worrying about encoding is to use the
split() and join() methods. The
split() method will split the string up into an Array of individual characters. Once
it is split, you can traverse the array using character, not byte, indexes. After you are finished you can join the
array back into a string.
string str = "Σ=σ";
Array asArray = str.split();
for (int i = 1; i <= asArray.length; i++) {
if (asArray[i] == "=")
asArray[i] = "≠";
}
str = asArray.join(); // "Σ≠σ"
If you want to search the string by character instead of by byte, but don't want to split it into an array,
you can loop through the string and,
using the charAt() method, increment the looping index based
on the byte-length of each character.
string str = "Σ=σ";
string curChar = "";
for (int i = 1; i <= str.length; i += curChar.length) {
curChar = str.charAt(i);
if (curChar == "=") {
str = str.substring(1, i) + "≠" + str.substring(i + curChar.length);
curChar = "≠";
}
}
If you are searching the string looking for/replacing only ASCII character values, searching the string using the [ ]
array operator can be used.
string str = "Σ=σ";
for (int i = 1; i <= str.length; i++) {
if (str[i] == '=') // comparing a byte to an ASCII character works fine
str[i] = '#'; // setting a byte to an ASCII character works fine,
// as long as the existing byte was already an ASCII character.
}
Finally, the string provides the byteToCharIndex() and charToByteIndex()
methods to convert between character and byte indexes. Note that these methods do require a search of the string, so they will be slow for long strings.

## Methods

### fromNum

```flexscript
string (static) string.fromNum(double num, int precision = -1)
```

Converts the given number to a string.

**Parameters:**
- `num` *(double)* — The number to be converted.
- `precision` *(int, default -1)* — The desired number of decimal places to be printed. Use -1 for automatic precision.

**Returns:** The number as a string.

string text1 = string.fromNum(1.5); // "1.5"
string text2 = string.fromNum(1.5, 2); // "1.50"

### byteToCharIndex

```flexscript
int string.byteToCharIndex(int byteIndex)
```

Returns the character index associated with a specific byte index in the string.

**Parameters:**
- `byteIndex` *(int)* — The 1-based byte index of the target character.

**Returns:** The 1-based character index of the character.

string text = "Σ=σ";
int charIndex = text.byteToCharIndex(text.search("=")); // 2
int numChars = text.byteToCharIndex(text.length); // 3 - the number of characters in the string

### charAt

```flexscript
string string.charAt(int byteIndex)
```

Returns the character at a given byte index in the string.

**Parameters:**
- `byteIndex` *(int)* — The 1-based byte index of the target character.

**Returns:** The target character, as a string.

string text = "Hello World!";
string e = text.charAt(2); // "e"

### charToByteIndex

```flexscript
int string.charToByteIndex(int charIndex)
```

Returns the byte index associated with a specific character index in the string.

**Parameters:**
- `charIndex` *(int)* — The 1-based character index of the target character.

**Returns:** The 1-based byte index of the character.

string text = "Σ=σ";
int byteIndex = text.charToByteIndex(2); // 3

### endsWith

```flexscript
int string.endsWith(string str)
```

Checks if the string ends with the specified substring.

**Parameters:**
- `str` *(string)* — The substring to look for.

**Returns:** 1 if the string ends with the specified substring, 0 otherwise.

string text = "Hello World!";
int worldEnd = text.endsWith("World!"); // 1

### includes

```flexscript
int string.includes(string str)
```

Checks if the string contains the specified substring.

**Parameters:**
- `str` *(string)* — The substring to look for.

**Returns:** 1 if the string contains the specified substring, 0 otherwise.

string text = "Hello World!";
int containsWorld = text.includes("World"); // 1

### indexOf

```flexscript
int string.indexOf(string str)
```

Returns the index of the first occurence of the specified substring.

**Parameters:**
- `str` *(string)* — The substring to look for.

**Returns:** The byte index of the first occurence of the specified substring, -1 if there are no occurences.

string text = "Hello World!";
int oIndex = text.indexOf("o"); // 5

### lastIndexOf

```flexscript
int string.lastIndexOf(string str)
```

Returns the index of the last occurence of the specified substring.

**Parameters:**
- `str` *(string)* — The substring to look for.

**Returns:** The byte index of the last occurence of the specified substring, -1 if there are no occurences.

string text = "Hello World!";
int oIndex = text.indexOf("o"); // 8

### match

```flexscript
RegExResult string.match(RegExp pattern)
```

Searches the string for matches to a pattern.

**Parameters:**
- `pattern` *(RegExp)* — The pattern of characters to look for, a regular expression.

**Returns:** A RegExResult object that provides access to matches

string str = "A noisy noise annoys an oyster.";
var result = str.match(/nois/g);
Array matches = [result[1], result[2]]; // ["nois", "nois"]

See the replace() method for further discussion of regular expressions.

### padEnd

```flexscript
string string.padEnd(int targetLength, string padString = " ")
```

Pads the end of the string with the padString to the given length.

**Parameters:**
- `targetLength` *(int)* — The desired length of the padded string.
- `padString` *(string, default " ")* — The string used to pad the current string.

**Returns:** The string padded to the specified length.

Pads the end of the string with the padString to the given targetLength. If the targetLength
is less than the string's length, the string will be returned unpadded.
string s = "abc";
s.padEnd(10); // "abc "
s.padEnd(10, "foo"); // "abcfoofoof"
s.padEnd(6,"123465"); // "abc123"
s.padEnd(8, "0"); // "abc00000"
s.padEnd(1); // "abc"

### padStart

```flexscript
string string.padStart(int targetLength, string padString = " ")
```

Pads the start of the string with the padString to the given length.

**Parameters:**
- `targetLength` *(int)* — The desired length of the padded string.
- `padString` *(string, default " ")* — The string used to pad the current string.

**Returns:** The string padded to the specified length.

Pads the start of the string with the padString to the given targetLength. If the targetLength
is less than the string's length, the string will be returned unpadded.
string s = "abc";
s.padStart(10); // " abc"
s.padStart(10, "foo"); // "foofoofabc"
s.padStart(6,"123465"); // "123abc"
s.padStart(8, "0"); // "00000abc"
s.padStart(1); // "abc"

### repeat

```flexscript
string string.repeat(int numRepeats)
```

Makes a new string out of copies of the original.

**Parameters:**
- `numRepeats` *(int)* — The number of repetitions.

**Returns:** A new string made up of copies of the original.

string str = "Clap";
string applause = str.repeat(3); // "ClapClapClap"

### replace

```flexscript
string string.replace(string findStr, string replaceWith, int replaceAll = 0)
```

Replaces a series of characters with another.

**Parameters:**
- `findStr` *(string)* — The string to look for.
- `replaceWith` *(string)* — The string to replace findStr with.
- `replaceAll` *(int, default 0)* — Set to 1 to replace all instances of findStr.

**Returns:** A copy of the string with a find pattern replaced with a new string.

Regular Expressions
A regular expression is a concise way of specifying a pattern of characters to look for. You start and end
a regular expression with a forward slash "/" (similar to how you start and end a string with quotes) and then add modifiers:

/pattern/modifiers
/[0-9]+/g

Here is a brief explanation of some regular expression syntax.

Modifiers
These come after the closing / of the regular expression and modify their behavior
g - Global Match, matches all occurences, and not just the first one.
i - Case-insensitive match.

Brackets
[] - Matches any character in the brackets. Can use a dash to specify a range of characters.
[abc] - matches an a, b or c.
[a-z] - matches any lowercase letter.
[0-9] - matches any numerical digit.
[^abc] - matches any character that is not an a, b or c.

Or
| - Matches any of the alternatives separated by a |.
(abc|cba) - matches the sequence "abc" or "cba".
(gray|grey) or gr(a|e)y - matches "gray" and "grey".

Quantifiers
Define the quantity of characters the preceding expression will match

* - Matches any string that has zero or more of the specified characters.
ab*c - matches "ac", "abc", "abbc", "abbbc", etc. Basically any sequence of an a, any number of b's, and then a c.
+ - Matches any string that has at least one of the specified characters.
ab+c - almost the same as ab*c except that "ac" no longer matches.
? - Matches any string that has zero or one of the specified characters.
colou?r - matches both "color" and "colour", but not "colouur".

Repetitions
{n} - Matches n number of occurences.
{m,n} - Matches at least m, up to n number of occurences.

Periods
. - Matches any character.
\. - Matches a period.

Capture Groups
You can also define what are called capture groups using parentheses. By wrapping parentheses around a character pattern,
results found within the parentheses will be saved for use in the replace string. Use the terms $1, $2, etc. in the replace
string to reference the first, second, third, etc. captured group in the search pattern.

Examples
This code removes all numbers. We are using [0-9] to find characters in the numeric range and then we put a plus
after it to find any sequence of numbers no matter how long. Finally we add the g modifier to match all occurences.

string text = "300 out of 500 soda cans";
return text.replace(/[0-9]+/g, ""); // " out of soda cans"

Replace any instances of blue or green regardless of case with red.

string text = "Blue cars are really green.";
return text.replace(/blue|green/gi, "red"); // "red cars are really red."

Replace all 3 letter words starting with b and ending with t.

string text = "My favorite words are: bit bat but bot bet.";
return text.replace(/b[aeiou]t/g, "Money"); // My favorite words are: Money Money Money Money Money.

Hide any email addresses. Note that "\." is how you specify you want to see a period and not just any character.

string text = "Email me at sim-guy34@gmail.com or dev@flexsim.com";
return text.replace(/([a-z0-9_\.-]+)@([0-9a-z\.-]+)\.([a-z\.]{2,6})/g, "###"); //Email me at ### or ###

FlexScript's string regular expression implementation uses c++'s regex
library for its functionality, using the ECMAScript grammar.
Our design was also guided by JavaScript's
regular expression implementation.
The string.replace() method's regular expression overload is a wrapper around a call to
std::regex_replace.
For more detailed information on building regular expressions, refer to that documentation. Note that we do not (yet) implement
JavaScript's /m, /y, or /u regular expression flags.

### search

```flexscript
int string.search(string str)
```

Searches the string for an internal string or pattern.

**Parameters:**
- `str` *(string)* — The string or pattern to search for.

**Returns:** If found, the 1-based byte index of the found string. Otherwise -1.

string str = "Hello, how are you?";
int howIndex = str.search("how"); // 8
A regular expression can also be used to find strings.
string str = "Hello, how are you?";
int howIndex = str.search(/h[a-z]+/); // 8 (first word starting with h)
See the replace() method for further discussion of regular expressions.

### slice

```flexscript
string string.slice(int beginIndex, int endIndex = 0)
```

Extracts a section of a string and returns a new string.

**Parameters:**
- `beginIndex` *(int)* — The 1-based byte index of the first character to extract. If this index is in the middle of a multi-byte character, it will extract the full character. If it is a negative number, it is indexed offset from the end of the string.
- `endIndex` *(int, default 0)* — The 1-based byte index of the end character. The extraction will go up-to-but-not-including the character at this index. If the index is in the middle of a multi-byte character, the extraction will include that full character. If this parameter is left out, the method will extract to the end of the string. If the parameter is negative, it is indexed offset from the end of the string.

**Returns:** The extracted string.

string text = "Hello World!";
string subText = text.slice(1, 6); // "Hello"
Using a negative number it starts from the end of the string.
string text = "Hello World!";
string subText = text.slice(-6); // "World!"

### split

```flexscript
Array string.split(string delimiter = "", int limit = -1)
```

Splits the string into an array of substrings.

**Parameters:**
- `delimiter` *(string, default "")* — The string that marks the separators for where the string should be split. If this parameter is excluded or is an empty string, every character will be separated into its own array element.
- `limit` *(int, default -1)* — The maximum number of array elements to return. If excluded, it will split the whole string.

**Returns:** An array of substrings made by splitting the string whenever the specified separator is found.

string str = "Hello, how are you?";
Array subStrings = str.split(" ", 3); // ["Hello,", "how", "are"]

string csv = "foo,bar,,baz";
Array subStrings = csv.split(","); // ["foo", "bar", "", "baz"]

A regular expression can also be used to find strings to delimit by. This code removes numbers:
string str = "abcd123efg456hij789klm0";
Array subStrings = str.split(/[0-9]+/g); // ["abcd", "efg", "hij", "klm", ""]

See the replace() method for further discussion of regular expressions.

### startsWith

```flexscript
int string.startsWith(string str)
```

Checks if the string starts with the specified substring.

**Parameters:**
- `str` *(string)* — The substring to look for.

**Returns:** 1 if the string starts with the specified substring, 0 otherwise.

string text = "Hello World!";
int worldStart = text.startsWith("Hello"); // 1

### substr

```flexscript
string string.substr(int beginIndex, int length)
```

Extracts a section of a string and returns a new string.

**Parameters:**
- `beginIndex` *(int)* — The 1-based byte index of the first character to extract. If this index is in the middle of a multi-byte character, it will extract the full character. If it is a negative number it becomes an offset from the end of the string.
- `length` *(int)* — The length of the target string, in bytes.

**Returns:** The extracted string.

string text = "Hello World!";
string subText = text.substr(1, 5); // "Hello"
With the first parameter negative, it starts that many characters from the end.
string text = "Hello World!";
string subText = text.substr(-6, 5); // "World" without the "!"

### toLowerCase

```flexscript
string string.toLowerCase()
```

Converts all letters to lowercase.

**Returns:** A new string with all lowercase letters.

string text = "Hello World!";
string lower = text.toLowerCase(); // "hello world!"

### toNum

```flexscript
double string.toNum()
```

Interprets the string's content as a floating point number.

**Returns:** The interpreted content of the string as a double.

string text = "1.5";
double number = text.toNum(); // 1.500

### toUpperCase

```flexscript
string string.toUpperCase()
```

Converts all letters to uppercase.

**Returns:** A new string with all uppercase letters.

string text = "Hello World!";
string lower = text.toUpperCase(); // "HELLO WORLD!"

### trim

```flexscript
string string.trim()
```

Removes leading and trailing whitespace.

**Returns:** The trimmed string.

string text = " Hello World! ";
string trim = text.trim(); // "Hello World!"

## Properties

### length

```flexscript
int string.length (readonly)
```

The length, in bytes, of the string.

int numBytes = str.length;


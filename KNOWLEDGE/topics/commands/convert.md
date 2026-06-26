---
id: convert
name: "convert"
kind: command
module: ""
signature: "convert(var value, var from, var to) -> var"
aliases: []
tags: ["command", "group-conversions"]
deprecated: false
replacedBy: null
description: "Converts the given value (a time) from one time format to another"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# convert

```flexscript
convert(var value, var from, var to) -> var
```

Converts the given value (a time) from one time format to another

Converts the given value (a time) from one format to another. The from and to arguments only accept the following values:

FS_DATETIME
XL_DATETIME
MODEL_TIME
DATE_STR
TIME_STR
DATETIME_STR

FS_DATETIME - the number of seconds since Jan 1, 1601. getmodelunit(START_TIME) returns a value in FS_DATETIME units.

XL_DATETIME - the serialized day number, where day 1 is Jan 1, 1900. Feb 10, 2025 1:30 PM is 45698.5625 in XL_DATETIME units. This format is used in Microsoft Excel for dates.

MODEL_TIME - the number of model units since the model start time

DATE_STR - a string representing the date portion of a datetime value. The resulting text will depend on the date format specified in the model settings. If converting from this format, the value must be formatted according to those settings.

TIME_STR - a string representing the time portion of a datetime value. The resulting text will depend on the time format specified in the model settings. If converting from this format, the value must be formatted according to those settings.

DATETIME_STR - a string representing the time and date of a datetime value. The resulting text will depend on the time and date formats specified in the model settings. If converting from this format, the value must be formatted according to those settings; the time portion must be first, separated from the date portion by a single space.

Note that year 3000 and beyond are not supported by this command.

## Example

```flexscript
convert(42308, XL_DATETIME, MODEL_TIME) // converts an Excel date to a valid model time
```


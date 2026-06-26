---
id: datetime
name: "DateTime"
kind: class
module: ""
signature: "DateTime"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class containing functions for getting and manipulating dates and times."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\DateTime.xml"
---

# DateTime

A class containing functions for getting and manipulating dates and times.

This DateTime class allows you to create and manipulate dates and times that
are stored as FlexSim time. FlexSim time is defined as the number of seconds since
January 1st, 1601. The specified Model Time Units does not affect FlexSim time. It is always
in seconds.
Model Settings
When you start a new model, you can specify the start date and time of the model. That, and
model stop times, can later be changed through the
Run Time Options
on the Simulation Control Bar. You can also specify the date and time display format in the
Model Settings
window. These formats can be used when displaying a DateTime as a string or for creating a DateTime.
Excel Import/Export
When you import dates and times from an Excel document, you can do so as either strings
or as numbers which are in Excel time. This DateTime class can be used to convert those
strings or Excel time into FlexSim time. For exporting, you can write formatted dates and/or
times to a global table using the DateTime.toString()
method.
Dashboard Charts
Dashboard charts like the Time Plot can use FlexSim time to display the dates and times
along the X or Y axis. This can be done by recording DateTimes in a Statistics Collector.

## Methods

### compose

```flexscript
DateTime (static) DateTime.compose(int year, int month = 1, int day = 1, int hour = 0, int minute = 0, int second = 0, int millisecond = 0)
```

Constructs a new DateTime from component parts.

**Parameters:**
- `year` *(int)* — The year of the DateTime
- `month` *(int, default 1)* — The month of the DateTime (1 - 12)
- `day` *(int, default 1)* — The day of the DateTime (1 - 28,29,30,31)
- `hour` *(int, default 0)* — The hour of the DateTime (0 - 23)
- `minute` *(int, default 0)* — The minute of the DateTime (0 - 59)
- `second` *(int, default 0)* — The second of the DateTime (0 - 59)
- `millisecond` *(int, default 0)* — The millisecond of the DateTime (0 - 999)

DateTime composedDateTime = DateTime.compose(2019, 12, 31, 23, 59, 59);

### days

```flexscript
DateTime (static) DateTime.days(double days)
```

Constructs a new DateTime from the number of days.

**Parameters:**
- `days` *(double)*

DateTime oneDay = DateTime.days(1);

### fromExcelTime

```flexscript
DateTime (static) DateTime.fromExcelTime(double excelTime)
```

Converts Excel time into FlexSim time.

**Parameters:**
- `excelTime` *(double)*

When performing an Excel Import,
the Data Distinction can be set to Values Only. This will cause cells that store dates to be
imported into FlexSim as numbers that are in Excel time. This method will convert that Excel time
into FlexSim time.
DateTime dateTime = DateTime.fromExcelTime(excelTime);

### hours

```flexscript
DateTime (static) DateTime.hours(double hours)
```

Constructs a new DateTime from the number of hours.

**Parameters:**
- `hours` *(double)*

DateTime oneHour = DateTime.hours(1);

### milliseconds

```flexscript
DateTime (static) DateTime.milliseconds(double milliseconds)
```

Constructs a new DateTime from the number of milliseconds.

**Parameters:**
- `milliseconds` *(double)*

DateTime oneMillisecond = DateTime.milliseconds(1);

### minutes

```flexscript
DateTime (static) DateTime.minutes(double minutes)
```

Constructs a new DateTime from the number of minutes.

**Parameters:**
- `minutes` *(double)*

DateTime oneMinute = DateTime.minutes(1);

### seconds

```flexscript
DateTime (static) DateTime.seconds(double seconds)
```

Constructs a new DateTime from the number of seconds.

**Parameters:**
- `seconds` *(double)*

DateTime oneSecond = DateTime.seconds(1);

### DateTime

```flexscript
DateTime DateTime.DateTime(double time)
```

Constructs a DateTime.

**Parameters:**
- `time` *(double)* — The number of seconds since January 1st, 1601.

DateTime theDate = DateTime(3600);

The time can be treated as a duration if you add two DateTimes together. For example,

DateTime futureDate = Model.dateTime + DateTime(340); //Add 340 seconds to the current model time

The second constructor can be used to create a DateTime from a string representation of a date.
See toString() for formatting options.

DateTime theDate = DateTime("12/31/2019 8:00:00 AM", "%m/%d/%Y %I:%M:%S %p");

The third constructor can be used to create a DateTime that is rounded down to the nearset
important marker. For example, if the current model time is between 8:00 and 9:00 on a particular day, you could use
the following code to create a date time that represents 8:00 on that same day:

DateTime roundedToHour = DateTime(Model.dateTime.totalDays, Model.dateTime.hours);

### toString

```flexscript
string DateTime.toString(string format)
```

Converts the DateTime into a human readable string.

**Parameters:**
- `format` *(string)* — If format is NULL, the default windows date/time format will be used.

This method uses the windows settings for formatting. When using a custom format, the
following formatting options are available:

Specifier
Explanation
Example

%%
A % character
%

%n
New line character

%t
Horizontal tab character

Year

%Y
The four digit year
2017

%y
The last two digits of the year
17

%C
The first two digits of the year
20

Month

%b
The abbreviated month name
Oct

%B
The full month name
October

%m
The month as a number (1 - 12)
10

Week

%U
The week of the year as a number with Sunday being the first day of the week (00 - 53)
15

%W
The week of the year as a number with Monday being the first day of the week (00 - 53)
15

Days of the year/month

%j
The day of the year as a three digit number (001 - 366)
045

%d
The day of the month as a two digit number (01 - 31)
05

%e
The day of the month as a single digit number (1 - 31)
5

Day of the week

%a
The abbreviated weekday name
Fri

%A
The full weekday name
Friday

%w
The weekday as a number where Sunday is 0 (0 - 6)
3

Hour, minute, second

%H
The hour as a two digit number, 24 hour clock (00 - 23)
08

%I
The hour as a two digit number, 12 hour clock (01 - 12)
08

%M
The minute as a two digit number (00 - 59)
30

%S
The second as a two digit number (00 - 60)
25

Other

%c
The standard date and time (windows default setting)
09/24/17 08:00:00

%x
The standard date (windows default setting)
09/24/17

%X
The standard time (windows default setting)
08:00:00

%D
Equivalent to %m/%d/%y
09/24/17

%F
Equivalent to %Y-%m-%d
2017-09-24

%r
The 12-hour clock time (based on windows setting)
08:00:00

%R
Equivalent to %H:%M
08:00

%T
Equivalent to %H:%M:%S
08:00:00

%p
AM or PM (based on windows setting)
AM

%z
The offset from UTC time
-0600

%Z
The time zone name or abbreviation
Mountain Daylight Time

string currentDateTime = Model.dateTime.toString();
string currentDateTime = Model.dateTime.toString("%H:%M:%S");

## Properties

### day

```flexscript
double DateTime.day (readonly)
```

The day of the month

Value ranges from 1 to 31.
double currentDay = Model.dateTime.day;

### dayOfWeek

```flexscript
double DateTime.dayOfWeek (readonly)
```

The day of the week

Value ranges from 1 to 7 with 1 being Sunday.
double currentDayOfWeek = Model.dateTime.dayOfWeek;

### hour

```flexscript
double DateTime.hour (readonly)
```

The hour of the day

Value ranges from 0 to 23 with the 0 hour being from 12:00:00AM to 12:59:59AM.
double currentHour = Model.dateTime.hour;

### millisecond

```flexscript
double DateTime.millisecond (readonly)
```

The millisecond of the second.

Value ranges from 0 to 999.
double currentMillisecond = Model.dateTime.millisecond;

### minute

```flexscript
double DateTime.minute (readonly)
```

The minute of the hour.

Value ranges from 0 to 59.
double currentMinute = Model.dateTime.minute;

### month

```flexscript
double DateTime.month (readonly)
```

The month of the year

Value ranges from 1 to 12.
double currentMonth = Model.dateTime.month;

### second

```flexscript
double DateTime.second (readonly)
```

The second of the minute.

Value ranges from 0 to 59.
double currentSecond = Model.dateTime.second;

### totalDays

```flexscript
double DateTime.totalDays (readonly)
```

The number of days since January 1st, 1601.

This will return a floating point number where the fraction represents the time into the current day.
double totalDays = Model.dateTime.totalDays;

### totalHours

```flexscript
double DateTime.totalHours (readonly)
```

The number of hours since January 1st, 1601.

This will return a floating point number where the fraction represents the time into the current hour.
double totalHours = Model.dateTime.totalHours;

### totalMilliseconds

```flexscript
double DateTime.totalMilliseconds (readonly)
```

The number of milliseconds since January 1st, 1601.

This will return a floating point number where the fraction represents the time into the current millisecond.
double totalMilliseconds = Model.dateTime.totalMilliseconds;

### totalMinutes

```flexscript
double DateTime.totalMinutes (readonly)
```

The number of minutes since January 1st, 1601.

This will return a floating point number where the fraction represents the time into the current minute.
double totalMinutes = Model.dateTime.totalMinutes;

### totalSeconds

```flexscript
double DateTime.totalSeconds (readonly)
```

The number of seconds since January 1st, 1601.

This is equivalent to DateTime.value or putting the DateTime into a double.
double totalSeconds = Model.dateTime.totalSeconds;

### value

```flexscript
double DateTime.value (readonly)
```

The value of the date and time in FlexSim time.

This property is equivalent to DateTime.totalSeconds or putting a DateTime into a double.

double currentTime = Model.dateTime.value;
double currentTime = Model.dateTime;

### year

```flexscript
double DateTime.year (readonly)
```

The year

double currentYear = Model.dateTime.year;


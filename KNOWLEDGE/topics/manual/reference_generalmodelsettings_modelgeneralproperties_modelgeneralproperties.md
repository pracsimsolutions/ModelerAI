---
id: reference_generalmodelsettings_modelgeneralproperties_modelgeneralproperties
name: "Model Settings"
kind: manual
breadcrumb: ["Reference","GeneralModelSettings","ModelGeneralProperties","ModelGeneralProperties"]
tags: ["manual","reference","generalmodelsettings","modelgeneralproperties"]
source: "manual/Reference/GeneralModelSettings/ModelGeneralProperties/ModelGeneralProperties.html"
---

# Model Settings

## Overview and Key Concepts

 

You can access the model settings from the Edit menu on the main toolbar.

 

 

 

## Properties

 

The model has the following settings:

 

 

### Decimal Precision

 

This option controls the precision of editable values seen in the interface.

 

### Spline Tension

 

This option controls how straight the lines in a curved network node path are. A value close to 0 will produce straight lines. A value close to 1 will produce curved lines.

 

### Random Number Streams

 

This option controls the range (between 0 and the specified value) of random number streams that will be initialized by Flexsim when the model is reset.

 

### Length Units

 

This displays the length units of the model.

 

### Time Units

 

This displays the time units of the model.

 

### Fluid Units

 

This displays the fluid units of the model.

 

### Time Format

 

This specifies what format the time portion of model date times should be displayed in. The following is a list of characters or elements that may be used in a time format. Body text can be added to the format string by enclosing the text in single quotes.

 

| Element | Description |
| --- | --- |
| h | The one- or two-digit hour in 12-hour format. |
| hh | The two-digit hour in 12-hour format. Single-digit values are preceded by a zero. |
| H | The one- or two-digit hour in 24-hour format. |
| HH | The two-digit hour in 24-hour format. Single-digit values are preceded by a zero. |
| m | The one- or two-digit minute. |
| mm | The two-digit minute. Single-digit values are preceded by a zero. |
| s | The one- or two-digit second. |
| ss | The two-digit second. Single-digit values are preceded by a zero. |
| t | The one-letter AM/PM abbreviation (that is, AM is displayed as "A"). |
| tt | The two-letter AM/PM abbreviation (that is, AM is displayed as "AM"). |

 

### Date Format

 

This specifies what format the date portion of model date times should be displayed in. The following is a list of characters or elements that may be used in a date format. Body text can be added to the format string by enclosing the text in single quotes.

 

| Element | Description |
| --- | --- |
| d | The one- or two-digit day. |
| dd | The two-digit day. Single-digit day values are preceded by a zero. |
| ddd | The three-character weekday abbreviation. |
| dddd | The full weekday name. |
| M | The one- or two-digit month number |
| MM | The two-digit month number. Single-digit values are preceded by a zero. |
| MMM | The three-character month abbreviation. |
| MMMM | The full month name. |
| yy | The last two digits of the year (that is, 1996 would be displayed as "96"). |
| yyyy | The full year (that is, 1996 would be displayed as "1996"). |

  

#### Time and Date Formats Don't Mix

 

Time formats and date formats are exclusive and may not be mixed.

  

### Embed media with model

 

This option allows you to embed all of the 3D and image files associated with your model into the model file.

 

### Export Embedded Media

 

This option allows you to export the embedded 3D shapes and image files to the current directory the model is saved in.

  

#### Additional Files - Embedded Media

 

Some 3D shape formats include support for referencing external files from the main file. When loading a shape with external file references, FlexSim's packed media feature only embeds the main file and referenced textures. Other externally referenced files will not be embedded into the fsm file.

  

### Disable AutoSave for this model

 

This option allows you to disable the AutoSave feature as defined in Global Preferences.

 

### Use Legacy SQL Nulls

 

This option enables legacy SQL nulls. Upgraded models will automatically enable this option, but new models will default to using the SQL ANSI standard for nulls.

 

### Encrypt Model

 

This area allows you to set a password for your model, so that only those with the password can open it. There is no way to recover lost passwords. To set a password, click the *Set Password button*. Then enter the desired password and click the *Enter Password* button. Enter the password again and click the *Confirm Password* button. To remove a password, click the button.

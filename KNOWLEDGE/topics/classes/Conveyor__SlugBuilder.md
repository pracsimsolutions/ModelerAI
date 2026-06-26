---
id: conveyor__slugbuilder
name: "Conveyor::SlugBuilder"
kind: class
module: "Conveyor"
signature: "Conveyor::SlugBuilder"
aliases: []
tags: ["class", "conveyor-module"]
deprecated: false
replacedBy: null
description: "Accesses data dealing with a conveyor's slug building state"
seeAlso: []
source: "help\\FlexScriptAPIReference\\Conveyor\\Conveyor.SlugBuilder.xml"
---

# Conveyor::SlugBuilder

Accesses data dealing with a conveyor's slug building state

This object can be accessed through the Conveyor.slugBuilder property,
or through the Conveyor.MergeController.lanes property. This object
only exists if the conveyor is connected to a merge controller object.

## Methods

### makeReady

```flexscript
Conveyor::SlugBuilder.makeReady()
```

Manually marks the slug ready for release

This method can be used to make a slug ready before it would otherwise become ready.
slugBuilder.makeReady();

### release

```flexscript
Conveyor::SlugBuilder.release()
```

Manually releases a slug.

Can be used to release a slug, disregarding ready requirements and lane clear requirements.
slugBuilder.release()

## Properties

### conveyor

```flexscript
Conveyor Conveyor::SlugBuilder.conveyor (readonly)
```

Returns the conveyor associated with this slug builder

Conveyor c = slugBuilder.conveyor;

### isClear

```flexscript
int Conveyor::SlugBuilder.isClear (readonly)
```

Returns 1 if all necessary control points are clear for the current slug to be released, 0 otherwise.

if (slugBuilder.isClear) slugBuilder.release();

### isEnabled

```flexscript
int Conveyor::SlugBuilder.isEnabled
```

Gets or sets whether the conveyor associated with this slugBuilder will build slugs.

Before disabling, be sure that the lane does not have a slug.slugBuilder.isEnabled = 1;

### lastAddedItem

```flexscript
Object Conveyor::SlugBuilder.lastAddedItem (readonly)
```

Gets the item that was last added to the slug.

### state

```flexscript
int Conveyor::SlugBuilder.state (readonly)
```

Gets the slug-building state of the slug builder

0 - None
1 - Building - a slug has begun building.
2 - Ready - the slug has acheived its ready criteria
3 - Releasing - the slug is being released


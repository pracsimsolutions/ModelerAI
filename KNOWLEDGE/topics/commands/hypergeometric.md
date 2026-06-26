---
id: hypergeometric
name: "hypergeometric"
kind: command
module: ""
signature: "hypergeometric(num trials, num successes, num size[, num stream]) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "Returns a random sample from a hypergeometric distribution"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# hypergeometric

```flexscript
hypergeometric(num trials, num successes, num size[, num stream]) -> num
```

Returns a random sample from a hypergeometric distribution

hypergeometric( t, m, n, stream ) is a discrete probability distribution
used to represent the number of successes in t draws without replacement from a population of size n
containing exactly m success states.


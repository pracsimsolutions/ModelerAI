---
id: addbundlefield
name: "addbundlefield"
kind: command
module: ""
signature: "addbundlefield(node datanode, str fieldname, num type[, num maxstrsize]) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: "with"
description: "Adds a field to the bundle"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# addbundlefield

```flexscript
addbundlefield(node datanode, str fieldname, num type[, num maxstrsize]) -> num
```

Adds a field to the bundle

Adds a field to the bundle with the given field name. Returns the field rank (base 0) of the added field.
All fields for a bundle must be added before any entries are added.
The type is a bitwise flag that must be one of the following values:
BUNDLE_FIELD_TYPE_DOUBLE : stores a 64-bit double
BUNDLE_FIELD_TYPE_FLOAT : stores a 32-bit float
BUNDLE_FIELD_TYPE_INT : stores a 32-bit integer
BUNDLE_FIELD_TYPE_STR : stores a string. If used, you should also define the maximum string size, which will be the space allocated for each string entry.
If maxstrsize is unspecified, the default will be 32 bytes. The field will be able to store strings at least as long as the specified length, and any longer strings will be truncated.
BUNDLE_FIELD_TYPE_VARCHAR: stores a string. This field type does not impose
a limit on string length. This field type is recommended if you need variable string lengths,
or if strings are frequently repeated.
BUNDLE_FIELD_TYPE_NODEREF : stores a reference to a node
BUNDLE_FIELD_TYPE_BINARY : stores a 1 or 0 as a single bit. Bits are allocated in blocks of 32, so that
one binary field requires 32 bits. However, 32 binary fields can share a single block.

You can optionally bitwise combine any of the above values (except BUNDLE_FIELD_TYPE_BINARY) with BUNDLE_FIELD_INDEX_MAP. All values in this type of field
will be indexed for quick lookup (O(log n)) using getbundleindexentries().

You can optionally combine BUNDLE_FIELD_TYPE_INT, BUNDLE_FIELD_TYPE_STR, BUNDLE_FIELD_TYPE_VARCHAR, or BUNDLE_FIELD_TYPE_NODEREF with BUNDLE_FIELD_INDEX_HASH. All values in this type of field
will be indexed for quick lookup (O(1)) using getbundleindexentries(). This type of field is only recommended for use with
bundles that rarely, if ever, change size while the model is running.

You can optionally combine any of the above values (except BUNDLE_FIELD_TYPE_BINARY) with BUNDLE_FIELD_NULLABLE, to allow those fields to contain null values.
If a field is nullable, you can pass a null Variant in to setbundlevalue(), and it is possible to get a null Variant from getbundlevalue().

## Example

```flexscript
addbundlefield(x, "Object Name", BUNDLE_FIELD_TYPE_STR | BUNDLE_FIELD_NULLABLE, 64);
```


# Parser: Read String

Strings store their length followed by its zero terminated content.

E.g. `Test` is stored as

```text
| 04 00 | 54 65 73 74 00 |
|  len  |    content     |
```

- `len`: 2 Byte unsigned integer
- `content`: Zero terminated string (1 character = 1 Byte)

# Structure Example

An excerpt of a structure is shown below. At first ist looks complicated and uninformative but lets start to apply some knowledge we obtained to dissect the Byte stream and try to understand it.

```text
1F 21 00 00 00 00 00 00 00 1F 0B 00 00 00 00 00
00 00 1F 00 00 FF E4 5C 39 00 00 00 00 07 00 54
44 2D 32 35 58 41 00 00 00 00 02 00 53 57 00 00
00 00 07 00 54 44 2D 32 35 58 41 00 01 00 20 37
00 00 00 00 00 00 00 20 0B 00 00 00 00 00 00 00
20 FF FF FF E4 5C 39 00 00 00 00 00 00 00 07 00
54 44 2D 32 35 58 41 00 04 00 01 00 33 00 7F 01
00 32 00 7F 01 00 34 00 7F 01 00 31 00 7F
```

In the following I added line breaks at data boundaries and some comments.

```bash
#   Structure (Type 0x1F)
    1F 7E 00 00 00 00 00 00 00 # Prefix 0
    1F 21 00 00 00 00 00 00 00 # Prefix 1
    1F 0B 00 00 00 00 00 00 00 # Prefix 2
#      |
#      ---- Length Specification for Checkpoints

    1F 00 00                   # Short Prefix 3
#   |
#   ------- Structure Type: 0x1F

    FF E4 5C 39 00 00 00 00    # Preamble

#   Start of Structure content

#   Checkpoint: 0x0B

#   String
    07 00 54 44 2D 32 35 58 41 00

#   String
    00 00 00

#   Checkpoint: 0x21

#   String
    02 00 53 57 00

#   String
    00 00 00

#   String
    07 00 54 44 2D 32 35 58 41 00

#   Unsigned Integer (Number of following Structures)
    01 00

#   Inner Structure
    20 37 00 00 00 00 00 00 00
    20 0B 00 00 00 00 00 00 00
    20 FF FF
    FF E4 5C 39 00 00 00 00
    00 00 00
    07 00 54 44 2D 32 35 58 41 00
    04 00
    01 00 33 00 7F
    01 00 32 00 7F
    01 00 34 00 7F
    01 00 31 00 7F

#   Checkpoint: 0x7E
```

Take a look at the large undocumented data block at the end of the example. It is a structure contained in the outer structure but the way it is built is quit similar. Of course the content is different but the general layout is the same. Try to recognize some of the elements described in the outer structure as an exercise. I inserted a few line breaks to make it a bit more obvious.

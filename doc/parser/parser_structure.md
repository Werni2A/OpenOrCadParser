# Parser: Reading Structures

There are different types of structures but all are built in a similar way with some similar bits. You find the different parser implementations in `/src/Structures`.

Lets take a look at how the structure parsers are set up. In the beginning you will find similar code like the following.

```C++
FutureDataLst localFutureLst{mCtx};

auto_read_prefixes(localFutureLst);
// read_prefixes(3, localFutureLst);

readPreamble();
```

Every structure starts with a certain number of prefixes that is fixed for each structure type. For simplicity we just try out different numbers of prefixes and take the one that works (`auto_read_prefixes`). Each prefix contains a length specification that we save into `localFutureLst` and verify at `localFutureLst.checkpoint`.

```C++
localFutureLst.checkpoint();
```

At the end all checkpoints from the prefixes must have been verified if the parser works correctly.

```C++
localFutureLst.sanitizeCheckpoints();
```

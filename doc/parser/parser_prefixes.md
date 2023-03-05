# Parser: Read Prefixes

Each structure starts with a certain number of prefixes where two properties hold:

- Number of prefixes vary for different structures,
- however all structures of the same type have constant number of prefixes.

If you want to know the number of prefixes in your structure you can check the logging output because the following lines try to figure this out and write it down.

```C++
FutureDataLst localFutureLst{mCtx};

auto_read_prefixes(localFutureLst);
```

Each prefix contains the type of structure as well as some length definition. Due to the fact that we have multiple prefixes, there are also multiple lengths defined in the prefixes but they all deviate from each other because they point to different parts in the structure. However, it's unknown what the meaning of each part is.

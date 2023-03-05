# Parser: Reading Primitive

Like structures, primitives are common in their data layout. However, they are much simpler, e.g. there are no prefixes. Must importantly the preamble is located at the end of each structure instead of the beginning. Further there is length specifier but it seems to me that it varies and is not that easy to verify.

```C++
    const uint32_t byteLength = ds.readUint32();

    ds.assumeData({0x00, 0x00, 0x00, 0x00});

    // Actual content

    readPreamble();
```

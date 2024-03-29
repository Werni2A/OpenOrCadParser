# Parser Implementation

Implementing a stream parser consists of two consecutive steps.

1. Parsing the stream layout and
2. parsing the content.

Purpose of the first step is to recognize the structure of the stream layout. I.e. it's important to know what data structures exist, how to calculate their structure size and offsets in the stream. In this step it's not relevant what the meaning of the bits and bytes is, as long as they don't change the general structure size. E.g. strings are easy to recognize and change the structure size due to their dynamic size. However, in this step we only care about how the string size contributes to the structure size. Beside, strings and lists most -- if not all -- elements have fixed sizes. Checking wether the layout parser for a stream is correct is easy because we can check if the parser finishes its operation exactly at the end of the stream. Stopping before the end or reading beyond EoF is a sign for failure. This means there is some change in the layout that was not anticipated by the parser.

In the second step we assume that the layout was mapped out correctly therefore it is only necessary to find correct data types and the meaning of the bytes. This is a much harder step because users often keep default settings of the application instead of changing them very often. This results in blocks with seemingly constant data, often all zero for the default values. In contrast to not changing values, some data elements have arbitrary values that change very frequently. For example a offset represented by 8 Bit takes values 0 to 255. In this case we can not automatically check if the value is correct. All values are possible, but we would have to compare it against its ground truth to see if it is correct. A a lot easier are enumerations since they can only take a set of fixed values that can be validated. Parsing a value that is not in the enumeration set is an indicator that the attribute is located somewhere else than was expected. In a large set of files we assume that every possible value is taken at some point. I.e. the parser must fail if an invalid value for e.g. an enumeration is read from the stream.

More detailed parser implementation:

- [String](/doc/parser/parser_string.md)
- [Prefixes](/doc/parser/parser_prefixes.md)
- [Structure](/doc/parser/parser_structure.md)
- [Primitive](/doc/parser/parser_primitive.md)
- [Structure Example](/doc/parser/example_structure.md)

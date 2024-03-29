
# OpenOrCadParser

Purpose of this project is to provide a C++17 library for parsing [Cadence](https://en.wikipedia.org/wiki/Cadence_Design_Systems)'s OrCAD Capture binary file formats. This library can be linked to other software for reading/modifying?/writing? OrCAD binary files.

**See also the complementary project [OpenAllegroParser](https://github.com/Werni2A/OpenAllegroParser).**

The focus lies on schematics (`*.DSN`/`*.DBK`) and symbol libraries (`*.OLB`/`*.OBK`), the second file format is just the extension for its corresponding backup file. Backups should be identical, just with a different file extension.

OrCAD allows exporting its designs to XML files which are easy to read and verify the interpretation of bytes in the `DSN`/`OLB`-files. E.g. enums are stored as a numeric value which should be the same that is used in the binary formats. However, it seems like not all information is stored in the XML.

The following two XSD files provide a good overview of the `XML` file structure, types and much more. They can be used as a reference what features are actually stored in the binary file and which we do already parse and which not.

`C:\Cadence\SPB_17.4\tools\capture\tclscripts\capDB\dsn.xsd`

`C:\Cadence\SPB_17.4\tools\capture\tclscripts\capDB\olb.xsd`

---

# Documentation

1. [File Container](/doc/file_container.md)
   1. [Container Structure](/doc/container_structure.md)
2. [File Format Changes](/doc/file_format_changes.md)
3. [Tests](/doc/tests.md)
4. [Parser Implementation](/doc/parser/parser.md)

---

# Current State &mdash; January 2023

- I plan on implementing a very basic XML exporter for `Package`
  - This allows to generate images by importing into KiCad via [pyorlib2ki](https://github.com/fjullien/pyorlib2ki)
- It's planned to finish the `Package` parser around spring 2023
- A PoC KiCad importer should follow in summer 2023

---

# Build

```bash
# Install and set up vcpkg
git clone https://github.com/microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh

# Set path to vcpkg
VCPKG_DIR=$(realpath ./vcpkg)

# Get OpenOrCadParser
git clone https://github.com/Werni2A/OpenOrCadParser.git

cd OpenOrCadParser

# Build
cmake -B build -DCMAKE_BUILD_TYPE=Release -S . -DCMAKE_TOOLCHAIN_FILE=$VCPKG_DIR/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

## Dependencies

- Compiler with C++17 Support
- [CMake](https://cmake.org/) >= 3.8
- [Boost](https://www.boost.org/) - Program Options
- [compoundfilereader](https://github.com/Microsoft/compoundfilereader)
- [fmt](https://github.com/fmtlib/fmt)
- [Magic Enum](https://github.com/Neargye/magic_enum)
- [Nameof](https://github.com/Neargye/nameof)
- [spdlog](https://github.com/gabime/spdlog)
- [TinyXML2](https://github.com/leethomason/tinyxml2)

---

# Usage

```bash
./cli/OpenOrCadParser-cli --help
Allowed options:
  -h [ --help ]               produce help message
  -t [ --print_tree ]         print container tree
  -e [ --extract ]            extract binary files from CFBF container
  -i [ --input ] arg          input file to parse
  -o [ --output ] arg         output path (required iff extract is set)
  -v [ --verbosity ] arg (=4) verbosity level (0 = off, 6 = highest)
  -s [ --stop ]               stop parsing on low severity errors
  -k [ --keep ]               keep temporary files after parser completed

./cli/OpenOrCadParser-cli --input file.DSN
./cli/OpenOrCadParser-cli --input file.DSN --extract --output out/
./cli/OpenOrCadParser-cli --input file.DSN --print_tree
./cli/OpenOrCadParser-cli --input file.OLB --verbosity 6 --keep >> file.txt
```

## Generate Image from Library `Package`

[pyorlib2ki](https://github.com/fjullien/pyorlib2ki) makes it possible to import XML files into KiCad getting a graphical representation of the `Package`.

Pictures make it easy to compare against OrCAD's original but do not expect much of it at the moment. The required XML export feature is still in a very early stage. See the following image as an example.

![Example XML import into KiCad](/doc/img/example_xml_kicad_import.png)

```bash
#!/bin/bash

# Generate XML file
./build/cli/OpenOrCadParser-cli -k -i "Library.OLB"

# Copy XML path to this variable
XML_PATH=/tmp/OpenOrCadParser/bffb65106f5b2365049dfc9de4a8b7e3/Package.OLB/xml/PackageName0.xml

SYM_PATH=PackageName0.kicad_sym

# Use https://github.com/fjullien/pyorlib2ki
python pyorlib2ki.py -i $XML_PATH -o $SYM_PATH

# Import symbol into KiCad and export graphical representation
```


---

# Unit Tests

[Test Documentation](doc/tests.md)

---

# Related Projects

[orlib2ki](https://github.com/fjullien/orlib2ki) (C based OrCAD XML Library to KiCad Converter)

[pyorlib2ki](https://github.com/fjullien/pyorlib2ki) (Python based OrCAD XML Library to KiCad Converter)

---

# How to Contribute?

There are different ways to help this project forward. Some are

- provide test files (manually created or automated via Tcl/Tk),
- help reverse engineering/documenting the file format, or
- implement some unit tests.

In case you don't have access to OrCAD you can get

- [OrCAD Viewer](https://www.orcad.com/downloads/orcad-viewer) free of charge,
- [OrCAD Trial](https://dev.orcad.com/orcad-free-trial) with all its functionality for a trial period of 30 days or
- [OrCAD Academic Program](https://www.orcad.com/orcad-academic-program) with all functionality for one semester.

What about OrCAD Lite? It was was replaced with OrCAD Viewer/Trial [[1]](https://www.orcad.com/resources/download-orcad-lite).

---

# Cadence Terms and Conditions

[Terms and Conditions](https://www.cadence.com/content/dam/cadence-www/global/en_US/documents/terms-and-conditions/cadence-orcad.pdf)

[Terms of Use Agreement](https://www.cadence.com/en_US/home/terms-of-use-agreement.html)

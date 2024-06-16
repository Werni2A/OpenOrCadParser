
# OpenOrCadParser

Purpose of this project is to provide a C++20 library for parsing [Cadence](https://en.wikipedia.org/wiki/Cadence_Design_Systems)'s OrCAD Capture binary file formats. This library can be linked to other software for reading/modifying?/writing? OrCAD binary files. This repository does not cover every possible use case, maybe you find what you are looking for in section [Related Projects](doc/related_projects.md).

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

# Current State &mdash; June 2024

- Working on a native KiCad 'Library' Importer

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

- Compiler with C++20 Support
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

## :construction: KiCad Import

An initial draft of the KiCad importer is provided on my [`add-orcad-importer`-Branch](https://gitlab.com/Werni2A/kicad/-/tree/add-orcad-importer?ref_type=heads). Current focus is to get the 'Library' import into a mature enough state to display most important features and merge it into upstream KiCad.

'Design' imports are not yet supported nor worked on, this will probably take some more time.

![KiCad Imported Package](/doc/img/example_kicad_import_package.png)

## :skull: XML Export

:warning: Previously I provided a preliminary XML export that could be used with [pyorlib2ki](https://github.com/fjullien/pyorlib2ki) to import into KiCad. I abandoned the XML exporter in favor of a native KiCad importer. :warning:

---

# Unit Tests

[Test Documentation](doc/tests.md)

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

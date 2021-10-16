
# OpenOrCadParser

Purpose of this project is to provide a C++17 library for parsing [Cadence](https://en.wikipedia.org/wiki/Cadence_Design_Systems)'s OrCAD Capture binary file formats. This library can be linked to other software for reading/modifying?/writing? OrCAD binary files.

**See also the complementary project [OpenAllegroParser](https://github.com/Werni2A/OpenAllegroParser).**

The focus lies on schematics (`*.DSN`/`*.DBK`) and symbol libraries (`*.OLB`/`*.OBK`), the second file format is just the extension for its corresponding backup file. Backups should be identical, just with a different file extension.

OrCAD allows exporting its designs to XML files which are easy to read and verify the interpretation of bytes in the `DSN`/`OLB`-files. E.g. enums are stored as a numeric value which should be the same that is used in the binary formats. However, it seems like not all information is stored in the XML.

The following two XSD files provide a good overview of the `XML` file structure, types and much more. They can be used as a reference what features are actually stored in the binary file and which we do already parse and which not.

`C:\Cadence\SPB_17.4\tools\capture\tclscripts\capDB\dsn.xsd`

`C:\Cadence\SPB_17.4\tools\capture\tclscripts\capDB\olb.xsd`

---

# Technical Information

## File Container

OrCAD's file formats `DSN` and `OLB` use [Microsoft's Compound File Binary File Format (CFBF)]((https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-cfb/53989ce4-7b05-4f8d-829b-d08d6148375b)) or the older version [OLE1.0 and OLE2.0 Formats](https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-oleds/fdc5e702-d09e-4344-a77f-eb079d41f23f), see also [Wikipedia](https://en.wikipedia.org/wiki/Compound_File_Binary_Format). According to the `file` command on Linux, version 2 is used. This format can be read via [CompoundFileReader](https://github.com/microsoft/compoundfilereader).

[OpenMCDF](http://openmcdf.sourceforge.net/)'s Structured Storage eXplorer allows a fast investigation of the file structure (Windows only).

## Container Structure

The file structure for DSN and OLB files is almost similar, DSN files only include a few more streams. The following list gives an overview of which streams are contained and where they are placed.

In front of each entry is a specifier placed that shows of which type the entry is. [D] represents a directory and [S] a stream (file). Directories are also bold. Cursive texts represent names that are provided by the designer. E.g. *Package Name 1* might become `NE555`.

- [S] `AdminData` (DSN only)
- [S] `Cache`
- [D] **`Cells`**
- [S] `Cells Directory`
- [S] `DsnStream` (DSN only)
- [D] **`ExportBlocks`**
- [S] `ExportBlocks Directory`
- [D] **`Graphics`**
  - [S] `$Types$`
- [S] `Graphics Directory`
- [S] `HSObjects` (DSN only)
- [S] `Library`
- [S] `NetBundleMapData` (since version...?)
- [D] **`Packages`**
  - [S] *Package Name 1*
  - [S] *Package Name 2*
  - [S] *...*
- [S] `Packages Directory`
- [D] **`Parts`**
- [S] `Parts Directory`
- [D] **`Symbols`**
  - [S] `$Types$`
  - [S] `ERC` (DSN only)
  - [S] *Symbol Name 1*
  - [S] *Symbol Name 2*
  - [S] *...*
- [S] `Symbols Directory`
- [D] **`Views`**
  - [D] ***View Name 1***
    - [D] **`Hierarchy`** (DSN only)
      - [S] `Hierarchy` (DSN only)
    - [D] **`Pages`**
      - [S] *Page Name 1*
      - [S] *Page Name 2*
      - [S] *...*
    - [S] `Schematic`
  - [D] ***View Name 2***
  - [D] ***...***
- [S] `Views Directory`

---

# File Format Changes

Changelogs and some history is provided in
- [OrCAD Capture What's New](https://www.orcad.com/products/orcad-capture/new).
- [OrCAD Version History](https://www.ema-eda.com/products/cadence-orcad/orcad-version-history)

Typically, over time a file format changes due to new features introduced and old ones or mistakes being removed. This section should provide some insight what changes were introduced in which version.

- OrCAD 16.2 (see [here](https://www.cetti.ro/v2/noutati/OrCAD_16.2_WhatIsNew.pdf))
  - Line styles can be further adjusted (see [here](https://www.cetti.ro/v2/noutati/OrCAD_16.2_WhatIsNew.pdf#page=12))
  - Junction dot size can be adjusted (see [here](https://www.cetti.ro/v2/noutati/OrCAD_16.2_WhatIsNew.pdf#page=14))
  - Introduced elliptical arcs (see [here](https://www.cetti.ro/v2/noutati/OrCAD_16.2_WhatIsNew.pdf#page=17))

- OrCAD 16.5 QIR #5 (see [here](https://dev.orcad.com/sites/orcad/files/resources/files/Cadence-OrCAD-166-QIR5-Whats-New.pdf))

- OrCAD 16.6 (see [here](https://www.flowcad.de/datasheets/Whats_New_in_OrCAD_Capture16-6_FlowCAD.pdf))
  - NetGroup enhancements (see [here](https://www.flowcad.de/datasheets/Whats_New_in_OrCAD_Capture16-6_FlowCAD.pdf#page=12))
  - Shapes for hierarchic blocks (see [here](https://www.flowcad.de/datasheets/Whats_New_in_OrCAD_Capture16-6_FlowCAD.pdf#page=13))
  - Introduced flag to indicate user specified references (see [here](https://www.flowcad.de/datasheets/Whats_New_in_OrCAD_Capture16-6_FlowCAD.pdf#page=15))
  - Introduce XML import/export and XSD schema (see [here](https://www.flowcad.de/datasheets/Whats_New_in_OrCAD_Capture16-6_FlowCAD.pdf#page=17))
  - Introduce user specific DRCs (see [here](https://www.flowcad.de/datasheets/Whats_New_in_OrCAD_Capture16-6_FlowCAD.pdf#page=18))
  - Introduce flag for waived DRCs (see [here](https://www.flowcad.de/datasheets/Whats_New_in_OrCAD_Capture16-6_FlowCAD.pdf#page=19))
  - Database upgrade (see [here](https://www.flowcad.de/datasheets/Whats_New_in_OrCAD_Capture16-6_FlowCAD.pdf#page=21))

- OrCAD 16.6 QIR #5 (see [here](https://www.orcad.com/sites/orcad/files/resources/files/Cadence-OrCAD-166-QIR5-Whats-New.pdf))

- OrCAD 16.6 QIR #7 HotFix 32 (see [here](https://www.orcad.com/sites/orcad/files/resources/files/OrCAD_16.6QIR7_WhatsNew.pdf) or [here](https://www.artedas.fr/documentsPDF/HotFix/Hotfix_SPB16.60.032_readme_product_note.pdf))

- OrCAD 16.6 QIR #8 HotFix 38 (see [here](https://www.artedas.it/attach/repository/document/201411/Hotfix_SPB16.60.038_readme_product_note.pdf))

- OrCAD 16.6 HotFix 51 (see [here](https://www.artedas.it/attach/repository/document/201506/Release16.6-2015_readme_product_note.pdf))

- OrCAD 17.0 (see [here](http://www.linelayout.com/bbs/bbs/uploadfile/image/2015041515460682682.pdf))
  - Allow negative x offset in intersheet references window (see [here](http://www.linelayout.com/bbs/bbs/uploadfile/image/2015041515460682682.pdf#page=74))

- OrCAD 17.2 QIR #1 (see [here](https://www.ema-eda.com/sites/ema/files/resources/files/Cadence%20OrCAD_%2017.2%20QIR1.pdf))
  - Introduced design password protection (see [here](https://www.ema-eda.com/sites/ema/files/resources/files/Cadence%20OrCAD_%2017.2%20QIR1.pdf#page=16))

- OrCAD 17.2 QIR #4 (see [here](https://www.cb-distribution.nl/wp-content/uploads/2017/09/Hotfix_SPB17.20.026_README-Release_Notes.pdf))

- OrCAD 17.4 (see [here](http://www.brotech.com.cn/Public/ueditor/php/upload/20191027/15721506603742.pdf))

### OrCAD SDT File Format

What exactly is this format? OrCAD can export libraries (and probably schematics) to an SDT file format that was already reverse engineered.

[https://pcengines.ch/orcad.htm](https://pcengines.ch/orcad.htm)

[https://pcengines.ch/orcadff.htm](https://pcengines.ch/orcadff.htm)

The format seems really old, but maybe the current file format is somehow based on the old one?

[OrCAD SDT III Manual](https://archive.org/details/OrCAD3Manual/page/n141/mode/2up)

---

# Current State &mdash; October 2021

- Working on parsing library symbols.

---

# Build

```bash
# Get build dependency
mkdir thirdparty
cd thirdparty
git clone https://github.com/microsoft/compoundfilereader.git
cd ..

# Build
mkdir build
cd build
cmake ..
make
cd ..
```

---

# Usage

```bash
./OpenOrCadParser --help
Allowed options:
  -h [ --help ]         produce help message
  -t [ --print_tree ]   print container tree
  -e [ --extract ]      extract files from within the binary
  -i [ --input ] arg    input file to parse
  -o [ --output ] arg   output path (required iff extract is set)

./OpenOrCadParser --input file.DSN --extract --output out/
./OpenOrCadParser --input file.DSN --print_tree
```

## Dependencies

- Compiler with C++17 Support
- CMake 2.8.12
- Boost - Program Options
- [compoundfilereader](https://github.com/Microsoft/compoundfilereader)

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

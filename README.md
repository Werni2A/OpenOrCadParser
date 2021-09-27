
# OpenOrCadParser

Purpose of this project is to provide an C++ library for parsing [Cadence](https://en.wikipedia.org/wiki/Cadence_Design_Systems)'s OrCAD Capture binary file formats. This library can be linked to other software for reading/modifying?/writing? OrCAD binary files.

The focus lies on schematics (`*.DSN`) and symbol libraries (`*.OLB`/`*.OBK`).

OrCAD allows exporting its designs to XML files which are easy to read and verify the interpretation of bytes in the `DSN`/`OLB`-files. E.g. enums are stored as a numeric value which should be the same that is used in the binary formats. However, it seems like not all information is stored in the XML.

The following two XSD files provide a good overview of the `XML` file structure, types and much more. They can be used as a reference what features are actually stored in the binary file and which we do already parse and which not.

`C:\Cadence\SPB_17.4\tools\capture\tclscripts\capDB\dsn.xsd`

`C:\Cadence\SPB_17.4\tools\capture\tclscripts\capDB\olb.xsd`

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

# Designs

## Distributed with OrCAD/Allegro

Some files are provided in:

`C:\Cadence\SPB_17.4\share\orcad\examples\pcbdesign`

`C:\Cadence\SPB_17.4\share\pcb\pcb_lib\symbols`

## Online Cadence Designs

### OrCAD 16.04 Schematics

[multio](https://github.com/fenlogic/multio)

## OrCAD and Allegro

[TRS-80 Power Supply](https://github.com/tbriggs6/trs80_powersupply)

[CutiePi](https://github.com/cutiepi-io/cutiepi-board)

## OrCAD Libraries

[OrCAD Lib](https://github.com/BerZerKku/ORCAD_lib)

[OrCAD Lib](https://github.com/18959263172/orcad_lib)

[ORCAD Library](https://github.com/looke/ORCAD_Lib)

[Old Allegro/OrCad Library](https://github.com/jmerdich/allegro-library)

[Raspberry Pi Compute Module](https://www.raspberrypi.org/documentation/hardware/computemodule/designfiles.md)

[Arrow Reference Designs](https://www.orcad.com/about/news/arrow-reference-designs)

[Open Source Raspberry Pi Design Files for Allegro and OrCAD](https://community.cadence.com/cadence_blogs_8/b/can/posts/open-source-raspberry-pi-design-files-for-allegro-and-orcad)

[CP_SmartGarden](https://github.com/sangphanthanh/CP_SmartGarden)

[BeagleBoard Cadence Libraries](https://github.com/beagleboard/beagle-cadence-libraries)

[BeagleBone](https://github.com/beagleboard/BeagleBone)

[BeagleBone Black](https://github.com/beagleboard/beaglebone-black)

[BeagleBoard](https://github.com/beagleboard/BeagleBoard)

[BeagleBoard xM](https://github.com/beagleboard/beagleboard-xm)

[BeagleBoard x15](https://github.com/beagleboard/beagleboard-x15)

[Kinoma Create PCB](https://github.com/Kinoma/kinoma-create-pcb)

[Kinoma Element](https://github.com/Kinoma/Kinoma-Element-Open-source-hardware-1st-Generation)

# Related Projects

[orlib2ki - OrCAD Library to KiCad Converter](https://github.com/fjullien/orlib2ki)

[pyorlib2ki - OrCAD Library to KiCad Converter](https://github.com/fjullien/pyorlib2ki)

[rnif2ki - OrCAD to KiCad Netlist Converter](https://github.com/fjullien/rnif2ki)

[EDIF to KiCad Converter by svn2github](https://github.com/svn2github/edif2kicad)

[EDIF to KiCad Converter by THLN47](https://github.com/THLN47/Edif_to_Kicad)

[OrCAD2KiCADtranslator](https://github.com/ehrenberdg/OrCAD2KiCADtranslator)

[Orcad2Kicad](https://github.com/Michifumi/Orcad2Kicad)

# OrCAD Terms and Conditions

[Terms and Conditions](https://www.cadence.com/content/dam/cadence-www/global/en_US/documents/terms-and-conditions/cadence-orcad.pdf)
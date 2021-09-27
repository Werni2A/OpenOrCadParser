
# OpenOrCadParser

Purpose of this project is to provide an C++ library for parsing [Cadence](https://en.wikipedia.org/wiki/Cadence_Design_Systems)'s OrCAD Capture binary file formats. This library can be linked to other software for reading/modifying?/writing? OrCAD binary files.

The focus lies on schematics (`*.DSN`) and symbol libraries (`*.OLB`/`*.OBK`).

OrCAD allows exporting its designs to XML files which are easy to read and verify the interpretation of bytes in the `DSN`/`OLB`-files. E.g. enums are stored as a numeric value which should be the same that is used in the binary formats. However, it seems like not all information is stored in the XML.

The following two XSD files provide a good overview of the `XML` file structure, types and much more. They can be used as a reference what features are actually stored in the binary file and which we do already parse and which not.

`C:\Cadence\SPB_17.4\tools\capture\tclscripts\capDB\dsn.xsd`
`C:\Cadence\SPB_17.4\tools\capture\tclscripts\capDB\olb.xsd`

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
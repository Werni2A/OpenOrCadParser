# File Container

OrCAD's file formats `DSN` and `OLB` use [Microsoft's Compound File Binary File Format (CFBF)]((https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-cfb/53989ce4-7b05-4f8d-829b-d08d6148375b)) or the older version [OLE1.0 and OLE2.0 Formats](https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-oleds/fdc5e702-d09e-4344-a77f-eb079d41f23f), see also [Wikipedia](https://en.wikipedia.org/wiki/Compound_File_Binary_Format). According to the `file` command on Linux, version 2 is used. This format can be read via [CompoundFileReader](https://github.com/microsoft/compoundfilereader).

[OpenMCDF](http://openmcdf.sourceforge.net/)'s Structured Storage eXplorer allows a fast investigation of the file structure (Windows only).

Typically CFBF container store a custom CLSID in the root storage object to identify the applicable application e.g. OrCAD or Microsoft Office. However, OrCAD provides this CLSID in a generic application independent form as `{00000000-0000-0000-0000-000000000000}`. See ['Root storage object CLSIDs'](http://fileformats.archiveteam.org/wiki/Microsoft_Compound_File#Root_storage_object_CLSIDs) for technical details. As a result it's not easy to identify an unknown CFBF container as an OrCAD design, e.g. via the Linux `file` application.

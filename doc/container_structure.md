# Container Structure

The file structure for DSN and OLB files is almost similar, DSN files only include a few more streams. The following list gives an overview of which streams are contained and where they are placed.

In front of each entry is a specifier placed that shows of which type the entry is. [D] represents a directory and [S] a stream (file). Directories are also bold. Cursive texts represent names that are provided by the designer. E.g. *Package Name 1* might become `NE555`.

- [S] `AdminData` (DSN only)
- [S] `Cache`
- [D] **`Cells`**
- [S] `Cells Directory`
- [D] **`CIS`** (since version ...?)
  - [D] **`CISSchematicStore`**
    - [S] `CISSchematicStream`
  - [D] **`VariantStore`**
    - [D] **`BOM`**
      - [S] `BOMDataStream`
    - [D] **`Groups`**
      - [S] `GroupsDataStream`
    - [S] `VariantNames`
- [S] `DsnStream` (DSN only)
- [D] **`ExportBlocks`**
- [S] `ExportBlocks Directory`
- [D] **`Graphics`**
  - [S] `$Types$`
- [S] `Graphics Directory`
- [S] `HSObjects` (DSN only)
- [D] **`LayoutReuse`** (since version...?)
  - [S] `ReuseSchematics`
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

## `NetBundleMapData`

The `NetBundleMapData` stream was introduced in later file format versions and contains the association of wires and busses to a net group. This structure is identified with `0x67`.

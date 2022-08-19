# File Format Changes

Changelogs and some history is provided in

- [OrCAD Capture What's New](https://www.orcad.com/products/orcad-capture/new).

- [OrCAD Version History](https://www.ema-eda.com/products/cadence-orcad/orcad-version-history)

Typically, over time a file format changes due to new features introduced and old ones or mistakes being removed. This section should provide some insight what changes were introduced in which version.

- **OrCAD 16.2** (see [here](https://www.cetti.ro/v2/noutati/OrCAD_16.2_WhatIsNew.pdf))
  - Line styles can be further adjusted (see [here](https://www.cetti.ro/v2/noutati/OrCAD_16.2_WhatIsNew.pdf#page=12))
  - Junction dot size can be adjusted (see [here](https://www.cetti.ro/v2/noutati/OrCAD_16.2_WhatIsNew.pdf#page=14))
  - Introduced elliptical arcs (see [here](https://www.cetti.ro/v2/noutati/OrCAD_16.2_WhatIsNew.pdf#page=17))

- **OrCAD 16.5 QIR #5** (see [here](https://dev.orcad.com/sites/orcad/files/resources/files/Cadence-OrCAD-166-QIR5-Whats-New.pdf))

- **OrCAD 16.6** (see [here](https://www.flowcad.de/datasheets/Whats_New_in_OrCAD_Capture16-6_FlowCAD.pdf))
  - NetGroup enhancements (see [here](https://www.flowcad.de/datasheets/Whats_New_in_OrCAD_Capture16-6_FlowCAD.pdf#page=12))
  - Shapes for hierarchic blocks (see [here](https://www.flowcad.de/datasheets/Whats_New_in_OrCAD_Capture16-6_FlowCAD.pdf#page=13))
  - Introduced flag to indicate user specified references (see [here](https://www.flowcad.de/datasheets/Whats_New_in_OrCAD_Capture16-6_FlowCAD.pdf#page=15))
  - Introduce XML import/export and XSD schema (see [here](https://www.flowcad.de/datasheets/Whats_New_in_OrCAD_Capture16-6_FlowCAD.pdf#page=17))
  - Introduce user specific DRCs (see [here](https://www.flowcad.de/datasheets/Whats_New_in_OrCAD_Capture16-6_FlowCAD.pdf#page=18))
  - Introduce flag for waived DRCs (see [here](https://www.flowcad.de/datasheets/Whats_New_in_OrCAD_Capture16-6_FlowCAD.pdf#page=19))
  - Database upgrade (see [here](https://www.flowcad.de/datasheets/Whats_New_in_OrCAD_Capture16-6_FlowCAD.pdf#page=21))

- **OrCAD 16.6 QIR #5** (see [here](https://www.orcad.com/sites/orcad/files/resources/files/Cadence-OrCAD-166-QIR5-Whats-New.pdf))

- **OrCAD 16.6 QIR #7** HotFix 32 (see [here](https://www.orcad.com/sites/orcad/files/resources/files/OrCAD_16.6QIR7_WhatsNew.pdf) or [here](https://www.artedas.fr/documentsPDF/HotFix/Hotfix_SPB16.60.032_readme_product_note.pdf))

- **OrCAD 16.6 QIR #8** HotFix 38 (see [here](https://www.artedas.it/attach/repository/document/201411/Hotfix_SPB16.60.038_readme_product_note.pdf))

- **OrCAD 16.6** HotFix 51 (see [here](https://www.artedas.it/attach/repository/document/201506/Release16.6-2015_readme_product_note.pdf))

- **OrCAD 17.0** (see [here](http://www.linelayout.com/bbs/bbs/uploadfile/image/2015041515460682682.pdf))
  - Allow negative x offset in intersheet references window (see [here](http://www.linelayout.com/bbs/bbs/uploadfile/image/2015041515460682682.pdf#page=74))

- **OrCAD 17.2 QIR #1** (see [here](https://www.ema-eda.com/sites/ema/files/resources/files/Cadence%20OrCAD_%2017.2%20QIR1.pdf))
  - Introduced design password protection (see [here](https://www.ema-eda.com/sites/ema/files/resources/files/Cadence%20OrCAD_%2017.2%20QIR1.pdf#page=16))

- **OrCAD 17.2 QIR #4** HotFix 025 (see [here](https://www.cb-distribution.nl/wp-content/uploads/2017/09/Hotfix_SPB17.20.026_README-Release_Notes.pdf))

- **OrCAD 17.4** (see [here](http://www.brotech.com.cn/Public/ueditor/php/upload/20191027/15721506603742.pdf))

- **OrCAD 17.4 QIR #3** HotFix 019 (see [here](https://www.artedas.it/wp-content/uploads/2021/11/WhatsNew_Hotfix_SPB17.40.019_Release_Notes.pdf))

- **OrCAD 17.4 QIR #4** HotFix 028 (see [here](https://www.flowcad.de/datasheets/cadence-Hotfix-SPB17.40.028-QIR4.pdf))

## OrCAD SDT File Format

What exactly is this format? OrCAD can export libraries (and probably schematics) to an SDT file format that was already reverse engineered.

[https://pcengines.ch/orcad.htm](https://pcengines.ch/orcad.htm)

[https://pcengines.ch/orcadff.htm](https://pcengines.ch/orcadff.htm)

The format seems really old, but maybe the current file format is somehow based on the old one?

[OrCAD SDT III Manual](https://archive.org/details/OrCAD3Manual/page/n141/mode/2up)
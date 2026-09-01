# Third-Party Notices & Dependency Provenance

This document lists the third-party software libraries, fonts, and assets used by **VectorPDF**, along with their respective licenses and copyright notices, in compliance with open-source licensing obligations.

---

## Summary of Third-Party Dependencies

| Component | Version / Commit | License (SPDX) | Upstream URL | Usage in VectorPDF |
|---|---|---|---|---|
| **PDF4QT Core** | v1.6.0.0 (MIT) | `MIT` | https://github.com/JakubMelka/PDF4QT | Core PDF parser, rendering, and manipulation baseline |
| **Blend2D** | v0.10.x | `Zlib` | https://blend2d.com | High-performance 2D vector graphics rendering engine |
| **FreeType** | 2.13.x | `FTL` / `GPL-2.0` (FTL chosen) | https://freetype.org | Font rendering, glyph rasterization, and OpenType/TrueType parsing |
| **Little CMS (lcms2)** | 2.16 | `MIT` | https://littlecms.com | ICC color profile management and CMS color conversions |
| **OpenJPEG** | 2.5.x | `BSD-2-Clause` | https://www.openjpeg.org | JPEG 2000 image stream compression and decompression |
| **libjpeg-turbo** | 3.0.x | `BSD-3-Clause` / `IJG` | https://libjpeg-turbo.org | Fast JPEG image encoding and decoding |
| **libpng** | 1.6.x | `libpng-2.0` | http://www.libpng.org | PNG image rasterization and alpha transparency support |
| **zlib** | 1.3.x | `Zlib` | https://zlib.net | FlateDecode PDF stream compression and decompression |
| **OpenSSL** | 3.x LTS | `Apache-2.0` | https://www.openssl.org | Cryptographic hash, AES-256 decryption, PAdES/CMS signature verification |
| **Tesseract OCR** | 5.3.x | `Apache-2.0` | https://github.com/tesseract-ocr/tesseract | Offline Optical Character Recognition engine |
| **Leptonica** | 1.83.x | `BSD-2-Clause` | http://www.leptonica.org | Image processing, deskew, binarization, and scaling for OCR |
| **Liberation Fonts** | 2.1.5 | `OFL-1.1` | https://github.com/liberationfonts | Standard 14 PDF font fallback substitution |
| **Qt 6 Framework** | 6.5+ LTS | `LGPL-3.0` / `GPL-3.0` | https://qt.io | Desktop UI widgets, event loop, and cross-platform runtime |

---

## Detailed License Texts

### 1. PDF4QT Baseline
```text
MIT License
Copyright (c) 2018-2025 Jakub Melka and Contributors

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
```

### 2. Blend2D
```text
Zlib License
Copyright (c) 2017-2024 Petr Kobalicek

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.
```

### 3. Little CMS (lcms2)
```text
MIT License
Copyright (c) 1998-2024 Marti Maria Saguer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software.
```

### 4. OpenJPEG
```text
BSD 2-Clause License
Copyright (c) 2002-2024, Communications and Remote Sensing Laboratory, Universite catholique de Louvain (UCLouvain), Belgium
Copyright (c) 2002-2024, Professor Benoit Macq
Copyright (c) 2003-2024, Antonin Descampe
Copyright (c) 2003-2024, Francois-Olivier Devaux
Copyright (c) 2005, Herve Drolon, FreeImage Team
All rights reserved.
```

### 5. OpenSSL
```text
Apache License, Version 2.0
Copyright (c) 1998-2024 The OpenSSL Project. All rights reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
```

### 6. Liberation Fonts
```text
SIL OPEN FONT LICENSE Version 1.1 - 26 February 2007
Copyright (c) 2012 Red Hat, Inc. with Reserved Font Name Liberation.
```

# ⚖️ WoMA3Dengine – Third-Party Licenses

This document lists all external components, libraries, and third-party tools integrated into the **WoMA3Dengine** project.  
Each library retains its original license, and all required attributions are preserved.

---

## 🧩 Core System (Multi-OS Foundations)
<details>
<summary>Click to expand</summary>

**UltimateTCPIP_LIBX64_d.lib**  
- **Purpose:** Networking and socket communication  
- **License:** Commercial / Custom (UltimateTCP/IP SDK)  
- **Used in:** Levels 03+

**TinyXMLv2_LIBX64_d.lib**  
- **Purpose:** XML parsing and settings loader  
- **License:** [MIT License](https://github.com/leethomason/tinyxml2/blob/master/LICENSE.txt)  
- **Source:** https://github.com/leethomason/tinyxml2  
- **Used in:** Level 05+

**ZipUtils_LIBX64_d.lib**  
- **Purpose:** ZIP compression/decompression utilities  
- **License:** zlib / BSD-like License  
- **Used in:** Level 06+

**GeoLite2PP_LIBX64_d.lib**, **maxminddb_LIBX64_d.lib**  
- **Purpose:** IP-based geolocation database and access  
- **License:** [Apache 2.0](https://www.apache.org/licenses/LICENSE-2.0) / [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/)  
- **Source:** https://github.com/DinoTools/GeoLite2PP  
- **Used in:** Level 08+

</details>

---

## 🖼️ Image & Texture System
<details>
<summary>Click to expand</summary>

**TIFF_LIBX64_d.lib**, **ZLIB_LIBX64_d.lib**, **PNG_LIBX64_d.lib**, **JPG_LIBX64_d.lib**  
- **Purpose:** Image decoding and encoding  
- **License:** zlib/libpng License (TIFF under BSD-like)  
- **Used in:** Level 22+

**DirectXTex (DX11)**  
- **Purpose:** Texture import (TGA) and manipulation utilities  
- **License:** [MIT License](https://github.com/microsoft/DirectXTex/blob/main/LICENSE)  
- **Source:** https://github.com/microsoft/DirectXTex  
- **Used in:** Level 22+

**DirectXTK (DX11)**  
- **Purpose:** Texture handling (DDS, BMP, JPG, PNG, TIF) and WIC texture saving  
- **License:** [MIT License](https://github.com/microsoft/DirectXTK/blob/main/LICENSE)  
- **Source:** https://github.com/microsoft/DirectXTK  
- **Used in:** Level 22+

</details>

---

## 🔊 Audio System
<details>
<summary>Click to expand</summary>

**OpenAL32_LIBX64_d.lib**, **ALUT_LIBX64_d.lib**, **OGG_LIBX64_d.lib**, **VORBIS_LIBX64_d.lib**, **VORBISFILE_LIBX64_d.lib**  
- **Purpose:** 3D sound and OGG music playback  
- **License:** BSD / LGPL (depending on component)  
- **Sources:** [OpenAL Soft](https://openal-soft.org), [Xiph.org](https://xiph.org)  
- **Used in:** Level 29+

</details>

---

## 🪟 OpenGL (Cross-Platform Support)
<details>
<summary>Click to expand</summary>

**freeglut**  
- **Purpose:** OpenGL context management and input handling  
- **License:** [MIT/X11 License](https://github.com/FreeGLUTProject/freeglut/blob/master/COPYING)  
- **Source:** https://github.com/FreeGLUTProject/freeglut  

**GL3Plus**  
- **Purpose:** Cross-platform OpenGL support library  
- **License:** MIT/X11 License  

</details>

---

## 💡 Rendering / PBR Engine
<details>
<summary>Click to expand</summary>

**PPG-master**  
- **Purpose:** Physically-Based Rendering (DX11)  
- **Original Author:** [Nicholas Chu](https://github.com/nicholaschuayunzhi/PPG)  
- **License:** MIT License  
- **Modified By:** Pedro Borges (pmborg)  
- **Used in:** Level 82+  
- **Notes:** Integrated and extended for WoMA3Dengine.

</details>

---

## 🧱 Model Import / Asset Pipeline
<details>
<summary>Click to expand</summary>

**Assimp**  
- **Purpose:** Asset Import Library (OBJ, FBX, glTF, COLLADA, etc.)  
- **License:** [BSD 3-Clause License](https://github.com/assimp/assimp/blob/master/LICENSE)  
- **Source:** https://github.com/assimp/assimp  
- **Used in:** Level 82+ (PPG-master integration)  
- **Notes:** Handles skeletal animation, materials, and multi-format I/O.

</details>

---

## 🌐 Network & Online Services
<details>
<summary>Click to expand</summary>

**libcurl**  
- **Purpose:** HTTP/HTTPS file download support (used for retrieving data and music files from Dropbox or online sources)  
- **License:** [MIT/X11 License](https://curl.se/docs/copyright.html)  
- **Source:** https://curl.se/libcurl/  
- **Used in:** CoreEngine_000_OSENGINE_SYSTEM (OSengine.cpp)  
- **Notes:** Linked into runtime; provides lightweight network access for data streaming.

</details>

---

## ⚙️ Utility and Diagnostic Libraries
<details>
<summary>Click to expand</summary>

**rastertek_text_fonts**  
- **Purpose:** Text font rendering (Rastertek tutorial-derived)  
- **License:** Free educational use / tutorial source  
- **Used in:** Levels 25–27  
- **Source:** [Rastertek DX11 Tutorial 14](https://rastertek.com/dx11win10tut14.html)  
- **Notes:** Only small parts of the original code were used.

**MiniDumper (minidumperClass.cpp)**  
- **Purpose:** Generates `.dmp` crash dump files for post-mortem debugging.  
- **Current Status:** Fully integrated WoMA code.  
- **Original Concept:** Based on the CodeProject article *“Post-Mortem Debugging Your Application with Minidump”* by Andy Pennell (2002).  
- **Notes:** Unicode and FTP upload functionality implemented by Pedro Borges.

**StackTrace (stackTrace.cpp)**  
- **Purpose:** Runtime call stack resolution for debugging.  
- **Current Status:** Fully rewritten and integrated into WoMA logging system.  
- **Original Concept:** Inspired by Microsoft Developer Network example code for generating stack traces in C/C++.

**IDEA Encryption (idea.cpp)**  
- **Purpose:** Implements the IDEA block cipher algorithm used for internal data encryption.  
- **Current Status:** Maintained and customized version within WoMA3Dengine.  
- **Original Algorithm:** Xuejia Lai and James L. Massey (ETH Zürich), 1990; Implementation inspired by Colin Plumb’s 1992 public-domain code.  
- **Notes:** Optimized and refactored for safety and modern standards by Pedro Borges.

</details>

---

## 🧭 Platform-Specific Dependencies
```
Windows:  
  DirectXTex, DirectXTK, TinyXMLv2, ZipUtils, Assimp, PPG-master,  
  OpenAL, Vorbis, libcurl, UltimateTCP/IP

Linux:  
  FreeGLUT, GL3Plus, TinyXMLv2, ZipUtils, OpenAL, Vorbis,  
  GeoLite2PP, maxminddb

Android:  
  TinyXMLv2, ZipUtils, OpenAL, Vorbis, rastertek_text_fonts
```

---

## 📜 License Overview
```
MIT License:  
    TinyXMLv2, DirectXTex, DirectXTK, PPG-master, libcurl,  
    freeglut, GL3Plus

BSD / BSD-like:  
    Assimp, OpenAL, ALUT, Vorbis, TIFF, ZipUtils

Apache 2.0 / CC BY-SA:  
    GeoLite2PP, maxminddb

Commercial / Proprietary:  
    UltimateTCP/IP SDK

Educational / Tutorial:  
    rastertek_text_fonts
```

---

## 🪶 Copyright & Attributions
```
TinyXMLv2 – © Lee Thomason  
Assimp – © 2006–2025 The Open Asset Import Library (Assimp) Contributors  
DirectXTex / DirectXTK – © Microsoft Corporation  
PPG-master – © Nicholas Chu, Modified by Pedro Borges (pmborg)  
libcurl – © Daniel Stenberg, <daniel@haxx.se>  
OpenAL / ALUT – © Creative Labs Inc. and contributors  
Vorbis / Ogg / Vorbisfile – © Xiph.Org Foundation  
GeoLite2PP – © DinoTools, Licensed under Apache 2.0  
maxminddb – © MaxMind, Licensed under CC BY-SA 4.0  
UltimateTCP/IP – © The Ultimate TCP/IP SDK (Commercial)  
rastertek_text_fonts – Derived from Rastertek DirectX11 Tutorials, © Rastertek.com  
minidumperClass.cpp – Concept © Andy Pennell (CodeProject, 2002)  
stackTrace.cpp – Concept © Microsoft Developer Network Example  
idea.cpp – Algorithm © Xuejia Lai & James Massey, ETH Zürich (1990)
```

---

**End of Third-Party License Summary**


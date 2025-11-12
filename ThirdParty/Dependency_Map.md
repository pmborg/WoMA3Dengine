# 🧭 WoMA3Dengine – Dependency Map

This document provides an overview of all third-party and internal dependencies integrated into the **WoMA3Dengine**, grouped by system and showing where each dependency is used.

Each entry includes:  
- **Library or File**  
- **Used In (Engine Levels / Modules)**  
- **Purpose**  
- **Dependencies (if any)**

---

## ⚙️ Core System Dependencies
<details>
<summary>Click to expand</summary>

| Library | Used In | Purpose | Dependencies |
|----------|----------|----------|---------------|
| **UltimateTCPIP_LIBX64_d.lib** | Levels 03+ | Cross-platform networking and socket communication | — |
| **TinyXMLv2_LIBX64_d.lib** | Levels 05+ | XML parsing (settings.xml) and configuration loader | — |
| **ZipUtils_LIBX64_d.lib** | Levels 06+ | ZIP compression/decompression utilities | ZLIB |
| **GeoLite2PP_LIBX64_d.lib** | Level 08+ | IP-based geolocation interface | maxminddb |
| **maxminddb_LIBX64_d.lib** | Level 08+ | Low-level MaxMind database reader | — |

</details>

---

## 🖼️ Image & Texture System
<details>
<summary>Click to expand</summary>

| Library | Used In | Purpose | Dependencies |
|----------|----------|----------|---------------|
| **TIFF_LIBX64_d.lib** | Level 22+ | TIFF image decoding | ZLIB |
| **ZLIB_LIBX64_d.lib** | Level 22+ | Compression backend (PNG, ZIP, TIFF) | — |
| **PNG_LIBX64_d.lib** | Level 22+ | PNG image decoding | ZLIB |
| **JPG_LIBX64_d.lib** | Level 22+ | JPEG image decoding | ZLIB |
| **DirectXTex** | Level 22+ | Texture import (TGA) and conversion utilities | DirectX 11 SDK |
| **DirectXTK** | Level 22+ | Texture handling (DDS, BMP, PNG, JPG, TIF) and WIC export | DirectX 11 SDK |

</details>

---

## 🔊 Audio System
<details>
<summary>Click to expand</summary>

| Library | Used In | Purpose | Dependencies |
|----------|----------|----------|---------------|
| **OpenAL32_LIBX64_d.lib** | Level 29+ | 3D positional sound | — |
| **ALUT_LIBX64_d.lib** | Level 29+ | OpenAL utility functions | OpenAL |
| **OGG_LIBX64_d.lib** | Level 29+ | OGG file decoding | — |
| **VORBIS_LIBX64_d.lib** | Level 29+ | Vorbis audio codec | OGG |
| **VORBISFILE_LIBX64_d.lib** | Level 29+ | File I/O for Vorbis streams | VORBIS |

</details>

---

## 🪟 OpenGL & Cross-Platform Rendering
<details>
<summary>Click to expand</summary>

| Library | Used In | Purpose | Dependencies |
|----------|----------|----------|---------------|
| **freeglut** | Linux builds | Window/context management + input | OpenGL |
| **GL3Plus** | Linux builds | OpenGL rendering layer abstraction | OpenGL |

</details>

---

## 💡 Rendering / PBR System
<details>
<summary>Click to expand</summary>

| Library | Used In | Purpose | Dependencies |
|----------|----------|----------|---------------|
| **PPG-master** | Level 82+ | Physically-Based Rendering Engine (DX11) | Assimp, DirectXTK |
| **Assimp** | Level 82+ | Asset import (OBJ, FBX, glTF, COLLADA, MD5, etc.) | ZLIB, DirectXTex |
| **rastertek_text_fonts** | Levels 25–27 | Font rendering (based on Rastertek tutorials) | DirectXTK |

</details>

---

## 🌐 Networking / Online Features
<details>
<summary>Click to expand</summary>

| Library | Used In | Purpose | Dependencies |
|----------|----------|----------|---------------|
| **libcurl** | CoreEngine_000_OSENGINE_SYSTEM | HTTP/HTTPS file download (Dropbox data & music) | OpenSSL (static) |

</details>

---

## 🧠 Utility Libraries (Internal Tools)
<details>
<summary>Click to expand</summary>

| File | Used In | Purpose | Origin / Attribution |
|------|----------|----------|----------------------|
| **minidumperClass.cpp** | CoreEngine crash handler | Generate `.dmp` crash dump files | Concept by Andy Pennell (CodeProject, 2002) |
| **stackTrace.cpp** | CoreEngine debug layer | Runtime stack trace capture | Inspired by MSDN example |
| **idea.cpp** | Tools / Encryption utilities | IDEA cipher implementation | Algorithm by Lai & Massey (ETH Zürich, 1990) |

</details>

---

## 🧭 Platform-Specific Dependencies
<details>
<summary>Click to expand</summary>

### 🪟 Windows
- DirectXTex  
- DirectXTK  
- TinyXMLv2  
- ZipUtils  
- Assimp  
- PPG-master  
- OpenAL  
- Vorbis  
- libcurl  
- UltimateTCP/IP  

### 🐧 Linux
- freeglut  
- GL3Plus  
- TinyXMLv2  
- ZipUtils  
- OpenAL  
- Vorbis  
- GeoLite2PP  
- maxminddb  

### 🤖 Android
- TinyXMLv2  
- ZipUtils  
- OpenAL  
- Vorbis  
- rastertek_text_fonts  

</details>

---

## 🔗 Dependency Tree Overview
```text
CoreEngine
 ├── TinyXMLv2
 ├── ZipUtils ──┬── ZLIB
 │              └── PNG/JPG/TIFF
 ├── GeoLite2PP ──> maxminddb
 ├── DirectXTK ──> DirectXTex
 ├── PPG-master ──┬── Assimp
 │                ├── DirectXTK
 │                └── WoMA scene manager
 ├── AudioSystem ─┬── OpenAL ─── ALUT
 │                └── Vorbis ─── OGG ─── VORBISFILE
 ├── Network ────> libcurl
 └── Utilities ──> minidumperClass.cpp, stackTrace.cpp, idea.cpp
```

---

## 📘 Notes
- Libraries marked as “MIT” or “BSD” are open for commercial redistribution, provided attributions are included.  
- Educational/tutorial sources (like Rastertek) are used **only as learning material fragments**.  
- Commercial SDKs (like UltimateTCP/IP) must be acquired separately for legal use in distributed binaries.

---

**End of Dependency Map**


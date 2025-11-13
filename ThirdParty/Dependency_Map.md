# 🧩 Engine Dependency Map

This document shows how each module of the **WoMA Multi-Platform Engine** depends on internal code and external libraries.  
Useful for contributors, maintainers, and portability analysis.

---

## 📘 Legend

- **Engine Module** — Internal subsystem or tutorial level  
- **Depends On** — Internal or external component it requires  
- **Platform** — Windows / Linux / Android  
- **Notes** — Additional info  

---

## 🔧 Core System

| Module | Depends On | Platform | Notes |
|--------|-------------|----------|-------|
| **Level 01 – MessageBox** | — | All | Multi-OS dialog wrapper |
| **Level 02 – WomaLog** | — | All | Logging system |
| **Level 03 – Exception + Minidumper** | `minidumperClass.cpp` | All | Crash handling (no external libs) |
| **Level 04 – System Check + Timers** | — | All | CPU, memory, OS checks |
| **Level 05 – Settings Loader** | TinyXMLv2 | All | Loads `settings.xml` |
| **Level 06 – File Packaging + IDEA + ZIP** | ZipUtils, idea.cpp | All | Asset packaging/encryption |
| **Level 07 – Sun/Moon Position** | Math only | All | Astro calculations |
| **Level 08 – Geolocation** | GeoLite2PP, MaxMindDB | All | IP-based geolocation |
| **Level 09 – Weather** | libcurl | All | HTTP requests |

---

## 🎨 Rendering System

| Module | Depends On | Platform | Notes |
|--------|-------------|----------|-------|
| **Level 19 – Renderer Window** | — | All | OS-specific |
| **Level 20 – Window Resizing** | — | All | — |
| **Level 21 – Color Shader + 3D Camera** | — | All | — |
| **Level 22 – Texture Shader + Image Loading** | TIFF, PNG, JPG, ZLIB, DirectXTex, DirectXTK | Win/Linux | Loads all major formats |
| **Level 23 – Light Shader** | — | All | Basic lighting |
| **Level 24 – Sprites + Rasterizer** | — | All | 2D support |
| **Level 25 – DX Native Fonts** | DirectXTK | Windows | — |
| **Level 27 – Rastertek Text Fonts** | Rastertek text fonts | All | Customized font renderer |
| **Level 28 – Direct Input** | — | Windows | Keyboard/mouse |

---

## 🔊 Audio System

| Module | Depends On | Platform | Notes |
|--------|-------------|----------|-------|
| **Level 29 – 3D Sound & Music** | OpenAL, ALUT, Vorbis, VorbisFile | All | WAV/OGG audio playback |

---

## 🗺 Scene Management

| Level | Depends On | Notes |
|-------|--------------|-------|
| 30–36 | Wavefront OBJ loader | Color, UVs, light, specular, bump, shadows |
| 37 | Threaded Loader | Parallel model loading |
| 38 | W3D Export Tool | Fast binary format |
| 39 | W3D Importer | 20× faster loading |
| 40–42 | Instances + Shadows | Efficient rendering |

---

## 🏞 Terrain System

| Level | Depends On | Notes |
|-------|--------------|-------|
| 49–55 | Heightmap terrain | Underwater, fog, normals, collision |
| 60–61 | Advanced multilayer | 256×256 → 512×512 |
| 62–63 | Main Map / Mini-Map | — |
| 64–65 | Objects + Collision | Animated windmill |
| 70–75 | Billboards, fire, grass, progress bar | — |
| 76 | MPG Video Intro | Uses native video loader |

---

## 🌲 Instances

| Level | Depends On | Notes |
|-------|--------------|-------|
| 77 | Tree Instances | High-performance forest rendering |
| 78 | Collision with XML Objects | Physics + navigation |

---

## 🦴 Advanced Mesh System (PBR)

| Level | Depends On | Notes |
|-------|--------------|-------|
| **82 – MD5 Mesh Loading** | Assimp, PPG-master, DirectXTK | PBR renderer + animation |

---

## 🔗 Summary of External Libraries

TinyXMLv2
Assimp
DirectXTex
DirectXTK
PPG-master (PPG modified fork)
libcurl
OpenAL / ALUT
Vorbis / OvFile
GeoLite2PP
MaxMindDB
Rastertek Text Fonts
ZLib / PNG / JPG / TIFF loaders
ZipUtils


---

<sub>All dependencies listed above follow their respective open-source licenses.</sub>

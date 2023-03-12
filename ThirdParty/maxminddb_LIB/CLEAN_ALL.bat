@echo off
del /S /F /Q Release 2> nul
rmdir /S /Q Release 2> nul

del /S /F /Q dbgRel 2> nul
rmdir /S /Q dbgRel 2> nul

del /S /F /Q Debug 2> nul
rmdir /S /Q Debug 2> nul

del /S /F /Q x64 2> nul
rmdir /S /Q x64 2> nul
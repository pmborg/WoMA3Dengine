@echo off
del /S /F /Q REPORT.txt 2> nul

del /S /F /Q .vs 2> nul
rmdir /S /Q .vs 2> nul

del /S /F /Q Debug 2> nul
rmdir /S /Q Debug 2> nul

del /S /F /Q Release 2> nul
rmdir /S /Q Release 2> nul

del /S /F /Q WDebug 2> nul
rmdir /S /Q WDebug 2> nul

del /S /F /Q WRelease 2> nul
rmdir /S /Q WRelease 2> nul

del /S /F /Q x64 2> nul
rmdir /S /Q x64 2> nul

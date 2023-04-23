@echo off

for /f "usebackq tokens=1* delims=: " %%i in (`"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -latest`) do (
  if /i "%%i"=="productPath" set devenv=%%j
)
echo "Using Visual Studio: %devenv%"
del /S /F /Q ENGINE_*.txt 2> nul
del /S /F /Q DXENGINE_*.txt 2> nul

REM -------------------------
echo "Compiling: Debug|x64"
echo on

"%devenv%" CoreEngine_000_OSENGINE_SYSTEM\CoreEngine_000.sln /project CoreEngine_000 /Build "Debug|x64" /out ENGINE_000_d.txt
"%devenv%" CoreEngine_001_LANG_LOG_MEMLEAKs_OSMAINDIRs\CoreEngine_001.sln /project CoreEngine_001 /Build "Debug|x64" /out ENGINE_001_d.txt
"%devenv%" CoreEngine_002_MAINWINDOW_EVENTS\CoreEngine_002.sln /project CoreEngine_002 /Build "Debug|x64" /out ENGINE_002_d.txt
"%devenv%" CoreEngine_003_MINIDUMPER_STACK_EXCEPTION\CoreEngine_003.sln /project CoreEngine_003 /Build "Debug|x64" /out ENGINE_003_d.txt
"%devenv%" CoreEngine_004_SYSTEMMANAGER_TIMER_FPS_MATH\CoreEngine_004.sln /project CoreEngine_004 /Build "Debug|x64" /out ENGINE_004_d.txt
"%devenv%" CoreEngine_005_PAINT_SETTINGS_XML\CoreEngine_005.sln /project CoreEngine_005 /Build "Debug|x64" /out ENGINE_005_d.txt
"%devenv%" CoreEngine_006_PACKMANAGER_IDEA_CPU_BENCHMARK\CoreEngine_006.sln /project CoreEngine_006 /Build "Debug|x64" /out ENGINE_006_d.txt
"%devenv%" CoreEngine_007_PAINT_REALTIMECELESTIAL_ASTROCLASS\CoreEngine_007.sln /project CoreEngine_007 /Build "Debug|x64" /out ENGINE_007_d.txt
"%devenv%" CoreEngine_008_PAINT_REALTIME_GPS_MAP_LOCATION\CoreEngine_008.sln /project CoreEngine_008 /Build "Debug|x64" /out ENGINE_008_d.txt
"%devenv%" CoreEngine_009_PAINT_METAR_WEATHER_REALTIME\CoreEngine_009.sln /project CoreEngine_009 /Build "Debug|x64" /out ENGINE_009_d.txt

for /D %%f in (DXENGINE_*) do (
"%devenv%" %%f/%%f.sln /project %%f /Build "Debug|x64" /out %%f_d.txt 
)
 
@echo off
echo "Compiled:"
dir /S /B *.exe | find /v "fxc"
echo "How to Run one demo using DEBUG build ?"
echo "Execute (example):"
echo "cd C:\WoMA3Dengine\CoreEngine_Tutorials\DXENGINE_019"
echo "x64\Debug\CoreEngine_019.exe"
pause
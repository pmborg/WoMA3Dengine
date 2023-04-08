@echo off
for /f "usebackq tokens=1* delims=: " %%i in (`"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -latest`) do (
  if /i "%%i"=="productPath" set devenv=%%j
)
echo "Using Visual Studio: %devenv%"
REM -------------------------
echo "Compiling: Debug|x64"
echo on

del /S /F /Q ENGINE_*.txt 2> nul
del /S /F /Q DXENGINE_*.txt 2> nul

"%devenv%" CoreEngine_000_OSENGINE_SYSTEM\CoreEngine_000.sln /project CoreEngine_000 /Build "Debug|x64" /out ENGINE_000.txt
"%devenv%" CoreEngine_001_LANG_LOG_MEMLEAKs_OSMAINDIRs\CoreEngine_001.sln /project CoreEngine_001 /Build "Debug|x64" /out ENGINE_001.txt
"%devenv%" CoreEngine_002_MAINWINDOW_EVENTS\CoreEngine_002.sln /project CoreEngine_002 /Build "Debug|x64" /out ENGINE_002.txt
"%devenv%" CoreEngine_003_MINIDUMPER_STACK_EXCEPTION\CoreEngine_003.sln /project CoreEngine_003 /Build "Debug|x64" /out ENGINE_003.txt
"%devenv%" CoreEngine_004_SYSTEMMANAGER_TIMER_FPS_MATH\CoreEngine_004.sln /project CoreEngine_004 /Build "Debug|x64" /out ENGINE_004.txt
"%devenv%" CoreEngine_005_PAINT_SETTINGS_XML\CoreEngine_005.sln /project CoreEngine_005 /Build "Debug|x64" /out ENGINE_005.txt
"%devenv%" CoreEngine_006_PACKMANAGER_IDEA_CPU_BENCHMARK\CoreEngine_006.sln /project CoreEngine_006 /Build "Debug|x64" /out ENGINE_006.txt
"%devenv%" CoreEngine_007_PAINT_REALTIMECELESTIAL_ASTROCLASS\CoreEngine_007.sln /project CoreEngine_007 /Build "Debug|x64" /out ENGINE_007.txt
"%devenv%" CoreEngine_008_PAINT_REALTIME_GPS_MAP_LOCATION\CoreEngine_008.sln /project CoreEngine_008 /Build "Debug|x64" /out ENGINE_008.txt
"%devenv%" CoreEngine_009_PAINT_METAR_WEATHER_REALTIME\CoreEngine_009.sln /project CoreEngine_009 /Build "Debug|x64" /out ENGINE_009.txt

"%devenv%" DXENGINE_019/DXENGINE_019.sln /project DXENGINE_019 /Build "Debug|x64" /out DXENGINE_019.txt
"%devenv%" DXENGINE_020/DXENGINE_020.sln /project DXENGINE_020 /Build "Debug|x64" /out DXENGINE_020.txt
"%devenv%" DXENGINE_021/DXENGINE_021.sln /project DXENGINE_021 /Build "Debug|x64" /out DXENGINE_021.txt
"%devenv%" DXENGINE_022/DXENGINE_022.sln /project DXENGINE_022 /Build "Debug|x64" /out DXENGINE_022.txt
"%devenv%" DXENGINE_023/DXENGINE_023.sln /project DXENGINE_023 /Build "Debug|x64" /out DXENGINE_023.txt
"%devenv%" DXENGINE_024/DXENGINE_024.sln /project DXENGINE_024 /Build "Debug|x64" /out DXENGINE_024.txt
"%devenv%" DXENGINE_025/DXENGINE_025.sln /project DXENGINE_025 /Build "Debug|x64" /out DXENGINE_025.txt
"%devenv%" DXENGINE_026/DXENGINE_026.sln /project DXENGINE_026 /Build "Debug|x64" /out DXENGINE_026.txt
"%devenv%" DXENGINE_027/DXENGINE_027.sln /project DXENGINE_027 /Build "Debug|x64" /out DXENGINE_027.txt
"%devenv%" DXENGINE_028/DXENGINE_028.sln /project DXENGINE_028 /Build "Debug|x64" /out DXENGINE_028.txt
"%devenv%" DXENGINE_029/DXENGINE_029.sln /project DXENGINE_029 /Build "Debug|x64" /out DXENGINE_029.txt
@echo off
echo "Compiled:"
dir /S /B *.exe | find /v "fxc"
echo "How to Run one demo using DEBUG build ?"
echo "Execute (example):"
echo "cd C:\WoMA3Dengine\CoreEngine_Tutorials\DXENGINE_019"
echo "x64\Debug\CoreEngine_019.exe"
pause
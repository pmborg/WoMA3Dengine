REM COMPILE ALL:
@echo off

goto RUN_ALL

for /f "usebackq tokens=1* delims=: " %%i in (`"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -latest`) do (
  if /i "%%i"=="productPath" set devenv=%%j
)
echo "Using Visual Studio: %devenv%"
del /S /F /Q ENGINE_*.txt 2> nul

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

:RUN_ALL

REM RUN ALL:

REM cd C:\WoMA3Dengine\CoreEngine_Tutorials\CoreEngine_000_OSENGINE_SYSTEM
REM start C:\WoMA3Dengine\CoreEngine_Tutorials\x64\Debug\CoreEngine_000.exe /renderOnce /Xpos 000  /Ypos 000 /WIDTH 640 /HEIGHT 360
									
REM cd C:\WoMA3Dengine\CoreEngine_Tutorials\CoreEngine_001_LANG_LOG_MEMLEAKs_OSMAINDIRs									
REM start C:\WoMA3Dengine\CoreEngine_Tutorials\x64\Debug\CoreEngine_001.exe /renderOnce /Xpos 000  /Ypos 000 /WIDTH 640 /HEIGHT 360
cd C:\WoMA3Dengine\CoreEngine_Tutorials\CoreEngine_002_MAINWINDOW_EVENTS
start C:\WoMA3Dengine\CoreEngine_Tutorials\x64\Debug\CoreEngine_002.exe /renderOnce /Xpos 640  /Ypos 000 /WIDTH 640 /HEIGHT 360
REM cd C:\WoMA3Dengine\CoreEngine_Tutorials\CoreEngine_003_MINIDUMPER_STACK_EXCEPTION
REM start C:\WoMA3Dengine\CoreEngine_Tutorials\x64\Debug\CoreEngine_003.exe /renderOnce /Xpos 1280 /Ypos 000 /WIDTH 640 /HEIGHT 360

cd C:\WoMA3Dengine\CoreEngine_Tutorials\CoreEngine_004_SYSTEMMANAGER_TIMER_FPS_MATH				 
start C:\WoMA3Dengine\CoreEngine_Tutorials\x64\Debug\CoreEngine_004.exe /renderOnce /Xpos 000  /Ypos 360 /WIDTH 640 /HEIGHT 360
cd C:\WoMA3Dengine\CoreEngine_Tutorials\CoreEngine_005_PAINT_SETTINGS_XML
start C:\WoMA3Dengine\CoreEngine_Tutorials\x64\Debug\CoreEngine_005.exe /renderOnce /Xpos 640  /Ypos 360 /WIDTH 640 /HEIGHT 360
cd C:\WoMA3Dengine\CoreEngine_Tutorials\CoreEngine_006_PACKMANAGER_IDEA_CPU_BENCHMARK
start C:\WoMA3Dengine\CoreEngine_Tutorials\x64\Debug\CoreEngine_006.exe /renderOnce /Xpos 1280 /Ypos 360 /WIDTH 640 /HEIGHT 360

cd C:\WoMA3Dengine\CoreEngine_Tutorials\CoreEngine_007_PAINT_REALTIMECELESTIAL_ASTROCLASS														 
start C:\WoMA3Dengine\CoreEngine_Tutorials\x64\Debug\CoreEngine_007.exe /renderOnce /Xpos 000  /Ypos 720 /WIDTH 640 /HEIGHT 360
cd C:\WoMA3Dengine\CoreEngine_Tutorials\CoreEngine_008_PAINT_REALTIME_GPS_MAP_LOCATION
start C:\WoMA3Dengine\CoreEngine_Tutorials\x64\Debug\CoreEngine_008.exe /renderOnce /Xpos 640  /Ypos 720 /WIDTH 640 /HEIGHT 360
cd C:\WoMA3Dengine\CoreEngine_Tutorials\CoreEngine_009_PAINT_METAR_WEATHER_REALTIME
start C:\WoMA3Dengine\CoreEngine_Tutorials\x64\Debug\CoreEngine_009.exe /renderOnce /Xpos 1280 /Ypos 720 /WIDTH 640 /HEIGHT 360

cd C:\WoMA3Dengine\CoreEngine_Tutorials
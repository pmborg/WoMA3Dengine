del /S /F /Q ENGINE_*.txt 2> nul
del /S /F /Q DXENGINE_*.txt 2> nul

cd ../ThirdParty/DirectXTK
del /S /F /Q x64 2> nul
del /S /F /Q Bin 2> nul
rmdir /S /Q x64 2> nul
rmdir /S /Q Bin 2> nul
cd ..

cd CoreEngine_000_OSENGINE_SYSTEM
call CLEAN_ALL.bat
cd ..
cd CoreEngine_001_LANG_LOG_MEMLEAKs_OSMAINDIRs
call CLEAN_ALL.bat
cd..
cd CoreEngine_002_MAINWINDOW_EVENTS
call CLEAN_ALL.bat
cd..
cd CoreEngine_003_MINIDUMPER_STACK_EXCEPTION
call CLEAN_ALL.bat
cd..
cd CoreEngine_004_SYSTEMMANAGER_TIMER_FPS_MATH
call CLEAN_ALL.bat
cd..
cd CoreEngine_005_PAINT_SETTINGS_XML
call CLEAN_ALL.bat
cd..
cd CoreEngine_006_PACKMANAGER_IDEA_CPU_BENCHMARK
call CLEAN_ALL.bat
cd..
cd CoreEngine_007_PAINT_REALTIMECELESTIAL_ASTROCLASS
call CLEAN_ALL.bat
cd..
cd CoreEngine_008_PAINT_REALTIME_GPS_MAP_LOCATION
call CLEAN_ALL.bat
cd..
cd CoreEngine_009_PAINT_METAR_WEATHER_REALTIME
call CLEAN_ALL.bat
cd..
cd DXENGINE_019
call CLEAN_ALL.bat
cd..
cd DXENGINE_020
call CLEAN_ALL.bat
cd..
cd DXENGINE_021
call CLEAN_ALL.bat
cd..
cd DXENGINE_022
call CLEAN_ALL.bat
cd..
cd DXENGINE_023
call CLEAN_ALL.bat
cd..
cd DXENGINE_024
call CLEAN_ALL.bat
cd..
cd DXENGINE_025
call CLEAN_ALL.bat
cd..
cd DXENGINE_026
call CLEAN_ALL.bat
cd..
cd DXENGINE_027
call CLEAN_ALL.bat
cd..
cd DXENGINE_028
call CLEAN_ALL.bat
cd..
cd DXENGINE_029
call CLEAN_ALL.bat
cd..
cd DXENGINE_030
call CLEAN_ALL.bat
cd..
cd DXENGINE_031
call CLEAN_ALL.bat
cd..
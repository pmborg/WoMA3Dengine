@echo off
for /f "usebackq tokens=1* delims=: " %%i in (`"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -latest`) do (
  if /i "%%i"=="productPath" set devenv=%%j
)
echo "Using Visual Studio: %devenv%"
REM -------------------------
echo "Compiling: Release|x64"
echo on

del /S /F /Q ENGINE_*.txt 2> nul
del /S /F /Q DXENGINE_*.txt 2> nul
"%devenv%" CoreEngine_006_PACKMANAGER_IDEA_CPU_BENCHMARK\CoreEngine_006.sln /project CoreEngine_006 /Build "Debug|x64" /out ENGINE_006.txt
"%devenv%" DXENGINE_029/DXENGINE_029.sln /project DXENGINE_029 /Build "Release|x64" /out DXENGINE_029.txt
copy /Y C:\WoMA3Dengine\HowToInstallVisualStudio2022Community\VC_redist.x64.exe C:\WoMA3Dengine\CoreEngine_Tutorials\DXENGINE_029\output
@echo off
echo "Compiled:"
dir /S /B *.exe | find /v "fxc"
call \WoMA3Dengine\HowToInstallWindows-InnoSetup\generate-installer-demo29.bat
pause
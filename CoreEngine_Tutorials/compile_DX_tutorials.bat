@echo off
for /f "usebackq tokens=1* delims=: " %%i in (`"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -latest`) do (
  if /i "%%i"=="productPath" set devenv=%%j
)
echo "Using Visual Studio: %devenv%"
REM -------------------------
echo "Compiling: Debug|x64"
echo on
"%devenv%" DXENGINE_019/DXENGINE_019.sln /project DXENGINE_019 /Build "Debug|x64" /out DXENGINE_019.txt
"%devenv%" DXENGINE_020/DXENGINE_020.sln /project DXENGINE_020 /Build "Debug|x64" /out DXENGINE_020.txt
"%devenv%" DXENGINE_021/DXENGINE_021.sln /project DXENGINE_021 /Build "Debug|x64" /out DXENGINE_021.txt
"%devenv%" DXENGINE_022/DXENGINE_022.sln /project DXENGINE_022 /Build "Debug|x64" /out DXENGINE_022.txt
"%devenv%" DXENGINE_023/DXENGINE_023.sln /project DXENGINE_023 /Build "Debug|x64" /out DXENGINE_023.txt
"%devenv%" DXENGINE_024/DXENGINE_024.sln /project DXENGINE_024 /Build "Debug|x64" /out DXENGINE_024.txt
"%devenv%" DXENGINE_025/DXENGINE_025.sln /project DXENGINE_025 /Build "Debug|x64" /out DXENGINE_025.txt
"%devenv%" DXENGINE_026/DXENGINE_026.sln /project DXENGINE_026 /Build "Debug|x64" /out DXENGINE_026.txt
"%devenv%" DXENGINE_027/DXENGINE_027.sln /project DXENGINE_027 /Build "Debug|x64" /out DXENGINE_027.txt
@echo off
echo "Compiled:"
dir /S /B *.exe | find /v "fxc"
echo "How to Run one demo?"
echo "Execute (example):"
echo "cd C:\WoMA3Dengine\CoreEngine_Tutorials\DXENGINE_019"
echo "x64\Debug\CoreEngine_019.exe"
pause
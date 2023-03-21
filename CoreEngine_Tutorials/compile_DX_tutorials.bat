@echo off
for /f "usebackq tokens=1* delims=: " %%i in (`"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -latest`) do (
  if /i "%%i"=="productPath" set devenv=%%j
)
echo "Using Visual Studio: %devenv%"
REM -------------------------
echo "Compiling: Debug|x64"
echo on
"%devenv%" DXENGINE_019/DXENGINE_019.sln /project DXENGINE_019 /Build "Debug|x64"
"%devenv%" DXENGINE_020/DXENGINE_020.sln /project DXENGINE_020 /Build "Debug|x64"
"%devenv%" DXENGINE_021/DXENGINE_021.sln /project DXENGINE_021 /Build "Debug|x64"
"%devenv%" DXENGINE_022/DXENGINE_022.sln /project DXENGINE_022 /Build "Debug|x64"
"%devenv%" DXENGINE_023/DXENGINE_023.sln /project DXENGINE_023 /Build "Debug|x64"
"%devenv%" DXENGINE_024/DXENGINE_024.sln /project DXENGINE_024 /Build "Debug|x64"
"%devenv%" DXENGINE_025/DXENGINE_025.sln /project DXENGINE_025 /Build "Debug|x64"
@echo off
echo "Compiled:"
dir /S /B *.exe | find /v "fxc"

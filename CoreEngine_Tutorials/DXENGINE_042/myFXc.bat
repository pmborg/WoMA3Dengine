@echo off
REM ----------------------------------------------------------------------------------------------------
REM DirectX 8.0  - Shader Model 1.0 & 1.1
REM DirectX 8.0a - Shader Model 1.3
REM DirectX 8.1  - Shader Model 1.4
REM 
REM WINXP:
REM DirectX 9.0  - Shader Model 2.0		PS 2.0  = DirectX 9.0 original Shader Model 2 specification.
REM DirectX 9.0a - Shader Model 2.0a	PS 2.0a = DirectX 9.0a NVIDIA GeForce FX-optimized model.
REM DirectX 9.0b - Shader Model 2.0b	PS 2.0b = DirectX 9.0b ATI Radeon X700, X800, X850 shader model
REM DirectX 9.0c - Shader Model 3.0
REM 
REM // Supported by WOMA:
REM ----------------------------------------------------------------------------------------------------
REM WIN VISTA + SP2
REM DirectX 10.0* - Shader Model 4.0
REM DirectX 10.1* - Shader Model 4.1
REM DirectX 11.0* - Shader Model 5.0
REM 
REM WIN 10:
REM DirectX 12.0* - Shader Model 5.1
REM ----------------------------------------------------------------------------------------------------

set RelativePath="%1"
set Filename="%2"
set FullPath="%3"
set EngineLevel="%4"

echo RelativePath="%1"
echo Filename="%2"
echo FullPath="%3"
echo EngineLevel="%4"

if "%1" == "" goto error
if "%2" == "" goto error
if "%3" == "" goto error
if "%4" == "" goto error

REM Deprecated:
REM fxc.exe /DENGINE_LEVEL=%EngineLevel% /DDX9  /nologo /O3 /T vs_2_0 /Vn MyVertexShader%Filename%_20 /E MyVertexShader%Filename% /Fh ..\DXengine/%Filename%Vs_20.h %FullPath%
REM REM fxc.exe /DENGINE_LEVEL=%EngineLevel% /DDX9 /nologo /O3 /T gs_2_0 /Vn MyGeometryShader%Filename%_20 /E MyGeometryShader%Filename% /Fh ..\DXengine/%Filename%Gs_20.h %FullPath%
REM fxc.exe /DENGINE_LEVEL=%EngineLevel% /DDX9 /nologo /O3 /T ps_2_0 /Vn MyPixelShader%Filename%_20 /E MyPixelShader%Filename% /Fh ..\DXengine/%Filename%Ps_20.h %FullPath%

REM fxc.exe /DENGINE_LEVEL=%EngineLevel% /DDX9 /nologo /O3 /T vs_3_0 /Vn MyVertexShader%Filename%_30 /E MyVertexShader%Filename% /Fh ..\DXengine/%Filename%Vs_30.h %FullPath%
REM REM fxc.exe /DENGINE_LEVEL=%EngineLevel% /DDX9 /nologo /O3 /T gs_3_0 /Vn MyGeometryShader%Filename%_30 /E MyGeometryShader%Filename% /Fh ..\DXengine/%Filename%Gs_30.h %FullPath%
REM fxc.exe /DENGINE_LEVEL=%EngineLevel% /DDX9 /nologo /O3 /T ps_3_0 /Vn MyPixelShader%Filename%_30 /E MyPixelShader%Filename% /Fh ..\DXengine/%Filename%Ps_30.h %FullPath%

REM Supported by WOMA:
fxc.exe /DENGINE_LEVEL=%EngineLevel% /D VS /nologo /O3 /T vs_4_0 /Vn MyVertexShader%Filename%_40 /E MyVertexShader%Filename% /Fh ..\DXengine/%Filename%Vs_40.h %FullPath%
REM fxc.exe /DENGINE_LEVEL=%EngineLevel% /nologo /O3 /T gs_4_0 /Vn MyGeometryShader%Filename%_40 /E MyGeometryShader%Filename% /Fh ..\DXengine/%Filename%Gs_40.h %FullPath%
fxc.exe /DENGINE_LEVEL=%EngineLevel% /D PS /nologo /O3 /T ps_4_0 /Vn MyPixelShader%Filename%_40 /E MyPixelShader%Filename% /Fh ..\DXengine/%Filename%Ps_40.h %FullPath%

fxc.exe /DENGINE_LEVEL=%EngineLevel% /D VS /nologo /O3 /T vs_4_1 /Vn MyVertexShader%Filename%_41 /E MyVertexShader%Filename% /Fh ..\DXengine/%Filename%Vs_41.h %FullPath%
REM fxc.exe /DENGINE_LEVEL=%EngineLevel% /nologo /O3 /T gs_4_1 /Vn MyGeometryShader%Filename%_41 /E MyGeometryShader%Filename% /Fh ..\DXengine/%Filename%Gs_41.h %FullPath%
fxc.exe /DENGINE_LEVEL=%EngineLevel% /D PS /nologo /O3 /T ps_4_1 /Vn MyPixelShader%Filename%_41 /E MyPixelShader%Filename% /Fh ..\DXengine/%Filename%Ps_41.h %FullPath%

fxc.exe /DENGINE_LEVEL=%EngineLevel% /D VS /nologo /O3 /T vs_5_0 /Vn MyVertexShader%Filename%_50 /E MyVertexShader%Filename% /Fh ..\DXengine/%Filename%Vs_50.h %FullPath%
REM fxc.exe /DENGINE_LEVEL=%EngineLevel% /nologo /O3 /T gs_5_0 /Vn MyGeometryShader%Filename%_50 /E MyGeometryShader%Filename% /Fh ..\DXengine/%Filename%Gs_50.h %FullPath%
fxc.exe /DENGINE_LEVEL=%EngineLevel% /D PS /nologo /O3 /T ps_5_0 /Vn MyPixelShader%Filename%_50 /E MyPixelShader%Filename% /Fh ..\DXengine/%Filename%Ps_50.h %FullPath%

fxc.exe /DENGINE_LEVEL=%EngineLevel% /D VS /D WIN10 /nologo /O3 /T vs_5_1 /Vn MyVertexShader%Filename%_51 /E MyVertexShader%Filename% /Fh ..\DXengine/%Filename%Vs_51.h %FullPath%
REM fxc.exe /DENGINE_LEVEL=%EngineLevel% /nologo /O3 /T gs_5_1 /Vn MyGeometryShader%Filename%_51 /E MyGeometryShader%Filename% /Fh ..\DXengine/%Filename%Gs_51.h %FullPath%
fxc.exe /DENGINE_LEVEL=%EngineLevel% /D PS /D WIN10 /nologo /O3 /T ps_5_1 /Vn MyPixelShader%Filename%_51 /E MyPixelShader%Filename% /Fh ..\DXengine/%Filename%Ps_51.h %FullPath%
goto end

:error
echo myFXc.bat missing argument!
echo usage %0 <Filename> <FullPath>

:end

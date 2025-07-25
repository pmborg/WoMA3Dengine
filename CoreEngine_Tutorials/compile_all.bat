@echo off
setlocal
cls

cd /D C:\WoMA3Dengine\CoreEngine_Tutorials

for /D %%S in (WindowsEngine_000-009-Core.sln) do (
    echo =====================================
    echo Building %%S in Debug...
    "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" %%S /m /p:Configuration=Debug

    echo Building %%S in Release...
    "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" %%S /m /p:Configuration=Release
    echo =====================================
)

for /D %%S in (WindowsEngine_019-029-Basic.sln) do (
    echo =====================================
    echo Building %%S in Debug...
    "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" %%S /m /p:Configuration=Debug

    echo Building %%S in Release...
    "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" %%S /m /p:Configuration=Release
    echo =====================================
)

for /D %%S in (WindowsEngine_030-045-Intermediate.sln) do (
    echo =====================================
    echo Building %%S in Debug...
    "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" %%S /m /p:Configuration=Debug

    echo Building %%S in Release...
    "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" %%S /m /p:Configuration=Release
    echo =====================================
)

for /D %%S in (WindowsEngine_049-089-Terrain.sln) do (
    echo =====================================
    echo Building %%S in Debug...
    "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" %%S /m /p:Configuration=Debug

    echo Building %%S in Release...
    "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" %%S /m /p:Configuration=Release
    echo =====================================
)

rem for /D %%S in (ANDROID1_engine_ALL.sln) do (
rem     echo =====================================
rem     echo Building %%S in Debug...
rem     "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" %%S /m /p:Configuration=Debug
rem 
rem     REM echo Building %%S in Release...
rem     "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" %%S /m /p:Configuration=Release
rem     REM echo =====================================
rem )

rem for /D %%S in (LINUX_engine_ALL.sln) do (
rem     echo =====================================
rem     echo Building %%S in Debug...
rem     "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" %%S /m /p:Configuration=Debug
rem 
rem     REM echo Building %%S in Release...
rem     "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" %%S /m /p:Configuration=Release
rem     REM echo =====================================
rem )

endlocal

pause
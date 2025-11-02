@echo off
title WoMA3Dengine - PulseAudio Installer (bin only)
color 0A

echo ==============================================================
echo  WoMA3Dengine PulseAudio Setup Script
echo  Purpose: Install/Update PulseAudio binaries for WSL2 sound bridge
echo ==============================================================
echo.

setlocal

REM ------------------------------------------------------------------------------
REM Paths
REM ------------------------------------------------------------------------------
set ROOT_DIR=%~dp0
set BIN_DIR=%ROOT_DIR%bin
set ZIP_URL=http://bosmans.ch/pulseaudio/pulseaudio-1.1.zip
set ZIP_FILE=%ROOT_DIR%pulseaudio.zip
set TMP_DIR=%ROOT_DIR%tmp_unzip

REM ------------------------------------------------------------------------------
REM Ensure folder structure
REM ------------------------------------------------------------------------------
if not exist "%BIN_DIR%" (
    echo [INFO] Creating bin folder...
    mkdir "%BIN_DIR%"
)

REM ------------------------------------------------------------------------------
REM Download package if not already cached
REM ------------------------------------------------------------------------------
echo [INFO] Downloading PulseAudio prebuilt package (binaries only)...
powershell -Command "try { (New-Object Net.WebClient).DownloadFile('%ZIP_URL%', '%ZIP_FILE%') } catch { exit 1 }"
if errorlevel 1 (
    echo [ERROR] Failed to download from %ZIP_URL%
    echo        Please check your connection or download manually.
    pause
    exit /b 1
)

REM ------------------------------------------------------------------------------
REM Extract only "bin" folder from the archive
REM ------------------------------------------------------------------------------
echo [INFO] Extracting binaries...
if exist "%TMP_DIR%" rd /s /q "%TMP_DIR%"
mkdir "%TMP_DIR%"

powershell -Command "Expand-Archive -Path '%ZIP_FILE%' -DestinationPath '%TMP_DIR%' -Force"

REM Move bin contents only
echo [INFO] Copying files to "%BIN_DIR%" ...
xcopy "%TMP_DIR%\pulseaudio-1.1\bin\*" "%BIN_DIR%\" /E /Y >nul

REM Cleanup temporary files
rd /s /q "%TMP_DIR%"
del "%ZIP_FILE%" >nul 2>&1

REM ------------------------------------------------------------------------------
REM Done
REM ------------------------------------------------------------------------------
echo.
echo [OK] PulseAudio binaries installed or updated successfully.
echo [OK] Config files were NOT modified.
echo.
echo To test the installation, run:
echo     test_pulse_win.bat
echo.
pause
endlocal
exit /b 0

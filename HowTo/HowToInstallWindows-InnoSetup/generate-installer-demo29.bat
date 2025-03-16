
REM Re-generate: pck files: 
cd C:\WoMA3Dengine\CoreEngine_Tutorials
x64\Debug\CoreEngine_006.exe

REM Generate: RELEASE BIN:
cd C:\WoMA3Dengine\CoreEngine_Tutorials\DXENGINE_029
"C:\Program Files (x86)\Inno Setup 6\Compil32.exe" /cc Setup.iss

REM Run Installer:
cd C:\WoMA3Dengine\CoreEngine_Tutorials
C:\WoMA3Dengine\CoreEngine_Tutorials\DXENGINE_029\Output\WoMAsetup029.exe
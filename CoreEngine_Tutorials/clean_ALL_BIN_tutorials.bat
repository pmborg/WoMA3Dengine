ECHO ON
del /S /F /Q ENGINE_*.txt 2> nul
del /S /F /Q DXENGINE_*.txt 2> nul

for /D %%f in (CoreEngine_*) do (
cd %%f
call CLEAN_ALL.bat
cd ..
)

for /D %%f in (DXENGINE_*) do (
cd %%f
for /D %%d in (DXENGINE_*) do (
	rmdir /S /Q %%d 2> nul
)
call CLEAN_ALL.bat
cd ..
)

rem cd ../ThirdParty/DirectXTK
rem del /S /F /Q x64 2> nul
rem del /S /F /Q Bin 2> nul
rem rmdir /S /Q x64 2> nul
rem rmdir /S /Q Bin 2> nul
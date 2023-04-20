del /S /F /Q ENGINE_*.txt 2> nul
del /S /F /Q DXENGINE_*.txt 2> nul

cd ../ThirdParty/DirectXTK
del /S /F /Q x64 2> nul
del /S /F /Q Bin 2> nul
rmdir /S /Q x64 2> nul
rmdir /S /Q Bin 2> nul
cd ..

for /D %%f in (CoreEngine_*) do (
cd %%f
call CLEAN_ALL.bat
cd ..
)

for /D %%f in (DXENGINE_*) do (
cd %%f
call CLEAN_ALL.bat
cd ..
)

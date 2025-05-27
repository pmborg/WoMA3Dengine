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

del /S /F /Q Libs 2> nul
rmdir /S /Q Libs 2> nul

del /S /F /Q x64 2> nul
rmdir /S /Q x64 2> nul

del /S /F /Q Int 2> nul
rmdir /S /Q Int 2> nul

del /S /F /Q Int-latest 2> nul
rmdir /S /Q Int-latest 2> nul
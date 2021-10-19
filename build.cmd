@echo off
setlocal

call clean.cmd

set vc="%VS140COMNTOOLS%..\..\VC\vcvarsall.bat"
if exist %vc% goto :build
set vc="%VS150COMNTOOLS%..\..\VC\Auxiliary\Build\vcvarsall.bat"
if exist %vc% goto :build
echo Microsoft Visual C++ 2015/2017 is missing. Please go to http://www.visualstudio.com/ and install free Community edition.
exit /b 1

:build
echo Building...
call %vc% x86
msbuild Localization.sln /v:m /t:Rebuild /p:Configuration=Release /p:Platform=Win32
if exist Win32\release\exe2po.exe goto pack
echo Binary file is missing. Compile project first please.
exit /b 1

:pack
echo.
echo Detecting 7-Zip...
set zip="%ProgramFiles%\7-Zip\7z.exe"
if exist %zip% goto zip
set zip="%ProgramFiles(x86)%\7-Zip\7z.exe"
if exist %zip% goto zip
set zip="%ProgramW6432%\7-Zip\7z.exe"
if exist %zip% goto zip
echo The 7-Zip utility is missing. Please go to http://www.7-zip.org/ and install 7-Zip.
exit /b 1

:zip
echo.
echo Packing by 7-Zip: %zip%...
for /F "tokens=3" %%i in ( 'findstr ProductVersion exe2po\exe2po.rc' ) do set version=%%~i
md "..\redist" 2>nul:
set dst="%CD%\..\redist\po-localization-%version%.zip"
del %dst% 2>nul:
cd exe2po
%zip% a -tzip -mx9 -mm=Deflate64 -mpass=15 -stl %dst% gpl-2.0.txt
cd ..\src
%zip% a -tzip -mx9 -mm=Deflate64 -mpass=15 -stl %dst% lgpl-2.1.txt Localization.h Localization.cpp COMPRESS.EXE
cd ..\Win32\release
%zip% a -tzip -mx9 -mm=Deflate64 -mpass=15 -stl %dst% exe2po.exe
cd ..\..
%zip% a -tzip -mx9 -mm=Deflate64 -mpass=15 -stl -r -xr!*.user -xr!*.aps -xr!*.p_ -xr!win32 -xr!x64 %dst% sample ReadMe.txt *.reg
echo.

echo Done.
exit /b 0
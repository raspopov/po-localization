@echo off
setlocal
set "cur=%cd%\"
pushd "%~dp0"
for /F "tokens=3" %%i in ( 'findstr ProductVersion %~dp0exe2po\exe2po.rc' ) do set "version=%%~i"
set "dst=%cur%po-localization-%version%.zip"
echo Packing %dst%...
del "%dst%" 2>nul
rd /q /s sample\Win32\ 2>nul
rd /q /s sample\x64\ 2>nul
del sample\*.user 2>nul
del sample\*.aps 2>nul
del sample\res\*.p_ 2>nul
powershell -Command "& { Compress-Archive -Path '%cur%exe2po.exe','%cur%sample.exe',ReadMe*,LICENSE,*.reg,src\*,sample\ -DestinationPath '%dst%' }"
popd
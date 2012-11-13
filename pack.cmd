set version=1.0.0.2
md "..\redist"
del "..\redist\po-localization-%version%.zip"
"%ProgramFiles%\WinRar\winrar.exe" a -ep -m5 -cfg- -afzip "..\redist\po-localization-%version%.zip" exe2po\gpl-2.0.txt src\lgpl-2.1.txt src\Localization.h src\Localization.cpp ReadMe.txt src\COMPRESS.EXE Win32\release\exe2po.exe *.reg
"%ProgramFiles%\WinRar\winrar.exe" a -m5 -cfg- -afzip "..\redist\po-localization-%version%.zip" sample\ sample\res -x*.user -x*.aps

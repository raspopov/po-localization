set version=1.0.0.2
md "..\redist"
del "..\redist\po-localization-%version%.zip"
"%ProgramFiles%\WinRar\winrar.exe" a -ep -m5 -cfg- -afzip "..\redist\po-localization-%version%.zip" gpl-2.0.txt lgpl-2.1.txt Localization.h Localization.cpp ReadMe.txt COMPRESS.EXE Win32\release\exe2po.exe *.reg
"%ProgramFiles%\WinRar\winrar.exe" a -m5 -cfg- -afzip "..\redist\po-localization-%version%.zip" sample\ sample\res -x*.user -x*.aps

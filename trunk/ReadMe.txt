PO-Localization

This is a library designed to help C++ developers in creating Windows
application to translate it on world languages. Project contains an
utility to convert existing resources of application in PO-file and
adds ability to use translated PO-files to application itself.
The PO-file is a native format of poEdit (a very convenient application
for translators). 

========================================================================

EXE to PO file converter usage:

	exe2po.exe {input file.exe} {output file.po}

Generated .po-file blocks for menu resources:

	#: MENUPOPUP.{menu-id}.{ordinal}
	msgid "English menu popup"
	msgstr ""

	#: MENUITEM.{menu-id}.{menu-item-id}
	msgid "English menu item"
	msgstr ""

	Where {ordinal} is a number 0, 1, 2 etc.

Generated .po-file blocks for dialog box resources:

	#: DIALOGCAPTION.{dialog-id}
	msgid "English dialog caption"
	msgstr ""

	#: DIALOGCONTROL.{dialog-id}.{control-class}.{dialog-control-id}
	msgid "English dialog control"
	msgstr ""

	Where {control-class} is a "Button", "Static" etc.
	If dialog control id is a 0xffff then {dialog-control-id} = ( 0x80000000 | ordinal ) where ordinal is a number 0, 1, 2 etc.

Generated .po-file blocks for string resources:

	#: STRING.{string-id}
	msgid "English string"
	msgstr ""

========================================================================

License:

	Localization.h and Localization.cpp licensed under LGPL 2.1.
	exe2po and sample licensed under GPL 2.0.

========================================================================

Copyrights:

Copyright (c) Nikolay Raspopov, 2011.
E-mail: ryo.rabbit@gmail.com
Web: http://code.google.com/p/po-localization/

EXE to PO file converter.
Copyright (c) Nikolay Raspopov, 2011.
E-mail: raspopov@cherubicsoft.com
Web: http://www.cherubicsoft.com/

Usage:

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

License:

	exe2po is free software; you can redistribute it
	and/or modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2 of
	the License, or (at your option) any later version.

	exe2po is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with exe2po; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

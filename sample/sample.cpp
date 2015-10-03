/*
This file is part of Localization library
https://sourceforge.net/projects/po-localization/

Copyright (C) 2011-2015 Nikolay Raspopov <raspopov@cherubicsoft.com>

This program is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
( at your option ) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see < http://www.gnu.org/licenses/>.
*/

#include "stdafx.h"
#include "sample.h"
#include "sampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CSampleApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CSampleApp theApp;

BOOL CSampleApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls = { sizeof( INITCOMMONCONTROLSEX ), ICC_WIN95_CLASSES };
	InitCommonControlsEx( &InitCtrls );

	CWinApp::InitInstance();

	SetRegistryKey( _T("Localization") );

	// Parse command-line options
	LANGID nLang = 0;
	int nArgs = 0;
	if ( LPWSTR* szArglist = CommandLineToArgvW( GetCommandLineW(), &nArgs ) )
	{
		for ( int i = 0; i < nArgs; ++i )
		{
			// Option: -lang:XX, where XX - hexadecimal language code ( 09 - English, 19 - Russian etc.)
			if ( _tcsnicmp( szArglist[ i ], _T( "-lang:" ), 6 ) == 0|| _tcsnicmp( szArglist[ i ], _T( "/lang:" ), 6 ) == 0 )
			{
				int lang;
				if ( _stscanf_s( szArglist[ i ] + 6, _T( "%x" ), &lang ) == 1 )
				{
					nLang = (LANGID)lang;
				}
			}
		}
		LocalFree( szArglist );
	}

	m_pLoc.Load();			// Load all translations
	m_pLoc.Select( nLang );	// Select initial user language

	for (;;)
	{
		CSampleDlg dlg;
		// m_pMainWnd = &dlg;
		if ( dlg.DoModal() != IDRETRY )
			break;
	}

	return FALSE;
}
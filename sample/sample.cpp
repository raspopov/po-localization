// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/*
This file is part of Localization library
https://www.cherubicsoft.com/en/projects/po-localization/

Copyright (C) 2011-2023 Nikolay Raspopov <raspopov@cherubicsoft.com>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
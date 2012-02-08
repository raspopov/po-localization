//
// sample.cpp
//
// This file is part of Localization library.
// Copyright (c) Nikolay Raspopov, 2011.
// E-mail: ryo.rabbit@gmail.com
// Web: http://code.google.com/p/po-localization/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//

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

	m_pLoc.Load();		// Load all translations
	m_pLoc.Select();	// Select user language

	for (;;)
	{
		CSampleDlg dlg;
		// m_pMainWnd = &dlg;
		if ( dlg.DoModal() != IDRETRY )
			break;
	}

	return FALSE;
}
//
// sample.cpp 
//
// This file is part of Localization library.
// Copyright (c) Nikolay Raspopov, 2011.
// E-mail: raspopov@cherubicsoft.com
// Web: http://www.cherubicsoft.com/
//
// sample is free software; you can redistribute it
// and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// sample is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with sample; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
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

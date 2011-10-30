//
// sampleDlg.cpp
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

CSampleDlg::CSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSampleDlg::IDD, pParent)
	, m_nCombo ( 0 )
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT2, m_sEdit);
	DDX_CBIndex(pDX, IDC_COMBO1, m_nCombo);
	DDX_Control(pDX, IDC_LANGS, m_pLangs);
}

BEGIN_MESSAGE_MAP(CSampleDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CSampleDlg::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_LANGS, &CSampleDlg::OnCbnSelchangeLangs)
END_MESSAGE_MAP()

BOOL CSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon( m_hIcon, TRUE );		// Set big icon
	SetIcon( m_hIcon, FALSE );		// Set small icon

	theApp.m_pLoc.FillComboBox( m_pLangs.GetSafeHwnd() );

	theApp.m_pLoc.Translate( GetSafeHwnd(), CSampleDlg::IDD );

	m_sEdit = theApp.m_pLoc.LoadString( IDS_STRING130 );
	m_sEdit.Replace( _T("\n"), _T("\r\n") );

	UpdateData( FALSE );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSampleDlg::OnBnClickedButton1()
{
	CPoint cursor;
	GetCursorPos( &cursor );

	CMenu pMenu;
	pMenu.LoadMenu( IDR_MENU1 );
	theApp.m_pLoc.Translate( pMenu.GetSafeHmenu(), IDR_MENU1 );
	pMenu.GetSubMenu( 0 )->TrackPopupMenu( TPM_LEFTALIGN | TPM_RETURNCMD, cursor.x, cursor.y, this );
}

void CSampleDlg::OnCbnSelchangeLangs()
{
	theApp.m_pLoc.Select( m_pLangs.GetSafeHwnd() );

	EndDialog( IDRETRY );
}

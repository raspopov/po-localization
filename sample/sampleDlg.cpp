// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/*
This file is part of Localization library
https://github.com/raspopov/po-localization

Copyright (C) 2011-2018 Nikolay Raspopov <raspopov@cherubicsoft.com>

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
	DDX_CBIndex(pDX, IDC_COMBO2, m_nCombo);
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
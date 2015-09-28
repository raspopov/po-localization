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

#pragma once

class CSampleDlg : public CDialog
{
public:
	CSampleDlg(CWnd* pParent = NULL);	// standard constructor

	enum { IDD = IDD_SAMPLE_DIALOG };

protected:
	HICON		m_hIcon;
	CString		m_sEdit;
	int			m_nCombo;
	CComboBox	m_pLangs;

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnSelchangeLangs();

	DECLARE_MESSAGE_MAP()
};
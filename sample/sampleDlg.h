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
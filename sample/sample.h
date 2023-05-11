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

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

class CSampleApp : public CWinApp
{
public:
	CLocalization m_pLoc;

protected:

	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CSampleApp theApp;
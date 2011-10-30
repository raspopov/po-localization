//
// sample.h
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

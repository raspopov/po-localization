//
// stdafx.h
//
// EXE to PO file converter.
// Copyright (c) Nikolay Raspopov, 2011.
// E-mail: raspopov@cherubicsoft.com
// Web: http://www.cherubicsoft.com/
//
// exe2po is free software; you can redistribute it
// and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// exe2po is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with exe2po; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#ifndef WINVER					// Specifies that the minimum required platform is Windows Vista.
#define WINVER 0x0600			// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT			// Specifies that the minimum required platform is Windows Vista.
#define _WIN32_WINNT 0x0600		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINDOWS          // Specifies that the minimum required platform is Windows 98.
#define _WIN32_WINDOWS 0x0410	// Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE				// Specifies that the minimum required platform is Internet Explorer 7.0.
#define _WIN32_IE 0x0700		// Change this to the appropriate value to target other versions of IE.
#endif

#define WIN32_LEAN_AND_MEAN
#define STRICT
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#define _ATL_CSTRING_NO_CRT
#define _ATL_NO_COM_SUPPORT

#include <atlbase.h>
#include <atlwin.h>
#include <atlstr.h>
#include <shlwapi.h>

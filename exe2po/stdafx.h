/*
This file is part of EXE to PO file converter (EXE2PO)
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

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#define _ATL_CSTRING_NO_CRT
#define _ATL_NO_COM_SUPPORT
#define _AFX_ALL_WARNINGS
#define NO_PRINT

#define _WIN32_WINNT 0x0501
#include <sdkddkver.h>

#include <atlbase.h>
#include <atlwin.h>
#include <atlstr.h>
#include <atlcoll.h>
#include <shlwapi.h>

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

//
// exe2po.cpp
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

#include "stdafx.h"

#define ORDINAL	0x80000000

static const struct
{
	LPCTSTR szName;
	LPCTSTR szType;
}
StandardResTypes[] =
{
	{ _T("RT_CURSOR"),		RT_CURSOR },
	{ _T("RT_BITMAP"),		RT_BITMAP },
	{ _T("RT_ICON"),		RT_ICON },
	{ _T("RT_MENU"),		RT_MENU },
	{ _T("RT_DIALOG"),		RT_DIALOG },
	{ _T("RT_STRING"),		RT_STRING },
	{ _T("RT_FONTDIR"),		RT_FONTDIR },
	{ _T("RT_FONT"),		RT_FONT },
	{ _T("RT_ACCELERATOR"),	RT_ACCELERATOR },
	{ _T("RT_RCDATA"),		RT_RCDATA },
	{ _T("RT_MESSAGETABLE"),RT_MESSAGETABLE },
	{ _T("RT_GROUP_CURSOR"),RT_GROUP_CURSOR },
	{ _T("RT_GROUP_ICON"),	RT_GROUP_ICON },
	{ _T("RT_VERSION"),		RT_VERSION },
	{ _T("RT_DLGINCLUDE"),	RT_DLGINCLUDE },
	{ _T("RT_PLUGPLAY"),	RT_PLUGPLAY },
	{ _T("RT_VXD"),			RT_VXD },
	{ _T("RT_ANICURSOR"),	RT_ANICURSOR },
	{ _T("RT_ANIICON"),		RT_ANIICON },
	{ _T("RT_HTML"),		RT_HTML },
	{ _T("RT_MANIFEST"),	RT_MANIFEST },
	{ NULL,					NULL }
};

LPCTSTR GetResType(LPTSTR szType)
{
	for ( int i = 0; StandardResTypes[ i ].szName; ++i )
	{
		if ( StandardResTypes[ i ].szType == szType )
			return StandardResTypes[ i ].szName;
	}
	return NULL;
}

CStringA UTF8Encode(__in_bcount(nInput) LPCWSTR psInput, __in int nInput)
{
	CStringA strUTF8;
	int nUTF8 = ::WideCharToMultiByte( CP_UTF8, 0, psInput, nInput,
		strUTF8.GetBuffer( nInput * 4 + 1 ), nInput * 4 + 1, NULL, NULL );

	if ( nUTF8 == 0 && GetLastError() == ERROR_INSUFFICIENT_BUFFER )
	{
		nUTF8 = ::WideCharToMultiByte( CP_UTF8, 0, psInput, nInput,
			NULL, 0, NULL, NULL );

		nUTF8 = ::WideCharToMultiByte( CP_UTF8, 0, psInput, nInput,
			strUTF8.GetBuffer( nUTF8 ), nUTF8, NULL, NULL );
	}
	strUTF8.ReleaseBuffer( nUTF8 );

	return strUTF8;
}

CStringA UTF8Encode(__in const CStringW& strInput)
{
	return UTF8Encode( strInput, strInput.GetLength() );
}

void OutputMsg(FILE* hFile, const CStringW& sString)
{
	CStringA sString8 = UTF8Encode( sString );
	sString8.Replace( "\"", "\\\"" );
	sString8.Replace( "\t", "\\t" );
	sString8.Replace( "\n", "\\n" );
	fprintf( hFile, "msgid \"%s\"\nmsgstr \"\"\n\n", sString8 );
}

void Output(FILE* hFile, UINT dwID, HMENU hMenu, UINT& nPopup)
{
	int nCount = GetMenuItemCount( hMenu );
	for ( int i = 0; i < nCount; ++i )
	{
		TCHAR szString[ 128 ] = {};

		MENUITEMINFO mii =
		{
			sizeof( MENUITEMINFO ),
			MIIM_SUBMENU | MIIM_STRING | MIIM_ID,
			0,
			0,
			0,
			NULL,
			NULL,
			NULL,
			NULL,
			szString,
			_countof( szString ),
			NULL
		};
		if ( GetMenuItemInfo( hMenu, i, TRUE, &mii ) )
		{
			if ( mii.hSubMenu )
			{
				_ftprintf( stderr, _T("\tMENUPOPUP.%u.%u\n"), dwID, nPopup );
				if ( mii.dwTypeData && *mii.dwTypeData )
				{
					fprintf( hFile, "#. Menu popup\n#: MENUPOPUP.%u.%u\n", dwID, nPopup );
					OutputMsg( hFile, mii.dwTypeData );
				}

				nPopup++;
				Output( hFile, dwID, mii.hSubMenu, nPopup );
			}
			else
			{
				_ftprintf( stderr, _T("\tMENUITEM.%u.%u\n"), dwID, mii.wID );
				if ( mii.dwTypeData && *mii.dwTypeData )
				{
					fprintf( hFile, "#. Menu item\n#: MENUITEM.%u.%u\n", dwID, mii.wID );
					OutputMsg( hFile, mii.dwTypeData );
				}	
			}
		}
	}
}

void Output(FILE* hFile, UINT dwID, LPCWSTR szBuf, int cchSize)
{
	CStringW sString;
	sString.Append( szBuf, cchSize );
	fprintf( hFile, "#. String resource\n#: STRING.%u\n", dwID );
	OutputMsg( hFile, sString );
}

BOOL CALLBACK EnumResNameProc(HMODULE hModule, LPCTSTR lpszType, LPTSTR lpszName, LONG_PTR lParam)
{
	FILE* hFile = (FILE*)lParam;
	UINT uID = 0;

	if ( IS_INTRESOURCE( lpszName ) )
	{
		uID = (UINT)(ULONG_PTR)lpszName;
		_ftprintf( stderr, _T("ID = %u\n"), uID );
	}
	else
	{
		if ( lpszName[ 0 ] == _T('#') )
		{
			uID = _tstol( &lpszName[ 1 ] );
			_ftprintf( stderr, _T("ID = #%u\n"), uID );
		}
		else
			_ftprintf( stderr, _T("ID = \"%s\"\n"), lpszName );
	}

	if ( IS_INTRESOURCE( lpszType ) )
	{
		if ( lpszType == RT_MENU )
		{
			if ( HMENU hMenu = LoadMenu( hModule, lpszName ) )
			{
				UINT nPopup = 0;
				Output( hFile, uID, hMenu, nPopup );

				DestroyMenu( hMenu );
			}
		}
		else if ( lpszType == RT_STRING ||
				  lpszType == RT_DIALOG )
		{
			if ( HRSRC hResInfo = FindResource( hModule, lpszName, lpszType ) )
			{
				if ( HGLOBAL hResData = LoadResource( hModule, hResInfo ) )
				{
					if ( LPVOID hData = LockResource( hResData ) )
					{
						if ( lpszType == RT_STRING )
						{
							DWORD dwStringID = ( uID - 1 ) * 16;
							LPCWSTR pwchCur = (LPCWSTR)hData;
							for ( int i = 0; i < 16; ++i, ++dwStringID )
							{
								if ( *pwchCur )
								{
									int cchString = *pwchCur++;  // String size in characters.

									if ( dwStringID < 0xE000 )	// Skip system resources
									{
										Output( hFile, dwStringID, pwchCur, cchString );
									}

									pwchCur += cchString;
								}
								else
									pwchCur++;
							}
						}
						else if ( lpszType == RT_DIALOG )
						{
							DLGTEMPLATE* pTemplate = (DLGTEMPLATE*)hData;
							BOOL bDialogEx = _DialogSplitHelper::IsDialogEx( pTemplate );
							
							WORD* pw;
							if ( bDialogEx )
								pw = (WORD*)((_DialogSplitHelper::DLGTEMPLATEEX*)pTemplate + 1);
							else
								pw = (WORD*)(pTemplate + 1);
							
							if (*pw == 0xFFFF)
								pw += 2;				// Has menu ID, so skip 2 words
							else
								while (*pw++);			// Either No menu, or string, skip past terminating NULL
							
							if (*pw == 0xFFFF)
								pw += 2;				// Has class atom, so skip 2 words
							else
								while (*pw++);			// Either No class, or string, skip past terminating NULL

							_ftprintf( stderr, _T("\tDIALOGCAPTION.%u\n"), uID );
							if ( *pw )
							{
								fprintf( hFile, "#. Dialog caption\n#: DIALOGCAPTION.%u\n", uID );
								OutputMsg( hFile, (LPCWSTR)pw );
							}

							UINT i = 0;
							DLGITEMTEMPLATE* pItem = _DialogSplitHelper::FindFirstDlgItem( pTemplate );
							WORD nItems = _DialogSplitHelper::DlgTemplateItemCount( pTemplate );
							for ( WORD nItem = 0; nItem < nItems; ++nItem )
							{
								UINT wID = bDialogEx ? (WORD)((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->id : pItem->id;
								
								WORD* pw;
								if ( bDialogEx )
									pw = (WORD*)((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem + 1);
								else
									pw = (WORD*)(pItem + 1);
								
								LPCWSTR szClass = L"";
								if ( *pw == 0xFFFF )			// Skip class name ordinal or string
								{
									switch ( *++pw )
									{
									case 0x0080: szClass = L"Button"; break;
									case 0x0081: szClass = L"Edit"; break;
									case 0x0082: szClass = L"Static"; break;
									case 0x0083: szClass = L"ListBox"; break;
									case 0x0084: szClass = L"ScrollBar"; break;
									case 0x0085: szClass = L"ComboBox"; break;
									}
									++pw;
								}
								else
								{
									szClass = (LPCWSTR)pw;
									while (*pw++);
								}

								if ( wID == 0xffff )
									wID = ORDINAL | i++;

								_ftprintf( stderr, _T("\tDIALOGCONTROL.%u.%s.%u\n"), uID, szClass, wID );
								if ( *pw )
								{
									fprintf( hFile, "#. Dialog control\n#: DIALOGCONTROL.%u.%s.%u\n", uID, (LPCSTR)CW2A(szClass), wID );
									OutputMsg( hFile, (LPCWSTR)pw );
								}

								pItem = _DialogSplitHelper::FindNextDlgItem( pItem, bDialogEx );
							}
						}
					}
					FreeResource( hResData );
				}
			}
		}
		// else
	}

	return TRUE;
}

BOOL CALLBACK EnumResTypeProc(HMODULE hModule, LPTSTR lpszType, LONG lParam)
{
	if ( IS_INTRESOURCE( lpszType ) )
	{
		if ( LPCTSTR szType = GetResType( lpszType ) )
			_ftprintf( stderr, _T("\nResource type: %s\n"), szType );
		else
			_ftprintf( stderr, _T("\nResource type: %u\n"), (ULONG_PTR)lpszType );
	}
	else
	{
		if ( lpszType[ 0 ] == _T('#') )
			_ftprintf( stderr, _T("\nResource type: #%d\n"), _tstol( &lpszType[ 1 ] ) );
		else
			_ftprintf( stderr, _T("\nResource type: \"%s\"\n"), lpszType );
	}

	return EnumResourceNames( hModule, lpszType, EnumResNameProc, lParam ); 
}


int _tmain(int argc, _TCHAR* argv[])
{
	if ( argc < 2 )
	{
		_ftprintf( stderr, _T("Usage: %s {input file.exe} {output file.po}\n"), PathFindFileName( argv[ 0 ] ) );
		return 1;
	}

	LPCTSTR szSource = argv[ 1 ];
 	LPCTSTR szDestination;
	CString sDest;
	if ( argc < 3 )
	{
		sDest = szSource;
		sDest += _T(".xxxx.po");
		szDestination = sDest;
	}
	else
		szDestination = argv[ 2 ];

	HMODULE hExe = LoadLibraryEx( szSource, NULL, LOAD_LIBRARY_AS_DATAFILE ); 
	if ( ! hExe )
	{
		_ftprintf( stderr, _T("Can't load input file: %s\n"), szSource );
		return 1;
	}

	FILE* hFile = NULL;
	if ( _tfopen_s( &hFile, szDestination, _T("wb,ccs=UTF-8") ) == 0 )
	{
		_ftprintf( stderr, _T("File: %s\n"), szSource );

		fprintf( hFile, "msgid \"\"\n"
			"msgstr \"\"\n"
			"\"Project-Id-Version: %s\\n\"\n"
			"\"POT-Creation-Date: \\n\"\n"
			"\"PO-Revision-Date: \\n\"\n"
			"\"Last-Translator: \\n\"\n"
			"\"Language-Team: \\n\"\n"
			"\"MIME-Version: 1.0\\n\"\n"
			"\"Content-Type: text/plain; charset=UTF-8\\n\"\n"
			"\"Content-Transfer-Encoding: 8bit\\n\"\n\n",
			(LPCSTR)UTF8Encode( CString( PathFindFileName( szSource ) ) ) );

		if ( EnumResourceTypes( hExe, EnumResTypeProc, (LONG_PTR)hFile ) )
		{
			// OK
		}
		fclose( hFile );
	}
	else
	{
		_ftprintf( stderr, _T("Can't create output file: %s\n"), szDestination );
		FreeLibrary( hExe );
		return 1;
	}

	FreeLibrary( hExe );

	return 0;
}

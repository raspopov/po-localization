// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/*
This file is part of EXE to PO file converter (EXE2PO)
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

#include "stdafx.h"

#define RT_DLGINIT  MAKEINTRESOURCE(240)
#define RT_TOOLBAR  MAKEINTRESOURCE(241)

#define ORDINAL		0x80000000

typedef struct
{
	CStringA sID;
	CStringA sText;
} Item;

typedef CAtlList < Item* >	CItems;

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
	{ _T("RT_DLGINIT"),		RT_DLGINIT },
	{ _T("RT_TOOLBAR"),		RT_TOOLBAR },
	{ NULL,					NULL }
};

inline LPCTSTR GetResType(LPTSTR szType)
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

inline CStringA UTF8Encode(__in const CStringW& strInput)
{
	return UTF8Encode( strInput, strInput.GetLength() );
}

inline CStringW Escape(CStringW sText)
{
	sText.Replace( L"\"", L"\\\"" );
	sText.Replace( L"\t", L"\\t" );
	sText.Replace( L"\n", L"\\n" );
	sText.Remove( L'\r' );
	return sText;
}

inline CStringA Escape(CStringA sText)
{
	sText.Replace( "\"", "\\\"" );
	sText.Replace( "\t", "\\t" );
	sText.Replace( "\n", "\\n" );
	sText.Remove( '\r' );
	return sText;
}

inline bool IsGood(LPCTSTR szText)
{
	if ( szText )
	{
		for ( LPCTSTR p = szText; *p; ++ p )
		{
			if ( _istalpha( *p ) )
				return true;
		}
	}
	return false;
}

void Add(CItems& pItems, LPCTSTR szID, LPCTSTR szText)
{
	CStringA sTextA = Escape( UTF8Encode( szText ) );

	for ( POSITION pos = pItems.GetHeadPosition(); pos; )
	{
		Item* item = pItems.GetNext( pos );
		if ( item->sText == sTextA )
		{
			// Duplicate text
			item->sID += szID;
			return;
		}
	}

	// New text
	Item* new_item = new Item;
	new_item->sID = szID;
	new_item->sText = sTextA;

	// Sort by ID
	for ( POSITION pos = pItems.GetHeadPosition(); pos; )
	{
		POSITION posOrigin = pos;
		const Item* item = pItems.GetNext( pos );
		if ( item->sID > new_item->sID )
		{
			pItems.InsertBefore( posOrigin, new_item );
			return;
		}
	}
	pItems.AddTail( new_item );
}

void Clear(CItems& pItems)
{
	for ( POSITION pos = pItems.GetHeadPosition(); pos; )
	{
		delete pItems.GetNext( pos );
	}
	pItems.RemoveAll();
}

void OutputAll(const CItems& pItems, FILE* hFile)
{
	for ( POSITION pos = pItems.GetHeadPosition(); pos; )
	{
		const Item* item = pItems.GetNext( pos );

		fprintf( hFile, "%smsgid \"%s\"\nmsgstr \"\"\n\n", (LPCSTR)item->sID, (LPCSTR)item->sText );
	}
}

void AddMenu(CItems& pItems, UINT dwID, HMENU hMenu, UINT& nPopup)
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
				if ( IsGood( mii.dwTypeData ) )
				{
					printf( "\tMENUPOPUP.%u.%u = \"%s\"\n", dwID, nPopup, (LPCSTR)CT2A( (LPCTSTR)Escape( mii.dwTypeData ) ) );
					CString sID;
					sID.Format( _T("#: MENUPOPUP.%u.%u\n"), dwID, nPopup );
					Add( pItems, sID, mii.dwTypeData );
				}
				else
					printf( "\tMENUPOPUP.%u.%u (ignored)\n", dwID, nPopup );

				nPopup++;
				AddMenu( pItems, dwID, mii.hSubMenu, nPopup );
			}
			else
			{
				if ( IsGood( mii.dwTypeData ) )
				{
					printf( "\tMENUITEM.%u.%u = \"%s\"\n", dwID, mii.wID, (LPCSTR)CT2A( (LPCTSTR)Escape( mii.dwTypeData ) ) );
					CString sID;
					sID.Format( _T("#: MENUITEM.%u.%u\n"), dwID, mii.wID );
					Add( pItems, sID, mii.dwTypeData );
				}
				else
					printf( "\tMENUITEM.%u.%u (ignored)\n", dwID, mii.wID );
			}
		}
	}
}

BOOL CALLBACK EnumResNameProc(HMODULE hModule, LPCTSTR lpszType, LPTSTR lpszName, LONG_PTR lParam)
{
	CItems& pItems = *(CItems*)lParam;
	UINT uID = 0;

	if ( IS_INTRESOURCE( lpszName ) )
	{
		uID = (UINT)(ULONG_PTR)lpszName;
		printf( "ID = %u\n", uID );
	}
	else
	{
		if ( lpszName[ 0 ] == _T('#') )
		{
			uID = _tstol( &lpszName[ 1 ] );
			printf( "ID = #%u\n", uID );
		}
		else
			printf( "ID = \"%s\"\n", (LPCSTR)CT2A( lpszName ) );
	}

	if ( IS_INTRESOURCE( lpszType ) )
	{
		if ( lpszType == RT_MENU )
		{
			if ( HMENU hMenu = LoadMenu( hModule, lpszName ) )
			{
				UINT nPopup = 0;
				AddMenu( pItems, uID, hMenu, nPopup );

				DestroyMenu( hMenu );
			}
		}
		else if ( lpszType == RT_STRING ||
				  lpszType == RT_DIALOG ||
				  lpszType == RT_DLGINIT )
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

									const CString sText( pwchCur, cchString );
									if ( cchString && IsGood( sText ) )
									{
										if ( dwStringID < 0xE000 )	// Skip system resources
										{
											printf( "\tSTRING.%u = \"%s\"\n", dwStringID, (LPCSTR)CT2A( (LPCTSTR)Escape( sText ) ) );
											CString sID;
											sID.Format( _T("#: STRING.%u\n"), dwStringID );
											Add( pItems, sID, sText );
										}
										else
											printf( "\tSTRING.%u (system ignored) = \"%s\"\n", dwStringID, (LPCSTR)CT2A( (LPCTSTR)Escape( sText ) ) );
									}
									else
										printf( "\tSTRING.%u (ignored)\n", dwStringID );

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

							if ( *pw != 0xffff && IsGood( (LPCWSTR)pw ) )
							{
								printf( "\tDIALOGCAPTION.%u = \"%s\"\n", uID, (LPCSTR)CT2A( (LPCTSTR)Escape( (LPCWSTR)pw ) ) );
								CString sID;
								sID.Format( _T("#: DIALOGCAPTION.%u\n"), uID );
								Add( pItems, sID, (LPCWSTR)pw );
							}
							else
								printf( "\tDIALOGCAPTION.%u (ignored)\n", uID );

							UINT i = 0;
							DLGITEMTEMPLATE* pItem = _DialogSplitHelper::FindFirstDlgItem( pTemplate );
							WORD nItems = _DialogSplitHelper::DlgTemplateItemCount( pTemplate );
							for ( WORD nItem = 0; nItem < nItems; ++nItem )
							{
								UINT wID = bDialogEx ? (WORD)((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->id : pItem->id;

								if ( bDialogEx )
									pw = (WORD*)((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem + 1);
								else
									pw = (WORD*)(pItem + 1);

								BOOL bUse = FALSE;
								LPCWSTR szClass = L"?";
								if ( *pw == 0xFFFF )			// Skip class name ordinal or string
								{
									switch ( *++pw )
									{
									case 0x0080:
										szClass = L"Button"; bUse = TRUE; break;
									case 0x0081:
										szClass = L"Edit"; break;
									case 0x0082:
										szClass = L"Static"; bUse = TRUE; break;
									case 0x0083:
										szClass = L"ListBox"; break;
									case 0x0084:
										szClass = L"ScrollBar"; break;
									case 0x0085:
										szClass = L"ComboBox"; break;
									}
									++pw;
								}
								else
								{
									szClass = (LPCWSTR)pw;
									while (*pw++);

									bUse =	( _tcsicmp( szClass, _T("SysLink") ) == 0 ) ||
											( _tcsicmp( szClass, _T("MfcLink") ) == 0 );
								}

								if ( wID == 0xffff )
									wID = ORDINAL | i++;

								if ( bUse && *pw != 0xffff && IsGood( (LPCWSTR)pw ) )
								{
									printf( "\tDIALOGCONTROL.%u.%s.%u = \"%s\"\n",
										uID, (LPCSTR)CT2A( szClass ), wID, (LPCSTR)CT2A( (LPCTSTR)Escape( (LPCWSTR)pw ) ) );
									CString sID;
									sID.Format( _T("#: DIALOGCONTROL.%u.%s.%u\n"), uID, szClass, wID );
									Add( pItems, sID, (LPCWSTR)pw );
								}
								else
									printf( "\tDIALOGCONTROL.%u.%s.%u (ignored)\n", uID, (LPCSTR)CT2A( szClass ), wID );

								pItem = _DialogSplitHelper::FindNextDlgItem( pItem, bDialogEx );
							}
						}
						else if ( lpszType == RT_DLGINIT )
						{
							const char* p = (const char*)hData;
							UINT nOrdinal = 0, nOldIDC = 0;
							for ( ; ; )
							{
								const _DialogSplitHelper::DLGINITSTRUCT& pItem = *(const _DialogSplitHelper::DLGINITSTRUCT*)p;
								if ( ! pItem.nIDC )
									break;
								LPCSTR szData = p + sizeof( _DialogSplitHelper::DLGINITSTRUCT );

								if ( nOldIDC == pItem.nIDC )
									++ nOrdinal;
								else
									nOrdinal = 0;
								nOldIDC = pItem.nIDC;

								const CString sText = (LPCTSTR)CA2T( CStringA( szData, pItem.dwSize ) );
								if ( pItem.message == 0x0403 && IsGood( sText ) )
								{
									printf( "\tDLGINIT.%u.%u.%u 0x%04x %2u bytes = \"%s\"\n",
										uID, pItem.nIDC, nOrdinal, pItem.message, pItem.dwSize, (LPCSTR)CT2A( (LPCTSTR)Escape( sText ) ) );
									CString sID;
									sID.Format( _T( "#: DLGINIT.%u.%u.%u\n" ), uID, pItem.nIDC, nOrdinal );
									Add( pItems, sID, sText );
								}
								else
									printf( "\tDLGINIT.%u.%u 0x%04x (ignored)\n", uID, pItem.nIDC, pItem.message );

								p += pItem.dwSize + sizeof( _DialogSplitHelper::DLGINITSTRUCT );
							}
						}
					}
					FreeResource( hResData );
				}
			}
		}
	}

	return TRUE;
}

BOOL CALLBACK EnumResTypeProc(HMODULE hModule, LPTSTR lpszType, LONG_PTR lParam)
{
	if ( IS_INTRESOURCE( lpszType ) )
	{
		if ( LPCTSTR szType = GetResType( lpszType ) )
			printf( "\nResource type: %s\n", (LPCSTR)CT2A( szType ) );
		else
			printf( "\nResource type: %u\n", (UINT)(ULONG_PTR)lpszType );
	}
	else
	{
		if ( lpszType[ 0 ] == _T('#') )
			printf( "\nResource type: #%d\n", _tstol( &lpszType[ 1 ] ) );
		else
			printf( "\nResource type: \"%s\"\n", (LPCSTR)CT2A( lpszType ) );
	}

	return EnumResourceNames( hModule, lpszType, EnumResNameProc, lParam );
}

int _tmain(int argc, _TCHAR* argv[])
{
	SetConsoleOutputCP( GetACP() );

	if ( argc < 2 )
	{
		printf( "Usage: %s {input file.exe} {output file.pot}\n", (LPCSTR)CT2A( PathFindFileName( argv[ 0 ] ) ) );
		return 1;
	}

	LPCTSTR szSource = argv[ 1 ];
 	LPCTSTR szDestination;
	CString sDest;
	if ( argc < 3 )
	{
		sDest = szSource;
		sDest += _T(".xxxx.pot");
		szDestination = sDest;
	}
	else
		szDestination = argv[ 2 ];

	HMODULE hExe = LoadLibraryEx( szSource, NULL, LOAD_LIBRARY_AS_DATAFILE );
	if ( ! hExe )
	{
		printf( "Can't load input file: %s\n", (LPCSTR)CT2A( szSource ) );
		return 1;
	}

	FILE* hFile = NULL;
	if ( _tfopen_s( &hFile, szDestination, _T("wb,ccs=UTF-8") ) == 0 && hFile )
	{
		printf( "File: %s\n", (LPCSTR)CT2A( szSource ) );

		fprintf( hFile,
			"msgid \"\"\n"
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

		CItems pItems;
		if ( EnumResourceTypes( hExe, EnumResTypeProc, (LONG_PTR)&pItems ) )
		{
			// OK
		}

		OutputAll( pItems, hFile );

		Clear( pItems );

		fclose( hFile );
	}
	else
	{
		printf( "Can't create output file: %s\n", (LPCSTR)CT2A( szDestination ) );
		FreeLibrary( hExe );
		return 1;
	}

	FreeLibrary( hExe );

	return 0;
}
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

#include <atlstr.h>			// for CString
#include <atlcoll.h>		// for CAtlMap, CAtlList

using namespace ATL;

// Class to load language .po-files (Portable Object) directly and
// to translate strings, menus and dialogs according selected language.

class ATL_NO_VTABLE CLocalization
{
public:
	CLocalization(LANGID nDefault = LANG_ENGLISH); // Set default/native language
	~CLocalization();

	// Load all available translations for specified module using two methods:
	//
	// 1) Loading from external files. Files must be named as "szModule[XX]XX.po"
	// where XXXX is a 2 or 4 hexadecimal digits of language ID for example
	// 0409 or 09 for English and 0419 or 19 for Russian.
	//
	// 2) Loading from resources. Resource ID treated as language ID, resource type: "PO".
	//
	// szModule can be module path or folder path (i.e. ends by "\"), or NULL then
	// current module name will be used.
	BOOL Load(LPCTSTR szModule = NULL);

	// Select language. Use LANG_NEUTRAL for language auto-detection.
	BOOL Select(LANGID nLangID = LANG_NEUTRAL);

	// Reload translation
	void Reload();

	// Load translated string
	CString LoadString(UINT nID) const;

	// Translate entire menu
	BOOL Translate(HMENU hMenu, UINT nMenuID, UINT* pnOrdinal = NULL) const;

	// Translate entire dialog box
	void Translate(HWND hDialog, UINT nDialogID) const;

	// Fill combo box with loaded languages and select current one
	// (combo box item data field will be set to corresponding LANGID)
	void FillComboBox(HWND hwndCombo) const;

	// Select language by combo box
	// (assuming that it was filled by FillComboBox method earlier)
	BOOL Select(HWND hwndCombo);

	// Empty any loaded translation and select default language
	void Empty();

	// Load .po-translation from language file (including compressed one)
	BOOL LoadPoFromFile(LANGID nLangID, LPCTSTR szFilename);

	// Load .po-translation from resource (including compressed one)
	BOOL LoadPoFromResource(LANGID nLangID, LPCTSTR szFilename);

	// Load .po-translation from compressed language file
	// (Compressed by old Microsoft (R) File Compression Utility Version 2.50)
	BOOL LoadPoFromArchive(LANGID nLangID, LPCTSTR szFilename);

	// Load .po-translation from string
	BOOL LoadPoFromString(LANGID nLangID, const CStringA& sContent);

	// Get selected language
	inline LANGID GetLang() const
	{
		return m_nLangID;
	}

	// Get language enumerator of available translations
	inline POSITION GetStartPosition() const
	{
		return m_pLanguages.GetStartPosition();
	}

	// Get next language of available translations
	inline LANGID GetNext(POSITION& pos) const
	{
		return m_pLanguages.GetNextKey( pos );
	}

	// Get language name in form of string: "English name - Localized name"
	static CString GetLangName(LANGID nLangID);

protected:
	CString							m_strModule;		// Path used to load language files
	CAtlMap< LANGID, CString >		m_pLanguages;		// List of available language files
	LANGID							m_nDefaultLangID;	// Default language
	LANGID							m_nLangID;			// Selected language
	CAtlMap< UINT, CString >		m_pStrings;			// Loaded strings
	CAtlMap< UINT, CString >		m_pMenus;			// Loaded menus
	CAtlMap< ULONGLONG, CString >	m_pDialogs;			// Loaded dialogs

	BOOL Add(const CAtlList< CString >& lRef, const CString& sTranslated);

	static BOOL CALLBACK EnumResNameProc(HMODULE hModule, LPCTSTR lpszType, LPTSTR lpszName, LONG_PTR lParam);
};

#define RT_PO	_T("PO")	// Resource type used to store .po-translations inside modules
//#define RT_MO	_T("MO")	// Resource type used to store .mo-translations inside modules

/*
#pragma pack( push, 1 )

// .mo-file header
typedef struct
{
	DWORD magic;			// magic number = 0x950412de
	DWORD revision;			// file format revision
	DWORD number;			// number of strings
	DWORD offset_orig;		// offset of table with original strings
	DWORD offset_trans;		// offset of table with translation strings
	DWORD hash_size;		// size of hashing table
	DWORD hash_offset;		// offset of hashing table
}
MO_HEADER;

#pragma pack( pop )
*/
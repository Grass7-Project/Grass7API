#include "stdafx.h"
#include "Gr7API.h"

// Function to create a rich edit control
HWND Grass7API::RichEditControl::CreateRichEdit(HWND &hwndOwner, int x, int y, int width, int height, HINSTANCE hinst)
{
	LoadLibrary(L"Msftedit.dll");

	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_USEREX_CLASSES;
	InitCommonControlsEx(&icex);

	HWND hwndEdit = CreateWindowExW(WS_EX_CLIENTEDGE, MSFTEDIT_CLASS, L"",
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_HSCROLL | WS_VSCROLL,
		x, y, width, height,
		hwndOwner, NULL, hinst, NULL);

	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwEffects = 0;
	cf.dwMask = CFM_FACE | CFE_BOLD;
	SendMessageW(hwndEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

	return hwndEdit;
}

DWORD CALLBACK Grass7API::RichEditControl::EditStreamCallback(DWORD_PTR dwCookie, LPBYTE lpBuff, LONG cb, PLONG pcb)
{
	HANDLE hFile = (HANDLE)dwCookie;
	return !ReadFile(hFile, lpBuff, cb, (DWORD *)pcb, NULL);
}

// Reads the file and puts the contents into the rich edit control
BOOL Grass7API::RichEditControl::FillRichEditFromFile(HWND &hwnd, LPCTSTR pszFile, WPARAM Type)
{
	BOOL fSuccess = FALSE;
	HANDLE hFile = CreateFile(pszFile, GENERIC_READ, FILE_SHARE_READ,
		0, OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		EDITSTREAM es = { (DWORD_PTR)hFile, 0, Grass7API::RichEditControl::EditStreamCallback };
		if (SendMessageW(hwnd, EM_STREAMIN, Type, (LPARAM)&es) && es.dwError == 0)
		{
			fSuccess = TRUE;
		}
		CloseHandle(hFile);
	}
	return fSuccess;
}
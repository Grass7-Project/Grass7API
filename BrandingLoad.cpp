#include "stdafx.h"
#include "Gr7Api.h"

void Grass7API::Branding::LoadOSBrandingString(PWSTR &osbranding, wchar_t *SpecifierWstr)
{
	HMODULE hMod = LoadLibraryExW(L"winbrand.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hMod) {
		PWSTR(WINAPI* pfnBrandingFormatString)(PCWSTR pstrFormat);

		(FARPROC&)pfnBrandingFormatString = GetProcAddress(hMod, "BrandingFormatString");

		if (pfnBrandingFormatString) {
			osbranding = pfnBrandingFormatString(SpecifierWstr);
		}
		FreeLibrary(hMod);
	}
}

void Grass7API::Branding::LoadOSBrandingImage(HBITMAP &hBitmap)
{
	std::wstring windirW(MAX_PATH, 0);
	UINT errWinDir = GetWindowsDirectoryW(&windirW[0], (int)windirW.size());
	if (errWinDir == 0) {
		MessageBoxW(NULL, L"GetWindowsDirectoryW returned 0", L"Error", MB_OK | MB_ICONQUESTION);
		hBitmap = NULL;
	}
	std::wstring path = windirW.c_str();
	path.append(L"\\Branding\\Basebrd\\basebrd.dll");
	HMODULE hMod = LoadLibraryExW(path.c_str(), NULL, LOAD_LIBRARY_AS_DATAFILE);
	if (hMod) {
		hBitmap = static_cast<HBITMAP>(LoadImageW(hMod, MAKEINTRESOURCE(121), IMAGE_BITMAP, 0, 0, 0));
		FreeLibrary(hMod);
	}
}
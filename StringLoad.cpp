#include "stdafx.h"
#include "Gr7Api.h"

wchar_t * gr7::LoadStringToW(HINSTANCE hInstance, UINT uID)
{
	// Function to load a resource std::string to a std::wstring
	char stringbuffer[256];
	size_t convertedChars = 0;
	LoadStringA(hInstance, uID, stringbuffer, 256);
	const size_t cSize = strlen(stringbuffer) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs_s(&convertedChars, wc, cSize, stringbuffer, _TRUNCATE);
	memset(stringbuffer, 0, sizeof(stringbuffer));
	return wc;
}

wchar_t * gr7::LoadOSBrandingString()
{
	// Function to load the branding of the OS from basebrd
	PWSTR osbranding = L"";
	HMODULE hMod = LoadLibraryExW(L"winbrand.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hMod)
	{
		PWSTR(WINAPI* pfnBrandingFormatString)(PCWSTR pstrFormat);

		(FARPROC&)pfnBrandingFormatString = GetProcAddress(hMod, "BrandingFormatString");

		if (pfnBrandingFormatString)
		{
			osbranding = pfnBrandingFormatString(L"%WINDOWS_GENERIC%");
		}
		FreeLibrary(hMod);
	}
	return osbranding;
}
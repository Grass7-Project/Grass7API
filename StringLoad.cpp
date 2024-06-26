#include "stdafx.h"
#include "Gr7Api.h"

// Function to load the branding of the OS from basebrd
void Grass7API::String::LoadOSBrandingString(PWSTR &osbranding, wchar_t *SpecifierWstr)
{
	HMODULE hMod = LoadLibraryExW(L"winbrand.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hMod)
	{
		PWSTR(WINAPI* pfnBrandingFormatString)(PCWSTR pstrFormat);

		(FARPROC&)pfnBrandingFormatString = GetProcAddress(hMod, "BrandingFormatString");

		if (pfnBrandingFormatString)
		{
			osbranding = pfnBrandingFormatString(SpecifierWstr);
		}
		FreeLibrary(hMod);
	}
}
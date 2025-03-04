#include "stdafx.h"
#include "Gr7Api.h"

void Grass7API::DWM::DwmIsCompositionEnabled(BOOL &dwmEnabled)
{
	dwmEnabled = 0;
	HMODULE hMod = LoadLibraryExW(L"dwmapi.dll", nullptr, NULL);
	if (hMod)
	{
		HRESULT(WINAPI* pfnDwmIsCompositionEnabled)(BOOL *pfEnabled);

		(FARPROC&)pfnDwmIsCompositionEnabled = GetProcAddress(hMod, "DwmIsCompositionEnabled");

		if (pfnDwmIsCompositionEnabled)
		{
			pfnDwmIsCompositionEnabled(&dwmEnabled);
		}
		FreeLibrary(hMod);
	}
}
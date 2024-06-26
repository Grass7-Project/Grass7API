#include "stdafx.h"
#include "Gr7Api.h"

void Grass7API::Version::GetBuildString(wchar_t *Data, DWORD BufferSize)
{
	RegGetValueW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Grass7\\CurrentVersion", L"BuildString", RRF_RT_ANY, NULL, (PVOID)Data, &BufferSize);
}

void Grass7API::Version::GetCurrentBuild(wchar_t *Data, DWORD BufferSize)
{
	RegGetValueW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Grass7\\CurrentVersion", L"CurrentBuild", RRF_RT_ANY, NULL, (PVOID)Data, &BufferSize);
}

void Grass7API::Version::GetPatchLevel(wchar_t *Data, DWORD BufferSize)
{
	RegGetValueW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Grass7\\CurrentVersion", L"PatchLevel", RRF_RT_ANY, NULL, (PVOID)Data, &BufferSize);
}

void Grass7API::Version::GetUpdateLevel(wchar_t *Data, DWORD BufferSize)
{
	RegGetValueW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Grass7\\CurrentVersion", L"UpdateLevel", RRF_RT_ANY, NULL, (PVOID)Data, &BufferSize);
}
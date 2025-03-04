#include "stdafx.h"
#include "Gr7Api.h"

void Grass7API::SystemInfo::GetBuildString(wchar_t *Data, DWORD BufferSize)
{
	RegGetValueW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Grass7\\CurrentVersion", L"BuildString", RRF_RT_ANY, nullptr, (PVOID)Data, &BufferSize);
}

void Grass7API::SystemInfo::GetCurrentBuild(wchar_t *Data, DWORD BufferSize)
{
	RegGetValueW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Grass7\\CurrentVersion", L"CurrentBuild", RRF_RT_ANY, nullptr, (PVOID)Data, &BufferSize);
}

void Grass7API::SystemInfo::GetPatchLevel(wchar_t *Data, DWORD BufferSize)
{
	RegGetValueW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Grass7\\CurrentVersion", L"PatchLevel", RRF_RT_ANY, nullptr, (PVOID)Data, &BufferSize);
}

void Grass7API::SystemInfo::GetUpdateLevel(wchar_t *Data, DWORD BufferSize)
{
	RegGetValueW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Grass7\\CurrentVersion", L"UpdateLevel", RRF_RT_ANY, nullptr, (PVOID)Data, &BufferSize);
}

void Grass7API::SystemInfo::GetRegisteredOwner(wchar_t *Data, DWORD BufferSize)
{
	RegGetValueW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"RegisteredOwner", RRF_RT_ANY, nullptr, (PVOID)Data, &BufferSize);
}

void Grass7API::SystemInfo::GetRegisteredOrganization(wchar_t *Data, DWORD BufferSize)
{
	RegGetValueW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"RegisteredOrganization", RRF_RT_ANY, nullptr, (PVOID)Data, &BufferSize);
}
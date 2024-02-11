#include "stdafx.h"
#include "Gr7Api.h"
#include <io.h>
#include <string.h>
#include <Shobjidl.h>
#include <shellapi.h>
#include <strsafe.h>
#include <wchar.h>

// Function to get the drive letter where the OS is installed to
int gr7::GetSystemDriveLetter(wchar_t *driveletter)
{
	TCHAR windir[MAX_PATH];
	UINT errWinDir = GetWindowsDirectoryW(windir, MAX_PATH);
	if (errWinDir == 0) {
		MessageBoxW(NULL, L"GetWindowsDirectoryW returned 0", L"Error", MB_OK | MB_ICONQUESTION);
		return 1;
	}
	std::wstring windirWSTR = windir;
	std::wstring driveletter1 = windirWSTR.substr(0, windirWSTR.find(L":"));
	driveletter1.append(L":\\");
	const wchar_t *drivelettertext = driveletter1.c_str();

	wcsncpy_s(driveletter, sizeof(driveletter), drivelettertext, sizeof(drivelettertext));
	return 0;
}

// Function to check if file exists
int gr7::fileExistsA(LPCSTR file)
{
	WIN32_FIND_DATAA FindFileData;
	HANDLE handle = FindFirstFileA(file, &FindFileData);
	int found = handle != INVALID_HANDLE_VALUE;
	if (found)
	{
		FindClose(handle);
	}
	return found;
}

int gr7::fileExistsW(LPCWSTR file)
{
	WIN32_FIND_DATAW FindFileData;
	HANDLE handle = FindFirstFileW(file, &FindFileData);
	int found = handle != INVALID_HANDLE_VALUE;
	if (found)
	{
		FindClose(handle);
	}
	return found;
}

// Function to check if directory exists
int gr7::dirExists(LPCWSTR pathname)
{
	struct _stat64i32 info;
	if (_wstat(pathname, &info) != 0)
		return 0;
	else if (info.st_mode & S_IFDIR)
		return 1;
	else
		return 0;
}

// Function to delete directory recursively
LONG gr7::DeleteDirectory(LPCWSTR sPath)
{
	WCHAR szDir[MAX_PATH + 1];  // +1 for the double null terminate
	SHFILEOPSTRUCTW fos = { 0 };

	StringCchCopyW(szDir, MAX_PATH, sPath);
	int len = lstrlenW(szDir);
	szDir[len + 1] = 0; // double null terminate for SHFileOperation

						// delete the folder and everything inside
	fos.wFunc = FO_DELETE;
	fos.pFrom = szDir;
	fos.fFlags = FOF_NO_UI;
	return SHFileOperationW(&fos);
}
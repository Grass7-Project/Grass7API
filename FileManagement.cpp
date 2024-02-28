#include "stdafx.h"
#include "Gr7Api.h"
#include <io.h>
#include <string.h>
#include <Shobjidl.h>
#include <shellapi.h>
#include <strsafe.h>
#include <wchar.h>
#include <Shlwapi.h>
#include <iostream>

// Function to get the drive letter where the OS is installed to
int Grass7API::FileManagement::GetSystemDriveLetter(wchar_t *driveletter)
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
int Grass7API::FileManagement::fileExistsA(LPCSTR file)
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

int Grass7API::FileManagement::fileExistsW(LPCWSTR file)
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
int Grass7API::FileManagement::dirExists(LPCWSTR pathname)
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
LONG Grass7API::FileManagement::DeleteDirectory(LPCWSTR sPath)
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

BOOL Grass7API::FileManagement::FindFile(LPCWSTR path, LPCWSTR fileName, LPWSTR fullPath)
{
	WIN32_FIND_DATAW fd = {};
	DWORD dwError;

	WCHAR str[MAX_PATH] = {};
	PathCombineW(str, path, L"*");

	HANDLE hFind = FindFirstFileW(str, &fd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		dwError = GetLastError();
		if (dwError != ERROR_FILE_NOT_FOUND)
			std::cout << "ERROR " << dwError;
		return false;
	}

	do
	{
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if ((lstrcmpW(fd.cFileName, L".") != 0) &&
				(lstrcmpW(fd.cFileName, L"..") != 0))
			{
				PathCombineW(str, path, fd.cFileName);
				if (Grass7API::FileManagement::FindFile(str, fileName, fullPath))
				{
					CloseHandle(hFind);
					return true;
				}
			}
		}
		else
		{
			if ((lstrcmpiW(fd.cFileName, fileName) == 0) ||
				(lstrcmpiW(fd.cAlternateFileName, fileName) == 0))
			{
				if (fullPath)
					PathCombineW(fullPath, path, fd.cFileName);

				CloseHandle(hFind);
				return true;
			}
		}
	} while (FindNextFileW(hFind, &fd));

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
		std::cout << "ERROR " << dwError;

	CloseHandle(hFind);
	return false;
}
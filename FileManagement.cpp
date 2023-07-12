#include "stdafx.h"
#include "Gr7Api.h"
#include <io.h>
#include <string.h>


struct stat info;

const char * gr7::GetSystemDriveLetter()
{
	// Better implementation for global use, since this is in the OS and not winRE as before.
	const char *driveletter = "";
	char bg1[256];
	char bg2[256];
	TCHAR windir[MAX_PATH];
	CHAR path[MAX_PATH];
	GetWindowsDirectory(windir, MAX_PATH);
	wcstombs_s(NULL, path, windir, wcslen(windir) + 1);
	std::string path1 = path;
	std::string letter1 = path1.substr(0, path1.find(":"));
	const char *letter(letter1.c_str());
	strncpy_s(bg1, letter, sizeof(bg1));
	strncat_s(bg1, ":\\", sizeof(bg1));
	strncpy_s(bg2, bg1, sizeof(bg2));
	strncat_s(bg2, "Windows\\System32\\identifier", sizeof(bg2));
	std::fstream fileStream;
	fileStream.open(bg2);
	if (fileStream.fail()) {
	}
	else {
		driveletter = bg1;
	}
	fileStream.close();

	return driveletter;
}

int gr7::dirExists(const char *pathname)
{
	if (stat(pathname, &info) != 0)
		return 0;
	else if (info.st_mode & S_IFDIR)
		return 1;
	else
		return 0;
}

BOOL IsDots(const TCHAR* str) {
	if (wcscmp(str, _T(".")) && wcscmp(str, _T(".."))) return FALSE;
	return TRUE;
}

BOOL gr7::DeleteDirectory(const TCHAR* sPath) {
	// Better implementation of MS's own function to delete directories
	// This one is recursive
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;

	TCHAR DirPath[MAX_PATH];
	TCHAR FileName[MAX_PATH];

	wcscpy_s(DirPath, MAX_PATH, sPath);
	wcscat_s(DirPath, MAX_PATH, _T("\\*"));
	wcscpy_s(FileName, MAX_PATH, sPath);
	wcscat_s(FileName, MAX_PATH, _T("\\"));

	hFind = FindFirstFile(DirPath, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) return FALSE;
	wcscpy_s(DirPath, MAX_PATH, FileName);

	bool bSearch = true;
	while (bSearch) {
		if (FindNextFile(hFind, &FindFileData)) {
			if (IsDots(FindFileData.cFileName)) continue;
			wcscat_s(FileName, MAX_PATH, FindFileData.cFileName);
			if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				if (!DeleteDirectory(FileName)) {
					FindClose(hFind);
					return FALSE;
				}
				RemoveDirectory(FileName);
				wcscpy_s(FileName, MAX_PATH, DirPath);
			}
			else {
				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
					_chmod((const char*)FileName, _S_IWRITE);
				if (!DeleteFile(FileName)) {
					FindClose(hFind);
					return FALSE;
				}
				wcscpy_s(FileName, MAX_PATH, DirPath);
			}
		}
		else {
			if (GetLastError() == ERROR_NO_MORE_FILES)
				bSearch = false;
			else {
				FindClose(hFind);
				return FALSE;
			}

		}

	}
	FindClose(hFind);
	return RemoveDirectory(sPath);
}
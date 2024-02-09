#include "stdafx.h"
#include "Gr7Api.h"
#include <io.h>
#include <string.h>
#include <Shobjidl.h>
#include <shellapi.h>
#include <strsafe.h>

struct stat info;

const char * gr7::GetSystemDriveLetter()
{
	// Better implementation for global use, since this is in the OS and not winRE as before.
	const char *driveletter = "";
	char bg1[256] = { 0 };
	char bg2[256] = { 0 };
	TCHAR windir[MAX_PATH];
	CHAR path[MAX_PATH];
	UINT errWinDir = GetWindowsDirectoryW(windir, MAX_PATH);
	if (errWinDir == 0) {
		MessageBoxW(NULL, L"GetWindowsDirectoryW returned 0", L"Error", MB_OK | MB_ICONQUESTION);
	}
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

int gr7::fileExists(TCHAR * file)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE handle = FindFirstFile(file, &FindFileData);
	int found = handle != INVALID_HANDLE_VALUE;
	if (found)
	{
		FindClose(handle);
	}
	return found;
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

LONG gr7::DeleteDirectory(const TCHAR* sPath)
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

// Backup Implementation incase gr7::DeleteDirectory does not work.
// Note: Appears to not work with large directories with alot of files.
BOOL gr7::DeleteDirectory2(const TCHAR* sPath)
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr)) {
		//Couldn't initialize COM library - clean up and return
		CoUninitialize();
		return FALSE;
	}
	//Initialize the file operation
	IFileOperation* fileOperation;
	hr = CoCreateInstance(CLSID_FileOperation, NULL, CLSCTX_ALL, IID_PPV_ARGS(&fileOperation));
	if (FAILED(hr)) {
		//Couldn't CoCreateInstance - clean up and return
		CoUninitialize();
		return FALSE;
	}
	hr = fileOperation->SetOperationFlags(FOF_NOCONFIRMATION | FOF_SILENT | FOF_NOERRORUI);
	if (FAILED(hr)) {
		//Couldn't add flags - clean up and return
		fileOperation->Release();
		CoUninitialize();
		return FALSE;
	}
	IShellItem* fileOrFolderItem = NULL;
	hr = SHCreateItemFromParsingName(sPath, NULL, IID_PPV_ARGS(&fileOrFolderItem));
	if (FAILED(hr)) {
		//Couldn't get file into an item - clean up and return (maybe the file doesn't exist?)
		fileOrFolderItem->Release();
		fileOperation->Release();
		CoUninitialize();
		return FALSE;
	}
	hr = fileOperation->DeleteItem(fileOrFolderItem, NULL);
	fileOrFolderItem->Release();
	if (FAILED(hr)) {
		//Failed to mark file/folder item for deletion - clean up and return
		fileOperation->Release();
		CoUninitialize();
		return FALSE;
	}
	hr = fileOperation->PerformOperations();
	fileOperation->Release();
	CoUninitialize();
	if (FAILED(hr)) {
		//failed to carry out delete - return
		return FALSE;
	}
	return TRUE;
}
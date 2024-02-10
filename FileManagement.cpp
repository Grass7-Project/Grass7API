#include "stdafx.h"
#include "Gr7Api.h"
#include <io.h>
#include <string.h>
#include <Shobjidl.h>
#include <shellapi.h>
#include <strsafe.h>

// Function to get the OS drive
// Better implementation for global use, since this is in the OS and not winRE as before.
int gr7::GetSystemDriveLetterW(wchar_t *driveletter)
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

// Function to check if directory exists
int gr7::dirExists(const char *pathname)
{
	struct stat info;
	if (stat(pathname, &info) != 0)
		return 0;
	else if (info.st_mode & S_IFDIR)
		return 1;
	else
		return 0;
}

// Function to delete directory recursively
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
BOOL gr7::DeleteDirectoryAlternative(const TCHAR* sPath)
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
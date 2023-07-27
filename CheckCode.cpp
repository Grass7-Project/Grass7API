#include "stdafx.h"
#include "Gr7API.h"

BOOL gr7::isGrass7()
{
	// Function to check if the running system is actually Grass7, for compatibility reasons.
	TCHAR windir[MAX_PATH];
	CHAR identfilepath[MAX_PATH];
	std::string myText;

	UINT errWinDir = GetWindowsDirectoryW(windir, MAX_PATH);
	if (errWinDir == 0) {
		MessageBoxW(NULL, L"GetWindowsDirectoryW returned 0", L"Error", MB_OK | MB_ICONQUESTION);
		return 1;
	}
	const char *identfile = ("\\system32\\identifier");
	wcstombs_s(NULL, identfilepath, windir, wcslen(windir) + 1);
	strcat_s(identfilepath, identfile);
	std::ifstream indentifier(identfilepath);
	getline(indentifier, myText);
	if (myText.find("gr7") != std::string::npos) {
		return 0;
	}
	return 1;
}

BOOL gr7::isProcessElevated(HANDLE process)
{
	// Function to check if the specified process is runnning as Administrator
	BOOL fIsElevated = FALSE;
	HANDLE hToken = NULL;
	TOKEN_ELEVATION elevation;
	DWORD dwSize;

	if (!OpenProcessToken(process, TOKEN_QUERY, &hToken))
	{
		goto Cleanup;
	}


	if (!GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize))
	{
		goto Cleanup;
	}

	fIsElevated = elevation.TokenIsElevated;

Cleanup:
	if (hToken)
	{
		CloseHandle(hToken);
		hToken = NULL;
	}
	return fIsElevated;
}
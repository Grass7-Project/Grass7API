#include "stdafx.h"
#include "Gr7API.h"
#include <wincrypt.h>
#include <wintrust.h>
#include <Softpub.h>

// Function to check if the running system is actually Grass7, for compatibility reasons.
BOOL Grass7API::Check::isGrass7()
{
	std::wstring identfilepath;
	std::wstring myText;
	std::wstring windir(MAX_PATH, 0);
	UINT errWinDir = GetWindowsDirectoryW(&windir[0], (int)windir.size());
	if (errWinDir == 0) {
		MessageBoxW(nullptr, L"GetWindowsDirectoryW returned 0", L"Error", MB_OK | MB_ICONQUESTION);
		return 1;
	}
	identfilepath.append(windir.c_str());
	identfilepath.append(L"\\system32\\identifier");
	std::wifstream indentifier(identfilepath);
	getline(indentifier, myText);
	if (myText.find(L"gr7") != std::wstring::npos) {
		return 0;
	}
	return 1;
}

// Function to check if the specified process is runnning as Administrator
BOOL Grass7API::Check::isProcessElevated(HANDLE process)
{
	BOOL fIsElevated = FALSE;
	HANDLE hToken = nullptr;
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
		hToken = nullptr;
	}
	return fIsElevated;
}

// Function to verify signature of file
BOOL Grass7API::Check::VerifyEmbeddedSignature(LPCWSTR pwszSourceFile)
{
	LONG lStatus;
	DWORD dwLastError;

	WINTRUST_FILE_INFO FileData;
	memset(&FileData, 0, sizeof(FileData));
	FileData.cbStruct = sizeof(WINTRUST_FILE_INFO);
	FileData.pcwszFilePath = pwszSourceFile;
	FileData.hFile = nullptr;
	FileData.pgKnownSubject = nullptr;

	GUID WVTPolicyGUID = WINTRUST_ACTION_GENERIC_VERIFY_V2;
	WINTRUST_DATA WinTrustData;

	// Initialize the WinVerifyTrust input data structure.

	// Default all fields to 0.
	memset(&WinTrustData, 0, sizeof(WinTrustData));

	WinTrustData.cbStruct = sizeof(WinTrustData);

	// Use default code signing EKU.
	WinTrustData.pPolicyCallbackData = nullptr;

	// No data to pass to SIP.
	WinTrustData.pSIPClientData = nullptr;

	// Disable WVT UI.
	WinTrustData.dwUIChoice = WTD_UI_NONE;

	// No revocation checking.
	WinTrustData.fdwRevocationChecks = WTD_REVOKE_NONE;

	// Verify an embedded signature on a file.
	WinTrustData.dwUnionChoice = WTD_CHOICE_FILE;

	// Verify action.
	WinTrustData.dwStateAction = WTD_STATEACTION_VERIFY;

	// Verification sets this value.
	WinTrustData.hWVTStateData = nullptr;

	// Not used.
	WinTrustData.pwszURLReference = nullptr;

	WinTrustData.dwUIContext = 0;

	// Set pFile.
	WinTrustData.pFile = &FileData;

	// WinVerifyTrust verifies signatures as specified by the GUID 
	// and Wintrust_Data.
	lStatus = WinVerifyTrust(
		nullptr,
		&WVTPolicyGUID,
		&WinTrustData);

	switch (lStatus)
	{
	case ERROR_SUCCESS:
		wprintf_s(L"The file \"%s\" is signed and the signature "
			L"was verified.\n",
			pwszSourceFile);
		break;

	case TRUST_E_NOSIGNATURE:
		// The file was not signed or had a signature 
		// that was not valid.

		// Get the reason for no signature.
		dwLastError = GetLastError();
		if (TRUST_E_NOSIGNATURE == dwLastError ||
			TRUST_E_SUBJECT_FORM_UNKNOWN == dwLastError ||
			TRUST_E_PROVIDER_UNKNOWN == dwLastError)
		{
			// The file was not signed.
			wprintf_s(L"The file \"%s\" is not signed.\n",
				pwszSourceFile);
		}
		else
		{
			// The signature was not valid or there was an error 
			// opening the file.
			wprintf_s(L"An unknown error occurred trying to "
				L"verify the signature of the \"%s\" file.\n",
				pwszSourceFile);
		}

		break;

	case TRUST_E_EXPLICIT_DISTRUST:
		// The hash that represents the subject or the publisher 
		// is not allowed by the admin or user.
		wprintf_s(L"The signature is present, but specifically "
			L"disallowed.\n");
		break;

	case TRUST_E_SUBJECT_NOT_TRUSTED:
		// The user clicked "No" when asked to install and run.
		wprintf_s(L"The signature is present, but not "
			L"trusted.\n");
		break;

	case CRYPT_E_SECURITY_SETTINGS:
		/*
		The hash that represents the subject or the publisher
		was not explicitly trusted by the admin and the
		admin policy has disabled user trust. No signature,
		publisher or time stamp errors.
		*/
		wprintf_s(L"CRYPT_E_SECURITY_SETTINGS - The hash "
			L"representing the subject or the publisher wasn't "
			L"explicitly trusted by the admin and admin policy "
			L"has disabled user trust. No signature, publisher "
			L"or timestamp errors.\n");
		break;

	default:
		// The UI was disabled in dwUIChoice or the admin policy 
		// has disabled user trust. lStatus contains the 
		// publisher or time stamp chain error.
		wprintf_s(L"Error is: 0x%x.\n",
			lStatus);
		break;
	}

	// Any hWVTStateData must be released by a call with close.
	WinTrustData.dwStateAction = WTD_STATEACTION_CLOSE;

	lStatus = WinVerifyTrust(
		nullptr,
		&WVTPolicyGUID,
		&WinTrustData);

	return true;
}
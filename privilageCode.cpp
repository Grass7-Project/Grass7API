#include "stdafx.h"
#include "Gr7Api.h"

// Function to change privalage of the specified process in function call
HRESULT gr7::ModifyPrivilege(IN LPCTSTR szPrivilege, IN BOOL fEnable, HANDLE process)
{
	HRESULT hr = S_OK;
	TOKEN_PRIVILEGES NewState;
	LUID             luid;
	HANDLE hToken = NULL;

	if (!OpenProcessToken(process,
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
		&hToken))
	{
		return ERROR_FUNCTION_FAILED;
	}

	if (!LookupPrivilegeValueW(NULL,
		szPrivilege,
		&luid))
	{
		CloseHandle(hToken);
		return ERROR_FUNCTION_FAILED;
	}

	NewState.PrivilegeCount = 1;
	NewState.Privileges[0].Luid = luid;
	NewState.Privileges[0].Attributes =
		(fEnable ? SE_PRIVILEGE_ENABLED : 0);

	if (!AdjustTokenPrivileges(hToken,
		FALSE,
		&NewState,
		0,
		NULL,
		NULL))
	{
		hr = ERROR_FUNCTION_FAILED;
	}

	CloseHandle(hToken);

	return hr;
}
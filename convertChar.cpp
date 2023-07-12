#include "stdafx.h"
#include "Gr7Api.h"

wchar_t * gr7::convertchar(const char *stringchar)
{
	// Apperently this function memory leaks but whatever diffrent implementation i do, it still says the same thing in the debugger.
	size_t newsize = strlen(stringchar) + 1;
	std::wstring wc(newsize, L'#');
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, &wc[0], newsize, stringchar, _TRUNCATE);
	return _wcsdup(&wc[0]);
}

LPCWSTR gr7::convertchartoLPCWSTR(const char *stringchar)
{
	// Function to convert a char to a LPCWSTR
	LPCWSTR ptr;
	wchar_t * wcharint = convertchar(stringchar);
	ptr = _wcsdup(wcharint);
	delete wcharint;
	return ptr;
}

std::string gr7::WStringToString(const std::wstring& wstr)
{
	// Function to convert a std::wstring to a std::string
	std::string str;
	size_t size;
	str.resize(wstr.length());
	wcstombs_s(&size, &str[0], str.size() + 1, wstr.c_str(), wstr.size());
	return str;
}
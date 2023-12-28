#pragma once

namespace gr7 {
	extern "C" __declspec(dllexport) wchar_t * convertchar(const char *stringchar);
	extern "C" __declspec(dllexport) LPCWSTR convertchartoLPCWSTR(const char *stringchar);
	__declspec(dllexport) std::string WStringToString(const std::wstring& wstr);
	extern "C" __declspec(dllexport) const char *GetSystemDriveLetter();
	extern "C" __declspec(dllexport) int dirExists(const char *pathname);
	extern "C" __declspec(dllexport) LONG DeleteDirectory(const TCHAR* sPath);
	extern "C" __declspec(dllexport) BOOL DeleteDirectory2(const TCHAR* sPath);
	extern "C" __declspec(dllexport) HRESULT ModifyPrivilege(IN LPCTSTR szPrivilege, IN BOOL fEnable, HANDLE process);
	extern "C" __declspec(dllexport) wchar_t *LoadStringToW(HINSTANCE hInstance, UINT uID);
	extern "C" __declspec(dllexport) wchar_t *LoadOSBrandingString();
	extern "C" __declspec(dllexport) BOOL isGrass7();
	extern "C" __declspec(dllexport) BOOL isProcessElevated(HANDLE process);
	extern "C" __declspec(dllexport) BOOL VerifyEmbeddedSignature(LPCWSTR pwszSourceFile);
	extern "C" __declspec(dllexport) HWND CreateRichEdit(HWND hwndOwner, int x, int y, int width, int height, HINSTANCE hinst);
	extern "C" __declspec(dllexport) DWORD CALLBACK EditStreamCallback(DWORD_PTR dwCookie, LPBYTE lpBuff, LONG cb, PLONG pcb);
	extern "C" __declspec(dllexport) BOOL FillRichEditFromFile(HWND hwnd, LPCTSTR pszFile, WPARAM Type);
}
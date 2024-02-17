#pragma once

typedef struct {
	int xPos;
	int yPos;
	LPCWSTR font;
	COLORREF color;
	LPCWSTR text;
	int nSize;
	int SizeMode;
	int BkMode;
	int cWeight;
} PaintTextOptions;

namespace gr7 {
	extern "C" __declspec(dllexport) wchar_t * convertchar(const char *stringchar);
	extern "C" __declspec(dllexport) LPCWSTR convertchartoLPCWSTR(const char *stringchar);
	__declspec(dllexport) std::string WStringToString(const std::wstring& wstr);
	extern "C" __declspec(dllexport) int GetSystemDriveLetter(wchar_t *driveletter);
	extern "C" __declspec(dllexport) int fileExistsA(LPCSTR file);
	extern "C" __declspec(dllexport) int fileExistsW(LPCWSTR file);
	extern "C" __declspec(dllexport) int dirExists(LPCWSTR pathname);
	extern "C" __declspec(dllexport) LONG DeleteDirectory(LPCWSTR sPath);
	extern "C" __declspec(dllexport) HRESULT ModifyPrivilege(IN LPCTSTR szPrivilege, IN BOOL fEnable, HANDLE process);
	extern "C" __declspec(dllexport) void LoadOSBrandingString(PWSTR &osbranding);
	extern "C" __declspec(dllexport) BOOL isGrass7();
	extern "C" __declspec(dllexport) BOOL isProcessElevated(HANDLE process);
	extern "C" __declspec(dllexport) BOOL VerifyEmbeddedSignature(LPCWSTR pwszSourceFile);
	extern "C" __declspec(dllexport) HWND CreateRichEdit(HWND &hwndOwner, int x, int y, int width, int height, HINSTANCE hinst);
	extern "C" __declspec(dllexport) DWORD CALLBACK EditStreamCallback(DWORD_PTR dwCookie, LPBYTE lpBuff, LONG cb, PLONG pcb);
	extern "C" __declspec(dllexport) BOOL FillRichEditFromFile(HWND &hwnd, LPCTSTR pszFile, WPARAM Type);
	extern "C" __declspec(dllexport) void PaintTransparentBitmap(HDC &hdc, int xPos, int yPos, HBITMAP hBitmap, BLENDFUNCTION bf);
	extern "C" __declspec(dllexport) BOOL PaintText(HDC &hdc, PaintTextOptions PaintTextOpt);
	extern "C" __declspec(dllexport) void GetDesktopResolution(int& horizontal, int& vertical);
}
#pragma once

class __declspec(dllexport) Grass7API {
public:
	class __declspec(dllexport) FileManagement {
	public:
		static int GetSystemDriveLetter(wchar_t *driveletter);
		static int fileExistsA(LPCSTR file);
		static int fileExistsW(LPCWSTR file);
		static int dirExists(LPCWSTR pathname);
		static LONG DeleteDirectory(LPCWSTR sPath);
		static BOOL FindFile(LPCWSTR path, LPCWSTR fileName, LPWSTR fullPath);
	};

	class __declspec(dllexport) Privilege {
	public:
		static HRESULT ModifyPrivilege(IN LPCTSTR szPrivilege, IN BOOL fEnable, HANDLE process);
	};

	class __declspec(dllexport) Paint {
	public:
		static void CreateHFONT(HFONT &hFont, LPCWSTR font, int nSize, int SizeMode, int cWeight);
		static BOOL PaintTransparentBitmap(HDC &hdc, int xPos, int yPos, HBITMAP &hBitmap, BLENDFUNCTION bf);
		static BOOL PaintText(HDC &hdc, int xPos, int yPos, LPCWSTR font, COLORREF color, LPCWSTR text, int nSize, int SizeMode, int BkMode, int cWeight, LPRECT hWndRect, UINT format = DT_LEFT | DT_EXTERNALLEADING | DT_WORDBREAK);
		static HBITMAP ReplaceColor(HBITMAP &hBmp, COLORREF cOldColor, COLORREF cNewColor, HDC &hBmpDC);
	};

	class __declspec(dllexport) Monitor {
	public:
		static void GetDesktopResolution(int& horizontal, int& vertical);
	};

	class __declspec(dllexport) RichEditControl {
	public:
		static HWND CreateRichEdit(HWND &hwndOwner, int x, int y, int width, int height, HINSTANCE hinst);
		static BOOL FillRichEditFromFile(HWND &hwnd, LPCTSTR pszFile, WPARAM Type);
	private:
		static DWORD CALLBACK EditStreamCallback(DWORD_PTR dwCookie, LPBYTE lpBuff, LONG cb, PLONG pcb);
	};

	class __declspec(dllexport) Check {
	public:
		static BOOL isGrass7();
		static BOOL isProcessElevated(HANDLE process);
		static BOOL VerifyEmbeddedSignature(LPCWSTR pwszSourceFile);
	};

	class __declspec(dllexport) DWM {
	public:
		static void DwmIsCompositionEnabled(BOOL &dwmEnabled);
	};

	class __declspec(dllexport) Branding {
	public:
		static void LoadOSBrandingString(HMODULE &hMod, PWSTR &osbranding, wchar_t *SpecifierWstr);
		static void LoadOSBrandingImage(HBITMAP &hBitmap);
	};

	class __declspec(dllexport) SystemInfo {
	public:
		static void GetBuildString(wchar_t *Data, DWORD BufferSize);
		static void GetCurrentBuild(wchar_t *Data, DWORD BufferSize);
		static void GetPatchLevel(wchar_t *Data, DWORD BufferSize);
		static void GetUpdateLevel(wchar_t *Data, DWORD BufferSize);
		static void GetRegisteredOwner(wchar_t *Data, DWORD BufferSize);
		static void GetRegisteredOrganization(wchar_t *Data, DWORD BufferSize);
	};
};
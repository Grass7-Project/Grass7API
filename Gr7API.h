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
		static BOOL PaintTransparentBitmap(HDC &hdc, int xPos, int yPos, HBITMAP &hBitmap, BLENDFUNCTION bf);
		static BOOL PaintText(HDC &hdc, int xPos, int yPos, LPCWSTR font, COLORREF color, LPCWSTR text, int nSize, int SizeMode, int BkMode, int cWeight);
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

	class __declspec(dllexport) Convert {
	public:
		static wchar_t * convertchar(const char *stringchar);
		static LPCWSTR convertchartoLPCWSTR(const char *stringchar);
		static std::string WStringToString(const std::wstring& wstr);
	};

	class __declspec(dllexport) String {
	public:
		static void LoadOSBrandingString(PWSTR &osbranding);
	};
};
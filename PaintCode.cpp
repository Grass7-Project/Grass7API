#include "stdafx.h"
#include "Gr7API.h"

BOOL Grass7API::Paint::PaintTransparentBitmap(HDC &hdc, int xPos, int yPos, HBITMAP &hBitmap, BLENDFUNCTION bf)
{
	BITMAP Bitmap;
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);
	GetObjectW(hBitmap, sizeof(Bitmap), &Bitmap);
	BOOL ret = AlphaBlend(hdc, xPos, yPos, Bitmap.bmWidth, Bitmap.bmHeight, hdcMem, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, bf);
	SelectObject(hdcMem, oldBitmap);
	DeleteDC(hdcMem);
	DeleteObject(oldBitmap);
	return ret;
}

BOOL Grass7API::Paint::PaintText(HDC &hdc, int xPos, int yPos, LPCWSTR font, COLORREF color, LPCWSTR text, int nSize, int SizeMode, int BkMode, int cWeight)
{
	SetBkMode(hdc, BkMode);
	SetTextColor(hdc, color);
	HFONT hFont, hTmp;
	int nHeight = 0;

	if (SizeMode == 1) {
		nHeight = -MulDiv(nSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	}

	if (SizeMode == 2) {
		nHeight = nSize;
	}

	hFont = CreateFontW(nHeight, 0, 0, 0, cWeight, 0, 0, 0, 0, 0, 0, 2, 0, font);
	hTmp = (HFONT)SelectObject(hdc, hFont);
	size_t size = wcslen(text);
	int convertsize = static_cast<int>(size);
	BOOL ret = TextOutW(hdc, xPos, yPos, text, convertsize);
	DeleteObject(hFont);
	DeleteObject(hTmp);
	return ret;
}

#define COLORREF2RGB(Color) (Color & 0xff00) | ((Color >> 16) & 0xff) \
                                 | ((Color << 16) & 0xff0000)

HBITMAP Grass7API::Paint::ReplaceColor(HBITMAP hBmp, COLORREF cOldColor, COLORREF cNewColor, HDC hBmpDC)
{
	HBITMAP RetBmp = NULL;
	if (hBmp) {
		HDC BufferDC = CreateCompatibleDC(NULL);
		if (BufferDC) {
			HBITMAP hTmpBitmap = (HBITMAP)NULL;
			if (hBmpDC)
				if (hBmp == (HBITMAP)GetCurrentObject(hBmpDC, OBJ_BITMAP)) {
					hTmpBitmap = CreateBitmap(1, 1, 1, 1, NULL);
					SelectObject(hBmpDC, hTmpBitmap);
				}

			HGDIOBJ PreviousBufferObject = SelectObject(BufferDC, hBmp);

			HDC DirectDC = CreateCompatibleDC(NULL);
			if (DirectDC) {
				BITMAP bm;
				GetObject(hBmp, sizeof(bm), &bm);

				BITMAPINFO RGB32BitsBITMAPINFO;
				ZeroMemory(&RGB32BitsBITMAPINFO, sizeof(BITMAPINFO));
				RGB32BitsBITMAPINFO.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
				RGB32BitsBITMAPINFO.bmiHeader.biWidth = bm.bmWidth;
				RGB32BitsBITMAPINFO.bmiHeader.biHeight = bm.bmHeight;
				RGB32BitsBITMAPINFO.bmiHeader.biPlanes = 1;
				RGB32BitsBITMAPINFO.bmiHeader.biBitCount = 32;

				UINT * ptPixels;

				HBITMAP DirectBitmap = CreateDIBSection(DirectDC,
					(BITMAPINFO *)&RGB32BitsBITMAPINFO,
					DIB_RGB_COLORS,
					(void **)&ptPixels,
					NULL, 0);
				if (DirectBitmap) {
					HGDIOBJ PreviousObject = SelectObject(DirectDC, DirectBitmap);
					BitBlt(DirectDC, 0, 0,
						bm.bmWidth, bm.bmHeight,
						BufferDC, 0, 0, SRCCOPY);

					cOldColor = COLORREF2RGB(cOldColor);
					cNewColor = COLORREF2RGB(cNewColor);

					for (int i = ((bm.bmWidth*bm.bmHeight) - 1); i >= 0; i--) {
						if (ptPixels[i] == cOldColor) ptPixels[i] = cNewColor;
					}
					SelectObject(DirectDC, PreviousObject);
					RetBmp = DirectBitmap;
				}
				DeleteDC(DirectDC);
			}
			if (hTmpBitmap){
				SelectObject(hBmpDC, hBmp);
				DeleteObject(hTmpBitmap);
			}
			SelectObject(BufferDC, PreviousBufferObject);
			DeleteDC(BufferDC);
		}
	}
	return RetBmp;
}
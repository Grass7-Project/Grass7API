#include "stdafx.h"
#include "Gr7API.h"

void gr7::PaintTransparentBitmap(HDC &hdc, int xPos, int yPos, HBITMAP hBitmap, BLENDFUNCTION bf)
{
	BITMAP Bitmap;
	HDC hdcMem = CreateCompatibleDC(hdc);
	HGDIOBJ oldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);
	GetObjectW(hBitmap, sizeof(Bitmap), &Bitmap);
	HDC hdcMem02 = CreateCompatibleDC(hdc);
	AlphaBlend(hdc, xPos, yPos, Bitmap.bmWidth, Bitmap.bmHeight, hdcMem, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, bf);
	SelectObject(hdcMem, oldBitmap);
	DeleteDC(hdcMem);
}

void gr7::PaintText(HDC &hdc, int xPos, int yPos, wchar_t *font, COLORREF color, wchar_t *text, int nSize, int SizeMode)
{
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, color);
	HFONT hFont, hTmp;
	int nHeight;

	if (SizeMode == 1) {
		nHeight = -MulDiv(nSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	}

	if (SizeMode == 2) {
		nHeight = nSize;
	}

	hFont = CreateFontW(nHeight, 0, 0, 0, FW_LIGHT, 0, 0, 0, 0, 0, 0, 2, 0, font);
	hTmp = (HFONT)SelectObject(hdc, hFont);
	size_t size = wcslen(text);
	int convertsize = static_cast<int>(size);
	TextOutW(hdc, xPos, yPos, text, convertsize);
}
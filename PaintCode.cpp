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
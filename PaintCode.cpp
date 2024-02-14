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

BOOL gr7::PaintText(HDC &hdc, PaintTextOptions PaintTextOpt)
{
	// Make some variables set to a default value (1) if they are not defined
	if (PaintTextOpt.SizeMode < 0.0) {
		PaintTextOpt.SizeMode = 1;
	}
	if (PaintTextOpt.BkMode < 0.0) {
		PaintTextOpt.BkMode = TRANSPARENT;
	}
	if (PaintTextOpt.cWeight < 0.0) {
		PaintTextOpt.cWeight = FW_LIGHT;
	}

	SetBkMode(hdc, PaintTextOpt.BkMode);
	SetTextColor(hdc, PaintTextOpt.color);
	HFONT hFont, hTmp;
	int nHeight = 0;

	if (PaintTextOpt.SizeMode == 1) {
		nHeight = -MulDiv(PaintTextOpt.nSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	}
	if (PaintTextOpt.SizeMode == 2) {
		nHeight = PaintTextOpt.nSize;
	}

	hFont = CreateFontW(nHeight, 0, 0, 0, PaintTextOpt.cWeight, 0, 0, 0, 0, 0, 0, 2, 0, PaintTextOpt.font);
	if (hFont == NULL) {
		return 1;
	}
	hTmp = (HFONT)SelectObject(hdc, hFont);
	size_t size = wcslen(PaintTextOpt.text);
	int convertsize = static_cast<int>(size);
	BOOL ret = TextOutW(hdc, PaintTextOpt.xPos, PaintTextOpt.yPos, PaintTextOpt.text, convertsize);
	return ret;
}
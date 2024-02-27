#include "stdafx.h"
#include "Gr7API.h"

// Code to get the resolution of the screen
void Grass7API::Monitor::GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;

	const HWND hDesktop = GetDesktopWindow();

	GetWindowRect(hDesktop, &desktop);

	horizontal = desktop.right;
	vertical = desktop.bottom;
}
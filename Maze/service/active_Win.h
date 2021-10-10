#pragma once

bool active_game()
{
	TCHAR wnd_title[64];
	HWND Hwnd = GetForegroundWindow();

	GetWindowTextA(hwnd, wnd_title, 256);

	if (Hwnd == hwnd) { return true; }
	else { return false; }
}


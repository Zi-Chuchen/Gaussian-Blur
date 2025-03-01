#pragma once
#include<windows.h>

void clear_window(HWND hwnd) {
	SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & (~(WS_CAPTION | WS_SYSMENU | WS_SIZEBOX)));


	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) & (~(WS_EX_WINDOWEDGE | WS_EX_DLGMODALFRAME)) | WS_EX_LAYERED | WS_EX_TOOLWINDOW);

	SetWindowPos(hwnd, HWND_TOPMOST, (GetSystemMetrics(SM_CXSCREEN) - getwidth()) / 2, (GetSystemMetrics(SM_CYSCREEN) - getheight()) / 2, getwidth(), getheight(), SWP_SHOWWINDOW
		| SWP_FRAMECHANGED | SWP_DRAWFRAME);
	SetLayeredWindowAttributes(hwnd, 0x000000, 0, LWA_COLORKEY);
}
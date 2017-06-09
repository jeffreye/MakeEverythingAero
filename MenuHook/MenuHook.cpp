// MenuHook.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

bool bAddedMenu = false;

typedef HRESULT(WINAPI * SetWindowCompositionAttributeFunc)(HWND, WINCOMPATTR *);
static HRESULT(WINAPI * pSetWindowCompositionAttribute)(HWND, WINCOMPATTR *) = 0;

// Helper for loading a system library. Using LoadLibrary() directly is insecure
// because Windows might be searching the current working directory first.
static HMODULE load_sys_library(char *name)
{
	char path[MAX_PATH];
	UINT len = GetSystemDirectory(path, MAX_PATH);
	if (len && len + strlen(name) + 1 < MAX_PATH) {
		path[len] = '\\';
		strcpy(&path[len + 1], name);
		return LoadLibrary(path);
	}
	else
		return 0;
}

static void LoadFuncs(void)
{
	HMODULE user32 = load_sys_library(TEXT("user32.dll"));
	if (user32) {
		pSetWindowCompositionAttribute =
			(SetWindowCompositionAttributeFunc)GetProcAddress(user32, "SetWindowCompositionAttribute");
	}
}



VOID AddMenu(HWND hwnd, HMENU menu)
{
	if (bAddedMenu == true)
		return;

	HMENU systemMenu;
	if ((systemMenu = GetSystemMenu(hwnd, FALSE)) == NULL)
	{
		return;
	}


	if (menu != INVALID_HANDLE_VALUE && menu != systemMenu)
	{
		return;
	}

	bAddedMenu = true;


	MENUITEMINFO AeroItem = { 0 };
	AeroItem.cbSize = sizeof(AeroItem);
	AeroItem.fMask = MIIM_CHECKMARKS | MIIM_STATE | MIIM_ID | MIIM_STRING;
	AeroItem.fState = MFS_UNCHECKED;
	AeroItem.wID = AERO_ID;
	AeroItem.dwTypeData = TEXT("Make Background Aero");
	InsertMenuItem(systemMenu, SC_CLOSE, FALSE, &AeroItem);
	
}

VOID RemoveMenu(HWND hwnd, HMENU menu)
{
	if (bAddedMenu == false)
		return;
	HMENU systemMenu;
	if ((systemMenu = GetSystemMenu(hwnd, FALSE)) == NULL)
	{
		return;
	}

	if (menu != INVALID_HANDLE_VALUE && menu != systemMenu)
	{
		return;
	}

	bAddedMenu = false;

	MENUITEMINFO AeroItem = { 0 };
	AeroItem.cbSize = sizeof(AeroItem);
	AeroItem.fMask = MIIM_CHECKMARKS | MIIM_STATE | MIIM_ID | MIIM_STRING;
	if (GetMenuItemInfo(systemMenu, AERO_ID, MF_BYCOMMAND, &AeroItem) == NULL)
	{
		return;
	}
	DestroyMenu(AeroItem.hSubMenu);
	DeleteMenu(systemMenu, AeroItem.wID, MF_BYCOMMAND);
}

void HandleSysCommand(WPARAM wParam, HWND hwnd)
{
	if (wParam == AERO_ID)
	{
		if (pSetWindowCompositionAttribute) {
			DWMACCENTPOLICY policy = { ACCENT_ENABLE_BLURBEHIND, 0, 0, 0 };
			WINCOMPATTR data = { WCA_ACCENT_POLICY, (WINCOMPATTR_DATA*)&policy, sizeof(WINCOMPATTR_DATA) };
			pSetWindowCompositionAttribute(hwnd, &data);
		}
	}
}

LRESULT CALLBACK CallWndProc(INT code, WPARAM wParam, LPARAM lParam)
{
#define msg ((PCWPSTRUCT)lParam)
	if (code == HC_ACTION)
	{
		switch (msg->message)
		{
			// I am not sure if this is required, lets leve it in
		case WM_ACTIVATE:
		{
			Log("WM_ACTIVATE");
			GetSystemMenu(msg->hwnd, FALSE);
			break;
		}

		// Populate menu
		case WM_INITMENUPOPUP:
		{
			Log("WM_INITMENUPOPUP");
			AddMenu(msg->hwnd, (HMENU)msg->wParam);
			break;
		}

		// Some applications trigger WM_INITMENUPOPUP never or to late, thats why we use WM_ENTERIDLE
		case WM_ENTERIDLE:
		{
			Log("WM_ENTERIDLE");
			if (msg->wParam == MSGF_MENU)
			{
				AddMenu(msg->hwnd, (HMENU)INVALID_HANDLE_VALUE);
				break;
			}
			break;
		}

		// Remove Entry again
		case WM_UNINITMENUPOPUP:
		{
			Log("WM_UNINITMENUPOPUP");
			RemoveMenu(msg->hwnd, (HMENU)msg->wParam);
			break;
		}

		// For those who doesn't fire WM_UNINITMENUPOPUP 
		case WM_MENUSELECT:
		{
			Log("WM_ENTERIDLE");
			if (msg->lParam == NULL && HIWORD(msg->wParam) == 0xFFFF)
			{
				RemoveMenu(msg->hwnd, (HMENU)INVALID_HANDLE_VALUE);
			}
			break;
		}

		// Do the command
		case WM_SYSCOMMAND:
		{
			Log("WM_SYSCOMMAND %X %X", msg->wParam, msg->hwnd);
			HandleSysCommand(msg->wParam, msg->hwnd);
			break;
		}
		}
	}
	return CallNextHookEx(NULL, code, wParam, lParam);
#undef msg
}

LRESULT CALLBACK GetMsgProc(INT code, WPARAM wParam, LPARAM lParam)
{
#define msg ((PMSG)lParam)
	if (code == HC_ACTION)
	{
		switch (msg->message)
		{
		case WM_SYSCOMMAND:
		{
			Log("WM_SYSCOMMAND");
			HandleSysCommand(msg->wParam, msg->hwnd);
			break;
		}
		}
	}
	return CallNextHookEx(NULL, code, wParam, lParam);
#undef msg
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//ReadConfig();
		LoadFuncs();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		//FreeResources();
		break;
	}
	return TRUE;
}
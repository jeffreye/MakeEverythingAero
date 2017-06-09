#include <Windows.h>
#include <stdio.h>

#define TITLE "Make Everthing Aero"


int WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
) {

	DWORD error;
	HMODULE library;
	// load the hook library
	if ((library = LoadLibrary("MenuHook.dll")) == NULL)
	{
		error = GetLastError();
		char buffer[128] = "";
		sprintf_s(buffer, sizeof(buffer), "Could not load hook!\nErrorCode: %d", error);
		MessageBox(0, buffer, TITLE, MB_OK | MB_ICONERROR);
		return error;
	}
	try
	{
		HOOKPROC callWndProc;
		HHOOK callWndHook;

		if ((callWndProc = (HOOKPROC)GetProcAddress(library, "CallWndProc")) == NULL)
		{
			error = GetLastError();
			throw "Could not find CallWndProc in hook!";
		}
		if ((callWndHook = SetWindowsHookExA(WH_CALLWNDPROC, callWndProc, library, 0)) == NULL)
		{
			error = GetLastError();
			char buffer[128] = "";
			sprintf_s(buffer, sizeof(buffer), "Error on calling SetWindowsHookEx(WH_CALLWNDPROC)!\nErrorCode: %d", error);
			throw buffer;
		}
		try
		{
			HOOKPROC getMsgProc;
			HHOOK getMsgHook;

			if ((getMsgProc = (HOOKPROC)GetProcAddress(library, "GetMsgProc")) == NULL)
			{
				error = GetLastError();
				throw "Could not find GetMsgProc in hook.dll!";
			}
			if ((getMsgHook = SetWindowsHookExA(WH_GETMESSAGE, getMsgProc, library, 0)) == NULL)
			{
				error = GetLastError();
				char buffer[128] = "";
				sprintf_s(buffer, sizeof(buffer), "Error on calling SetWindowsHookEx(WH_GETMESSAGE)!\nErrorCode: %d", error);
				throw buffer;
			}

			try
			{
				try
				{
					MSG msg;
					BOOL ret;

					// pump the messages until the end of the session
					while ((ret = GetMessage(&msg, NULL, 0, 0)) != 0)
					{
						if (ret == -1)
						{
							return GetLastError();
						}
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
				catch (char *buffer)
				{
					MessageBox(0, buffer, TITLE, MB_OK | MB_ICONERROR);
				}
			}
			catch (char *buffer)
			{
				MessageBox(0, buffer, TITLE, MB_OK | MB_ICONERROR);
			}
			UnhookWindowsHookEx(getMsgHook);
		}
		catch (char *buffer)
		{
			MessageBox(0, buffer, TITLE, MB_OK | MB_ICONERROR);
		}
		UnhookWindowsHookEx(callWndHook);
	}
	catch (char *buffer)
	{
		MessageBox(0, buffer, TITLE, MB_OK | MB_ICONERROR);
	}

	FreeLibrary(library);


	// return success
	return ERROR_SUCCESS;
}
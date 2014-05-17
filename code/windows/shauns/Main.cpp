#include <windows.h>
#include "resource.h"
#include <iostream>
#include <direct.h>

using namespace std;


bool enter = FALSE;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, //Here we actually Start the form and tell it what to do
	LPSTR lpCmdLine, int iCmdShow)
{
	WNDCLASS wc;
	HWND hWnd;
	MSG msg;
	BOOL quit = FALSE; //Whether we are closing the application or not
	float theta = 0.0f;

	// register window class
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Unicorn shell";
	RegisterClass(&wc);

	// create main window
	hWnd = CreateWindow(
		"Unicorn Shell", "Unicorn Shell Alpha v1.0",
		WS_CAPTION | WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 560, 320, NULL, NULL, hInstance, NULL);
	// program main loop
	while (!quit)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{

			// handle or dispatch messages
			if (msg.message == WM_QUIT)
			{
				quit = TRUE;
			}
			else if (msg.message == WM_KEYUP) //Use keyup to prevent key spamming
			{
				if (msg.wParam == VK_RETURN) //Check if is return key
				{
					enter = true;
				}
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

		}
		else
		{
			try
			{
				int inputLen = GetWindowTextLength(GetDlgItem(hWnd, 102)) + 3; //Input controls current char length
				if (inputLen > 3 && enter == true)
				{
					char* Input = new char[inputLen]; //Character array to hold the Input controls text

					// Obtains input from the textbox and puts it into the char array
					SendMessage(GetDlgItem(hWnd, 102), WM_GETTEXT, (WPARAM)inputLen, (LPARAM)Input);
					//If the user has pressed return and the input is not empty
					if (Input[0] != '\0')
					{
						exe(Input, hWnd);
					}
					free(Input);
				}
				enter = false;
			}
			catch (...)
			{
			}

		}

	}

	// destroy the window explicitly
	DestroyWindow(hWnd);

	return msg.wParam;

}

void exe(char* a, HWND hw)
{
	int len = GetWindowTextLength(GetDlgItem(hw, 101)) + 1;
	string input;
	SetWindowText(GetDlgItem(hw, 102), ""); //Clear the Input control
	char * output = new char[len];
	SendMessage(GetDlgItem(hw, 101), WM_GETTEXT, (WPARAM)len, (LPARAM)output);
	char buffer[_MAX_PATH];
	if (strcmp(a, "chdir") == 0 || strcmp(a, "pwd") == 0)
	{
		// Get the current working directory: 
		if ((_getcwd(buffer, sizeof(buffer))) == NULL)
			perror("_getcwd error");
		else
		{
			input = input + "\r\n";
			input = input + buffer;
		}
	}
	else if (strcmp(a, "ls") == 0 || strcmp(a, "dir") == 0)
	{
		char substr[_MAX_PATH];
		_getcwd(buffer, sizeof(buffer));
		strcpy_s(substr, buffer);
		strcat_s(substr, "\\*");
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = FindFirstFile(substr, &FindFileData);
		input.append("\r\n");
		if (hFind == INVALID_HANDLE_VALUE)
		{
			perror("Nope");
		}
		else do
		{
			input.append(FindFileData.cFileName);
			input.append("\r\n");
		}
		while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}
	else if (strstr(a, "cd") != NULL)
	{
		char substr[_MAX_PATH];
		strncpy_s(substr, strstr(a, "cd")+3, _MAX_PATH);
		if (_chdir(substr) != -1)
		{
			
		}
	}
	else
	{
		PROCESS_INFORMATION ProcessInfo; //This is what we get as an [out] parameter
		STARTUPINFO StartupInfo; //This is an [in] parameter
		ZeroMemory(&StartupInfo, sizeof(StartupInfo));
		StartupInfo.cb = sizeof StartupInfo; //Only compulsory field
		if (CreateProcess(NULL, a, NULL, NULL, FALSE, 0, NULL, _getcwd(buffer, sizeof(buffer)), &StartupInfo, &ProcessInfo))
		{
			//WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
			//CloseHandle(ProcessInfo.hThread);
			//CloseHandle(ProcessInfo.hProcess);
		}
		else
		{
			input.append(": Cannot be found.");
		}
	}
	input = a + input;
	input = output + input + "\r\n";
	SetWindowText(GetDlgItem(hw, 101), input.c_str());
	SendMessage(GetDlgItem(hw, 101), WM_VSCROLL, (WPARAM)SB_BOTTOM, (LPARAM)NULL);
	SetFocus(GetDlgItem(hw, 102)); //Set focus to Input
	enter = false;
}

//Listen for callbacks from the Operating system
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
		case WM_CREATE:
		{
			HFONT hfDefault;
			HWND hEdit;
			HDC hdc;
			long lfHeight;
		
			hdc = GetDC(NULL);
			lfHeight = -MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 72);

			hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
			WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY | WS_VISIBLE,
			0, 0, 0, 0, hWnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
		
			if (hEdit == NULL)
			{
				MessageBox(hWnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);
			}

			hfDefault = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, "Times New Roman");
			SendMessage(hEdit, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));

			int const id = IDC_MAIN_EDIT + 1;
			hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
			WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0, hWnd, (HMENU)id, GetModuleHandle(NULL), NULL);
		
			if (hEdit == NULL)
			{
				MessageBox(hWnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);
			}
			SendMessage(hEdit, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
		}
		break;

		case WM_SIZE:
		{
			HWND hEdit;
			RECT rcClient;

			GetClientRect(hWnd, &rcClient);

			hEdit = GetDlgItem(hWnd, IDC_MAIN_EDIT);
			SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom - 30, SWP_NOZORDER);

			hEdit = GetDlgItem(hWnd, IDC_MAIN_EDIT + 1);
			SetWindowPos(hEdit, NULL, 0, rcClient.bottom - 30, rcClient.right, rcClient.bottom-10, SWP_NOZORDER);
		}
		break;

		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;

		case WM_DESTROY:
			return 0;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);

	}
	return 0;
}
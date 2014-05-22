#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <iostream>
#include <direct.h>
#include "resource.h"

bool enter = FALSE;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, //Here we actually Start the form and tell it what to do
	LPSTR lpCmdLine, int iCmdShow)
{
	string commands[128];
	int currentCommand = 0;
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
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 400, NULL, NULL, hInstance, NULL);
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
				if (msg.wParam == VK_UP) //Check if is up arrow
				{
					currentCommand = getCommand(commands, TRUE, currentCommand, hWnd); //Previous command
				}
				if (msg.wParam == VK_DOWN) //Check if is down arrow
				{
					currentCommand = getCommand(commands, FALSE, currentCommand, hWnd); //Next command
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
						while (commands[currentCommand] != "")
						{
							currentCommand++; //Go to next empty element
						}
						commands[currentCommand] = Input;
						currentCommand++;
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
int getCommand(string* commands, BOOL prev, int current, HWND hWnd)
{
	if (current > 0 && prev == TRUE)
	{
		SetWindowText(GetDlgItem(hWnd, 102), commands[current - 1].c_str()); //Set previous command
		current--;
	}
	else if (prev == FALSE && commands[current + 1] != "")
	{
		SetWindowText(GetDlgItem(hWnd, 102), commands[current + 1].c_str()); //Set next command
		current++;
	}
	return current;
}
void exe(char* a, HWND hw)
{
	int len = GetWindowTextLength(GetDlgItem(hw, 101)) + 1;
	string input;
	SetWindowText(GetDlgItem(hw, 102), ""); //Clear the Input control
	char * output = new char[len];
	SendMessage(GetDlgItem(hw, 101), WM_GETTEXT, (WPARAM)len, (LPARAM)output);
	char buffer[_MAX_PATH];
	if (strcmp(a, "help") == 0)
	{
		input.append("\r\nType pwd / chdir for current directory.\r\n");
		input.append("Type cd followed by directory name to change into a directory.\r\n");
		input.append("Type ls or dir to list the files in the current directory.\r\n");
		input.append("Type mkdir followed by the desired name to create a directory in the current directory.\r\n");
		input.append("Type rmdir followed by the desired name to remove a directory in the current directory.\r\n");
		input.append("You can open a system program by typing in its name followed by arguments if required (e.g. ping 127.0.0.1).\r\n");
		input.append("Or by changing into your programs directory then type in its name followed by arguments if required.\r\n");
	}
	else if (strcmp(a, "chdir") == 0 || strcmp(a, "pwd") == 0)
	{
		// Get the current working directory: 
		if ((_getcwd(buffer, sizeof(buffer))) == NULL)
			ErrorExit(TEXT("pwd"));
		else
		{
			input = input + "\r\n";
			input = input + buffer;
		}
	}
	else if (strstr(a, "mkdir") != NULL)
	{
		//Make a new directory
		if (strlen(a) > 6)
		{
			char substr[_MAX_PATH];
			int b = strlen(a);
			_getcwd(buffer, sizeof(buffer));
			strcpy_s(substr, buffer);
			strcat_s(substr, "\\");
			strncat_s(substr, strstr(a, "mkdir") + 6, strlen(strstr(a, "mkdir") + 6));
			if (CreateDirectory(substr, NULL) != 0)
			{
				input = input + "\r\n" + substr + " was successfully created.";
			}
			else
			{
				input = input + "\r\n Failed to create: " + substr + " do you have correct permissions?.";
				ErrorExit(TEXT("CreateDirectory"));
			}
		}
	}
	else if (strstr(a, "rmdir") != NULL)
	{
		//Remove a directory
		if (strlen(a) > 6)
		{
			char substr[_MAX_PATH];
			_getcwd(buffer, sizeof(buffer));
			strcpy_s(substr, buffer);
			strcat_s(substr, "\\");
			strncat_s(substr, strstr(a, "rmdir") + 6, strlen(strstr(a, "rmdir") + 6));
			if (RemoveDirectory(substr) == TRUE)
			{
				input = input + "\r\n" + substr + " was successfully deleted.";
			}
			else
			{
				input = input + "\r\n Failed to delete: " + substr + " , is it empty?.";
				ErrorExit(TEXT("RemoveDirectory"));
			}
		}
	}
	else if (strstr(a, "rm") != NULL)
	{
		//Remove a file
		if (strlen(a) > 3)
		{
			char substr[_MAX_PATH];
			_getcwd(buffer, sizeof(buffer));
			strcpy_s(substr, buffer);
			strcat_s(substr, "\\");
			strncat_s(substr, strstr(a, "rm") + 3, strlen(strstr(a, "rm")));
			if (DeleteFile(substr) != 0)
			{
				input = input + "\r\n" + substr + " , was successfully deleted.";
			}
			else
			{
				input = input + "\r\n Failed to delete: " + substr + " .";
				ErrorExit(TEXT("DeleteFile"));
			}
		}
	}
	else if (strcmp(a, "ls") == 0 || strcmp(a, "dir") == 0)
	{
		//List files in current directory
		char substr[_MAX_PATH];
		_getcwd(buffer, sizeof(buffer));
		strcpy_s(substr, buffer);
		strcat_s(substr, "\\*");
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = FindFirstFile(substr, &FindFileData);
		input.append("\r\n");
		if (hFind == INVALID_HANDLE_VALUE)
		{
			ErrorExit(TEXT("ls / dir"));
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
		//Change directory
		char substr[_MAX_PATH];
		strncpy_s(substr, strstr(a, "cd")+3, _MAX_PATH);
		if (_chdir(substr) != -1)
		{
			
		}
	}
	else
	{
		SECURITY_ATTRIBUTES saAttr;
		saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
		saAttr.bInheritHandle = TRUE;
		saAttr.lpSecurityDescriptor = NULL;

		// Create a pipe for the child process's STDOUT. 

		if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
			ErrorExit(TEXT("StdoutRd CreatePipe"));

		// Ensure the read handle to the pipe for STDOUT is not inherited.

		if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
			ErrorExit(TEXT("Stdout SetHandleInformation"));

		// Create a pipe for the child process's STDIN. 

		if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0))
			ErrorExit(TEXT("Stdin CreatePipe"));

		// Ensure the write handle to the pipe for STDIN is not inherited. 

		if (!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
			ErrorExit(TEXT("Stdin SetHandleInformation"));

		// Create the child process. 

		if (CreateChildProcess(a) == true)
		{	
				// Read from pipe that is the standard output for child process. 
				input.append(ReadFromPipe());
		}
		else
		{
			input.append(": cannot be found, enter help for supported commands.");
		}
	}
	input = a + input;
	input = output + input + "\r\n";
	SetWindowText(GetDlgItem(hw, 101), input.c_str()); //Update output
	SendMessage(GetDlgItem(hw, 101), WM_VSCROLL, (WPARAM)SB_BOTTOM, (LPARAM)NULL); //Set scrollbar to bottom
	SetFocus(GetDlgItem(hw, 102)); //Set focus to Input
	enter = false;
}

bool CreateChildProcess(char* a)
// Create a child process that uses the previously created pipes for STDIN and STDOUT.
{

	char buffer[_MAX_PATH];
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
	BOOL bSuccess = FALSE;

	// Set up members of the PROCESS_INFORMATION structure. 

	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

	// Set up members of the STARTUPINFO structure. 
	// This structure specifies the STDIN and STDOUT handles for redirection.

	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdError = g_hChildStd_OUT_Wr;
	siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
	siStartInfo.hStdInput = g_hChildStd_IN_Rd;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	// Create the child process. 

	if (CreateProcess(NULL, a, NULL, NULL, TRUE, 0, NULL, _getcwd(buffer, sizeof(buffer)), &siStartInfo, &piProcInfo))
	{
		// Close handles to the child process and its primary thread.
		// Some applications might keep these handles to monitor the status
		// of the child process, for example. 
		CloseHandle(piProcInfo.hThread);
		CloseHandle(piProcInfo.hProcess);
		return true;
	}
	else
	{
		ErrorExit(TEXT("CreateProcess"));
		return false;
	}
}

void WriteToPipe(void)

// Read from a file and write its contents to the pipe for the child's STDIN.
// Stop when there is no more data. 
{
	DWORD dwRead, dwWritten;
	CHAR chBuf[BUFSIZE];
	BOOL bSuccess = FALSE;

	for (;;)
	{
		bSuccess = ReadFile(g_hInputFile, chBuf, BUFSIZE, &dwRead, NULL);
		if (!bSuccess || dwRead == 0) break;

		bSuccess = WriteFile(g_hChildStd_IN_Wr, chBuf, dwRead, &dwWritten, NULL);
		if (!bSuccess) break;
	}

	// Close the pipe handle so the child process stops reading. 

	if (!CloseHandle(g_hChildStd_IN_Wr))
		ErrorExit(TEXT("StdInWr CloseHandle"));
	CloseHandle(g_hInputFile);
}

string ReadFromPipe()
// Read output from the child process's pipe for STDOUT
// and write to the parent process's pipe for STDOUT. 
// Stop when there is no more data. 
{
	int pos;
	CloseHandle(g_hChildStd_OUT_Wr);
	string s = "";
	DWORD dwRead, dwWritten;
	CHAR chBuf[BUFSIZE];
	BOOL bSuccess = FALSE;
	HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	for (;;)
	{
		bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
		if (dwRead > 0)
		{
			pos = dwRead;
		}
		bSuccess = WriteFile(hParentStdOut, chBuf,
			dwRead, &dwWritten, NULL);
		if (dwRead == 0 && !bSuccess)
		{
			break;
		}
		chBuf[pos] = '\0';
		s.append(chBuf);
	}
	CloseHandle(hParentStdOut);
	s = "\r\n" + s;
	return s;
}

void ErrorExit(PTSTR lpszFunction)

// Format a readable error message, display a message box
{
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40)*sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);
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

			//Create the Edit box
			hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
			WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY | WS_VISIBLE,
			0, 0, 0, 0, hWnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
		
			if (hEdit == NULL)
			{
				MessageBox(hWnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);
			}

			//Set a font
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
			//Update the forms if the main form is resized
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
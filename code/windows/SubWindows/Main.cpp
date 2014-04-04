#include <windows.h>
#include <commctrl.h>
#include <gl/gl.h>
#include "resource.h"
#include <stdlib.h> /* for the system() call */
#pragma comment( lib, "OpenGL32.Lib" )

#define GL_BGRA GL_BGRA_EXT

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void exe(char* a, HWND hw);
BOOL enter = FALSE;
int num = 0;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, //Here we actually Start the form and tell it what to do
	LPSTR lpCmdLine, int iCmdShow)
{
	WNDCLASS wc;
	HWND hWnd;
	HDC hDC;
	HGLRC hRC;
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
		"Unicorn Shell", "Unicorn shell v1.0",
		WS_CAPTION | WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 560, 320, NULL, NULL, hInstance, NULL);

	num = nextLine(hWnd, num);
	refresh_textbox(hWnd);

	EnableOpenGL(hWnd, &hDC, &hRC);
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
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glLoadIdentity();
			try
			{
				char szInput[MAX_PATH];
				// Obtains input from the textbox and puts it into the char array
				GetWindowText(GetDlgItem(hWnd, 100 + num), szInput, MAX_PATH);
				//system(szInput);
				if (enter == true && szInput[0] != '\0')
				{
					exe(szInput, hWnd);
				}
				else
				{
					enter = false;
				}
			}
			catch (...)
			{
			}
			SwapBuffers(hDC);
		}
	}
	DisableOpenGL(hWnd, hDC, hRC);
	// destroy the window explicitly
	DestroyWindow(hWnd);

	return msg.wParam;

}

void exe(char* a, HWND hw)
{
	if (system(a) >= 0)
	{
		num = nextLine(hw, num);
		refresh_textbox(hw);
		enter = false;
	}
}

int nextLine(HWND hWnd, int nums)
{
	HFONT hfDefault;
	HWND hEdit;
	HDC hdc;

	long lfHeight;
	nums++;

	hdc = GetDC(NULL);
	lfHeight = -MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 72);

	for (int i = 0; i < nums; i++)
	{
		int const id = 100 + i + 1;
		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
			WS_CHILD | LBS_STANDARD | WS_VISIBLE,
			0, i * 20, 500, 20, hWnd, (HMENU)id, NULL, NULL);

		if (hEdit == NULL)
			MessageBox(hWnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);

		hfDefault = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, "Times New Roman");
		SendMessageA(hEdit, LB_ADDSTRING, 0, (LPARAM)"name");
	}
	SetFocus(hEdit);
	return nums;
}

void refresh_textbox(HWND hWnd)
{
	HWND hEdit;
	RECT rcClient;

	GetWindowRect(hWnd, &rcClient);
	for (int i = 0; i < num; i++)
	{
		hEdit = GetDlgItem(hWnd, 100 + i + 1);
		SetWindowPos(hEdit, NULL, 0, i * 20, rcClient.right, 20, SWP_NOZORDER);
		RedrawWindow(hEdit, NULL, NULL, RDW_INVALIDATE | RDW_FRAME);
	}
}
void EnableOpenGL(HWND hWnd, HDC * hDC, HGLRC * hRC)
{
	PIXELFORMATDESCRIPTOR pfd;
	int iFormat;

	// get the device context (DC)
	*hDC = GetDC(hWnd);

	// set the pixel format for the DC
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	iFormat = ChoosePixelFormat(*hDC, &pfd);
	SetPixelFormat(*hDC, iFormat, &pfd);

	// create and enable the render context (RC)
	*hRC = wglCreateContext(*hDC);
	wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd, hDC);
}
//Listen for callbacks from the Operating system
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{

	case WM_SIZE:
	{
		HWND hEdit;
		RECT rcClient;

		GetWindowRect(hWnd, &rcClient);
		refresh_textbox(hWnd);

		hEdit = GetDlgItem(hWnd, IDC_MAIN_EDIT);
		SetWindowPos(hWnd, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
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
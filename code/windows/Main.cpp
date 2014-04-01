#include <windows.h>
#include "resource.h"
#include <stdlib.h> /* for the system() call */

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void exe(char* a, HWND hw);
BOOL enter = FALSE;
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
                     char szInput[MAX_PATH];
                     // Obtains input from the textbox and puts it into the char array
                     GetWindowText(GetDlgItem(hWnd, 101), szInput, MAX_PATH);
                     //system(szInput);
                     if (enter == true && szInput[0] != '\0')
                        {
                           exe(szInput, hWnd);
                        }
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
   system(a);
   SetWindowText(GetDlgItem(hw, 101), "");
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
                                   WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                                   0, 0, 100, 100, hWnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
            if (hEdit == NULL)
               MessageBox(hWnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);

            hfDefault = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, "Times New Roman");
            SendMessage(hEdit, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
         }
         break;
      case WM_SIZE:
         {
            HWND hEdit;
            RECT rcClient;

            GetClientRect(hWnd, &rcClient);

            hEdit = GetDlgItem(hWnd, IDC_MAIN_EDIT);
            SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
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

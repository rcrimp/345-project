#define IDR_MYMENU 101
#define IDI_MYICON 201

#define ID_FILE_EXIT 9001
#define ID_STUFF_GO 9002

#define IDC_MAIN_EDIT 101

int nextLine(HWND hWnd, int num);

void refresh_textbox(HWND hWnd);

void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);

void EnableOpenGL(HWND hWnd, HDC * hDC, HGLRC * hRC);
#define IDR_MYMENU 101
#define IDI_MYICON 201

#define ID_FILE_EXIT 9001
#define ID_STUFF_GO 9002

#define IDC_MAIN_EDIT 101

HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_IN_Wr = NULL;
HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;

HANDLE g_hInputFile = NULL;

#define BUFSIZE 4096
using namespace std;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void exe(char* a, HWND hw);
bool CreateChildProcess(char * a);
void WriteToPipe(void);
void ErrorExit(PTSTR);
string ReadFromPipe();
int getCommand(string* commands, BOOL prev, int current, HWND hWnd);
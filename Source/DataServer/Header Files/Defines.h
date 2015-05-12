#ifndef DEFINES_HEADER
#define DEFINES_HEADER


/* * * * * * * * * * * * * * * * * * *
 *	Integers Declarations
 */
#define MAX_STRING_LEN				100
#define LOG_LINE_LENGTH				190
#define LOG_LINE_COUNT				50
#define LOG_TEXT_FILE_LENGTH		512
#define	MAX_READ_FILE				1
#define MAX_LINE_LEN				1024
#define Unk238						7
#define MAX_DB_TEXT_LEN				500
#define MAX_OBJECT					20
#define MAX_IO_THREAD_HANDLES		16
#define MAX_IO_BUFFER_SIZE			7000
#define OP_SEND 					0
#define OP_RECV 					1
#define	MAX_KEY_INDEX				256
#define SLOT_FREE					-1
#define CHAR_SET_SIZE				18
#define INVENTORY_BINARY_SIZE		3776
#define	WM_LOG_PAINT				(0x101)
// -------------------------------------------------------------------------------------------------------------------------------------------------------
/* * * * * * * * * * * * * * * * * * *
 *	Strings Declarations
 */
#define SOFT_NAME					"DataServer Emu"
#define SOFT_VER					"1.2.0"
#define SOFT_CODERS					"IMX Software"
#define ErrorMSGCaption				"DataServer Emulator -> ERROR"
// -------------------------------------------------------------------------------------------------------------------------------------------------------
/* * * * * * * * * * * * * * * * * * *
 *	Other Declarations
 */
#define APP_MAIN					int APIENTRY
#define APP_ARGS					HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow
#define LOAD_APP_WIN				hInstance,IDC_ZODEAMUSUITE,m_WinClass,MAX_STRING_LEN
#define APP_ARGS_PUSH				hInstance,nCmdShow
#define APP_LPRESULTCB				LRESULT CALLBACK
#define WNDPROC_ARGS				HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam
#define APP_INTPTRCB				INT_PTR CALLBACK
#define ABOUT_ARGS					HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam
#define SOFT_WORK_THREAD			void MainTask(LPVOID Param) { gMainApp.Worker(); }
#define VA_START					_crt_va_start
#define VA_END						_crt_va_end
typedef __int64						QWORD, *LPQWORD;
#define SET_NUMBERH(x)				( (UCHAR)((unsigned long)(x)>>(unsigned long)8) )
#define SET_NUMBERL(x)				( (UCHAR)((unsigned long)(x) & 0xFF) )
#define BeginThread					_beginthread
#define EndThread					_endthread
#define	DWAPI						static DWORD WINAPI
#define VVOID						virtual void
#define	LPTHREAD					LPTHREAD_START_ROUTINE
#define CloseSocket					closesocket
#define LPPERSOCKET					_PER_SOCKET_CONTEXT * lpPerSocketContext
#define MAX_USER					(MAX_OBJECT * 1000)
// -----------------------------------------------------------------------------------------------------------------------------------------------------
#endif /* DEFINES_HEADER */
// -----------------------------------------------------------------------------------------------------------------------------------------------------
/* File WindowProgram.h
* The WindowProgram class sets up a window for a 
* window running program.
*/


#ifndef _WINDOWPROGRAM_H_
#define _WINDOWPROGRAM_H_

#include <windows.h>

class WindowProgram {
public:
	WindowProgram(HINSTANCE,HINSTANCE,LPSTR,int,WNDPROC,LPCTSTR);
	ATOM RegisterWindowClass();
	int MessageLoop();
	int MessageLoopEx(void (*)(HWND));
private:
	static bool m_fRegistered;
	PTSTR m_szClassName;
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	WNDPROC m_pfnWndProc;


};




#endif // _WINDOWPROGRAM_H_
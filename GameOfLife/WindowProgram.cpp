/* File WindowProgram.cpp
* The WindowProgram class sets up a window for a 
* window running program.
*/


#include "WindowProgram.h"

bool WindowProgram::m_fRegistered=false;

WindowProgram::WindowProgram(HINSTANCE hInstance,
							 HINSTANCE hPrevInstance,
							 LPSTR lpCmdLine,
							 int nShowCmd,
							 WNDPROC wndProc,
							 LPCTSTR szWindowTitle){
	if(!m_fRegistered){
		m_szClassName=TEXT("WindowProgram");
		m_hInstance=hInstance;
		m_pfnWndProc=wndProc;
		RegisterWindowClass();
		m_fRegistered=true;
	}
	m_hWnd=CreateWindowEx(0,m_szClassName,szWindowTitle,WS_OVERLAPPEDWINDOW,0,0,1024,768,NULL,NULL,hInstance,NULL);
	ShowWindow(m_hWnd,nShowCmd);
	UpdateWindow(m_hWnd);
}

ATOM WindowProgram::RegisterWindowClass(){
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize=sizeof(WNDCLASSEX);
	wc.hInstance=m_hInstance;
	wc.lpszClassName=m_szClassName;
	wc.lpfnWndProc=m_pfnWndProc;
	wc.style=CS_VREDRAW|CS_HREDRAW|CS_BYTEALIGNWINDOW;
	LOGBRUSH lb;
	lb.lbStyle=BS_HATCHED;
	lb.lbColor=RGB(0,128,255);
	lb.lbHatch=HS_CROSS;
	wc.hbrBackground=(HBRUSH)(GetStockObject(WHITE_BRUSH));//CreateBrushIndirect(&lb);
	wc.hCursor=LoadCursor(NULL, IDC_ARROW);

	return RegisterClassEx(&wc);
}

int WindowProgram::MessageLoop(){
	MSG msg;
	while(GetMessage(&msg,m_hWnd,0,0)==TRUE){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		//if(msg.message==WM_QUIT) break;
	}
	return (int) msg.wParam;
}

int WindowProgram::MessageLoopEx(void (*DoWork)(HWND)){
	MSG msg;
	while(TRUE){
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
			if(msg.message==WM_QUIT){
				//MessageBox(m_hWnd,TEXT("WM_QUIT received!"),TEXT("WM_QUIT message received by PeekMessage()"),MB_OK);
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		DoWork(m_hWnd);
		Sleep(5);
	}
	return (int) msg.wParam;
}
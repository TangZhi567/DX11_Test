#include <Windows.h>
#include "WindowsMessageMap.h"
//���ڴ�������
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static WindowsMessageMap mm;
	switch ( msg )
	{
	case WM_CLOSE:
		//MessageBox(NULL, TEXT("��ʾ"), TEXT("�˳�"), MB_OK);
		PostQuitMessage(69);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPreInstance,
	LPSTR         lpCmdLine,
	int				nCmdShow
)
{
	const auto pClassName = ("hw3dbutts");
	//ע�ᴰ����
	
	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;

	wc.hIcon = NULL;
	wc.hCursor = nullptr;
	wc.hIconSm = NULL;

	wc.hbrBackground = nullptr;	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	
	
	if( 0 == RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("�˳���������"), TEXT("��ʾ"), MB_OK);
		return 0;
	}
	//
	HWND  hWnd = CreateWindowEx(
		0, pClassName, 
		"Happy Hard Window",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		200, 200, 640, 480,
		nullptr, nullptr, hInstance, nullptr
	);
	//��ʾ����
	ShowWindow( hWnd, SW_SHOW);

	//message pump
	MSG msg;
	BOOL gResult;
	while ((gResult =GetMessage(&msg,nullptr,0,0)) > 0)
	{
		TranslateMessage( &msg);
		DispatchMessage( &msg );

	}
	if (gResult == -1)
	{
		return -1;
	}
	else
	{
		return msg.wParam;
	}
	
}
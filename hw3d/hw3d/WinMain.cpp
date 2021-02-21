#include <Windows.h>
#include "WindowsMessageMap.h"
#include <sstream>
//窗口处理函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static WindowsMessageMap mm;
	switch (msg)
	{
	case WM_CLOSE:
		//MessageBox(NULL, TEXT("提示"), TEXT("退出"), MB_OK);
		PostQuitMessage(69);
		break;

	case WM_KEYDOWN:
		if (wParam == 'D')
		{
			SetWindowText(hWnd, "Respects");
		}
	case WM_KEYUP:
		if (wParam == 'F')
		{
			SetWindowText(hWnd, "Dangerfield");
		}
		break;
	case WM_CHAR:
		{
			static std::string title;
			title.push_back((char)wParam);
			SetWindowText(hWnd, title.c_str());

		}
		break;
	case WM_LBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			std::ostringstream oss;
			oss << "(" << pt.x << "," << pt.y << ")";
			SetWindowText(hWnd, oss.str().c_str());
		}
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
	//注册窗口类

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


	if (0 == RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("此程序不能运行"), TEXT("提示"), MB_OK);
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
	//显示窗口
	ShowWindow(hWnd, SW_SHOW);

	//message pump
	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

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

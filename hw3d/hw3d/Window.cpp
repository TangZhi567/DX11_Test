#include "Window.h"
#include <sstream>
//#include "resource.h"
//#include "WindowsThrowMacros.h"
//#include "imgui/imgui_impl_win32.h"


// Window Class Stuff
Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
	:
	hInst(GetModuleHandle(nullptr))
{
	static
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	//wc.hIcon = static_cast<HICON>(LoadImage(
	//	GetInstance(), MAKEINTRESOURCE(IDI_ICON1),
	//	IMAGE_ICON, 32, 32, 0
	//));
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	//wc.hIconSm = static_cast<HICON>(LoadImage(
	//	GetInstance(), MAKEINTRESOURCE(IDI_ICON1),
	//	IMAGE_ICON, 16, 16, 0
	//));
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}


// Window Stuff
Window::Window(int width, int height, const char* name)
	:
	width(width),
	height(height)
{
	// calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{
		//throw CHWND_LAST_EXCEPT();
	}

	// create window & get hWnd
	hWnd = CreateWindow(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);
	// check for error
	if (hWnd == nullptr)
	{
		//throw CHWND_LAST_EXCEPT();
	}
	// newly created windows start off as hidden
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	//// Init ImGui Win32 Impl
	//ImGui_ImplWin32_Init(hWnd);
	//// create graphics object
	//pGfx = std::make_unique<Graphics>(hWnd, width, height);
	//// register mouse raw input device
	//RAWINPUTDEVICE rid;
	//rid.usUsagePage = 0x01; // mouse page
	//rid.usUsage = 0x02; // mouse usage
	//rid.dwFlags = 0;
	//rid.hwndTarget = nullptr;
	//if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
	//{
	//	throw CHWND_LAST_EXCEPT();
	//}
}


Window::~Window()
{
	//ImGui_ImplWin32_Shutdown();
	DestroyWindow(hWnd);
}
/*
void Window::SetTitle(const std::string& title)
{
	if (SetWindowText(hWnd, title.c_str()) == 0)
	{
		throw CHWND_LAST_EXCEPT();
	}
}

void Window::EnableCursor() noexcept
{
	cursorEnabled = true;
	ShowCursor();
	EnableImGuiMouse();
	FreeCursor();
}

void Window::DisableCursor() noexcept
{
	cursorEnabled = false;
	HideCursor();
	DisableImGuiMouse();
	ConfineCursor();
}

bool Window::CursorEnabled() const noexcept
{
	return cursorEnabled;
}

std::optional<int> Window::ProcessMessages() noexcept
{
	MSG msg;
	// while queue has messages, remove and dispatch them (but do not block on empty queue)
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// check for quit because peekmessage does not signal this via return val
		if (msg.message == WM_QUIT)
		{
			// return optional wrapping int (arg to PostQuitMessage is in wparam) signals quit
			return (int)msg.wParam;
		}

		// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// return empty optional when not quitting app
	return {};
}

Graphics& Window::Gfx()
{
	if (!pGfx)
	{
		throw CHWND_NOGFX_EXCEPT();
	}
	return *pGfx;
}

void Window::ConfineCursor() noexcept
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	MapWindowPoints(hWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
	ClipCursor(&rect);
}

void Window::FreeCursor() noexcept
{
	ClipCursor(nullptr);
}

void Window::HideCursor() noexcept
{
	while (::ShowCursor(FALSE) >= 0);
}

void Window::ShowCursor() noexcept
{
	while (::ShowCursor(TRUE) < 0);
}

void Window::EnableImGuiMouse() noexcept
{
	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}

void Window::DisableImGuiMouse() noexcept
{
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}
*/

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window instance
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		// forward message to window instance handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// retrieve ptr to window instance
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window instance handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	//if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	//{
	//	return true;
	//}
	//const auto& imio = ImGui::GetIO();

	switch (msg)
	{
		// we don't want the DefProc to handle this message because
		// we want our destructor to destroy the window, so return 0 instead of break
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		// clear keystate when window loses focus to prevent input getting "stuck"
	}


	return DefWindowProc(hWnd, msg, wParam, lParam);
}


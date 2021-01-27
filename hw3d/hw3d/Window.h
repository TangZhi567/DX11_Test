
#pragma once
#include "ChiliWin.h"
//#include "ChiliException.h"
//#include "Keyboard.h"
//#include "Mouse.h"
//#include "Graphics.h"
#include <optional>
#include <memory>


class Window
{
private:
	// singleton manages registration/cleanup of window class
	class WindowClass
	{
		public:
			static const char* GetName() noexcept;
			static HINSTANCE GetInstance() noexcept;
		private:
			WindowClass() noexcept;
			~WindowClass();
			WindowClass(const WindowClass&) = delete;
			WindowClass& operator=(const WindowClass&) = delete;
			static constexpr const char* wndClassName = "My Window";
			static WindowClass wndClass;
			HINSTANCE hInst;
	};
public:
	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	//void SetTitle(const std::string& title);
	//void EnableCursor() noexcept;
	//void DisableCursor() noexcept;
	//bool CursorEnabled() const noexcept;
	//static std::optional<int> ProcessMessages() noexcept;
	//Graphics& Gfx();
private:
	//void ConfineCursor() noexcept;
	//void FreeCursor() noexcept;
	//void ShowCursor() noexcept;
	//void HideCursor() noexcept;
	//void EnableImGuiMouse() noexcept;
	//void DisableImGuiMouse() noexcept;
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
//public:
//	Keyboard kbd;
//	Mouse mouse;
private:
	//bool cursorEnabled = true;
	int width;
	int height;
	HWND hWnd;
	//std::unique_ptr<Graphics> pGfx;
	//std::vector<BYTE> rawBuffer;
	//std::string commandLine;
};

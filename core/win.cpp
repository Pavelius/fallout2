#define _WIN32_WINNT 0x0500
#define WINVER  0x0500
#include <windows.h>
#include "f2lib.h"

struct video_buffer
{
	BITMAPINFO		bmp;
	unsigned char	pallette[256][4];
};

static HWND			hwnd;
static video_buffer	hvid;

char chlower(char c)
{
	return (char)CharLower((char*)c);
}

char chupper(char c)
{
	return (char)CharUpper((char*)c);
}

bool ischa(int c)
{
	return IsCharAlpha(c)!=0;
}

int timeticks()
{
	return GetTickCount();
}

static int tokey(int vk)
{
	switch(vk)
	{
	case VK_CONTROL:
		return Ctrl;
	case VK_MENU:
		return Alt;
	case VK_SHIFT:
		return Shift;
	case VK_LEFT:
		return KeyLeft;
	case VK_RIGHT:
		return KeyRight;
	case VK_UP:
		return KeyUp;
	case VK_DOWN:
		return KeyDown;
	case VK_PRIOR:
		return KeyPageUp;
	case VK_NEXT:
		return KeyPageDown;
	case VK_HOME:
		return KeyHome;
	case VK_DELETE:
		return KeyDelete;
	case VK_END:
		return KeyEnd;
	case VK_BACK:
		return KeyBackspace;
	case VK_RETURN:
		return KeyEnter;
	case VK_ESCAPE:
		return KeyEscape;
	case VK_SPACE:
		return KeySpace;
	case VK_TAB:
		return KeyTab;
	case VK_F1:
		return F1;
	case VK_F2:
		return F2;
	case VK_F3:
		return F3;
	case VK_F4:
		return F4;
	case VK_F5:
		return F5;
	case VK_F6:
		return F6;
	case VK_F7:
		return F7;
	case VK_F8:
		return F8;
	case VK_F9:
		return F9;
	case VK_F10:
		return F10;
	case VK_F11:
		return F11;
	case VK_F12:
		return F12;
	case VK_MULTIPLY:
		return Alpha+(unsigned)'*';
	case VK_DIVIDE:
		return Alpha+(unsigned)'/';
	default:
		return Alpha+vk;
	}
}

static LRESULT CALLBACK windowproc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch(uMsg)
	{
	case WM_ERASEBKGND:
		return 1;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SetDIBitsToDevice(hdc, 0, 0, ui::width, ui::height, 0, 0,
			0, ui::height,
			ui::ptr(0, 0), &hvid.bmp, DIB_RGB_COLORS);
		EndPaint(hwnd, &ps);
		break;
	case WM_CLOSE:
		PostQuitMessage(-1);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

static int handle(HWND hwnd, MSG& msg)
{
	switch(msg.message)
	{
	case WM_MOUSEMOVE:
		if(msg.hwnd!=hwnd)
			break;
		hot::mouse.x = LOWORD(msg.lParam);
		hot::mouse.y = HIWORD(msg.lParam);
		return MouseMove;
	case WM_LBUTTONDOWN:
		if(msg.hwnd!=hwnd)
			break;
		hot::pressed = true;
		return MouseLeft;
	case WM_LBUTTONDBLCLK:
		if(msg.hwnd!=hwnd)
			break;
		hot::pressed = true;
		return MouseLeftDBL;
	case WM_LBUTTONUP:
		if(msg.hwnd!=hwnd)
			break;
		if(!hot::pressed)
			break;
		hot::pressed = false;
		return MouseLeft;
	case WM_RBUTTONDOWN:
		if(msg.hwnd!=hwnd)
			break;
		hot::pressed = true;
		return MouseRight;
	case WM_RBUTTONUP:
		if(msg.hwnd!=hwnd)
			break;
		hot::pressed = false;
		return MouseRight;
	case WM_MOUSEWHEEL:
		if(msg.hwnd!=hwnd)
			break;
		if(msg.wParam&0x80000000)
			return MouseWheelDown;
		else
			return MouseWheelUp;
		break;
	case WM_TIMER:
		if(msg.hwnd!=hwnd)
			break;
		hot::frame++;
		if(msg.wParam==InputTimer)
			return InputTimer;
		break;
	case WM_KEYDOWN:
		return tokey(msg.wParam);
	case WM_CHAR:
		hot::symbol = msg.wParam;
		return InputSymbol;
	case WM_COMMAND:
		if(HIWORD(msg.wParam)==0)
			return msg.lParam;
		break;
	}
	return 0;
}

static const char* regclass()
{
	static WNDCLASS wc = {
		CS_DBLCLKS|CS_VREDRAW|CS_HREDRAW,
		windowproc,
		0,
		0,
		GetModuleHandle(0),
		LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(1)),
		0,
		0,
		0,
		"GuiWindow"
	};
	if(!RegisterClass(&wc))
		return 0;
	return wc.lpszClassName;
}

void ui::timer(int milliseconds)
{
	if(milliseconds)
		SetTimer(hwnd, InputTimer, milliseconds, 0);
	else
		KillTimer(hwnd, InputTimer);
}

bool ui::create(const char* title)
{
	DWORD	dwStyle = WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_VISIBLE; // Windows Style;
	DWORD	dwStyleEx = 0; // Windows Style;
	RECT	rc = {0, 0, width, height};
	// Adjust rectangle
	AdjustWindowRectEx(&rc, dwStyle, 0, dwStyleEx);
	hwnd = CreateWindowEx(dwStyleEx, regclass(), title, dwStyle,	// Required Window Style
		0x80000000, 0x80000000, rc.right-rc.left, rc.bottom-rc.top,
		0, 0, GetModuleHandle(0), 0);
	if(!hwnd)
		return false;
	SetFocus(hwnd);
	rmset(&hvid, 0, sizeof(hvid));
	hvid.bmp.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	hvid.bmp.bmiHeader.biWidth = ui::width;
	hvid.bmp.bmiHeader.biHeight = -ui::height;
	hvid.bmp.bmiHeader.biPlanes = 1;
	hvid.bmp.bmiHeader.biBitCount = 8;
	hvid.bmp.bmiHeader.biCompression = BI_RGB;
	hvid.bmp.bmiHeader.biSizeImage = ui::width*ui::height;
	scanline = width;
	clipping.x1 = 0;
	clipping.y1 = 0;
	clipping.x2 = width;
	clipping.y2 = height;
	font = res::FONT1;
	scol(ColorText);
	return true;
}

void ui::caption(const char* text)
{
	SetWindowText(hwnd, text);
}

void ui::usepal(unsigned char* p)
{
	rmset(hvid.bmp.bmiColors, 0, 256*4);
	for(int i = 0; i<256; i++)
	{
		if(p[0]!=255)
		{
			hvid.bmp.bmiColors[i].rgbRed = p[0]*4;
			hvid.bmp.bmiColors[i].rgbGreen = p[1]*4;
			hvid.bmp.bmiColors[i].rgbBlue = p[2]*4;
		}
		p += 3;
	}
}

void ui::inputex()
{
	MSG	msg;
	hot::symbol = 0;
	hot::key = 0;
	InvalidateRect(hwnd, 0, 0);
	UpdateWindow(hwnd);
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))								        // Loop That Runs
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		handle(hwnd, msg);
	}
}

int ui::input()
{
	MSG	msg;
	hot::symbol = 0;
	hot::key = hot::command;
	if(hot::command)
	{
		hot::command = 0;
		return hot::key;
	}
	// After create window prepare
	InvalidateRect((HWND)hwnd, 0, 0);
	UpdateWindow((HWND)hwnd);
	while(GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		hot::key = handle((HWND)hwnd, msg);
		if(hot::key)
		{
			switch(hot::key)
			{
			case InputSymbol:
				break;
			case InputTimer:
				break;
			default:
				if(hot::key>=(int)KeyLeft)
				{
					if(GetKeyState(VK_SHIFT)<0)
						hot::key |= Shift;
					if(GetKeyState(VK_MENU)<0)
						hot::key |= Alt;
					if(GetKeyState(VK_CONTROL)<0)
						hot::key |= Ctrl;
				}
				break;
			}
			return hot::key;
		}
	}
	return 0;
}

namespace io
{

	file::file() : handle(0)
	{
	}

	file::file(const char* url, access right) : handle(0)
	{
		create(url, right);
	}

	bool file::create(const char* url, access right)
	{
		if(handle)
			return true;
		switch(right)
		{
		case Read:
			handle = CreateFile(url, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			break;
		case Write:
			handle = CreateFile(url, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			break;
		case ReadWrite:
			handle = CreateFile(url, GENERIC_WRITE|GENERIC_READ, 0, 0, 0, FILE_ATTRIBUTE_NORMAL, 0);
			break;
		}
		if(handle==INVALID_HANDLE_VALUE)
			handle = 0;
		return isvalid();
	}

	file::~file()
	{
		close();
	}

	void file::close()
	{
		CloseHandle(handle);
		handle = 0;
	}

	bool file::isvalid() const
	{
		return handle!=0;
	}

	int file::read(void* p, int size)
	{
		DWORD sot;
		if(!ReadFile(handle, p, size, &sot, 0))
			return 0;
		return sot;
	}

	int file::write(const void* p, int size)
	{
		DWORD sot;
		if(!WriteFile(handle, p, size, &sot, 0))
			return 0;
		return sot;
	}

	int file::seek(int pos, relocations rel)
	{
		switch(rel)
		{
		case Begin:
			return SetFilePointer(handle, pos, 0, FILE_BEGIN);
		case Current:
			return SetFilePointer(handle, pos, 0, FILE_CURRENT);
		case End:
			return SetFilePointer(handle, pos, 0, FILE_END);
		}
		return 0;
	}

	void* file::load(const char* url, int* size, int additional)
	{
		void* p = 0;
		DWORD size_of_transaction = 0;
		if(size)
			*size = 0;
		if(szcmp(url, "res:\\", 5) == 0)
		{
			HMODULE hm = GetModuleHandle(0);
			HRSRC hr = FindResource(hm, url+5, "BIN");
			if(hr)
			{
				size_of_transaction = SizeofResource(hm, hr);
				int size_of_data = size_of_transaction+additional;
				if(size_of_transaction)
				{
					HGLOBAL hp = LoadResource(hm, hr);
					if(hp)
					{
						void* r = LockResource(hp);
						p = new char[size_of_data];
						rmset(p, 0, size_of_data);
						rmcpy(p, r, size_of_transaction);
					}
				}
			}
		}
		else
		{
			HANDLE f = CreateFile(url, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			if(f==INVALID_HANDLE_VALUE)
				return 0;
			int size_of_data = GetFileSize(f, 0)+additional;
			p = new char[size_of_data];
			rmset(p, 0, size_of_data);
			ReadFile(f, p, size_of_data-additional, &size_of_transaction, 0);
			CloseHandle(f);
		}
		if(size)
			*size = size_of_transaction;
		return p;
	}

	bool file::exist(const char* url)
	{
		HANDLE h = CreateFile(url, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if(h==INVALID_HANDLE_VALUE)
			return false;
		CloseHandle(h);
		return true;
	}

	void file::remove(const char* url)
	{
		DeleteFile(url);
	}

	const char* file::find(const char* mask)
	{
		static HANDLE           h;
		static WIN32_FIND_DATA  fd;
		if(!mask)
		{
			if(h)
			{
				FindClose(h);
				h = 0;
			}
			return 0;
		}
		else if(h)
		{
			if(!FindNextFile(h, &fd))
			{
				FindClose(h);
				h = 0;
				return 0;
			}
		}
		else
		{
			h = FindFirstFile(mask, &fd);
			if(h==INVALID_HANDLE_VALUE)
			{
				h = 0;
				return 0;
			}
		}
		return fd.cFileName;
	}

	void file::dir(char* path)
	{
		GetCurrentDirectory(MAX_PATH, path);
	}

	stream&	debug()
	{
		static struct writer : public stream
		{
			int read(void* p, int size) { return 0; }
			int write(const void* p, int size)
			{
				OutputDebugString((const char*)p);
				return size;
			}
			int seek(int pos, relocations rel) { return 0; }
		} data;
		return data;
	}
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowMode)
{
	ShowCursor(0);
	int r = ui::main(lpCmdLine);
	if(hwnd)
		DestroyWindow(hwnd);
	hwnd = 0;
	ShowCursor(1);
	return r;
}
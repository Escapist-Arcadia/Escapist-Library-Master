#include<Windows.h>
#include<CommCtrl.h>
#include"resource.h"
#include<crtdbg.h>
#include<ctime>
#include<sql.h>
#include<sqlext.h>
#include<sqltypes.h>
#include"EspString.hpp"
#include"EspArray.hpp"
#include"EspConvert.hpp"
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"comctl32.lib")
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

const char ClassName_Address[] = "EspLibMst_Client_Addr";
const char ClassName_Login[] = "EspLibMst_Client_Login";
const char ClassName_Register[] = "EspLibMst_Client_Register";
const char ClassName_Library[] = "EspLibMst_Client_Library";
const char ClassName_Console[] = "EspLibMst_Client_Console";

const char WindowName_Address[] = "Input Server Address";
const char WindowName_Login[] = "Please Login - Escapist Library Master";
const char WindowName_Register[] = "Register - Escapist Library Master";
const char WindowName_Library[] = "Library List - Escapist Library Master";
const char WindowName_Console[] = "System Console - Escapist Library Master";

unsigned char Pack_Head[3] = { 'E','L','M' };
unsigned char Pack_Tail[3] = { 'E','N','D' };
unsigned char Pack_Version[2] = { 1,0 };
unsigned char Pack_Sign_Login[2] = { 'D','L' };
unsigned char Pack_Sign_Library[2] = { 'L','B' };
unsigned char Pack_Sign_Book[2] = { 'B','K' };

HINSTANCE InstanceHandle;
HFONT Font_Caption = ::CreateFontA(32, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FF_DONTCARE, "微软雅黑");
HFONT Font_Login = ::CreateFontA(20, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "微软雅黑");
HFONT Font_Normal = ::CreateFontA(18, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "微软雅黑");

HWND Window_Address;
HWND Window_Login;
HWND Window_Register;
HWND Window_Library;
HWND Window_Console;

HWND Label_Address_Address;
HWND IPAddress_Address_Address;
HWND Button_Address_Connect;

HWND Label_Login_Caption;
HWND Label_Login_Account, Edit_Login_Account;
HWND Label_Login_Password, Edit_Login_Password;
HWND Label_Login_Mode, ComboBox_Login_Mode;
HWND Button_Login_Register, Button_Login_Login;

HWND Label_Console_Address;
HWND Label_Console_UserInfo;
HWND ListBox_Console_FuncList;
HWND Label_Console_Parameter, Edit_Console_Parameter;
HWND Button_Console_Request;
HWND Label_Console_Response, Edit_Console_Response;

EspString TempBuffer;
unsigned char IPAddress[4] = { 0 };
EspString IPAddressString;
SOCKET Socket;
unsigned char SendBuffer[8192] = { 0 };
unsigned char RecvBuffer[8192] = { 0 };
unsigned char Key[16] = { 0 };
EspString AccountString;
unsigned char AccountHex[4] = { 0 };
unsigned long Account = 0;

long __stdcall WndProc_Address(HWND WindowHandle, unsigned int Message, unsigned int wParam, long lParam)
{
	switch (Message)
	{
	case WM_CREATE:
	{
		RECT Rect;
		GetWindowRect(WindowHandle, &Rect);
		SetWindowPos(WindowHandle, HWND_TOP, ((GetSystemMetrics(SM_CXSCREEN) - Rect.right) / 2), ((GetSystemMetrics(SM_CYSCREEN) - Rect.bottom) / 2), Rect.right, Rect.bottom, SWP_SHOWWINDOW);

		::SendMessageA(Label_Address_Address = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "请输入服务端地址", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | SS_CENTERIMAGE, 40, 32, 300, 40, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Caption, false);
		::SendMessageA(IPAddress_Address_Address = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_IPADDRESSA, NULL, WS_VISIBLE | WS_CHILD, 40, 86, 300, 40, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Caption, false);
		::SendMessageA(Button_Address_Connect = ::CreateWindowExA(WS_EX_LEFT, WC_BUTTONA, "立即连接", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 115, 140, 150, 40, WindowHandle, (HMENU)10001, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Caption, false);
		::SetFocus(IPAddress_Address_Address);

		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT PaintStruct;
		HDC DCHandle = BeginPaint(WindowHandle, &PaintStruct);
		//添加绘图代码
		EndPaint(WindowHandle, &PaintStruct);
		break;
	}
	case WM_CTLCOLORSTATIC:
	{
		if ((HWND)wParam == Label_Address_Address)
			::SetBkColor((HDC)wParam, RGB(0, 0, 0));
		break;
	}
	case WM_COMMAND:
	{
		if (HIWORD(wParam) == BN_CLICKED)
		{
			switch (LOWORD(wParam))
			{
			case 10001:
			{
				//链接到服务端和数据库
				::SendMessageA(IPAddress_Address_Address, IPM_GETADDRESS, NULL, (long)IPAddress);
				if (IPAddress[3] == 0 || IPAddress[3] == 255 || IPAddress[0] == 255)
				{
					::MessageBoxExA(NULL, "无效的地址！请重新输入", "错误！", MB_OK | MB_ICONERROR, 0);
					return ::DefWindowProcA(WindowHandle, Message, wParam, lParam);
				}
				EspConvert::ToString(IPAddress[3], TempBuffer);
				IPAddressString.Append(TempBuffer).Append('.');
				EspConvert::ToString(IPAddress[2], TempBuffer);
				IPAddressString.Append(TempBuffer).Append('.');
				EspConvert::ToString(IPAddress[1], TempBuffer);
				IPAddressString.Append(TempBuffer).Append('.');
				EspConvert::ToString(IPAddress[0], TempBuffer);
				IPAddressString.Append(TempBuffer);
				::EnableWindow(Button_Address_Connect, false);
				::SetWindowTextA(Button_Address_Connect, "连接中......");
				Socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				if (Socket == INVALID_SOCKET)
					::PostQuitMessage(0);
				sockaddr_in SocketAddr;
				SocketAddr.sin_family = AF_INET;
				SocketAddr.sin_port = ::htons(6666);
				SocketAddr.sin_addr.S_un.S_addr = ::inet_addr(IPAddressString.GetAnsiStr());
				if (::connect(Socket, (sockaddr*)&SocketAddr, sizeof(sockaddr_in)))
					::PostQuitMessage(0);

				Window_Login = ::CreateWindowExA(WS_EX_CLIENTEDGE, ClassName_Login, WindowName_Login, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 0, 0, 330, 220, NULL, NULL, InstanceHandle, NULL);
				if (!Window_Login)
					return 0;
				::ShowWindow(Window_Login, SW_SHOW);
				::UpdateWindow(Window_Login);
				::SetForegroundWindow(Window_Login);
				::DestroyWindow(Window_Address);
				break;
			}
			}
		}
		break;
	}
	case WM_DESTROY:
	{
		if (Window_Login == NULL)
			::PostQuitMessage(0);
		break;
	}
	default:
	{
		return DefWindowProcA(WindowHandle, Message, wParam, lParam);
	}
	}
	return 0;
}
long __stdcall WndProc_Login(HWND WindowHandle, unsigned int Message, unsigned int wParam, long lParam)
{
	switch (Message)
	{
	case WM_CREATE:
	{

		RECT Rect;
		GetWindowRect(WindowHandle, &Rect);
		SetWindowPos(WindowHandle, HWND_TOP, ((GetSystemMetrics(SM_CXSCREEN) - Rect.right) / 2), ((GetSystemMetrics(SM_CYSCREEN) - Rect.bottom) / 2), Rect.right, Rect.bottom, SWP_SHOWWINDOW);

		::SendMessageA(Label_Login_Caption = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "Escapist Library Master", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | SS_CENTERIMAGE, 8, 8, 295, 40, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Caption, false);
		::SendMessageA(Label_Login_Account = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "账号:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | SS_CENTERIMAGE, 38, 56, 40, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, false);
		::SendMessageA(Edit_Login_Account = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_EDITA, NULL, WS_VISIBLE | WS_CHILD, 82, 56, 186, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, false);
		::SendMessageA(Label_Login_Password = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "密码:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | SS_CENTERIMAGE, 38, 86, 40, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, false);
		::SendMessageA(Edit_Login_Password = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_EDITA, NULL, WS_VISIBLE | WS_CHILD | ES_PASSWORD, 82, 86, 186, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, false);
		::SendMessageA(Label_Login_Mode = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "登录方式:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | SS_CENTERIMAGE, 38, 116, 73, 28, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, false);
		::SendMessageA(ComboBox_Login_Mode = ::CreateWindowExA(WS_EX_LEFT, WC_COMBOBOXA, NULL, WS_VISIBLE | WS_CHILD | CBS_HASSTRINGS | CBS_DROPDOWNLIST, 115, 116, 154, 28, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, false);
		::SendMessageA(Button_Login_Register = ::CreateWindowExA(WS_EX_LEFT, WC_BUTTONA, "注册账号", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 38, 148, 93, 24, WindowHandle, (HMENU)10001, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, false);
		::SendMessageA(Button_Login_Login = ::CreateWindowExA(WS_EX_LEFT, WC_BUTTONA, "立即登录", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 135, 148, 134, 24, WindowHandle, (HMENU)10002, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, false);
		::SendMessageA(ComboBox_Login_Mode, CB_ADDSTRING, NULL, (long)"后台系统控制台模式");
		::SendMessageA(ComboBox_Login_Mode, CB_ADDSTRING, NULL, (long)"客户端UI界面模式");
		::SendMessageA(ComboBox_Login_Mode, CB_SETCURSEL, 0, NULL);
		::SetFocus(Edit_Login_Account);
		break;
	}
	case WM_CTLCOLORSTATIC:
	{
		if ((HWND)wParam == Label_Login_Caption)
			::SetBkColor((HDC)wParam, RGB(0, 0, 0));
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT PaintStruct;
		HDC DCHandle = BeginPaint(WindowHandle, &PaintStruct);
		//添加绘图代码
		EndPaint(WindowHandle, &PaintStruct);
		break;
	}
	case WM_COMMAND:
	{
		if (HIWORD(wParam) == BN_CLICKED)
		{
			switch (LOWORD(wParam))
			{
			case 10001:
			{
				::EnableWindow(Window_Login, false);
				Window_Register = ::CreateWindowExA(WS_EX_CLIENTEDGE, ClassName_Register, WindowName_Register, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 0, 0, 330, 220, NULL, NULL, InstanceHandle, NULL);
				if (!Window_Register)
					return 0;
				::ShowWindow(Window_Register, SW_SHOW);
				::UpdateWindow(Window_Register);
				MSG Message;
				while (GetMessageA(&Message, NULL, 0, 0))
				{
					TranslateMessage(&Message);
					DispatchMessageA(&Message);
				}
				EnableWindow(Window_Login, true);
				SetForegroundWindow(Window_Login);
				break;
			}
			case 10002:
			{
				::GetWindowTextA(Edit_Login_Account, AccountString.GetBuffer(::GetWindowTextLengthA(Edit_Login_Account) + 1), ::GetWindowTextLengthA(Edit_Login_Account) + 1);
				Account = EspConvert::ParseLong(AccountString.RefreshLength());
				::memcpy(AccountHex, &Account, sizeof(unsigned long));

				//登陆验证
				unsigned char* Pos = SendBuffer;
				::memcpy(Pos, Pack_Head, 3);
				::memcpy(Pos += 3, Pack_Version, 2);
				::memcpy(Pos += 2, Pack_Sign_Login, 2);
				::memcpy(Pos += 2, AccountHex, 4);
				::srand((unsigned)::time(NULL));
				for (unsigned int TimeNum = 0; TimeNum < 16; TimeNum++)
					Key[TimeNum] = ::rand() % 255;
				::memcpy(Pos += 4, Key, 16);
				::memcpy(Pos += 16, IPAddress, 4);
				unsigned long Time = ::time(NULL);
				::memcpy(Pos += 4, &Time, 4);
				::memcpy(Pos += 4, AccountHex, 4);
				::memcpy(Pos += 1, Pack_Tail, 3);
				::send(Socket, (char*)SendBuffer, Pos + 1 - SendBuffer, 0);


				if (::SendMessageA(ComboBox_Login_Mode, CB_GETCURSEL, NULL, NULL) == 0)
				{
					Window_Console = ::CreateWindowExA(WS_EX_CLIENTEDGE, ClassName_Console, WindowName_Console, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 0, 0, 400, 600, NULL, NULL, InstanceHandle, NULL);
					if (!Window_Console)
						return 0;
					::ShowWindow(Window_Console, SW_SHOW);
					::UpdateWindow(Window_Console);
					::SetForegroundWindow(Window_Console);
					::DestroyWindow(Window_Login);
				}
				else
				{
					Window_Library = ::CreateWindowExA(WS_EX_CLIENTEDGE, ClassName_Library, WindowName_Library, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 0, 0, 750, 500, NULL, NULL, InstanceHandle, NULL);
					if (!Window_Library)
						return 0;
					::ShowWindow(Window_Library, SW_SHOW);
					::UpdateWindow(Window_Library);
					::SetForegroundWindow(Window_Library);
					::DestroyWindow(Window_Login);
				}
				break;
			}
			}
		}

		break;
	}
	case WM_DESTROY:
	{
		if (Window_Library == NULL && Window_Console == NULL)
			PostQuitMessage(0);
		break;
	}
	default:
	{
		return DefWindowProcA(WindowHandle, Message, wParam, lParam);
	}
	}
	return 0;
}
long __stdcall WndProc_Register(HWND WindowHandle, unsigned int Message, unsigned int wParam, long lParam)
{
	switch (Message)
	{
	case WM_CREATE:
	{
		RECT Rect;
		GetWindowRect(WindowHandle, &Rect);
		SetWindowPos(WindowHandle, HWND_TOP, ((GetSystemMetrics(SM_CXSCREEN) - Rect.right) / 2), ((GetSystemMetrics(SM_CYSCREEN) - Rect.bottom) / 2), Rect.right, Rect.bottom, SWP_SHOWWINDOW);

		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT PaintStruct;
		HDC DCHandle = BeginPaint(WindowHandle, &PaintStruct);
		//添加绘图代码
		EndPaint(WindowHandle, &PaintStruct);
		break;
	}
	case WM_DESTROY:
	{
		::PostQuitMessage(0);
		break;
	}
	default:
	{
		return DefWindowProcA(WindowHandle, Message, wParam, lParam);
	}
	}
	return 0;
}
long __stdcall WndProc_Console(HWND WindowHandle, unsigned int Message, unsigned int wParam, long lParam)
{
	switch (Message)
	{
	case WM_CREATE:
	{
		RECT Rect;
		GetWindowRect(WindowHandle, &Rect);
		SetWindowPos(WindowHandle, HWND_TOP, ((GetSystemMetrics(SM_CXSCREEN) - Rect.right) / 2), ((GetSystemMetrics(SM_CYSCREEN) - Rect.bottom) / 2), Rect.right, Rect.bottom, SWP_SHOWWINDOW);

		::SendMessageA(Label_Console_Address = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, EspString("服务端地址: ").Append(IPAddressString).GetAnsiStr(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 8, 8, 365, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, false);
		::SendMessageA(Label_Console_UserInfo = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, EspString("用户: ").Append("系统管理员(100000)").GetAnsiStr(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 8, 38, 365, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, false);
		::SendMessageA(ListBox_Console_FuncList = ::CreateWindowExA(WS_EX_LEFT, WC_LISTBOXA, NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_HASSTRINGS, 8, 68, 365, 140, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Normal, false);
		::SendMessageA(ListBox_Console_FuncList, LB_ADDSTRING, NULL, (long)"101 查看图书馆列表");
		::SendMessageA(Label_Console_Parameter = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "参数列表", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 8, 192, 60, 125, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, false);
		::SendMessageA(Edit_Console_Parameter = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_EDITA, NULL, WS_VISIBLE | WS_CHILD | ES_MULTILINE, 72, 192, 301, 125, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Normal, false);
		::SendMessageA(Button_Console_Request = ::CreateWindowExA(WS_EX_LEFT, WC_BUTTONA, "Send Request", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 8, 321, 365, 26, WindowHandle, (HMENU)40001, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, false);
		::SendMessageA(Label_Console_Parameter = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "返回列表", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 8, 351, 60, 193, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, false);
		::SendMessageA(Edit_Console_Parameter = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_EDITA, NULL, WS_VISIBLE | WS_CHILD | ES_MULTILINE, 72, 351, 301, 193, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Normal, false);

		break;
	}
	case WM_CTLCOLORSTATIC:
	{
		if ((HWND)wParam == Label_Console_Address)
			::SetBkColor((HDC)wParam, RGB(0, 0, 0));
		if ((HWND)wParam == Label_Console_UserInfo)
			::SetBkColor((HDC)wParam, RGB(0, 0, 0));
		if ((HWND)wParam == ListBox_Console_FuncList)
			::SetBkColor((HDC)wParam, RGB(0, 0, 0));
		if ((HWND)wParam == Edit_Console_Parameter)
			::SetBkColor((HDC)wParam, RGB(0, 255, 150));
		break;
	}
	case WM_COMMAND:
	{
		if (HIWORD(wParam) == BN_CLICKED)
		{
			switch (LOWORD(wParam))
			{
			case 40001:
			{
				::MessageBeep(MB_HELP);
				break;
			}
			}
		}
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT PaintStruct;
		HDC DCHandle = BeginPaint(WindowHandle, &PaintStruct);
		//添加绘图代码
		EndPaint(WindowHandle, &PaintStruct);
		break;
	}
	case WM_DESTROY:
	{
		::PostQuitMessage(0);
		break;
	}
	default:
	{
		return DefWindowProcA(WindowHandle, Message, wParam, lParam);
	}
	}
	return 0;
}
long __stdcall WndProc_Library(HWND WindowHandle, unsigned int Message, unsigned int wParam, long lParam)
{
	switch (Message)
	{
	case WM_CREATE:
	{
		RECT Rect;
		GetWindowRect(WindowHandle, &Rect);
		SetWindowPos(WindowHandle, HWND_TOP, ((GetSystemMetrics(SM_CXSCREEN) - Rect.right) / 2), ((GetSystemMetrics(SM_CYSCREEN) - Rect.bottom) / 2), Rect.right, Rect.bottom, SWP_SHOWWINDOW);

		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT PaintStruct;
		HDC DCHandle = BeginPaint(WindowHandle, &PaintStruct);
		//添加绘图代码
		EndPaint(WindowHandle, &PaintStruct);
		break;
	}
	case WM_DESTROY:
	{
		::PostQuitMessage(0);
		break;
	}
	default:
	{
		return DefWindowProcA(WindowHandle, Message, wParam, lParam);
	}
	}
	return 0;
}

int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE PreInstanceHandle, _In_ char* lpCmdLine, _In_ int nCmdShow)
{
	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	WSAData Data;
	if (::WSAStartup(MAKEWORD(2, 2), &Data))
		return 0;


	InstanceHandle = hInstance;

	WNDCLASSEXA Class_Address, Class_Login, Class_Register, Class_Library, Class_Console;
	Class_Address.cbSize = sizeof(WNDCLASSEXA);
	Class_Address.style = CS_HREDRAW | CS_VREDRAW;
	Class_Address.cbClsExtra = 0;
	Class_Address.cbWndExtra = 0;
	Class_Address.cbClsExtra = 0;
	Class_Address.hInstance = hInstance;
	Class_Address.hIcon = ::LoadIconA(hInstance, MAKEINTRESOURCEA(IDI_ICON_MAIN));
	Class_Address.hCursor = ::LoadCursorA(hInstance, MAKEINTRESOURCEA(32512));
	Class_Address.hbrBackground = ::CreateSolidBrush(RGB(255, 255, 255));
	Class_Address.lpszMenuName = NULL;
	Class_Address.hIconSm = ::LoadIconA(hInstance, MAKEINTRESOURCEA(IDI_ICON_MAIN));
	::memcpy(&Class_Login, &Class_Address, sizeof(WNDCLASSEXA));
	::memcpy(&Class_Register, &Class_Address, sizeof(WNDCLASSEXA));
	::memcpy(&Class_Library, &Class_Address, sizeof(WNDCLASSEXA));
	::memcpy(&Class_Console, &Class_Address, sizeof(WNDCLASSEXA));

	Class_Address.lpfnWndProc = (WNDPROC)WndProc_Address;
	Class_Address.lpszClassName = ClassName_Address;
	if (!::RegisterClassExA(&Class_Address))
		return 0;

	Class_Login.lpfnWndProc = (WNDPROC)WndProc_Login;
	Class_Login.lpszClassName = ClassName_Login;
	if (!::RegisterClassExA(&Class_Login))
		return 0;

	Class_Register.lpfnWndProc = (WNDPROC)WndProc_Register;
	Class_Register.lpszClassName = ClassName_Register;
	if (!::RegisterClassExA(&Class_Register))
		return 0;

	Class_Library.lpfnWndProc = (WNDPROC)WndProc_Library;
	Class_Library.lpszClassName = ClassName_Library;
	if (!::RegisterClassExA(&Class_Library))
		return 0;

	Class_Console.lpfnWndProc = (WNDPROC)WndProc_Console;
	Class_Console.lpszClassName = ClassName_Console;
	if (!::RegisterClassExA(&Class_Console))
		return 0;

	Window_Address = ::CreateWindowExA(WS_EX_CLIENTEDGE, ClassName_Address, WindowName_Address, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 0, 0, 400, 250, NULL, NULL, hInstance, NULL);
	if (!Window_Address)
		return 0;
	::ShowWindow(Window_Address, SW_SHOW);
	::UpdateWindow(Window_Address);
	MSG Message;
	while (GetMessageA(&Message, nullptr, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessageA(&Message);
	}

	::closesocket(Socket);
	::WSACleanup();
	return 0;
}
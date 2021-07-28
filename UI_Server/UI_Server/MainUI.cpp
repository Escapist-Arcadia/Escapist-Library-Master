#include<Windows.h>
#include<CommCtrl.h>
#include<crtdbg.h>
#include<ctime>
#include"resource.h"
#include<gdiplus.h>
#include<sql.h>
#include<sqlext.h>
#include<sqltypes.h>
#include"EspString.hpp"
#include"EspArray.hpp"
#include"EspConvert.hpp"

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"Gdiplus.lib")
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

using namespace Gdiplus;

const char ClassName_Login[] = "EspLibMst_Server_Login";
const char ClassName_NetMonitor[] = "EspLibMst_Server_NetMoninor";
const char ClassName_Log[] = "EspLibMst_Server_Log";
const char ClassName_User[] = "EspLibMst_Server_User";

const char WindowName_Login[] = "Please Login - Escapist Library Master";
const char WindowName_Log[] = "Log - Escapist Library Master";
const char WindowName_User[] = "User List - Escapist Library Master";

POINT Point_Screen, Point_Client;
SOCKET Socket, Socket_Cnet;
char HostName[128] = { 0 };
EspArray<unsigned long> IPAddressArr;
EspArray<EspString> IPAddressStringArr;
unsigned long IPAddress;
EspString IPAddressString;

unsigned int RecvLength;
unsigned char RecvBuffer[8192] = { 0 };

HINSTANCE InstanceHandle;
HFONT Font_Caption = ::CreateFontA(32, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FF_DONTCARE, "微软雅黑");
HFONT Font_Login = ::CreateFontA(20, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "微软雅黑");
HFONT Font_Normal = ::CreateFontA(18, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "微软雅黑");
bool IsDown = false;
RectF Rect_Time = { 70,0,75,20 };
RectF Rect_Count = { 70,20,75,20 };
HANDLE ThreadHandle;
unsigned long ThreadID;

unsigned char Pack_Head[3] = { 'E','L','M' };
unsigned char Pack_Tail[3] = { 'E','N','D' };
unsigned char Pack_Version[2] = { 1,0 };
unsigned char Pack_Sign_Login[2] = { 'D','L' };
unsigned char Pack_Sign_Library[2] = { 'L','B' };
unsigned char Pack_Sign_Book[2] = { 'B','K' };

SQLHENV SQL_HEnv;
SQLHDBC SQL_HDbc;
SQLHSTMT SQL_HStmt;
EspString  SQL_ExecCode;

HWND Window_Login;
HWND Window_NetMonitor; HMENU Menu_NM, Menu_NM_Sub;
HWND Window_Log;
HWND Window_User;

HWND Label_Login_Caption;
HWND Label_Login_IPAddressArr, ComboBox_Login_IPAddressArr;
HWND Label_Login_Account, Edit_Login_Account;
HWND Label_Login_Password, Edit_Login_Password;
HWND Button_Login_Login;

HWND ListView_Log_Log;
HWND Label_Log_Time, Edit_Log_Time;
HWND Label_Log_IPAddressArr, Edit_Log_IPAddressArr;
HWND Label_Log_User, Edit_Log_User;
HWND Label_Log_FuncID, Edit_Log_FuncID;
HWND Label_Log_Authority, ComboBox_Log_Authority;
HWND Label_Log_FuncAuthority, ComboBox_Log_FuncAuthority;
HWND Label_Log_Return, ComboBox_Log_Return;
HWND Label_Log_Status, ComboBox_Log_Status;
HWND Label_Log_Description, Edit_Log_Description;
HWND Button_Log_Delete;

HWND ListView_User_User;
HWND Label_User_ID, Edit_User_ID;
HWND Label_User_NickName, Edit_User_NickName;
HWND Label_User_Gender, ComboBox_User_Gender;
HWND Label_User_Email, Edit_User_Email;
HWND Label_User_Authority, ComboBox_User_Authority;
HWND Label_User_LastRequest, Edit_User_LastRequest;
HWND Label_User_IPAddressArr, Edit_User_IPAddressArr, IPAddressArr_User_IPAddressArr;
HWND Label_User_Description, Edit_User_Description;
HWND Label_User_Status, ComboBox_User_Status;
HWND Button_User_Modify, Button_User_Delete;

unsigned long Account;
EspString AccountString;

unsigned long __stdcall ThreadProc_Socket(void* lpParameter)
{
	while (true)
	{
		if ((Socket_Cnet = accept(Socket, NULL, NULL)) == -1)
			continue;
		RecvLength = ::recv(Socket_Cnet, (char*)RecvBuffer, 8192, 0);
		if (!::memcmp(RecvBuffer, Pack_Head, 3))
		{
			unsigned char* Pos = RecvBuffer + 3;
			if (!::memcmp(Pos, Pack_Version, 2))
			{
				Pos += 2;
				if (!::memcmp(Pos, Pack_Sign_Login, 2))
				{
					Pos += 2;
					unsigned long Account; ::memcpy(&Account, Pos, 4); Pos += 4;
					unsigned char Key[16]; ::memcpy(Key, Pos, 16); Pos += 16;
					unsigned char UserIP[4]; ::memcpy(UserIP, Pos, 4); Pos += 4;
					unsigned long Time; ::memcpy(&Time, Pos, 4); Pos += 4;

					SQL_ExecCode.Assign(
						"Create Database If Not Exist ELM\r\n"
						""
					);

					//数据库の查询：有无账号，是否已登录等等
					//数据库の写入：时间在哪个IP登录账号是多少，密码对不对，返回成功还是失败等等
					//组返回的包

				}
			}
		}
		::closesocket(Socket_Cnet);
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
		::SendMessageA(Label_Login_IPAddressArr = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "服务端IP:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | SS_CENTERIMAGE, 38, 56, 73, 28, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, false);
		::SendMessageA(ComboBox_Login_IPAddressArr = ::CreateWindowExA(WS_EX_LEFT, WC_COMBOBOXA, NULL, WS_VISIBLE | WS_CHILD | CBS_HASSTRINGS | CBS_DROPDOWNLIST, 115, 56, 154, 28, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, false);
		::SendMessageA(Label_Login_Account = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "账号:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | SS_CENTERIMAGE, 38, 88, 40, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, false);
		::SendMessageA(Edit_Login_Account = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_EDITA, NULL, WS_VISIBLE | WS_CHILD, 82, 88, 186, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, false);
		::SendMessageA(Label_Login_Password = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "密码:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | SS_CENTERIMAGE, 38, 118, 40, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, false);
		::SendMessageA(Edit_Login_Password = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_EDITA, NULL, WS_VISIBLE | WS_CHILD | ES_PASSWORD, 82, 118, 186, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, false);
		::SendMessageA(Button_Login_Login = ::CreateWindowExA(WS_EX_LEFT, WC_BUTTONA, "立即登录", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 38, 148, 230, 24, WindowHandle, (HMENU)10001, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, false);
		for (unsigned int TimeNum = 0; TimeNum < IPAddressStringArr.GetCount(); TimeNum++)
			::SendMessageA(ComboBox_Login_IPAddressArr, CB_ADDSTRING, NULL, (long)IPAddressStringArr.GetElementAt(TimeNum).GetAnsiStr());
		::SendMessageA(ComboBox_Login_IPAddressArr, CB_SETCURSEL, 0, NULL);
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
				if (!SQL_SUCCEEDED(::SQLAllocHandle(SQL_HANDLE_ENV, NULL, &SQL_HEnv)))
					::PostQuitMessage(0);
				if (!SQL_SUCCEEDED(::SQLSetEnvAttr(SQL_HEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, NULL)))
					::PostQuitMessage(0);
				if (!SQL_SUCCEEDED(::SQLAllocHandle(SQL_HANDLE_DBC, SQL_HEnv, &SQL_HDbc)))
					::PostQuitMessage(0);
				unsigned char lpszCnetStr[] = "Driver={SQL Server};Password=sa123;Persist Security Info=True;User ID=sa;Server=LAPTOP-3QSS3CR4\\SQL1;Trusted_Connection=Yes;";
				if (!SQL_SUCCEEDED(::SQLDriverConnectA(SQL_HDbc, NULL, lpszCnetStr, SQL_NTS, NULL, SQL_NTS, NULL, SQL_DRIVER_COMPLETE)))
					::PostQuitMessage(0);
				if (!SQL_SUCCEEDED(::SQLAllocHandle(SQL_HANDLE_STMT, SQL_HDbc, &SQL_HStmt)))
					::PostQuitMessage(0);

				::GetWindowTextA(Edit_Login_Account, AccountString.GetBuffer(::GetWindowTextLengthA(Edit_Login_Account) + 1), ::GetWindowTextLengthA(Edit_Login_Account) + 1);
				Account = EspConvert::ParseULong(AccountString.RefreshLength());

				SQL_ExecCode.Assign(
					"Use ELM\r\n"
					"Select [Password] From [Account_Password] Where [Account]="
				).Append(AccountString);
				unsigned char CorrectPassword[36] = { 0 };
				if (!SQL_SUCCEEDED(::SQLExecDirectA(SQL_HStmt, (unsigned char*)SQL_ExecCode.GetAnsiStr(), SQL_NTS)))
					::PostQuitMessage(0);

				::SQLBindCol(SQL_HStmt, 1, SQL_C_CHAR, (SQLPOINTER)CorrectPassword, 36, 0);
				if (::SQLFetch(SQL_HStmt) != SQL_NO_DATA)
				{
				}
				else
				{

				}
				unsigned char State[10];
				unsigned char Msg[256];
				SQLErrorA(SQL_HEnv, SQL_HDbc, SQL_HStmt, State, NULL, Msg, 256, NULL);


				/*	*/
				IPAddressString = IPAddressStringArr.GetElementAt(::SendMessageA(ComboBox_Login_IPAddressArr, CB_GETCURSEL, NULL, NULL));
				IPAddress = IPAddressArr.GetElementAt(::SendMessageA(ComboBox_Login_IPAddressArr, CB_GETCURSEL, NULL, NULL));
				Socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				if (Socket == INVALID_SOCKET)
					::PostQuitMessage(0);
				sockaddr_in SocketAddr;
				SocketAddr.sin_family = AF_INET;
				SocketAddr.sin_port = ::htons(6666);
				SocketAddr.sin_addr.S_un.S_addr = ::inet_addr(IPAddressString);
				if (::bind(Socket, (sockaddr*)&SocketAddr, sizeof(sockaddr_in)))
					::PostQuitMessage(0);
				if (::listen(Socket, SOMAXCONN))
					::PostQuitMessage(0);
				ThreadHandle = ::CreateThread(NULL, 0, ThreadProc_Socket, NULL, NULL, &ThreadID);


				Window_NetMonitor = ::CreateWindowExA(WS_EX_LEFT|WS_EX_LAYERED, ClassName_NetMonitor, NULL, WS_OVERLAPPED | WS_POPUP, 0, 0, 164, 42, NULL, NULL, InstanceHandle, NULL);
				if (!Window_NetMonitor)
					return 0;
				::ShowWindow(Window_NetMonitor, SW_SHOW);
				::UpdateWindow(Window_NetMonitor);
				::SetForegroundWindow(Window_NetMonitor);
				::DestroyWindow(Window_Login);

				break;
			}
			}
		}

		break;
	}
	case WM_DESTROY:
	{
		if (Window_NetMonitor == NULL)
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
long __stdcall WndProc_NetMonitor(HWND WindowHandle, unsigned int Message, unsigned int wParam, long lParam)
{
	switch (Message)
	{
	case WM_CREATE:
	{
		RECT Rect;
		GetWindowRect(WindowHandle, &Rect);
		SetWindowPos(WindowHandle, HWND_TOP, ((GetSystemMetrics(SM_CXSCREEN) - Rect.right) / 2), ((GetSystemMetrics(SM_CYSCREEN) - Rect.bottom) / 2), Rect.right, Rect.bottom, SWP_SHOWWINDOW);

		Menu_NM = ::CreateMenu();
		Menu_NM_Sub = ::CreatePopupMenu();
		::AppendMenuA(Menu_NM, MF_POPUP, (unsigned int)Menu_NM_Sub, NULL);
		::AppendMenuA(Menu_NM_Sub, MF_STRING | MF_ENABLED, (unsigned int)20001, "启动服务端");
		::AppendMenuA(Menu_NM_Sub, MF_STRING | MF_ENABLED, (unsigned int)20002, "打开日志");
		::AppendMenuA(Menu_NM_Sub, MF_STRING | MF_ENABLED, (unsigned int)20003, "打开用户列表");
		::AppendMenuA(Menu_NM_Sub, MF_STRING | MF_ENABLED, (unsigned int)20004, "打开功能列表");
		::AppendMenuA(Menu_NM_Sub, MF_STRING | MF_ENABLED, (unsigned int)20005, "直接退出");


		::SetMenu(NULL, Menu_NM);
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT PaintStruct;
		HDC DCHandle = BeginPaint(WindowHandle, &PaintStruct);
		Graphics Graphics(DCHandle);
		SolidBrush Brush(Color(255, 0, 200, 255));
		FontFamily FontFamily(L"Consolas");
		Font Font_Time(&FontFamily, 14);
		PointF Point(0, 0);
		Graphics.DrawString(L"E.L.M. Server:", -1, &Font_Time, Point, &Brush);
		Point = { 0,20 };
		wchar_t WIP[15] = { 0 };
		::MultiByteToWideChar(CP_ACP, 0, IPAddressString.GetAnsiStr(), -1, WIP, 15);
		int A = ::lstrlenW(WIP);
		Graphics.DrawString(WIP, -1, &Font_Time, Point, &Brush);
		//添加绘图代码
		EndPaint(WindowHandle, &PaintStruct);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case 20002:
		{
			Window_Log = ::CreateWindowExA(WS_EX_CLIENTEDGE, ClassName_Log, WindowName_Log, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 0, 0, 1000, 500, NULL, NULL, InstanceHandle, NULL);
			if (!Window_Log)
				return 0;
			::ShowWindow(Window_Log, SW_SHOW);
			::UpdateWindow(Window_Log);
			::SetForegroundWindow(Window_Log);
			break;
		}
		case 20003:
		{
			Window_User = ::CreateWindowExA(WS_EX_CLIENTEDGE, ClassName_User, WindowName_User, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 0, 0, 1100, 500, NULL, NULL, InstanceHandle, NULL);
			if (!Window_User)
				return 0;
			::ShowWindow(Window_User, SW_SHOW);
			::UpdateWindow(Window_User);
			::SetForegroundWindow(Window_User);
			break;
		}
		case 20005:
		{
			::PostQuitMessage(0);
			break;
		}
		}
	}
	case WM_TIMER:
	{
		switch (wParam)
		{
		case 30001:
		{

		}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		if (!(::IsWindow(Window_Log) || ::IsWindow(Window_User)))
		{
			IsDown = true;
			::GetCursorPos(&Point_Screen);
			RECT WindowRect;
			::GetWindowRect(WindowHandle, &WindowRect);
			Point_Screen.x = Point_Screen.x - WindowRect.left;
			Point_Screen.y = Point_Screen.y - WindowRect.top;
		}
		break;
	}
	case WM_LBUTTONUP: {
		IsDown = false;
		break;
	}
	case WM_MOUSEMOVE: {
		if (IsDown) {
			POINT CurrPoint;
			::GetCursorPos(&CurrPoint);
			::MoveWindow(WindowHandle, CurrPoint.x - Point_Screen.x, CurrPoint.y - Point_Screen.y, 250, 100, false);
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		POINT CursorPoint;
		::GetCursorPos(&CursorPoint);
		::TrackPopupMenu(Menu_NM_Sub, TPM_LEFTALIGN | TPM_RIGHTBUTTON, CursorPoint.x, CursorPoint.y, 0, WindowHandle, NULL);
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
long __stdcall WndProc_Log(HWND WindowHandle, unsigned int Message, unsigned int wParam, long lParam)
{
	switch (Message)
	{
	case WM_CREATE:
	{
		RECT Rect;
		GetWindowRect(WindowHandle, &Rect);
		SetWindowPos(WindowHandle, HWND_TOP, ((GetSystemMetrics(SM_CXSCREEN) - Rect.right) / 2), ((GetSystemMetrics(SM_CYSCREEN) - Rect.bottom) / 2), Rect.right, Rect.bottom, SWP_SHOWWINDOW);

		::SendMessageA(ListView_Log_Log = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_LISTVIEWA, NULL, WS_VISIBLE | WS_CHILD | WS_VSCROLL | LVS_REPORT, 8, 8, 700, 440, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Normal, NULL);
		::SendMessageA(ListView_Log_Log, LVM_SETEXTENDEDLISTVIEWSTYLE, NULL, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
		LVCOLUMNA Log_Column = { 0 };
		Log_Column.mask = LVCF_TEXT | LVCF_WIDTH;
		Log_Column.cx = 48;
		Log_Column.pszText = (char*)"ID";
		::SendMessageA(ListView_Log_Log, LVM_INSERTCOLUMNA, 0, (long)&Log_Column);
		Log_Column.cx = 126;
		Log_Column.pszText = (char*)"时间";
		::SendMessageA(ListView_Log_Log, LVM_INSERTCOLUMNA, 1, (long)&Log_Column);
		Log_Column.cx = 106;
		Log_Column.pszText = (char*)"IP地址";
		::SendMessageA(ListView_Log_Log, LVM_INSERTCOLUMNA, 2, (long)&Log_Column);
		Log_Column.cx = 75;
		Log_Column.pszText = (char*)"账号";
		::SendMessageA(ListView_Log_Log, LVM_INSERTCOLUMNA, 3, (long)&Log_Column);
		Log_Column.cx = 84;
		Log_Column.pszText = (char*)"用户权限";
		::SendMessageA(ListView_Log_Log, LVM_INSERTCOLUMNA, 4, (long)&Log_Column);
		Log_Column.cx = 84;
		Log_Column.pszText = (char*)"功能权限";
		::SendMessageA(ListView_Log_Log, LVM_INSERTCOLUMNA, 5, (long)&Log_Column);
		Log_Column.cx = 50;
		Log_Column.pszText = (char*)"功能ID";
		::SendMessageA(ListView_Log_Log, LVM_INSERTCOLUMNA, 6, (long)&Log_Column);
		Log_Column.cx = 60;
		Log_Column.pszText = (char*)"返回";
		::SendMessageA(ListView_Log_Log, LVM_INSERTCOLUMNA, 7, (long)&Log_Column);
		Log_Column.cx = 46;
		Log_Column.pszText = (char*)"状态";
		::SendMessageA(ListView_Log_Log, LVM_INSERTCOLUMNA, 8, (long)&Log_Column);
		::SendMessageA(Label_Log_Time = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "请求时间:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 712, 8, 60, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Edit_Log_Time = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_EDITA, NULL, WS_VISIBLE | WS_CHILD, 776, 8, 200, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Label_Log_IPAddressArr = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "请求地址:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 712, 38, 60, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Edit_Log_IPAddressArr = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_EDITA, NULL, WS_VISIBLE | WS_CHILD, 776, 38, 200, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Label_Log_User = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "用户账号:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 712, 68, 60, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Edit_Log_User = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_EDITA, NULL, WS_VISIBLE | WS_CHILD, 776, 68, 200, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Label_Log_FuncID = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "功能ID:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 712, 98, 60, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Edit_Log_FuncID = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_EDITA, NULL, WS_VISIBLE | WS_CHILD, 776, 98, 200, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Label_Log_Authority = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "用户权限:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 712, 128, 60, 28, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(ComboBox_Log_Authority = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_COMBOBOXA, NULL, WS_VISIBLE | WS_CHILD | CBS_HASSTRINGS | CBS_DROPDOWNLIST, 776, 128, 200, 28, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Label_Log_FuncAuthority = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "功能权限:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 712, 128, 60, 28, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(ComboBox_Log_FuncAuthority = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_COMBOBOXA, NULL, WS_VISIBLE | WS_CHILD | CBS_HASSTRINGS | CBS_DROPDOWNLIST, 776, 128, 200, 28, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Label_Log_Authority = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "功能权限:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 712, 160, 60, 28, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(ComboBox_Log_Authority = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_COMBOBOXA, NULL, WS_VISIBLE | WS_CHILD | CBS_HASSTRINGS | CBS_DROPDOWNLIST, 776, 160, 200, 28, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Label_Log_FuncID = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "功能ID:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 712, 160, 60, 28, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Edit_Log_FuncID = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_COMBOBOXA, NULL, WS_VISIBLE | WS_CHILD | CBS_HASSTRINGS | CBS_DROPDOWNLIST, 776, 160, 200, 28, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Label_Log_Return = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "返回:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 712, 192, 60, 28, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(ComboBox_Log_Return = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_COMBOBOXA, NULL, WS_VISIBLE | WS_CHILD | CBS_HASSTRINGS | CBS_DROPDOWNLIST, 776, 192, 200, 28, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Label_Log_Status = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "状态:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 712, 224, 60, 28, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(ComboBox_Log_Return = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_COMBOBOXA, NULL, WS_VISIBLE | WS_CHILD | CBS_HASSTRINGS | CBS_DROPDOWNLIST, 776, 224, 200, 28, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Label_Log_Time = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "简单描述:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 712, 256, 60, 160, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Edit_Log_Time = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_EDITA, NULL, WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE, 776, 256, 200, 160, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Normal, NULL);
		::SendMessageA(Button_Log_Delete = ::CreateWindowExA(WS_EX_LEFT, WC_BUTTONA, "删除此条记录", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 712, 420, 264, 28, WindowHandle, (HMENU)30001, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);

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
		if (HIWORD(wParam))
		{
			switch (LOWORD(wParam))
			{
			case 30001:
				::MessageBeep(MB_HELP);
			}
		}
		break;
	}
	case WM_DESTROY:
	{
		if (Window_NetMonitor == NULL)
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
long __stdcall WndProc_User(HWND WindowHandle, unsigned int Message, unsigned int wParam, long lParam)
{
	switch (Message)
	{
	case WM_CREATE:
	{
		RECT Rect;
		GetWindowRect(WindowHandle, &Rect);
		SetWindowPos(WindowHandle, HWND_TOP, ((GetSystemMetrics(SM_CXSCREEN) - Rect.right) / 2), ((GetSystemMetrics(SM_CYSCREEN) - Rect.bottom) / 2), Rect.right, Rect.bottom, SWP_SHOWWINDOW);

		::SendMessageA(ListView_User_User = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_LISTVIEWA, NULL, WS_VISIBLE | WS_CHILD | WS_VSCROLL | LVS_REPORT, 8, 8, 800, 440, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Normal, NULL);
		::SendMessageA(ListView_User_User, LVM_SETEXTENDEDLISTVIEWSTYLE, NULL, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
		LVCOLUMNA Log_Column = { 0 };
		Log_Column.mask = LVCF_TEXT | LVCF_WIDTH;
		Log_Column.cx = 48;
		Log_Column.pszText = (char*)"ID";
		::SendMessageA(ListView_User_User, LVM_INSERTCOLUMNA, 0, (long)&Log_Column);
		Log_Column.cx = 75;
		Log_Column.pszText = (char*)"账号";
		::SendMessageA(ListView_User_User, LVM_INSERTCOLUMNA, 1, (long)&Log_Column);
		Log_Column.cx = 126;
		Log_Column.pszText = (char*)"昵称";
		::SendMessageA(ListView_User_User, LVM_INSERTCOLUMNA, 2, (long)&Log_Column);
		Log_Column.cx = 36;
		Log_Column.pszText = (char*)"性别";
		::SendMessageA(ListView_User_User, LVM_INSERTCOLUMNA, 3, (long)&Log_Column);
		Log_Column.cx = 126;
		Log_Column.pszText = (char*)"电子邮箱";
		::SendMessageA(ListView_User_User, LVM_INSERTCOLUMNA, 4, (long)&Log_Column);
		Log_Column.cx = 126;
		Log_Column.pszText = (char*)"上次请求时间";
		::SendMessageA(ListView_User_User, LVM_INSERTCOLUMNA, 5, (long)&Log_Column);
		Log_Column.cx = 84;
		Log_Column.pszText = (char*)"用户权限";
		::SendMessageA(ListView_User_User, LVM_INSERTCOLUMNA, 6, (long)&Log_Column);
		Log_Column.cx = 106;
		Log_Column.pszText = (char*)"IP地址";
		::SendMessageA(ListView_User_User, LVM_INSERTCOLUMNA, 7, (long)&Log_Column);
		Log_Column.cx = 46;
		Log_Column.pszText = (char*)"状态";
		::SendMessageA(ListView_User_User, LVM_INSERTCOLUMNA, 8, (long)&Log_Column);

		::SendMessageA(Label_User_ID = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "用户账号:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 812, 8, 60, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Edit_User_ID = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_EDITA, NULL, WS_VISIBLE | WS_CHILD, 876, 8, 200, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Label_User_NickName = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "请求地址:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 812, 38, 60, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Edit_User_NickName = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_EDITA, NULL, WS_VISIBLE | WS_CHILD, 876, 38, 200, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Label_User_Gender = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "用户性别:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 812, 68, 60, 28, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(ComboBox_User_Gender = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_COMBOBOXA, NULL, WS_VISIBLE | WS_CHILD | CBS_HASSTRINGS | CBS_DROPDOWNLIST, 876, 68, 200, 28, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(ComboBox_User_Gender, CB_ADDSTRING, NULL, (long)"男");
		::SendMessageA(ComboBox_User_Gender, CB_ADDSTRING, NULL, (long)"女");
		::SendMessageA(Label_User_Email = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "电子邮箱:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 812, 100, 60, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Edit_User_Email = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_EDITA, NULL, WS_VISIBLE | WS_CHILD, 876, 100, 200, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Label_User_LastRequest = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "上次请求:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 812, 130, 60, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Edit_User_LastRequest = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_EDITA, NULL, WS_VISIBLE | WS_CHILD, 876, 130, 200, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Label_User_Authority = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "用户权限:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 812, 160, 60, 28, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(ComboBox_User_Authority = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_EDITA, NULL, WS_VISIBLE | WS_CHILD, 876, 160, 200, 28, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Label_User_IPAddressArr = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "IP地址:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 812, 192, 60, 56, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Edit_User_IPAddressArr = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_EDITA, NULL, WS_VISIBLE | WS_CHILD, 876, 192, 200, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(IPAddressArr_User_IPAddressArr = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_IPADDRESSA, NULL, WS_VISIBLE | WS_CHILD, 876, 222, 200, 26, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Label_User_Status = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "状态:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 812, 252, 60, 28, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(ComboBox_User_Status = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_COMBOBOXA, NULL, WS_VISIBLE | WS_CHILD | CBS_HASSTRINGS | CBS_DROPDOWNLIST, 876, 252, 200, 28, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Label_User_Description = ::CreateWindowExA(WS_EX_LEFT, WC_STATICA, "简单描述:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 812, 284, 60, 125, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Edit_User_Description = ::CreateWindowExA(WS_EX_CLIENTEDGE, WC_EDITA, NULL, WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE, 876, 284, 200, 125, WindowHandle, NULL, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Normal, NULL);
		::SendMessageA(Button_Log_Delete = ::CreateWindowExA(WS_EX_LEFT, WC_BUTTONA, "删除此条记录", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 812, 413, 130, 32, WindowHandle, (HMENU)30001, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);
		::SendMessageA(Button_Log_Delete = ::CreateWindowExA(WS_EX_LEFT, WC_BUTTONA, "修改此条记录", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 946, 413, 130, 32, WindowHandle, (HMENU)30001, InstanceHandle, NULL), WM_SETFONT, (unsigned int)Font_Login, NULL);

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

	Gdiplus::GdiplusStartupInput Input;
	unsigned long Token;
	Gdiplus::GdiplusStartup(&Token, &Input, NULL);
	WSAData Data;
	if (::WSAStartup(MAKEWORD(2, 2), &Data))
		return 0;
	if (::gethostname(HostName, 128))
		return 0;
	hostent* Hostent;
	if (!(Hostent = ::gethostbyname(HostName)))
		return 0;

	unsigned int Count = 0;
	while (Hostent->h_addr_list[Count] != NULL)
	{
		IPAddressStringArr.AddElement(::inet_ntoa(*(in_addr*)::gethostbyname(HostName)->h_addr_list[Count]));
		Count++;
	}
	for (unsigned int TimeNum = 0; TimeNum < IPAddressStringArr.GetCount(); TimeNum++)
	{
		EspArray<EspString> IPAddressArrPot;
		EspSplitString(IPAddressStringArr.GetElementAt(TimeNum), '.', IPAddressArrPot);
		unsigned char IPAddArr[4] = {
			(unsigned char)EspConvert::ParseUShort(IPAddressArrPot.GetElementAt(3)),
			(unsigned char)EspConvert::ParseUShort(IPAddressArrPot.GetElementAt(2)),
			(unsigned char)EspConvert::ParseUShort(IPAddressArrPot.GetElementAt(1)),
			(unsigned char)EspConvert::ParseUShort(IPAddressArrPot.GetElementAt(0))
		};
		IPAddressArr.AddElement((unsigned long)IPAddArr);
	}
	InstanceHandle = hInstance;

	WNDCLASSEXA Class_Login, Class_NetMonitor, Class_Log, Class_User;
	Class_Login.cbSize = sizeof(WNDCLASSEXA);
	Class_Login.style = CS_HREDRAW | CS_VREDRAW;
	Class_Login.cbClsExtra = 0;
	Class_Login.cbWndExtra = 0;
	Class_Login.cbClsExtra = 0;
	Class_Login.hInstance = hInstance;
	Class_Login.hIcon = ::LoadIconA(hInstance, MAKEINTRESOURCEA(IDI_ICON_MAIN));
	Class_Login.hCursor = ::LoadCursorA(hInstance, MAKEINTRESOURCEA(32512));
	Class_Login.hbrBackground = ::CreateSolidBrush(RGB(255, 255, 255));
	Class_Login.lpszMenuName = NULL;
	Class_Login.hIconSm = ::LoadIconA(hInstance, MAKEINTRESOURCEA(IDI_ICON_MAIN));
	::memcpy(&Class_NetMonitor, &Class_Login, sizeof(WNDCLASSEXA));
	::memcpy(&Class_Log, &Class_Login, sizeof(WNDCLASSEXA));
	::memcpy(&Class_User, &Class_Login, sizeof(WNDCLASSEXA));

	Class_Login.lpfnWndProc = (WNDPROC)WndProc_Login;
	Class_Login.lpszClassName = ClassName_Login;
	if (!::RegisterClassExA(&Class_Login))
		return 0;

	Class_Log.lpfnWndProc = (WNDPROC)WndProc_Log;
	Class_Log.lpszClassName = ClassName_Log;
	if (!::RegisterClassExA(&Class_Log))
		return 0;

	Class_NetMonitor.lpfnWndProc = (WNDPROC)WndProc_NetMonitor;
	Class_NetMonitor.lpszClassName = ClassName_NetMonitor;
	if (!::RegisterClassExA(&Class_NetMonitor))
		return 0;

	Class_User.lpfnWndProc = (WNDPROC)WndProc_User;
	Class_User.lpszClassName = ClassName_User;
	if (!::RegisterClassExA(&Class_User))
		return 0;

	Window_Login = ::CreateWindowExA(WS_EX_CLIENTEDGE, ClassName_Login, WindowName_Login, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 0, 0, 330, 220, NULL, NULL, hInstance, NULL);
	if (!Window_Login)
		return 0;
	::ShowWindow(Window_Login, SW_SHOW);
	::UpdateWindow(Window_Login);
	MSG Message;
	while (GetMessageA(&Message, nullptr, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessageA(&Message);
	}

	::SQLFreeHandle(SQL_HANDLE_STMT, SQL_HStmt);
	::SQLFreeHandle(SQL_HANDLE_DBC, SQL_HDbc);
	::SQLFreeHandle(SQL_HANDLE_ENV, SQL_HEnv);
	::CloseHandle(ThreadHandle);
	Gdiplus::GdiplusShutdown(Token);
	::closesocket(Socket);
	::WSACleanup();
	return 0;
}
// Pass.cpp : Defines the entry point for the application.
//
#pragma warning(disable:4715)
#pragma warning(disable:4996)
#pragma warning(disable:4995)
#pragma warning(disable:4018)
#define _WIN32_WINNT 0x0501
#include "stdafx.h"
#include <windows.h>
//HMODULE Hmod = LoadLibrary(L"comdlg32.dll");
//#include <afxdlgs.h>
//#pragma comment(lib,"Comctl32.dll")
#pragma comment(lib,"AutoAero.lib")
#include "AutoAero.h"
#include "Pass.h"
#include "core.h"
#include <CommDlg.h>
#include <Commctrl.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <string>
#include <WindowsX.h>
#include <Wincrypt.h>
#include "passcrypt.cpp"
#include <Dwmapi.h>
#include <GdiPlus.h>
#include <strsafe.h>
#include <vld.h>
#include "timemgr.h"
#pragma comment(lib,"GdiPlus.lib")
#pragma comment(lib,"Comdlg32.lib")
//#include "md5.h"
//#include "md5win.h"
//#include "crypt.h"
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


#define MAX_LOADSTRING 100
int num=8,savin=0,sa=0,row=0,column=1,n=0,k=0,change=0;
int hidepasswd=1;
char pset[]="abcdefghijklmnopqrstuvwxyz",buffer[10];
char nset[]="0123456789",buffern[10];
char sym[100]="",buffers[10];
int nnum=0,nsym=0,tempnum=2,tempsym=2,filesize=0,listflg=0,lremcou=0,listsel=0,editflag=0,readsize=0,cansave=0;//,savtitle=0;
int req_day=0,req_mon=0,req_year=0;
int tim = tim +((int)time(NULL));
wchar_t number[10],temp[10];
wchar_t symbol[100],master[100];
wchar_t * uname=0,* web=0,* oth=0;//,write[1200];//,newpass[100];
wchar_t alpha[10];
wchar_t snbuf[3];
wchar_t sy[10];
wchar_t * read=0,* write=0,* bufr=0,* mdwrite=0,* mdread=0;
wchar_t md5[33],mdcheck[33];
wchar_t * no;//=core(num,nnum,nsym,nset,pset,tim,sym);
//wchar_t * md;
int pos = 5;
bool opchk=FALSE;
wchar_t szFile[100];
int m=0;
HANDLE hf;
OPENFILENAMEW ofn;
DWORD byteswritten,bytestoread;
LVCOLUMN LvCol;
LVITEM LvItem;
void listman();
void MD5Hash(BYTE *,int,char *);
struct tm * givch = (struct tm *)malloc(sizeof(struct tm));
wchar_t * mon[]={L"Jan",L"Feb",L"Mar",L"Apr",L"May",L"Jun",L"Jul",L"Aug",L"Sep",L"Oct",L"Nov",L"Dec"};
DWORD zero = 0;
DWORD one = 1;
DWORD sz = sizeof(DWORD);
//DWORD colorred;
int colorflag = 0;
COLORREF colorred = RGB(255,25,2);

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
TCHAR szTitle2[MAX_LOADSTRING];
HWND hWnd;
HWND hWndEdit;
HWND hWndButton;
HWND hWndUpDown;
HWND hWndEditUpDown;
HWND hWndCheck;
HWND hWndStatic;
HWND hWndEditAlpha;
HWND hWndEditSymUpDown;
HWND hWndEditSym;
HWND hWndCheckSym;
HWND hWndDebEdit;
HWND hWndMD5Button;
HWND hWndDlgWeb,hWndDlgUname,hWndDlgEdit3,hWndDlgOth,hWndEdit3Chk;
HWND hWndAbtSt;
HWND spDlg1;
HWND hList;
HWND hWndMasterEdit;
HWND hWndViewCheck;
HFONT hFont,hFontCheck,hFontAbt,hFontHpl;
HWND hWndHelpSt;
HWND hWndHelpEdit;
HWND hWndPrefCombo;
//HWND hWndMenu;
HGLOBAL hMem;
HKEY pKey;
DWORD regDisposition;

using namespace Gdiplus;
ULONG_PTR m_GdiplusToken;
Status    m_GdiPlusStatus;

//HCRYPTPROV hCryptProv;
HMENU hMenu;
//MARGINS mar = {-1};

EDITBALLOONTIP alphaballoon;
EDITBALLOONTIP symbolballoon;
EDITBALLOONTIP symsetballoon;
EDITBALLOONTIP alphasymballoon;

wchar_t alphaballoontext[]=L"The total length of PassWd should be greater than number of numeric characters !";
wchar_t balloontitle[]=L"Error";
wchar_t symbolballoontext[]=L"The total length of PassWd should be greater than number of symbols";
wchar_t alphasymballoontext[]=L"The total length of PassWd should be greater than number of symbols plus numeric characters !";
wchar_t symsetballoontext[]=L"Please enter some symbols to choose from";
wchar_t symsetballoontitle[]=L"No Symbol Set";


// Forward declarations of functions included in this code module:
BOOL InitCommonControlsEx();
ATOM				MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR	CALLBACK	Sav(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Master(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	View(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Help(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Pref(HWND, UINT, WPARAM, LPARAM);
LRESULT ProcessCustomDraw (LPARAM);



int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	RegCreateKeyEx(HKEY_CURRENT_USER,L"Software\\\\PassWd Mgr\\\\Socket",NULL,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&pKey,&regDisposition);
	if(regDisposition==REG_CREATED_NEW_KEY)
	{
		RegSetValueEx(pKey,L"day",NULL,REG_DWORD,(CONST BYTE *)&zero,sizeof(DWORD));
		RegSetValueEx(pKey,L"month",NULL,REG_DWORD,(CONST BYTE *)&one,sizeof(DWORD));
		RegSetValueEx(pKey,L"year",NULL,REG_DWORD,(CONST BYTE *)&zero,sizeof(DWORD));
	}
	else
	{
		RegQueryValueEx(pKey,L"day",NULL,NULL,(LPBYTE)&req_day,&sz);
		RegQueryValueEx(pKey,L"month",NULL,NULL,(LPBYTE)&req_mon,&sz);
		RegQueryValueEx(pKey,L"year",NULL,NULL,(LPBYTE)&req_year,&sz);
	};

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PASS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_GdiplusToken, &gdiplusStartupInput, NULL);

   hInst = hInstance; // Store instance handle in our global variable

    hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|BN_SETFOCUS,
		   CW_USEDEFAULT, 0, 280, 350, NULL, NULL, hInstance, NULL);
   //hFont = CreateFont  (12,0,0,0,FW_THIN,TRUE,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
			//			NULL,L"Arial");
   /*HWND hWndButton = CreateWindow( 
    L"Button",   // Predefined class; Unicode assumed. 
    L"Oye!",       // Button text. 
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles. 
    100,         // x position. 
    100,         // y position. 
    100,        // Button width.
    25,        // Button height.
    hWnd,       // Parent window.
    NULL,       // No menu.
    (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), 
    NULL);      // Pointer not needed.
	*/
  
	/*hWndEdit = CreateWindow( L"Edit",
	           L"",
	           WS_VISIBLE|WS_CHILD|WS_BORDER,   
			   40,80,200,25,
	           hWnd,NULL ,
	           (HINSTANCE) GetWindowLong(hWnd, GWL_HINSTANCE),
			   NULL );*/
	//SendMessage(hWndEdit, WM_SETTEXT, 0, (LPARAM)L"I Bring It!"); 

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
//	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Pass File\0*.pass\0All Files\0*.*";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = new wchar_t[256];
	ofn.nMaxFileTitle = 256;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

   if (!hWnd)
   {
      return FALSE;
   }
   
   ShowWindow(hWnd, nCmdShow);
//   AeroAutoSubclass(hWnd, ASC_NO_FRAME_EXTENSION, 0L);
//   AnimateWindow(hWnd,200,AW_CENTER|AW_ACTIVATE|AW_SLIDE);
   UpdateWindow(hWnd);

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PASS));
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	/*	if(!IsDialogMessage(spDlg1, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}*/
	};
	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH) 2;
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PASS);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassEx(&wcex);
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	//wchar_t * itow(num,editnum,10);
	//RECT rect;
	//sprintf(number,"%d",num);
	hMenu = GetMenu(hWnd);
	COLORREF color = RGB(200, 201, 202);
	switch (message)
	{
	case WM_CREATE:
		
		hWndEdit = CreateWindowEx(	WS_EX_STATICEDGE,L"Edit",NULL,ES_AUTOHSCROLL|WS_VISIBLE|WS_CHILD|WS_BORDER,   
									10,250,253,25,hWnd,NULL,
									(HINSTANCE) GetWindowLong(hWnd, GWL_HINSTANCE),NULL );
		hWndStatic = CreateWindow(	L"STATIC",L"Length of Generated PassWd:",WS_VISIBLE|WS_CHILD,   
									10,10,200,25,hWnd,NULL,
									(HINSTANCE) GetWindowLong(hWnd, GWL_HINSTANCE),NULL );
		hWndEditUpDown = CreateWindowEx(	WS_EX_STATICEDGE,L"Edit",NULL,ES_NUMBER|WS_VISIBLE|WS_CHILD|WS_BORDER|WS_EX_STATICEDGE,   
										10,40,103,25,hWnd,(HMENU)IDC_UPD,
										(HINSTANCE) GetWindowLong(hWnd, GWL_HINSTANCE),NULL );
		hWndEditAlpha = CreateWindowEx(	WS_EX_STATICEDGE,L"Edit",NULL,ES_NUMBER|WS_VISIBLE|WS_CHILD|WS_BORDER|WS_EX_STATICEDGE,   
										160,70,103,25,hWnd,(HMENU)IDC_ALPHA,
										(HINSTANCE) GetWindowLong(hWnd, GWL_HINSTANCE),NULL );
		hWndEditSymUpDown = CreateWindowEx(	WS_EX_STATICEDGE,L"Edit",NULL,ES_NUMBER|WS_VISIBLE|WS_CHILD|WS_BORDER|WS_EX_STATICEDGE,   
										160,100,103,25,hWnd,(HMENU)IDC_SYMUPD,
										(HINSTANCE) GetWindowLong(hWnd, GWL_HINSTANCE),NULL );
		hWndEditSym = CreateWindowEx(	WS_EX_STATICEDGE,L"Edit",NULL,ES_AUTOHSCROLL|WS_VISIBLE|WS_CHILD|WS_BORDER|WS_EX_STATICEDGE,   
									10,140,253,25,hWnd,(HMENU)IDC_SYM,
									(HINSTANCE) GetWindowLong(hWnd, GWL_HINSTANCE),NULL );
		Edit_Enable(hWndEditSym,FALSE);
	/*	hWndUpDown = CreateWindow(	L"msctls_updown32",NULL,WS_VISIBLE|WS_CHILD,   
									60,20,20,25,hWnd,NULL,
									(HINSTANCE) GetWindowLong(hWnd, GWL_HINSTANCE),NULL );
	/*	hWndUpdate = CreateWindow( 	L"Button",L"Update",WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
									300,20,100,25,hWnd,(HMENU)IDC_UPD,
									(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);*/
		CreateUpDownControl(		UDS_ALIGNRIGHT|UDS_SETBUDDYINT|WS_CHILD|WS_VISIBLE|WS_BORDER,
									NULL,NULL,20,25,hWnd,ALPHA,
									hInst,hWndEditAlpha,99,0,2);
		CreateUpDownControl(		UDS_ALIGNRIGHT|UDS_SETBUDDYINT|WS_CHILD|WS_VISIBLE|WS_BORDER,
									NULL,NULL,20,25,hWnd,ALPHA,
									hInst,hWndEditSymUpDown,99,0,2);
		CreateUpDownControl(		UDS_ALIGNRIGHT|UDS_SETBUDDYINT|WS_CHILD|WS_VISIBLE|WS_BORDER,
									NULL,NULL,20,25,hWnd,UPDOWN,
									hInst,hWndEditUpDown,99,0,8);
		hWndButton = CreateWindow( 	L"Button",L"Generate",WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
									85,215,103,20,hWnd,(HMENU)IDC_BTN,
									(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
		hWndCheck = CreateWindow( 	L"button",L"Alphanumeric",WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
									10,70,103,20,hWnd,(HMENU)IDC_ALPHACHK,
									(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
		Edit_Enable(hWndEditAlpha,FALSE);
		hWndCheckSym = CreateWindow( 	L"button",L"Symbols",WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
									10,105,143,20,hWnd,(HMENU)IDC_SYMCHK,
									(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
		Edit_Enable(hWndEditSym,FALSE);
		Edit_Enable(hWndEditSymUpDown,FALSE);
	/*	hWndStaticSym = CreateWindow(	L"STATIC",L"Symbols(Recommended):",WS_VISIBLE|WS_CHILD,   
									10,95,200,25,hWnd,NULL,
									(HINSTANCE) GetWindowLong(hWnd, GWL_HINSTANCE),NULL );*/

	/*	hWndDebEdit = CreateWindowEx(	WS_EX_STATICEDGE,L"Edit",NULL,ES_AUTOHSCROLL|WS_VISIBLE|WS_CHILD|WS_BORDER|WS_EX_STATICEDGE,   
									10,330,253,25,hWnd,NULL,
									(HINSTANCE) GetWindowLong(hWnd, GWL_HINSTANCE),NULL );
		hWndMD5Button = CreateWindow( 	L"Button",L"MD5",WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
									85,305,100,20,hWnd,(HMENU)IDC_MD5BTN,
									(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);	*/
		hFont = CreateFont  (16,0,0,0,FW_MEDIUM,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
							NULL,L"Arial");
		hFontCheck = CreateFont  (14,0,0,0,FW_MEDIUM,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
							NULL,L"Arial");
		SendMessage(hWndStatic,WM_SETFONT,(WPARAM)hFont,TRUE);
		SendMessage(hWndCheck,WM_SETFONT,(WPARAM)hFontCheck,TRUE);
		SendMessage(hWndCheckSym,WM_SETFONT,(WPARAM)hFontCheck,TRUE);
		SendMessage(hWndEditSym, WM_SETTEXT, 0, (LPARAM)L"!@#$%^&*()_-=+");
		Edit_LimitText(hWndEditUpDown,2);
		Edit_LimitText(hWndEditAlpha,2);
		Edit_LimitText(hWndEditSymUpDown,2);
		AeroAutoSubclass(hWnd, ASC_SUBCLASS_ALL_CONTROLS, 0L);
	//	SetLayeredWindowAttributes((HWND)hMenu,color,255,LWA_COLORKEY);
		//InvalidateRect(hWndStatic,NULL,NULL);
	//	DwmExtendFrameIntoClientArea ( hWnd, &mar );    
	
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			//	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, SavePass);
				break;
			case ID_HELP_HELP:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_HELP), hWnd, Help);
				break;
			case ID_FILE_OPEN:
				{
					CoInitialize(0); 
					if(GetOpenFileName(&ofn))
					{
					//	GetOpenFileName(&ofn);
						int deb = CommDlgExtendedError();
						hf = CreateFile(ofn.lpstrFile, 
						GENERIC_READ,
						0,
						(LPSECURITY_ATTRIBUTES) NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						(HANDLE) NULL);
					//	HANDLE hf2=CreateFile(L"n.txt",GENERIC_WRITE,0,(LPSECURITY_ATTRIBUTES) NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);
						filesize=GetFileSize(hf,NULL);
						mdread = new wchar_t[(filesize/2)+4];
					//	for(int i=0;i<filesize/2;i++)read[i]=0;
					//	read=(wchar_t *)malloc(filesize);
						ReadFile(hf,mdread,filesize,&bytestoread,NULL);
						mdread[filesize/2]=0;
						CloseHandle(hf);
					//	read[filesize+1]=L'/0';
					//	WriteFile(hf2,read,filesize,&bytestoread,NULL);
						read = &mdread[32];
					//	mdcheck = new wchar_t[33];
						swprintf(mdcheck,L"%0.32s",mdread);
						md5fun(read,md5);
						if(!wcscmp(md5,mdcheck))
						{
							n=0;row=0;column=1;k=0;
							DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, Master);
							decrypt(master,read);
							if(read[0]==L'p'&&read[1]==L'a'&&read[2]==L's'&&read[3]==L's')
							{
								swprintf(szTitle2,L"PassWd Mgr - %s",ofn.lpstrFileTitle);
								SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)szTitle2);
								DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG3), hWnd, View);
								hMenu = GetMenu(hWnd);
								EnableMenuItem(hMenu,ID_VIEW_VIEWPASSWD,MF_ENABLED);
								EnableMenuItem(hMenu,ID_FILE_CLOSE,MF_ENABLED);
								DrawMenuBar(hWnd);
							}
							else
							{
								MessageBox(hWnd,L"Wrong Master Password !",NULL,MB_ICONERROR);
								savin=0;
							};
						}
						else
						MessageBox(hWnd,L"The pass file is Invalid or Damaged",NULL,MB_ICONERROR);
					};
				};
				break;
			case ID_FILE_CLOSE:
				{
					swprintf(szTitle2,L"PassWd Mgr");
					SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)szTitle2);
					master[0]=0;
					read=0;
					mdread=0;
					write=0;
					mdwrite=0;
					hf=0;
					szFile[0]=0;
					ofn.lpstrFile=szFile;
					savin=0;
					EnableMenuItem(hMenu,ID_VIEW_VIEWPASSWD,MF_DISABLED);
					EnableMenuItem(hMenu,ID_FILE_CLOSE,MF_DISABLED);
				};
				break;
			case ID_FILE_SAVEAS:
				{
					no = new wchar_t[Edit_GetTextLength(hWndEdit)+2];
					Edit_GetText(hWndEdit,no,Edit_GetTextLength(hWndEdit)+1);
					savin=1;
					DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Sav);
				/*	if(savtitle)
					{
						swprintf(szTitle2,L"PassWd Mgr - %s.pass",ofn.lpstrFileTitle);
						SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)szTitle2);
					};
					/*	GetSaveFileName(&ofn);
						hf = CreateFile(ofn.lpstrFile, FILE_WRITE_DATA,0,(LPSECURITY_ATTRIBUTES) NULL,
										CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);
						swprintf(write,L"pass %s|%s|%s|%s",web,uname,no,oth);
						WriteFile(hf,&write,sizeof(write),NULL,NULL);
						CloseHandle(hf);*/
					//	ShowWindow(spDlg1, SW_SHOW);
					/*	switch(sa)
						{
						case IDC_BUTTON1:
							if(sav!=0)
							{
								DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, About);
							};
							break;
						}*/
				};
				break;
			case ID_FILE_SAVE:
				{
					no = new wchar_t[Edit_GetTextLength(hWndEdit)+2];
					Edit_GetText(hWndEdit,no,Edit_GetTextLength(hWndEdit)+1);
					savin=0;
					if(!wcscmp(ofn.lpstrFile,L""))
					{
						savin=1;
						DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Sav);
					/*	if(savtitle)
						{
							swprintf(szTitle2,L"PassWd Mgr - %s.pass",ofn.lpstrFileTitle);
							SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)szTitle2);
						};*/
					}
					else
					{
						DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Sav);
						if(!cansave)
						{
							time_t givtim;
							time(&givtim);
							struct tm * giv = (struct tm *)malloc(sizeof(struct tm));
							giv = localtime(&givtim);
							giv->tm_mon--;
							wchar_t strtim[8];
							wcsftime(strtim,5,L"%d%m",giv);
							swprintf(strtim,L"%s%d",strtim,giv->tm_year);
							hf = CreateFile(ofn.lpstrFile/*L"pas.pass"*/, GENERIC_READ,0,(LPSECURITY_ATTRIBUTES) NULL,
											OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);
							filesize=GetFileSize(hf,NULL)+2;
							mdread = new wchar_t[filesize/2];
							ReadFile(hf,mdread,filesize,&bytestoread,NULL);
							mdread[filesize/2]=0;
							CloseHandle(hf);
							hf = CreateFile(ofn.lpstrFile/*L"pas.pass"*/, GENERIC_READ|GENERIC_WRITE,0,(LPSECURITY_ATTRIBUTES) NULL,
											CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);
						//	CloseHandle(hf);
							read = &mdread[32];
							decrypt(master,read);
							write = new wchar_t[wcslen(read)+5+wcslen(web)+wcslen(uname)+wcslen(no)+wcslen(oth)+10];
							swprintf(write,L"%s▲%s↔%s↔%s↔%s↔%s",read,web,uname,no,oth,strtim);		//↔ is alt 29 | 5s alt 30
							encrypt(master,write);
							md5fun(write,md5);
							mdwrite = new wchar_t[wcslen(write)+33];
							swprintf(mdwrite,L"%s%s",md5,write);
							WriteFile(hf,mdwrite,2*wcslen(mdwrite),&byteswritten,NULL);
							CloseHandle(hf);
							EnableMenuItem(hMenu,ID_VIEW_VIEWPASSWD,MF_ENABLED);
						}
						else cansave=0;
					};
				};
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			case IDC_BTN:
				if (wmEvent==BN_CLICKED)
				{
					if(num<nnum)
					{
						alphaballoon.cbStruct=sizeof(EDITBALLOONTIP);
						alphaballoon.pszTitle=balloontitle;
						alphaballoon.pszText=alphaballoontext;
						alphaballoon.ttiIcon=TTI_ERROR;
						SendMessage(hWndEditAlpha,EM_SHOWBALLOONTIP,NULL,(LPARAM)&alphaballoon);
					//	int x=Edit_ShowBalloonTip(hWndEditAlpha,&alphaballoon);
					//	int de=GetLastError();
					//	x=1;
					//	MessageBox(hWnd,L"The total length should be greater than total numbers + symbols !",L"Error",MB_ICONERROR);
					}
					else if(num<nsym)
					{
						symbolballoon.cbStruct=sizeof(EDITBALLOONTIP);
						symbolballoon.pszTitle=balloontitle;
						symbolballoon.pszText=symbolballoontext;
						symbolballoon.ttiIcon=TTI_ERROR;
						SendMessage(hWndEditSymUpDown,EM_SHOWBALLOONTIP,NULL,(LPARAM)&symbolballoon);
					}
					else if(num<(nnum+nsym))
					{
						alphasymballoon.cbStruct=sizeof(EDITBALLOONTIP);
						alphasymballoon.pszTitle=balloontitle;
						alphasymballoon.pszText=alphasymballoontext;
						alphasymballoon.ttiIcon=TTI_ERROR;
						SendMessage(hWndEditUpDown,EM_SHOWBALLOONTIP,NULL,(LPARAM)&alphasymballoon);
					}
					else if(sym[0]=='\0'&&nsym!=0)
					{
						symsetballoon.cbStruct=sizeof(EDITBALLOONTIP);
						symsetballoon.pszTitle=symsetballoontitle;
						symsetballoon.pszText=symsetballoontext;
						symsetballoon.ttiIcon=TTI_ERROR;
						SendMessage(hWndEditSym,EM_SHOWBALLOONTIP,NULL,(LPARAM)&symsetballoon);
					//	MessageBox(hWnd,L"Please enter some symbols to choose from",L"No Symbols",MB_ICONINFORMATION);
					}
					else
					{
						no=core(num,nnum,nsym,nset,pset,tim,sym);
						SendMessage(hWndEdit, WM_SETTEXT, 0, (LPARAM)no);
						InvalidateRect(hWndEdit,NULL,TRUE);
						tim++;
						
					//	swprintf(str,no);
					//	wcscpy();
						
					};
				};
				break;
			case IDC_UPD:
				{
					GetWindowText(hWndEditUpDown,number,10);
				//	GetWindowText(hWndEditAlpha,alpha,100);
				//	wcstombs(buffer,number,10);
				//	wcstombs(buffern,alpha,100);
				//	nnum=atoi(buffern);
					num=_wtoi(number);
				};
				break;
			case IDC_ALPHA:
				{
					if(Button_GetCheck(hWndCheck) == BST_CHECKED)
					{
						GetWindowText(hWndEditAlpha,alpha,10);
					//	wcstombs(buffern,alpha,10);
						nnum=_wtoi(alpha);
					}
				};
				break;
			case IDC_SYM:
				{
					GetWindowText(hWndEditSym,symbol,100);
					wcstombs(sym,symbol,100);
				};
				break;
			case IDC_SYMUPD:
				{	
					if (Button_GetCheck(hWndCheckSym) == BST_CHECKED)
					{
						GetWindowText(hWndEditSymUpDown,sy,10);
					//	wcstombs(buffers,alpha,10);
						nsym=_wtoi(sy);
					}
				};
				break;
			case IDC_ALPHACHK:
				{
					if (Button_GetCheck(hWndCheck) == BST_CHECKED)
					{
						Edit_Enable(hWndEditAlpha,TRUE);
						nnum=tempnum;
						InvalidateRect(hWndEditAlpha,NULL,NULL);
					}
					else if (Button_GetCheck(hWndCheck) == BST_UNCHECKED)
					{
						Edit_Enable(hWndEditAlpha,FALSE);
						tempnum=nnum;
						nnum=0;
						InvalidateRect(hWndEditAlpha,NULL,NULL);
					};
				};
				break;
			case IDC_SYMCHK:
				{
					if (Button_GetCheck(hWndCheckSym) == BST_CHECKED)
					{	
						Edit_Enable(hWndEditSym,TRUE);
						Edit_Enable(hWndEditSymUpDown,TRUE);
						nsym=tempsym;
						InvalidateRect(hWndEditSym,NULL,NULL);
						InvalidateRect(hWndEditSymUpDown,NULL,NULL);
					}
					else if (Button_GetCheck(hWndCheckSym) == BST_UNCHECKED)
					{
						Edit_Enable(hWndEditSym,FALSE);
						Edit_Enable(hWndEditSymUpDown,FALSE);
						tempsym=nsym;
						nsym=0;
						InvalidateRect(hWndEditSym,NULL,NULL);
						InvalidateRect(hWndEditSymUpDown,NULL,NULL);
					};
				};
				break;
		/*	case IDC_MD5BTN:
			//	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, About);
				if (wmEvent==BN_CLICKED)//&&savin!=0)
				{
				//	SendMessage(hWndDebEdit, WM_SETTEXT, 0, (LPARAM)md5(no,num);
				//	std::wstring str (master);
				//	std::string md=md5(master,num);
				//	std::wstring enstr=aesen(read,md.c_str());
				//	swprintf(temp,L"%d",filesize);
					
				//	DWORD result=0;
				//	wchar_t * md5=(wchar_t *)malloc(33);
					
				//	char md[33];
				//	long l=11;
					
				//	MD5Hash(da,sizeof(da),md);
				//	md5="abcdefghijklmno";
					
				//	md5w[16]=0;
				//	int sz=wcslen(read),wc;
				//	char readmb[1000];		
				//	char * readmb=(char *)malloc(sz=wcslen(read));
				//	char * readmb = new char[sz=wcslen(read)]; 
				//	WideCharToMultiByte(CP_UTF8,WC_NO_BEST_FIT_CHARS,read,-1,readmb,0,NULL,NULL);
				//	wcstombs(readmb,read,sz);
					
				//	int i;
				//	for (i = 0; i < 16; i++)
				//	{
				//		swprintf (&md5w[i*2],L"%02x", pm.digest[i]);
					//	md5w++;
				//	};
				//	i=0;
				//	md5=(char *)MDString((unsigned char *)"abcd");
					int readl=wcslen(read);
					wchar_t * md5w=new wchar_t[readl];
					MD5Context pm;
					int maslen=wcslen(master);
					BYTE * mdchar;
					char * readc=new char[readl];
					unsigned char * da=new unsigned char[maslen];
					wcstombs((char *)da,master,maslen);
				//	int a=wcstombs(readc,read,readl);
					int a=GetLastError();
					da[maslen]='\0';
				//	md5w=encrypt(da,read);
				//	mbstowcs(md5w,(char *)mdchar,readl);
				//	md5w[32]=0;
					SendMessage(hWndDebEdit, WM_SETTEXT, 0, (LPARAM) /*md.c_str());md5w);//ofn.lpstrFile);
					InvalidateRect(hWndDebEdit,NULL,TRUE);
				//	free(&pm);
				};
				break;*/
			case ID_VIEW_VIEWPASSWD:
				{
					hf = CreateFile(ofn.lpstrFile, 
						GENERIC_READ,
						0,
						(LPSECURITY_ATTRIBUTES) NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						(HANDLE) NULL);
					//	HANDLE hf2=CreateFile(L"n.txt",GENERIC_WRITE,0,(LPSECURITY_ATTRIBUTES) NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);
					filesize=GetFileSize(hf,NULL);
					mdread = new wchar_t[(filesize/2)+4];
					//	for(int i=0;i<filesize/2;i++)read[i]=0;
					//	read=(wchar_t *)malloc(filesize);
					ReadFile(hf,mdread,filesize,&bytestoread,NULL);
					mdread[filesize/2]=0;
					CloseHandle(hf);
					//	read[filesize+1]=L'/0';
					//	WriteFile(hf2,read,filesize,&bytestoread,NULL);
					n=0;row=0;column=1;k=0;
					read = &mdread[32];
					//	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, Master);
					decrypt(master,read);
					if(read[0]==L'p'&&read[1]==L'a'&&read[2]==L's'&&read[3]==L's')
					{
					//	swprintf(szTitle2,L"PassWd Mgr - %s",ofn.lpstrFileTitle);
					//	SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)szTitle2);
						DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG3), hWnd, View);
					//	hMenu = GetMenu(hWnd);
					//	EnableMenuItem(hMenu,ID_VIEW_VIEWPASSWD,MF_ENABLED);
					//	EnableMenuItem(hMenu,ID_FILE_CLOSE,MF_ENABLED);
					//	DrawMenuBar(hWnd);
					}
					else
					{
						MessageBox(hWnd,L"Wrong Master Password !",NULL,MB_ICONERROR);
						savin=0;
					};
				}
				break;
			case ID_VIEW_PREFERENCES:
				{
					DialogBox(hInst, MAKEINTRESOURCE(IDD_PREF), hWnd, Pref);
				}
					break;
			default:
					return DefWindowProc(hWnd, message, wParam, lParam);
					break;
		};
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//SelectObject(hdc,(HFONT)ANSI_FIXED_FONT);
		//SetBkColor(hdc,(COLORREF)2);
		//TextOut(hdc,NULL,NULL,L"The number of words :",21);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	
}};

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	hWndAbtSt=GetDlgItem(hDlg,IDC_STATIC2);
	hFontAbt = CreateFont  (28,0,0,0,0,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
							NULL,L"Arial");
	switch (message)
	{
	case WM_INITDIALOG:
		SendMessage(hWndAbtSt,WM_SETFONT,(WPARAM)hFontAbt,TRUE);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
	}
	return (INT_PTR)FALSE;
};

INT_PTR CALLBACK Pref(HWND hPrefDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int combotemp=0;
	UNREFERENCED_PARAMETER(lParam);
	hWndPrefCombo=GetDlgItem(hPrefDlg,IDC_COMBO1);
	/*hFontAbt = CreateFont  (28,0,0,0,0,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
							NULL,L"Arial");*/
	switch (message)
	{
	case WM_INITDIALOG:
		ComboBox_AddString(hWndPrefCombo,L"1 week");
		ComboBox_AddString(hWndPrefCombo,L"2 week");
		ComboBox_AddString(hWndPrefCombo,L"1 month");
		ComboBox_AddString(hWndPrefCombo,L"2 month");
		ComboBox_AddString(hWndPrefCombo,L"3 month");
		ComboBox_AddString(hWndPrefCombo,L"4 month");
		ComboBox_AddString(hWndPrefCombo,L"5 month");
		ComboBox_AddString(hWndPrefCombo,L"6 month");
		ComboBox_AddString(hWndPrefCombo,L"8 month");
		ComboBox_AddString(hWndPrefCombo,L"10 month");
		ComboBox_AddString(hWndPrefCombo,L"1 year");
		if(req_mon>0&&req_mon<7)
		{
			SendMessage(hWndPrefCombo,CB_SETCURSEL,req_mon+1,NULL);
		}
		else if(req_mon==0&&req_day==7)
		{
			SendMessage(hWndPrefCombo,CB_SETCURSEL,0,NULL);
		}
		else if(req_mon==0&&req_day==14)
		{
			SendMessage(hWndPrefCombo,CB_SETCURSEL,1,NULL);
		}
		else if(req_mon==8)
		{
			SendMessage(hWndPrefCombo,CB_SETCURSEL,8,NULL);
		}
		else if(req_mon==10)
		{
			SendMessage(hWndPrefCombo,CB_SETCURSEL,9,NULL);
		}
		else if(req_year==1)
		{
			SendMessage(hWndPrefCombo,CB_SETCURSEL,10,NULL);
		}
		/*SendMessage(hWndAbtSt,WM_SETFONT,(WPARAM)hFontAbt,TRUE);*/
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL) //|| LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hPrefDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDOK) //|| LOWORD(wParam) == IDCANCEL)
		{
			combotemp=SendMessage(hWndPrefCombo,CB_GETCURSEL,NULL,NULL);
			if(combotemp>1&&req_mon<8)
			{
				req_day = 0;
				req_mon = combotemp-1;
				req_year = 0;
			}
			if(combotemp==0)
			{
				req_day = 7;
				req_mon = 0;
				req_year = 0;
			}
			if(combotemp==1)
			{
				req_day = 14;
				req_mon = 0;
				req_year = 0;
			}
			if(combotemp==8)
			{
				req_day = 0;
				req_mon = 8;
				req_year = 0;
			}
			if(combotemp==9)
			{
				req_day = 0;
				req_mon = 10;
				req_year = 0;
			}
			if(combotemp==10)
			{
				req_day = 0;
				req_mon = 0;
				req_year = 1;
			}
			RegSetValueEx(pKey,L"day",NULL,REG_DWORD,(CONST BYTE *)&req_day,sizeof(DWORD));
			RegSetValueEx(pKey,L"month",NULL,REG_DWORD,(CONST BYTE *)&req_mon,sizeof(DWORD));
			RegSetValueEx(pKey,L"year",NULL,REG_DWORD,(CONST BYTE *)&req_year,sizeof(DWORD));
			EndDialog(hPrefDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

	}
	return (INT_PTR)FALSE;
};
INT_PTR CALLBACK Help(HWND hHelpDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	hWndHelpSt=GetDlgItem(hHelpDlg,IDC_HELP_STAT);
	hWndHelpEdit=GetDlgItem(hHelpDlg,IDC_HELP_EDIT);
	hFontHpl = CreateFont  (20,0,0,0,0,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
							NULL,L"Arial");
	switch (message)
	{
	case WM_INITDIALOG:
		SendMessage(hWndHelpSt,WM_SETFONT,(WPARAM)hFontHpl,TRUE);
		SendMessage(hWndHelpEdit, WM_SETTEXT, 0, (LPARAM)L"PassWd Mgr is a tiny password management utility just 200k(approx.). It can generate and store several passwords in encrypted form with just one master password. So no need to remember several passwords or use one password for several websites just remember one master password and stay secure.\r\n    If you experience any problem or have any suggestion about the application don't hesitate to contact the developer and help make this application better");
		return (INT_PTR)TRUE;

	case WM_CLOSE:
		EndDialog(hHelpDlg, LOWORD(wParam));
/*	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON1)
		{
			EndDialog(hHelpDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}	*/
	}	
	return (INT_PTR)FALSE;
};


	//Handler for Save Pass
INT_PTR CALLBACK Sav(HWND spDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	sa=0;
	int lav;
	hWndDlgWeb = GetDlgItem(spDlg,IDC_EDIT1);
	hWndDlgEdit3 = GetDlgItem(spDlg,IDC_EDIT3);
	hWndEdit3Chk = GetDlgItem(spDlg,IDC_CHECK1);
	hWndDlgUname = GetDlgItem(spDlg,IDC_EDIT2);
	hWndDlgOth = GetDlgItem(spDlg,IDC_EDIT4);
	UNREFERENCED_PARAMETER(lParam);
	switch(message)
	{
		case WM_INITDIALOG:
			if(editflag)
			{
				SetDlgItemText(spDlg, IDC_EDIT1, web);
				SetDlgItemText(spDlg, IDC_EDIT2, uname);
				SetDlgItemText(spDlg, IDC_EDIT4, oth);
			};
			SetDlgItemText(spDlg, IDC_EDIT3, no);
			return (INT_PTR)FALSE;
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
		/*	case IDC_EDIT1:
				GetDlgItemText(spDlg,IDC_EDIT1,web,100);
				break;
			case IDC_EDIT2:
				GetDlgItemText(spDlg,IDC_EDIT2,uname,50);
				break;
			case IDC_EDIT4:
				GetDlgItemText(spDlg,IDC_EDIT4,oth,1000);
				break;*/
			case IDC_CHECK1:
				{
					if (Button_GetCheck(hWndEdit3Chk) == BST_CHECKED)
					{
						Edit_Enable(hWndDlgEdit3,TRUE);
						InvalidateRect(hWndDlgEdit3,NULL,NULL);
					}
					else if (Button_GetCheck(hWndEdit3Chk) == BST_UNCHECKED)
					{
						Edit_Enable(hWndDlgEdit3,FALSE);
						InvalidateRect(hWndDlgEdit3,NULL,NULL);
					};
				};
				break;
		/*	case IDC_EDIT3:
				GetDlgItemText(spDlg,IDC_EDIT3,no,100);
				break; */
			case IDOK:
				web = new wchar_t[Edit_GetTextLength(hWndDlgWeb) +1];
				GetDlgItemText(spDlg,IDC_EDIT1,web,100);
				uname = new wchar_t[Edit_GetTextLength(hWndDlgUname) +1];
				GetDlgItemText(spDlg,IDC_EDIT2,uname,50);
				oth = new wchar_t[Edit_GetTextLength(hWndDlgOth) +1];
				GetDlgItemText(spDlg,IDC_EDIT4,oth,1000);
				no = new wchar_t[Edit_GetTextLength(hWndDlgEdit3) +1];
				GetDlgItemText(spDlg,IDC_EDIT3,no,100);
				lav = wcslen(no);
				EndDialog(spDlg,LOWORD(wParam));
				if(!editflag)
				{
					time_t givtim;
					time(&givtim);
					struct tm * giv = (struct tm *)malloc(sizeof(struct tm));
					giv = localtime(&givtim);
					giv->tm_mon--;
					wchar_t strtim[8];
					wcsftime(strtim,5,L"%d%m",giv);
					swprintf(strtim,L"%s%d",strtim,giv->tm_year);
					write = new wchar_t[20+wcslen(web)+wcslen(uname)+wcslen(no)+wcslen(oth)];
					swprintf(write,L"pass ▲%s↔%s↔%s↔%s↔%s",web,uname,no,oth,strtim);
					if(savin==1)
					{
						DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), spDlg, Master);
						if(savin==1)
						{
							if(GetSaveFileName(&ofn))
							{
								swprintf(ofn.lpstrFile,L"%s.pass",ofn.lpstrFile);
								hf = CreateFile(ofn.lpstrFile/*L"pas.pass"*/, GENERIC_WRITE,0,(LPSECURITY_ATTRIBUTES) NULL,
												CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);
							//	write = new wchar_t[9+wcslen(web)+wcslen(uname)+wcslen(no)+wcslen(oth)];
							//	swprintf(write,L"pass ▲%s↔%s↔%s↔%s",web,uname,no,oth);
								encrypt(master,write);
								md5fun(write,md5);
								mdwrite = new wchar_t[wcslen(write)+33];
								swprintf(mdwrite,L"%s%s",md5,write);
								WriteFile(hf,mdwrite,2*wcslen(mdwrite),&byteswritten,NULL);
								CloseHandle(hf);
							//	savtitle=1;
								swprintf(szTitle2,L"PassWd Mgr - %s.pass",ofn.lpstrFileTitle);
								SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)szTitle2);
								hMenu = GetMenu(hWnd);
								EnableMenuItem(hMenu,ID_VIEW_VIEWPASSWD,MF_ENABLED);
								EnableMenuItem(hMenu,ID_FILE_CLOSE,MF_ENABLED);
							};
						}
					};
				}
				//else
				//{
				//	write = new wchar_t[wcslen(read)+wcslen(web)+wcslen(uname)+wcslen(no)+wcslen(oth)+1];
				//	for(int i=0;i<m;i++)write[i]=read[i];
				//	n--;
				//	swprintf(&write[m],L"%s↔%s↔%s↔%s%s",web,uname,no,oth,&read[n]);
				//	read=write;
				//	/*SendMessageW(hList,LVM_DELETEALLITEMS,0,0);
				//	if(wcslen(read)>5)
				//	listman();*/
				//};
				return (INT_PTR)TRUE;
				break;
			case IDC_REGEN:
				if(num>(nnum+nsym))
				{
					no=core(num,nnum,nsym,nset,pset,tim,sym);
					SendMessage(hWndDlgEdit3, WM_SETTEXT, 0, (LPARAM)no);
					InvalidateRect(hWndDlgEdit3,NULL,TRUE);
					tim++;
				}
				break;
			case IDCANCEL:
				cansave=1;
				EndDialog(spDlg,LOWORD(wParam));
				return (INT_PTR)FALSE;
				break;
		//	default:
		//		return DefDlgProc(spDlg,messageSav,wSavParam,lSavParam);
			}break;
		}break;
	};
	return (INT_PTR)FALSE;
};

// Handler for Master Dialog

INT_PTR CALLBACK Master(HWND mpsav, UINT message, WPARAM wParam, LPARAM lParam)
{
	hWndMasterEdit=GetDlgItem(mpsav,IDC_MASTEREDIT1);
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;
			break;
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
		/*	case IDC_MASTEREDIT1:
				GetWindowText(hWndDlgEdit1,master,50);
				break;*/
			case IDOK:
				GetWindowText(hWndMasterEdit,master,50);
				savin=1;
				EndDialog(mpsav, LOWORD(wParam));
			//	return FALSE;
				break;
			case IDCANCEL:
				savin=0;
				EndDialog(mpsav, LOWORD(wParam));
				return (INT_PTR)FALSE;
				break;
			}
		}break;
	}
	return (INT_PTR)FALSE;
}



// Handler for View Dialog


INT_PTR CALLBACK View(HWND hView, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	hList=GetDlgItem(hView,IDC_LIST1);
	hWndViewCheck=GetDlgItem(hView,IDC_CHECK1);
//	int d;
	switch (message)
	{
	case WM_INITDIALOG:
			SendMessageW(hList,LVS_SINGLESEL,0,0);
		//	d= ListView_SetTextColor(hList,colorred);
		//	Memset(&LvCol,0,sizeof(LvCol));                  // Zero Members
			LvCol.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;    // Type of mask
		//	LvCol.cx=0x28;                                   // width between each coloum
			LvCol.pszText=L"S No.";                            // First Header Text
			LvCol.cx=0x26;                                   // width of column
		
			SendMessageW(hList,LVM_INSERTCOLUMN,0,(LPARAM)&LvCol); // Insert/Show the coloum
			LvCol.cx=0x70;
			LvCol.pszText=L"Website";                            // Next coloum
			SendMessageW(hList,LVM_INSERTCOLUMN,1,(LPARAM)&LvCol); // ...
			LvCol.cx=0x40;
			LvCol.pszText=L"Username";                            //
			SendMessageW(hList,LVM_INSERTCOLUMN,2,(LPARAM)&LvCol);
			LvCol.cx=0x50;
			LvCol.pszText=L"PassWd";                            //
			SendMessageW(hList,LVM_INSERTCOLUMN,3,(LPARAM)&LvCol);
			LvCol.cx=0x100;
			LvCol.pszText=L"Others";
			SendMessageW(hList,LVM_INSERTCOLUMN,4,(LPARAM)&LvCol);
			LvCol.cx=0x50;
			LvCol.pszText=L"Date Modified";
			SendMessageW(hList,LVM_INSERTCOLUMN,5,(LPARAM)&LvCol);
		//	SendMessageW(hList,LVM_SETTEXTCOLOR,NULL,(LPARAM)&colorred);
		//	ListView_SetTextColor(hList,colorred);
		listman();
		if(colorflag)
		{
			MessageBox(hWnd,L"One or more PassWd is obsolete. Please change your PassWd. ",L"Obsolete PassWd",MB_ICONINFORMATION);
		}
		return (INT_PTR)TRUE;
			
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			if(change==1)
			{
				change=0;
				if(MessageBox(hView,L"Save Changes ?",L"Save PassWd",MB_ICONQUESTION|MB_YESNO)==IDYES)
				{
						hf = CreateFile(ofn.lpstrFile/*L"pas.pass"*/, GENERIC_READ|GENERIC_WRITE,0,(LPSECURITY_ATTRIBUTES) NULL,
										CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);
					//	write = new wchar_t[wcslen(read)];
					//	swprintf(write,L"%s",read);
						encrypt(master,write);
						md5fun(write,md5);
						mdwrite = new wchar_t[wcslen(write)+33];
						swprintf(mdwrite,L"%s%s",md5,write);
						WriteFile(hf,mdwrite,2*wcslen(mdwrite),&byteswritten,NULL);
						CloseHandle(hf);
				};
			};
			EndDialog(hView, LOWORD(wParam));
			return (INT_PTR)TRUE;
		};
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hView, LOWORD(wParam));
			return (INT_PTR)TRUE;
		};
		if (LOWORD(wParam) == IDC_REMBTN)
		{
			change=1;
			lremcou=0;
			readsize=wcslen(read);
			listsel=ListView_GetSelectionMark(hList);
			listsel=listsel+2;
			listflg=ListView_GetSelectedCount(hList);
			if(listflg!=0)
			{
				n=0;k=0;
				while(lremcou<(listsel))
				{
					while(read[n]!=L'▲'&&n<readsize)
					{
						n++;
						if(lremcou<(listsel-1)){k++;};
					}
					lremcou++;
					n++;
					if(lremcou<(listsel-1)){k++;};
				};
				n--;
				while(n<readsize)
				{
					read[k]=read[n];
					k++;
					n++;
				};
				read[k]=0;
			//	filesize=2*k;
			};
			SendMessageW(hList,LVM_DELETEALLITEMS,0,0);
			if(wcslen(read)>5)
			listman();
			write = new wchar_t[wcslen(read)+2];
			swprintf(write,L"%s",read);
			UpdateWindow(hList);
		};	
		if (LOWORD(wParam) == IDC_CHECK1)
		{
			if (Button_GetCheck(hWndViewCheck) == BST_CHECKED)
			{
				hidepasswd=0;
			};
			if (Button_GetCheck(hWndViewCheck) == BST_UNCHECKED)
			{
				hidepasswd=1;
			};
			SendMessageW(hList,LVM_DELETEALLITEMS,0,0);
			if(wcslen(read)>5)
			listman();
			UpdateWindow(hList);
			write = new wchar_t[wcslen(read)+2];
			swprintf(write,L"%s",read);
		};
		if (LOWORD(wParam) == IDC_BUTTON2)
		{
			change=1;
			readsize=wcslen(read);
			bufr=new wchar_t[readsize+1]; 
			lremcou=0;
			listsel=ListView_GetSelectionMark(hList);
			listsel++;
			listflg=ListView_GetSelectedCount(hList);
			if(listflg>0)
			{
				n=0;k=0;m=0;
				while(lremcou<(listsel))
				{
					while(read[n]!=L'▲'&&n<readsize)
					{
						n++;
					//	if(lremcou<(listsel-1)){k++;};
					}
					lremcou++;
					n++;
					m=n;
				//	if(lremcou<(listsel-1)){k++;};
				};
			//	n--;
				for(int i=0;i<4&&n<wcslen(read);i++)
				{
					k=0;
					while(read[n]!=L'▲'&&read[n]!=L'↔'&&n<readsize)
					{
						bufr[k]=read[n];
						n++;
						k++;
					};
					n++;
					bufr[k]=0;
					if(i==0)
					{
						web = new wchar_t[wcslen(bufr)+1];
						swprintf(web,L"%s",bufr);
					};
					if(i==1)
					{
						uname = new wchar_t[wcslen(bufr)+1];
						swprintf(uname,L"%s",bufr);
					};
					if(i==2)
					{
						no = new wchar_t[wcslen(bufr)+1];
						swprintf(no,L"%s",bufr);
					};
					if(i==3)
					{
						oth = new wchar_t[wcslen(bufr)+1];
						swprintf(oth,L"%s",bufr);
					};
					editflag=1;
				};
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Sav);
							//conv. web,uname etc to * n malloc...sav init condition
				write = new wchar_t[wcslen(read)+wcslen(web)+wcslen(uname)+wcslen(no)+wcslen(oth)+10];
				for(int i=0;i<m;i++)write[i]=read[i];
				n--;
				time_t givtim;
				time(&givtim);
				struct tm * giv = (struct tm *)malloc(sizeof(struct tm));
				giv = localtime(&givtim);
				giv->tm_mon--;
				wchar_t strtim[8];
				wcsftime(strtim,5,L"%d%m",giv);
				swprintf(strtim,L"%s%d",strtim,giv->tm_year);
				swprintf(&write[m],L"%s↔%s↔%s↔%s↔%s%s",web,uname,no,oth,strtim,&read[n]);
				read=write;
				SendMessageW(hList,LVM_DELETEALLITEMS,0,0);
				if(wcslen(read)>5)
				listman();
			};
		};
	case WM_NOTIFY:
		{
			if(LOWORD(wParam)==IDC_LIST1)
			{
				if(((LPNMHDR)lParam)->code == NM_DBLCLK)
				{
					listflg=ListView_GetSelectedCount(hList);
					if(listflg>0)
					{
						listsel=ListView_GetSelectionMark(hList);
						listsel=listsel+1;
						n=0;
						while(lremcou<(listsel))
						{
							while(read[n]!=L'▲'&&n<filesize/2)
							{
								n++;
							//	if(lremcou<(listsel-1)){k++;};
							}
							lremcou++;
							n++;
						};
						for(int i=0;i<3;i++)
						{
							k=0;
							while(read[n]!=L'↔')
							{
								if(i==2)
								{
									bufr[k]=read[n];
									k++;
								};
								n++;
							};
							n++;
						};
						bufr[k]=0;
						if(OpenClipboard(NULL))
						{
							EmptyClipboard();
							hMem = GlobalAlloc(GMEM_MOVEABLE,2*(wcslen(bufr)+1));
							memcpy(GlobalLock(hMem),bufr,2*wcslen(bufr));
							GlobalUnlock(hMem);
							SetClipboardData(CF_UNICODETEXT,hMem);
							CloseClipboard();
						};
					};
				};
			};
			/*if(((LPNMHDR)lParam)->code == NM_CUSTOMDRAW)
			{
			   SetWindowLong(hWnd, DWL_MSGRESULT, 
					(LONG)ProcessCustomDraw(lParam));
			   return TRUE;
			};*/
		};
	};
	return (INT_PTR)FALSE;
};




void listman()
	{
		colorflag=0;
	//	COLORREF colorred = RGB(255,25,2);
		n=0;row=0;column=1;k=0;			//	Items
		LvItem.mask=LVIF_TEXT;   // Text Style
		//	LvItem.cchTextMax = 1000; // Max size of test

//////////////////////////////////////////////////////////////////////////////////////////////
		ListView_SetExtendedListViewStyle(hList, LVS_EX_FULLROWSELECT);
		LvItem.iItem=0;          // choose item  
		LvItem.iSubItem=0;       // Put in first coluom
		LvItem.pszText=L"1"; // Text to display (can be from a char variable) (Items)
		SendMessageW(hList,LVM_INSERTITEM,0,(LPARAM)&LvItem); // Send info to the Listview
		readsize=wcslen(read)+1;
		bufr = new wchar_t[readsize+1];
		while(read[n]!=L'▲'&&n<readsize)
		{
			n++;
		};
		n++;
		while(n<readsize)
		{
			k=0;
			while(read[n]!=L'▲'&&read[n]!=L'↔'&&n<readsize)
			{
				bufr[k]=read[n];
				n++;
				k++;
			};
			bufr[k]=0;
			//	LvItem.iItem=row;          // choose item  
			//	LvItem.mask=LVIF_TEXT;
			LvItem.iSubItem=column;       // Put in first coluom
			if(hidepasswd==1&&column==3)
			{
				for(int n=0;n<k;n++)
				bufr[n]=9679;
			};
			if(column==5)
			{
				wchar_t timtemp[4];
				timtemp[0] = bufr[0];
				timtemp[1] = bufr[1];
				timtemp[2] = 0;
				givch->tm_mday = _wtoi(timtemp);
				timtemp[0] = bufr[2];
				timtemp[1] = bufr[3];
				timtemp[2] = 0;
				givch->tm_mon = _wtoi(timtemp);
				timtemp[0] = bufr[4];
				timtemp[1] = bufr[5];
				timtemp[2] = bufr[6];
				timtemp[3] = 0;
				givch->tm_year = _wtoi(timtemp);
				swprintf(bufr,L"%d-%s-%d",givch->tm_mday,mon[givch->tm_mon],givch->tm_year+1900);
			}
			if(column==5&&(!timchk(givch,req_day,req_mon,req_year)))
			{
				colorflag=1;
			}
			//	n--;
			//	SendMessage(hList,LVM_DELETEITEM,row,0);
			//	while(read[n]!=L'▲')
			//	{
			//		n--;
			//	}
			////	ListView_SetTextColor(hList,colorred);
			//	colorflag=1;
			//	n++;
			//	if(n<7)
			//	SendMessageW(hList,LVM_INSERTITEM,0,(LPARAM)&LvItem);
			//}
			//else
			//{
			LvItem.pszText=bufr; // Text to display (can be from a char variable) (Items)
			LvItem.cchTextMax = wcslen(LvItem.pszText);
		//	ListView_SetTextColor(hList,NULL);
			SendMessageW(hList,LVM_SETITEM,0,(LPARAM)&LvItem); // Send info to the Listview
			column++;
			if(read[n]==L'▲'&&(n<readsize-1))
			{
				row++;
				column=1;
				//	LvItem.mask=0;
				LvItem.iItem=row;          // choose item  
				LvItem.iSubItem=0;       // Put in first coluom
				swprintf(snbuf,L"%d",row+1);
				LvItem.pszText=snbuf; // Text to display (can be from a char variable) (Items)
				SendMessageW(hList,LVM_INSERTITEM,0,(LPARAM)&LvItem); // Send info to the Listview
			//	ListView_SetTextColor(hList,NULL);
			};
			n++;
		//	colorflag=0;
			//	ListView_RedrawItems(hList,0,5);
		};
	};
/*void MD5Hash(BYTE hash[],int sz,char sec[]){
    HCRYPTPROV hProv = 0,hHash = 0;
    BYTE rgbHash[16];
    DWORD cbHash = 0;
    char finalhash[33],file[MAX_PATH],dig[] = "0123456789abcdef";
    int l=0;
    
    CryptAcquireContext(&hProv,NULL,NULL,PROV_RSA_FULL,CRYPT_VERIFYCONTEXT);
    CryptCreateHash(hProv,CALG_MD5,0,0,&hHash);
    CryptHashData(hHash,hash,sz,0);
    cbHash = 16;
    CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0);
    
    for(DWORD i=0;i<cbHash;i++){
        finalhash[l]=dig[rgbHash[i]>>4];
        l++;
        finalhash[l]=dig[rgbHash[i] & 0xf];
        l++;
    }
    
    for(l=32;l<strlen(finalhash);l++)finalhash[l]=0;
    strcpy(sec,finalhash);
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
}
//implement the other in bokmark*/


//LRESULT ProcessCustomDraw (LPARAM lParam)
//{
//    LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)lParam;
//	int iSelect=0;
//    switch(lplvcd->nmcd.dwDrawStage) 
//    {
//        case CDDS_PREPAINT : //Before the paint cycle begins
//            //request notifications for individual listview items
//            return CDRF_NOTIFYITEMDRAW;
//            
//        case CDDS_ITEMPREPAINT: //Before an item is drawn
//            if (((int)lplvcd->nmcd.dwItemSpec%2)==0)
//            {
//                //customize item appearance
//                lplvcd->clrText   = RGB(255,0,0);
//                lplvcd->clrTextBk = RGB(200,200,200);
//                return CDRF_NEWFONT;
//            }
//            else{
//                lplvcd->clrText   = RGB(0,0,255);
//                lplvcd->clrTextBk = RGB(255,255,255);
//            
//                return CDRF_NEWFONT;
//            }
//            break;
//
//        //Before a subitem is drawn
//        case CDDS_SUBITEM | CDDS_ITEMPREPAINT: 
//            if (iSelect == (int)lplvcd->nmcd.dwItemSpec)
//            {
//                if (0 == lplvcd->iSubItem)
//                {
//                    //customize subitem appearance for column 0
//                    lplvcd->clrText   = RGB(255,0,0);
//                    lplvcd->clrTextBk = RGB(255,255,255);
//
//                    //To set a custom font:
//                    //SelectObject(lplvcd->nmcd.hdc, 
//                    //    <your custom HFONT>);
//
//                    return CDRF_NEWFONT;
//                }
//                else if (1 == lplvcd->iSubItem)
//                {
//                    //customize subitem appearance for columns 1..n
//                    //Note: setting for column i 
//                    //carries over to columnn i+1 unless
//                    //      it is explicitly reset
//                    lplvcd->clrTextBk = RGB(255,0,0);
//                    lplvcd->clrTextBk = RGB(255,255,255);
//
//                    return CDRF_NEWFONT;
//                }
//            }
//    }
//    return CDRF_DODEFAULT;
//}
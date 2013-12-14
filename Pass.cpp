/*	Copyright 2013 Saket Srivastava
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License at
*
*		http://www.apache.org/licenses/LICENSE-2.0
*
*	Unless required by applicable law or agreed to in writing, software
*	distributed under the License is distributed on an "AS IS" BASIS,
*	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*	See the License for the specific language governing permissions and
*	limitations under the License.
*/


#include "Def.h"
#include "AutoAero.h"
#include "Pass.h"
#include "core.h"
#include "Crypt-AES\Rijndael.h"
#include "passcrypt.h"



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

   hInst = hInstance;						 // Store instance handle in our global variable

    hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|BN_SETFOCUS,
		   CW_USEDEFAULT, 0, 290, 350, NULL, NULL, hInstance, NULL);
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
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
	
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case ID_HELP_HELP:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_HELP), hWnd, Help);
				break;
			case ID_FILE_OPEN:
				{
					CoInitialize(0); 
					if(GetOpenFileName(&ofn))
					{
						int deb = CommDlgExtendedError();
						hf = CreateFile(ofn.lpstrFile, 
						GENERIC_READ,
						0,
						(LPSECURITY_ATTRIBUTES) NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						(HANDLE) NULL);
						filesize=GetFileSize(hf,NULL);
						mdread = new wchar_t[(filesize/2)+4];
						ReadFile(hf,mdread,filesize,&bytestoread,NULL);
						mdread[filesize/2]=0;
						CloseHandle(hf);
						read = &mdread[32];
						swprintf(mdcheck,L"%0.32s",mdread);
						md5fun(read,md5);
						read2 = new wchar_t[(filesize/2)-28];
						swprintf(read2,L"%s",read);
						if(!wcscmp(md5,mdcheck))
						{
							n=0;row=0;column=1;k=0;
							DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG4), hWnd, Master);
							if(savin)
							{
								read = decryptaes(master,read);
								if(read[0]==L'p'&&read[1]==L'a'&&read[2]==L's'&&read[3]==L's')
								{
									swprintf(szTitle2,L"PassWd Mgr - %s",ofn.lpstrFileTitle);
									SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)szTitle2);
									DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG3), hWnd, View);
									hMenu = GetMenu(hWnd);
									EnableMenuItem(hMenu,ID_VIEW_VIEWPASSWD,MF_ENABLED);
									EnableMenuItem(hMenu,ID_VIEW_CHANGEMASTERPASSWORD,MF_ENABLED);
									EnableMenuItem(hMenu,ID_FILE_CLOSE,MF_ENABLED);
									DrawMenuBar(hWnd);
								}
								else
								{
									read2 = decryptrc4(master,read2);
									if(read2[0]==L'p'&&read2[1]==L'a'&&read2[2]==L's'&&read2[3]==L's')
									{
										hf = CreateFile(ofn.lpstrFile, GENERIC_WRITE,0,(LPSECURITY_ATTRIBUTES) NULL,
														CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);
										write = encryptaes(master,read2);
										md5fun(read2,md5);
										mdread = new wchar_t[wcslen(read2)+33];
										swprintf(mdread,L"%s%s",md5,read2);
										WriteFile(hf,mdread,2*wcslen(mdread),&byteswritten,NULL);
										CloseHandle(hf);
										swprintf(szTitle2,L"PassWd Mgr - %s",ofn.lpstrFileTitle);
										SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)szTitle2);
										DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG3), hWnd, View);
										hMenu = GetMenu(hWnd);
										EnableMenuItem(hMenu,ID_VIEW_VIEWPASSWD,MF_ENABLED);
										EnableMenuItem(hMenu,ID_VIEW_CHANGEMASTERPASSWORD,MF_ENABLED);
										EnableMenuItem(hMenu,ID_FILE_CLOSE,MF_ENABLED);
										DrawMenuBar(hWnd);
									}
									else
									{
										MessageBox(hWnd,L"Wrong Master Password !",NULL,MB_ICONERROR);
										savin=0;
									}
								};
							}
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
					EnableMenuItem(hMenu,ID_VIEW_CHANGEMASTERPASSWORD,MF_DISABLED);
					EnableMenuItem(hMenu,ID_FILE_CLOSE,MF_DISABLED);
				};
				break;
			case ID_FILE_SAVEAS:
				{
					no = new wchar_t[Edit_GetTextLength(hWndEdit)+2];
					Edit_GetText(hWndEdit,no,Edit_GetTextLength(hWndEdit)+1);
					savin=1;
					DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Sav);
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
							hf = CreateFile(ofn.lpstrFile, GENERIC_READ,0,(LPSECURITY_ATTRIBUTES) NULL,
											OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);
							filesize=GetFileSize(hf,NULL)+2;
							mdread = new wchar_t[filesize/2];
							ReadFile(hf,mdread,filesize,&bytestoread,NULL);
							mdread[filesize/2]=0;
							CloseHandle(hf);
							hf = CreateFile(ofn.lpstrFile, GENERIC_READ|GENERIC_WRITE,0,(LPSECURITY_ATTRIBUTES) NULL,
											CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);
							read = &mdread[32];
							read = decryptaes(master,read);
							write = new wchar_t[wcslen(read)+5+wcslen(web)+wcslen(uname)+wcslen(no)+wcslen(oth)+10];
							swprintf(write,L"%s▲%s↔%s↔%s↔%s↔%s",read,web,uname,no,oth,strtim);		//↔ is alt 29 | 5s alt 30
							write = encryptaes(master,write);
							md5fun(write,md5);
							mdwrite = new wchar_t[wcslen(write)+33];
							swprintf(mdwrite,L"%s%s",md5,write);
							WriteFile(hf,mdwrite,2*wcslen(mdwrite),&byteswritten,NULL);
							CloseHandle(hf);
							EnableMenuItem(hMenu,ID_VIEW_VIEWPASSWD,MF_ENABLED);
							EnableMenuItem(hMenu,ID_VIEW_CHANGEMASTERPASSWORD,MF_ENABLED);
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
					}
					else
					{
						no=core(num,nnum,nsym,nset,pset,tim,sym);
						SendMessage(hWndEdit, WM_SETTEXT, 0, (LPARAM)no);
						InvalidateRect(hWndEdit,NULL,TRUE);
						tim++;
					};
				};
				break;
			case IDC_UPD:
				{
					GetWindowText(hWndEditUpDown,number,10);
					num=_wtoi(number);
				};
				break;
			case IDC_ALPHA:
				{
					if(Button_GetCheck(hWndCheck) == BST_CHECKED)
					{
						GetWindowText(hWndEditAlpha,alpha,10);
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
			case ID_VIEW_VIEWPASSWD:
				{
					hf = CreateFile(ofn.lpstrFile, 
						GENERIC_READ,
						0,
						(LPSECURITY_ATTRIBUTES) NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						(HANDLE) NULL);
					filesize=GetFileSize(hf,NULL);
					mdread = new wchar_t[(filesize/2)+4];
					ReadFile(hf,mdread,filesize,&bytestoread,NULL);
					mdread[filesize/2]=0;
					CloseHandle(hf);
					n=0;row=0;column=1;k=0;
					read = &mdread[32];
					read = decryptaes(master,read);
					if(read[0]==L'p'&&read[1]==L'a'&&read[2]==L's'&&read[3]==L's')
					{
						DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG3), hWnd, View);
					}
					else
					{
						MessageBox(hWnd,L"Wrong Master Password !",NULL,MB_ICONERROR);
						savin=0;
					};
				}
				break;
			case ID_VIEW_CHANGEMASTERPASSWORD:
				{
					hf = CreateFile(ofn.lpstrFile, 
						GENERIC_READ,
						0,
						(LPSECURITY_ATTRIBUTES) NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						(HANDLE) NULL);
					filesize=GetFileSize(hf,NULL);
					mdread = new wchar_t[(filesize/2)+4];
					ReadFile(hf,mdread,filesize,&bytestoread,NULL);
					mdread[filesize/2]=0;
					CloseHandle(hf);
					read = &mdread[32];
					swprintf(mdcheck,L"%0.32s",mdread);
					md5fun(read,md5);
					if(!wcscmp(md5,mdcheck))
					{
						n=0;row=0;column=1;k=0;
						DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG4), hWnd, Master);
						read = decryptaes(master,read);
						if(read[0]==L'p'&&read[1]==L'a'&&read[2]==L's'&&read[3]==L's')
						{
							DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, Master);
							hf = CreateFile(ofn.lpstrFile, GENERIC_WRITE,0,(LPSECURITY_ATTRIBUTES) NULL,
														CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);
							write = encryptaes(master,read);
							md5fun(write,md5);
							mdwrite = new wchar_t[wcslen(write)+33];
							swprintf(mdwrite,L"%s%s",md5,write);
							WriteFile(hf,mdwrite,2*wcslen(mdwrite),&byteswritten,NULL);
							CloseHandle(hf);	
						}
						else
						{
							MessageBox(hWnd,L"Wrong Master Password !",NULL,MB_ICONERROR);
							savin=0;
						};
					}
					else
					{
						MessageBox(hWnd,L"The pass file is Invalid or Damaged",NULL,MB_ICONERROR);
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
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	
}};

//Handle for About
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
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hPrefDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDOK)
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
								hf = CreateFile(ofn.lpstrFile, GENERIC_WRITE,0,(LPSECURITY_ATTRIBUTES) NULL,
												CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);
								write = encryptaes(master,write);
								md5fun(write,md5);
								mdwrite = new wchar_t[wcslen(write)+33];
								swprintf(mdwrite,L"%s%s",md5,write);
								WriteFile(hf,mdwrite,2*wcslen(mdwrite),&byteswritten,NULL);
								CloseHandle(hf);
								swprintf(szTitle2,L"PassWd Mgr - %s.pass",ofn.lpstrFileTitle);
								SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)szTitle2);
								hMenu = GetMenu(hWnd);
								EnableMenuItem(hMenu,ID_VIEW_VIEWPASSWD,MF_ENABLED);
								EnableMenuItem(hMenu,ID_VIEW_CHANGEMASTERPASSWORD,MF_ENABLED);
								EnableMenuItem(hMenu,ID_FILE_CLOSE,MF_ENABLED);
							};
						}
					};
				}
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
			case IDOK:
				GetWindowText(hWndMasterEdit,master,100);
				savin=1;
				EndDialog(mpsav, LOWORD(wParam));
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
	switch (message)
	{
	case WM_INITDIALOG:
			SendMessageW(hList,LVS_SINGLESEL,0,0);
			LvCol.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;				   // Type of mask                                
			LvCol.pszText=L"S No.";										   // First Header Text
			LvCol.cx=0x26;												   // width of column
		
			SendMessageW(hList,LVM_INSERTCOLUMN,0,(LPARAM)&LvCol);		  // Insert/Show the coloum
			LvCol.cx=0x70;												  // width between each coloum
			LvCol.pszText=L"Website";									  // Next coloum
			SendMessageW(hList,LVM_INSERTCOLUMN,1,(LPARAM)&LvCol);		// ...
			LvCol.cx=0x40;
			LvCol.pszText=L"Username";
			SendMessageW(hList,LVM_INSERTCOLUMN,2,(LPARAM)&LvCol);
			LvCol.cx=0x50;
			LvCol.pszText=L"PassWd";
			SendMessageW(hList,LVM_INSERTCOLUMN,3,(LPARAM)&LvCol);
			LvCol.cx=0x100;
			LvCol.pszText=L"Others";
			SendMessageW(hList,LVM_INSERTCOLUMN,4,(LPARAM)&LvCol);
			LvCol.cx=0x50;
			LvCol.pszText=L"Date Modified";
			SendMessageW(hList,LVM_INSERTCOLUMN,5,(LPARAM)&LvCol);
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
						hf = CreateFile(ofn.lpstrFile, GENERIC_READ|GENERIC_WRITE,0,(LPSECURITY_ATTRIBUTES) NULL,
										CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);
						write = encryptaes(master,write);
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
					}
					lremcou++;
					n++;
					m=n;
				};
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
		};
	};
	return (INT_PTR)FALSE;
};




void listman()
	{
		colorflag=0;
		n=0;row=0;column=1;k=0;														//	Items
		LvItem.mask=LVIF_TEXT;														// Text Style

//////////////////////////////////////////////////////////////////////////////////////////////
		ListView_SetExtendedListViewStyle(hList, LVS_EX_FULLROWSELECT);
		LvItem.iItem=0;															    // choose item  
		LvItem.iSubItem=0;															// Put in first coluom
		LvItem.pszText=L"1";														// Text to display (can be from a char variable) (Items)
		SendMessageW(hList,LVM_INSERTITEM,0,(LPARAM)&LvItem);						// Send info to the Listview
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
			LvItem.iSubItem=column;												 // Put in first coluom
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
			LvItem.pszText=bufr;												 // Text to display (can be from a char variable) (Items)
			LvItem.cchTextMax = wcslen(LvItem.pszText);
			SendMessageW(hList,LVM_SETITEM,0,(LPARAM)&LvItem);					 // Send info to the Listview
			column++;
			if(read[n]==L'▲'&&(n<readsize-1))
			{
				row++;
				column=1;
				LvItem.iItem=row;												 // choose item  
				LvItem.iSubItem=0;												 // Put in first coluom
				swprintf(snbuf,L"%d",row+1);
				LvItem.pszText=snbuf;											 // Text to display (can be from a char variable) (Items)
				SendMessageW(hList,LVM_INSERTITEM,0,(LPARAM)&LvItem);			 // Send info to the Listview
			};
			n++;
		};
	};
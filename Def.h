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

#include <CommDlg.h>
#include <Commctrl.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <string>
#include <WindowsX.h>
#include <Wincrypt.h>

#include <Dwmapi.h>
#include <GdiPlus.h>
#include <strsafe.h>
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
char pset[]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",buffer[10];
char nset[]="0123456789",buffern[10];
char sym[100]="",buffers[10];
int nnum=0,nsym=0,tempnum=2,tempsym=2,filesize=0,listflg=0,lremcou=0,listsel=0,editflag=0,readsize=0,cansave=0;//,savtitle=0;
int req_day=0,req_mon=0,req_year=0;
int tim = tim +((int)time(NULL));
wchar_t number[10],temp[10];
wchar_t symbol[100],master[100];
wchar_t * uname=0,* web=0,* oth=0;
wchar_t alpha[10];
wchar_t snbuf[3];
wchar_t sy[10];
wchar_t * read=0,* write=0,* bufr=0,* mdwrite=0,* mdread=0,* read2=0;
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
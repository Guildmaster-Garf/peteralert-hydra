#include <windows.h>
#include <wingdi.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "resource.h"

#define MAXACT 3
#define ID_TIMERPETERSCROLL 1
#define ID_TIMERPETERSCROLLPOPUP 4
#define ID_TIMERPETERPOPUP 3
#define ID_TIMERMAIN 2


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

void CreateDialogBox(HWND);
void RegisterDialogClass(HWND);

void ScrollOutImage(HWND, HBITMAP);
void ScrollInImage(HWND);

HINSTANCE ghInstance;
HBITMAP g_peter = NULL;
HBITMAP g_smallPeter = NULL;

int popupImgs[13] = { BM_WORSTMISTAKE1 , BM_WORSTMISTAKE2 , BM_WORSTMISTAKE3 , BM_WORSTMISTAKE4 , BM_WORSTMISTAKE5 , BM_WORSTMISTAKE6 , BM_WORSTMISTAKE7 , BM_WORSTMISTAKE8 , BM_WORSTMISTAKE9 , BM_WORSTMISTAKE10 , BM_WORSTMISTAKE11 , BM_WORSTMISTAKE12 , BM_WORSTMISTAKE13 };

HBITMAP popupImg = NULL;

int timer = 0;
int timebetween = 0;
int px = 0,py = 100;
int ppx = 0, ppy = 420;
int actions[3] = {0,0,0};
int latestaction = 0;
int firstcall = 1;
int scrolling = 1;

void PeterRedraw(HWND hwnd)
{
	//MessageBox(hwnd,"redrew peter","peter redraw",MB_OK);
	if(px < GetSystemMetrics(SM_CXFULLSCREEN))
    {	
    	px+=2;
	}
    else
    {
		px = 0;
		actions[0] = 0;
		KillTimer(hwnd,ID_TIMERPETERSCROLL);
	}
	InvalidateRect(hwnd,NULL,TRUE);
} 
void PopupPeter(HWND hwnd)
{
	scrolling = 0;
	//ppx = 0;
	KillTimer(hwnd,ID_TIMERPETERPOPUP);
	InvalidateRect(hwnd,NULL,TRUE);
} 

void ScrollOutImage(HWND hwnd, HBITMAP img)
{
	BITMAP bm;
	GetObject(popupImg, sizeof(bm), &bm);
	if(ppx > 0-bm.bmWidth)
	{
		ppx-=4;
	}
	else if (ppx <= 0-bm.bmWidth)
	{
		actions[1] = 0;
		KillTimer(hwnd,ID_TIMERPETERSCROLLPOPUP);
	}
	InvalidateRect(hwnd,NULL,TRUE);
}

void ScrollInImage(HWND hwnd)
{
	if(ppx < 0)
	{
		ppx+=4;
	}
	InvalidateRect(hwnd,NULL,TRUE);
}

HBITMAP randomPopUp()
{
	return LoadBitmap(GetModuleHandle(0),MAKEINTRESOURCE(popupImgs[rand()%7]));
}

int randact(HWND hwnd)
{
	int random = (rand()%3)+1;
	
	switch(random)
	{
		case 1:
			if(actions[0] == 0)
			{
				SetTimer(hwnd,ID_TIMERPETERSCROLL,50,(TIMERPROC)NULL);
				py = rand()% (GetSystemMetrics(SM_CYFULLSCREEN)-50);
				actions[0] = 1;
			}
			break;
		case 2:
			{
				if(actions[1] != 1)
				{
					popupImg = randomPopUp();
					BITMAP bm;
					GetObject(popupImg, sizeof(bm), &bm);
					ppy = rand()% (GetSystemMetrics(SM_CYFULLSCREEN)-bm.bmHeight);
					SetTimer(hwnd,ID_TIMERPETERSCROLLPOPUP,51,(TIMERPROC)NULL);
					SetTimer(hwnd,ID_TIMERPETERPOPUP,20000,(TIMERPROC)NULL);
					//MessageBox(hwnd,buf,"error",MB_OK);
					ppx = 0-bm.bmWidth;
					scrolling = 1;
					actions[1] = 1;
				}
			}
		break;
		case 3:
			CreateDialogBox(hwnd);
		break;
	}
	return random;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  	srand(time(NULL));
  	MSG  msg;
   	HWND hwnd;

  	WNDCLASSW wc = {0};
  	wc.lpszClassName = L"Window";
  	wc.hInstance     = hInstance;
  	wc.hbrBackground = CreateSolidBrush(RGB(255,0,0));
  	wc.lpfnWndProc   = WndProc;
  	//wc.hIcon = LoadIcon(NULL,MAKEINTRESOURCE(IDI_APPLICATION));

  	RegisterClassW(&wc);
  	hwnd = CreateWindowExW(WS_EX_TOOLWINDOW,wc.lpszClassName, L"Window",WS_POPUP | WS_VISIBLE ,0, 0, GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN), NULL, NULL, hInstance, NULL);
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT | WS_EX_LAYERED);
   	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SetLayeredWindowAttributes(hwnd, RGB(255,0,0), 0, LWA_COLORKEY);
  	ghInstance = hInstance;
  	
    SetTimer(hwnd,ID_TIMERMAIN,5000,(TIMERPROC) NULL);
  	//CreateDialogBox(hwnd);
  	while( GetMessage(&msg, NULL, 0, 0)) {
  		TranslateMessage(&msg);
    	DispatchMessage(&msg);
  	}
  
  	return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

  	switch(msg) {
  
   	case WM_CREATE:
   		SetWindowText(hwnd,"Fuck You");
   		g_peter = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(BM_PETER));
   		g_smallPeter = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(BM_PETER_SMALL));
   		if(g_peter == NULL)
        	MessageBox(hwnd, "Could not load BM_PETER!", "Error", MB_OK | MB_ICONEXCLAMATION);
        if(g_smallPeter == NULL)
        	MessageBox(hwnd, "Could not load BM_PETER_SMALL!", "Error", MB_OK | MB_ICONEXCLAMATION);
      	//g_hbmBall = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IC_PETER),IMAGE_ICON,0,0,LR_DEFAULTSIZE);
      	//if(g_hbmBall == NULL)
        	//MessageBox(hwnd, "Could not load IC_PETER!", "Error", MB_OK | MB_ICONEXCLAMATION);
      	RegisterDialogClass(hwnd);
	 	break;
          
    case WM_PAINT:
    	{
    		PAINTSTRUCT ps;
		    HDC hdc = BeginPaint(hwnd, &ps);
    		if(actions[0]==1)
    		{
				BITMAP bm;
		        HDC hdcMem = CreateCompatibleDC(hdc);
		        HBITMAP hbmOld = SelectObject(hdcMem, g_peter);
		        GetObject(g_peter, sizeof(bm), &bm);
		        StretchBlt(hdc,px,py,bm.bmWidth/2,bm.bmHeight/2,hdcMem,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
		        SelectObject(hdcMem, hbmOld);
		        DeleteDC(hdcMem);
		        //EndPaint(hwnd, &ps);
			}
		        /*if(px < GetSystemMetrics(SM_CXFULLSCREEN))
		        {
		        	px+=1;
				}
		        else
        			px = 0;	
				}*/

			if(actions[1]==1)
			{
				BITMAP bm;
		        //PAINTSTRUCT ps;
		        //HDC hdc = BeginPaint(hwnd, &ps);
		        HDC hdcMem = CreateCompatibleDC(hdc);
		        HBITMAP hbmOld = SelectObject(hdcMem, popupImg);
		        GetObject(popupImg, sizeof(bm), &bm);
		        StretchBlt(hdc,ppx,ppy,bm.bmWidth,bm.bmHeight,hdcMem,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
		        SelectObject(hdcMem, hbmOld);
		        DeleteDC(hdcMem);	
			}
			EndPaint(hwnd, &ps);
		}
    	break;
    	
    case WM_TIMER:
    {
    	switch(wParam)
    	{
    		case ID_TIMERMAIN:
    			latestaction = randact(hwnd);
    		break;
    		case ID_TIMERPETERSCROLL:
    			PeterRedraw(hwnd);
    		break;
    		case ID_TIMERPETERSCROLLPOPUP:
    			if(scrolling == 1)
    				ScrollInImage(hwnd);
    			if(scrolling == 0)
    				ScrollOutImage(hwnd, popupImg);
    		break;
    		case ID_TIMERPETERPOPUP:
    			PopupPeter(hwnd);
    		break;
		}
    	/*
		BITMAP bm;
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        SetStretchBltMode(hdc,HALFTONE);
        SetBrushOrgEx(hdc,px,py,NULL);
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbmOld = SelectObject(hdcMem, g_peter);
        GetObject(g_peter, sizeof(bm), &bm);
        StretchBlt(hdc,px,py,bm.bmWidth/4,bm.bmHeight/4,hdcMem,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
        SelectObject(hdcMem, hbmOld);
        DeleteDC(hdcMem);
        EndPaint(hwnd, &ps);
        if(px < GetSystemMetrics(SM_CXFULLSCREEN))
        {	
        	px+=1;
		}
        else
        	KillTimer(hwnd, ID_TIMER1);
        InvalidateRect(hwnd,NULL,TRUE);
        printf("timer called\n");*/
	}
	break;


      case WM_DESTROY:
      {
        DeleteObject(g_peter);
        DeleteObject(g_smallPeter);
        DeleteObject(popupImg);
          PostQuitMessage(0);
          return 0;
      }
  }
  return DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) {

    	case WM_CREATE:
      {
      	PlaySound(MAKEINTRESOURCE(WV_BOOWOMP), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC); 

        	CreateWindowW(L"button", L"Ok",
         WS_VISIBLE | WS_CHILD ,50, 50, 80, 25, hwnd, (HMENU) 1, NULL, NULL);
      }
    break;

    case WM_COMMAND:
        CreateDialogBox(hwnd);
        CreateDialogBox(hwnd);
        DestroyWindow(hwnd);
    break;
    case WM_PAINT:
    {
        BITMAP bm;
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbmOld = SelectObject(hdcMem, g_smallPeter);
        GetObject(g_smallPeter, sizeof(bm), &bm);
        BitBlt(hdc,75,10,32,32,hdcMem,0,0,SRCCOPY);
        //StretchBlt(hdc,75,10,32,32,hdcMem,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
        SelectObject(hdcMem, hbmOld);
        DeleteDC(hdcMem);
        EndPaint(hwnd, &ps);
        break;
    }
    break;

    case WM_CLOSE:
        CreateDialogBox(hwnd);
        CreateDialogBox(hwnd);
        DestroyWindow(hwnd);
        break;

  }
  
  return (DefWindowProcW(hwnd, msg, wParam, lParam));
}

void RegisterDialogClass(HWND hwnd) {

  	WNDCLASSEXW wc = {0};
  	wc.cbSize           = sizeof(WNDCLASSEXW);
  	wc.lpfnWndProc      = (WNDPROC) DialogProc;
  	wc.hInstance        = ghInstance;
  	wc.hbrBackground    = GetSysColorBrush(COLOR_3DFACE);
  	wc.lpszClassName    = L"DialogClass";
  	RegisterClassExW(&wc);

}

void CreateDialogBox(HWND hwnd) {

  HWND dlgHWND = CreateWindowExW(WS_EX_DLGMODALFRAME  | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,  L"DialogClass", L"Peter Alert", 
        WS_VISIBLE | WS_SYSMENU | WS_CAPTION , rand()%GetSystemMetrics(SM_CXFULLSCREEN), rand()%GetSystemMetrics(SM_CYFULLSCREEN), 200, 150,
        NULL, NULL, ghInstance,  NULL);
}

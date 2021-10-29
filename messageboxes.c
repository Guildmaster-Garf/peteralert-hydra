#include "resource.h"
#include <windows.h>
#include <wingdi.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

void CreateDialogBox(HWND);
void RegisterDialogClass(HWND);

HINSTANCE ghInstance;
HICON g_hbmBall = NULL;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
  srand(time(NULL));
  MSG  msg;    
  HWND hwnd;

  WNDCLASSW wc = {0};
  wc.lpszClassName = L"Window";
  wc.hInstance     = hInstance;
  wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
  wc.lpfnWndProc   = WndProc;
  
  RegisterClassW(&wc);
  hwnd = CreateWindowW(wc.lpszClassName, L"Window",
                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                8000, 8000, 250, 150, NULL, NULL, hInstance, NULL);  

  ghInstance = hInstance;
  //hIcon1 = LoadIcon(ghInstance, MAKEINTRESOURCE(IC_PETER));
  CreateDialogBox(hwnd);
  while( GetMessage(&msg, NULL, 0, 0)) {
    DispatchMessage(&msg);
  }
  
  return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

  switch(msg) {
  
      case WM_CREATE:
          g_hbmBall = (HICON)LoadImage(GetModuleHandle(NULL),MAKEINTRESOURCE(IC_PETER),IMAGE_ICON,0,0,LR_DEFAULTSIZE);
          printf("%i\n", GetLastError());
          if(g_hbmBall == NULL)
            MessageBox(hwnd, "Could not load IC_PETER!", "Error", MB_OK | MB_ICONEXCLAMATION);
          RegisterDialogClass(hwnd);
          CreateWindowW(L"button", L"Show dialog",    
              WS_VISIBLE | WS_CHILD ,
              20, 50, 95, 25, hwnd, (HMENU) 1, NULL, NULL);  
          break;

          

      case WM_COMMAND:
      {
          for(int x = 0; x < 20; x++)
          {
            CreateDialogBox(hwnd);
          }
      }
          break;

      case WM_DESTROY:
      {
        DeleteObject(g_hbmBall);
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

        CreateWindowW(L"button", L"Ok",    
          WS_VISIBLE | WS_CHILD ,
          50, 50, 80, 25, hwnd, (HMENU) 1, NULL, NULL);  
        }
    break;

    case WM_COMMAND:
        CreateDialogBox(hwnd);
        CreateDialogBox(hwnd);
        DestroyWindow(hwnd);
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT     ps;
        HDC             hdc;
        hdc = BeginPaint(hwnd,&ps);
        DrawIcon(hdc, 75, 10, g_hbmBall); 
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

  HWND dlgHWND = CreateWindowExW(WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,  L"DialogClass", L"Peter Alert", 
        WS_VISIBLE | WS_SYSMENU | WS_CAPTION , rand()%GetSystemMetrics(SM_CXFULLSCREEN)-150, rand()%GetSystemMetrics(SM_CYFULLSCREEN)-200, 200, 150, 
        NULL, NULL, ghInstance,  NULL);
}

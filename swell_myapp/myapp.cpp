/*
    swell_myapp

    This software is provided 'as-is', without any express or implied
    warranty.  In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
       claim that you wrote the original software. If you use this software
       in a product, an acknowledgment in the product documentation would be
       appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#ifdef _WIN32
#include <windows.h>
#include "../WDL/win32_utf8.h"
#else
#include "../WDL/swell/swell.h"
#include "../WDL/swell/swell-internal.h"
#endif

#include "../WDL/wingui/wndsize.h"

#include "resource.h"

HINSTANCE g_hInstance;
HWND g_hwnd;

WDL_DLGRET mainProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static WDL_WndSizer resize;
  switch (uMsg)
  {
    case WM_INITDIALOG:
      g_hwnd=hwndDlg;
#ifdef _WIN32
      {
        HICON icon=LoadIcon(g_hInstance,MAKEINTRESOURCE(IDI_ICON1));
        SetClassLongPtr(hwndDlg,GCLP_HICON,(LPARAM)icon);
      }
#endif

      resize.init(hwndDlg);
      resize.init_item(IDCANCEL,0,1,0,1);
    return 1;
    case WM_CLOSE:
      DestroyWindow(hwndDlg);
    return 1;
    case WM_DESTROY:
      g_hwnd=NULL;
#ifdef __APPLE__
      SWELL_PostQuitMessage(0);
#endif
    break;
    case WM_SIZE:
      if (wParam != SIZE_MINIMIZED)
        resize.onResize();
    break;
    case WM_COMMAND:
      switch (LOWORD(wParam))
      {
        case IDCANCEL:
          DestroyWindow(hwndDlg);
        break;
      }
    break;
  }
  return 0;
}



#ifdef _WIN32

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  g_hInstance = hInstance;

  DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG1), GetDesktopWindow(), mainProc);

  ExitProcess(0);
  
  return 0;
}

#else
/************** SWELL stuff ********** */

#ifdef __APPLE__
extern "C" {
#endif

const char **g_argv;
int g_argc;

#ifdef __APPLE__
};
#endif

INT_PTR SWELLAppMain(int msg, INT_PTR parm1, INT_PTR parm2)
{
  switch (msg)
  {
    case SWELLAPP_ONLOAD:
      {
      }
    break;
    case SWELLAPP_LOADED:
      {
        HWND h=CreateDialog(NULL,MAKEINTRESOURCE(IDD_DIALOG1),NULL,mainProc);
        ShowWindow(h,SW_SHOW);
      }
    break;
    case SWELLAPP_DESTROY:
      if (g_hwnd) DestroyWindow(g_hwnd);
    break;
  }
  return 0;
}


#ifndef __APPLE__

int main(int argc, const char **argv)
{
  extern HWND g_hwnd;

  g_argc=argc;
  g_argv=argv;
  SWELL_initargs(&argc,(char***)&argv);
  SWELL_Internal_PostMessage_Init();
  SWELL_ExtendedAPI("APPNAME",(void*)"MyApp");
  SWELLAppMain(SWELLAPP_ONLOAD,0,0);
  SWELLAppMain(SWELLAPP_LOADED,0,0);
  while (g_hwnd && !g_hwnd->m_hashaddestroy) {
    SWELL_RunMessageLoop();
    Sleep(10);
  }
  SWELLAppMain(SWELLAPP_DESTROY,0,0);
  return 0;
}

#endif


#include "../WDL/swell/swell-dlggen.h"
#include "res.rc_mac_dlg"
#undef BEGIN
#undef END
#include "../WDL/swell/swell-menugen.h"
#include "res.rc_mac_menu"

#endif

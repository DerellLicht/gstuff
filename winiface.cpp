//***********************************************************************
//  gstuff.c - Windows interface to graphics demo program
//                                           
//  Written by:   Daniel D. Miller           
//                                           
//  Last Update:  12/04/07 14:55             
//***********************************************************************

#include <windows.h>
#include <time.h>

#include "gstuff.h"     //  for timer functions
#include "palettes.h"   //  24-bit palette functions
#include "gobjects.h"   //  graphics-object classes
#include "gfuncs.h"
#include "demo.h"
#include "keywin32.h"

static unsigned key_mask = 0 ;
HINSTANCE g_hinst = 0;

//lint -esym(759, get_clocks_per_second)
//lint -esym(765, get_clocks_per_second)

//  demo.cpp, rainbow.cpp
extern rainbow rainbow0 ;
extern line_games lgames0 ;

//*************************************************************************
//  timer usage:
//    u64 tf, ti = proc_time ();
//    tf = (proc_time () - ti) / get_clocks_per_second ();
//*************************************************************************

//*************************************************************************
//  clock() and GetTickCount() have 15-16 msec resolution on WinXP
//  QueryPerformanceCounter() has nearly 1-usec resolution.
//*************************************************************************
u64 get_clocks_per_second(void)
{
   // return (unsigned long) CLOCKS_PER_SEC ;
   static u64 clocks_per_second = 1 ;
   if (clocks_per_second == 1) {
      LARGE_INTEGER tfreq ;
      QueryPerformanceFrequency(&tfreq) ;
      clocks_per_second = (u64) tfreq.QuadPart ;
   }
   // wsprintf(tempstr, "QPF=%I64u\n", clocks_per_second) ;
   // OutputDebugString(tempstr) ;
   return clocks_per_second ;
}

//*************************************************************************
u64 get_clocks_per_msec(void)
{
   return get_clocks_per_second() / 1000 ;
}

//*****************************************************************************
u64 proc_time(void)
{
   // return (unsigned) clock() ;
   LARGE_INTEGER ti ;
   QueryPerformanceCounter(&ti) ;
   return (u64) ti.QuadPart ;
}

//***********************************************************************
static void show_vital_data(HWND hwnd)
{
   wsprintf(tempstr, "Client= { %d,%d }, max= { %d,%d }", 
      cxClient, cyClient, maxx, maxy) ;
   show_message(hwnd, 100, 70, 0x02, tempstr) ;
}

//***********************************************************************
extern int read_a_font(HWND hwnd);

void process_keystroke(HWND hwnd, unsigned inchr)
{
   int prev_state = demo_state ;
   int result = check_for_state_change(hwnd, inchr) ;
   if (result < 0) {
      switch (inchr) {
      case kESC:
         if (demo_state == 0)
            PostQuitMessage (0) ;
         else 
            demo_state = 0 ;
         break;

      case kF2:
         if (demo_state == 19) {
            read_a_font(hwnd);
         } else {
            wsprintf(tempstr, "F2 is only valid in ASCII state\n") ;
            OutputDebugString(tempstr) ;
         }
         break;

      case kGPlus:
         // toggle_DAC_table() ;
         next_DAC_table() ;
         we_should_redraw = 1 ;
         break;

      case kGMinus:
         // toggle_DAC_table() ;
         prev_DAC_table() ;
         we_should_redraw = 1 ;
         break;

      case kSPACE:
         pause_the_race ^= 1 ;
         break;

      case kTAB:
         use_solid_pattern ^= 1 ;
         lgames0.update_line_algorithm() ;  //  move to LineGames class
         break;

      case kQMark:
         show_vital_data(hwnd) ;
         break;

#ifndef SKIP_FLAME_PALETTES
      case kPGUP:
         update_flame_page(0) ;
         we_should_redraw = 1 ;
         break;

      case kPGDN:
         update_flame_page(1) ;
         we_should_redraw = 1 ;
         break;
#endif

      case kCc:
         PostQuitMessage (0) ;
         break;

      default:
         // demo_state = 0 ;
         wsprintf(tempstr, "PRESS=0x%04X", inchr) ;
         show_message(hwnd, 100, 40, 0xA, tempstr) ;
         break;
      }
   }
   if (prev_state != demo_state)
      we_should_redraw = 1 ;
}

//***********************************************************************
void update_menu_item(HWND hwnd, int xPos, int yPos);
void select_menu_item(HWND hwnd, int xPos, int yPos);

static LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
   // HBRUSH hBrush ;
   HDC    hdc ;
   PAINTSTRUCT ps;
   unsigned cx, cy ;

   switch (iMsg) {
    
   case WM_CREATE:
      hdc = GetDC (hwnd) ;

      // Fill_DAC_Table(0);

      // GetTextMetrics (hdc, &tm) ;
      // cxChar = tm.tmAveCharWidth ;
      // cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2 ;
      // cyChar = tm.tmHeight + tm.tmExternalLeading ;
      cxClient = 0 ;
      cyClient = 0 ;

      we_should_redraw = 1 ;
      ReleaseDC (hwnd, hdc) ;
      return 0 ;

   case WM_SETFOCUS:
      we_should_redraw = 1 ;
      return 0 ;

   case WM_MOUSEMOVE:
      // fwKeys = wParam;              // key flags 
      cx = LOWORD (lParam) ;  // horizontal position of cursor 
      cy = HIWORD (lParam) ;  // vertical position of cursor 
      update_menu_item(hwnd, cx, cy) ;
      return 0;
      
   case WM_LBUTTONDOWN :
      cx = LOWORD (lParam) ;
      cy = HIWORD (lParam) ;
      select_menu_item(hwnd, cx, cy) ;
      return 0;

   case WM_RBUTTONDOWN :
      process_keystroke(hwnd, kESC) ;
      return 0;

   case WM_SIZE:
      cxClient = LOWORD (lParam) ;
      cyClient = HIWORD (lParam) ;
      maxx = cxClient - 1 ;
      maxy = cyClient - 1 ;
      //  these four are used only by DrawRainbow
      rainbow0.update_boundaries(cxClient, cyClient) ;

      we_should_redraw = 1 ;
      return 0 ;

   case WM_PAINT:
      hdc = BeginPaint (hwnd, &ps) ;

      if (cxClient == 0 || cyClient == 0)
         return 0 ;

      if (demo_state == 12) {
         Clear_Screen(hdc, 0) ;
      }
      display_current_operation(hwnd) ;
      // show_vital_data(hwnd);

      EndPaint (hwnd, &ps) ;
      return 0 ;
    
   case WM_KEYDOWN :
   case WM_SYSKEYDOWN :
        // ShowKey (hwnd, 0, "WM_KEYDOWN", wParam, lParam) ;
        if (wParam == VK_SHIFT) {
           key_mask |= kShift ;
        } else if (wParam == VK_CONTROL) {
           key_mask |= kCtrl ;
        } else if (wParam == VK_MENU) {
           key_mask |= kAlt ;
        } else {
           wParam |= key_mask ;
           process_keystroke(hwnd, wParam) ;
        }
        return 0 ;

   case WM_KEYUP :
   case WM_SYSKEYUP :
        // ShowKey (hwnd, 0, "WM_KEYUP", wParam, lParam) ;
        if (wParam == VK_SHIFT) {
           key_mask &= ~kShift ;
        } else if (wParam == VK_CONTROL) {
           key_mask &= ~kCtrl ;
        } else if (wParam == VK_MENU) {
           key_mask &= ~kAlt ;
        } else {
           // wParam |= key_mask ;
           // process_keystroke(hwnd, wParam) ;
        }
        return 0 ;

   // case WM_CHAR:
   //    // sprintf(tempstr, "key=0x%04X, mask=0x%04X", wParam, key_mask) ;
   //    // show_message(hwnd, 100, 20, 0x1B, tempstr) ;
   //    wParam |= key_mask ;
   //    process_keystroke(hwnd, wParam) ;
   // 
   //    return 0;

   case WM_DESTROY:
      PostQuitMessage (0) ;
      return 0 ;
   }  //lint !e744  no default on switch

   return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
}

//***********************************************************************
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                  PSTR szCmdLine, int iCmdShow)
   {
   static char const szAppName[] = "gstuff" ;
   HWND        hwnd ;
   MSG         msg ;
   WNDCLASSEX  wndclass ;
   g_hinst = hInstance;

   /* Seed the random-number generator with current time so that
    * the numbers will be different every time we run.
    */
#ifndef _lint
   srand( (unsigned)time( NULL ) );
#endif

   wndclass.cbSize        = sizeof (wndclass) ;
   wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
   wndclass.lpfnWndProc   = WndProc ;
   wndclass.cbClsExtra    = 0 ;
   wndclass.cbWndExtra    = 0 ;
   wndclass.hInstance     = hInstance ;
   // wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
   // wndclass.hIconSm       = LoadIcon (NULL, IDI_APPLICATION) ;
   wndclass.hIcon         = LoadIcon (hInstance, MAKEINTRESOURCE(GSTUFF));
   wndclass.hIconSm       = LoadIcon (hInstance, MAKEINTRESOURCE(GSTUFF));
   wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
   // wndclass.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH) ;
   wndclass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1) ;
   wndclass.lpszMenuName  = NULL ;
   wndclass.lpszClassName = szAppName ;

   RegisterClassEx (&wndclass) ;

   hwnd = CreateWindow (szAppName, "Draw practice graphics",
                        WS_OVERLAPPEDWINDOW,
                        // CW_USEDEFAULT, CW_USEDEFAULT,
                        // CW_USEDEFAULT, CW_USEDEFAULT,
                        200, 100,
                        1024, 768,
                        NULL, NULL, hInstance, NULL) ;

   ShowWindow (hwnd, iCmdShow) ;
   UpdateWindow (hwnd) ;

   while (TRUE) {
      if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)) {
         if (msg.message == WM_QUIT)
              break ;
   
         TranslateMessage (&msg) ;
         DispatchMessage (&msg) ;
      }
      else {
         display_current_operation(hwnd) ;
      }
   }

   // while (GetMessage (&msg, NULL, 0, 0)) {
   //    TranslateMessage (&msg) ;
   //    DispatchMessage (&msg) ;
   // }
   return msg.wParam ;
}  //lint !e715


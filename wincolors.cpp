#include <windows.h>

#include "gstuff.h"     //  u8, etc
#include "palettes.h"   //  24-bit palette functions
#include "gobjects.h"   //  graphics-object classes
#include "gfuncs.h"     //  graphics primitives
#include "demo.h"

#define  WCLR_NAME_CT   30
static char const * const wincolor_names[WCLR_NAME_CT] = {
"COLOR_SCROLLBAR",
"COLOR_BACKGROUND",  //  1: use for background
"COLOR_ACTIVECAPTION",
"COLOR_INACTIVECAPTION",
"COLOR_MENU",
"COLOR_WINDOW",
"COLOR_WINDOWFRAME",
"COLOR_MENUTEXT",
"COLOR_WINDOWTEXT",  //  8: use for text
"COLOR_CAPTIONTEXT",
"COLOR_ACTIVEBORDER ",
"COLOR_INACTIVEBORDER",
"COLOR_APPWORKSPACE",
"COLOR_HIGHLIGHT",
"COLOR_HIGHLIGHTTEXT",
"COLOR_3DFACE",
"COLOR_3DSHADOW",
"COLOR_GRAYTEXT",
"COLOR_BTNTEXT",
"COLOR_INACTIVECAPTIONTEXT",
"COLOR_3DHILIGHT",
"COLOR_3DDKSHADOW",
"COLOR_3DLIGHT",
"COLOR_INFOTEXT",
"COLOR_INFOBK",
"COLOR_UNUSED",   //  25: not used
"COLOR_HOTLIGHT",
"COLOR_GRADIENTACTIVECAPTION",
"COLOR_GRADIENTINACTIVECAPTION",
"COLOR_MENUHILIGHT"
} ;

//***********************************************************************
wincolors::wincolors(char *title_text)
: graph_object(title_text)
//  per http://www.acm.org/crossroads/xrds1-4/ovp.html
// , _v1(v1), _v2(v2), _v3(v3)
{
}

//************************************************************************
void wincolors::update_display(HWND hwnd)
{
   HDC hdc ;

   if (!we_should_redraw)
      return ;

   set_DAC_table(0) ;
   hdc = GetDC (hwnd) ;
   Clear_Window(hdc, 0);

   unsigned gapx = 30 ;
   unsigned gapy = 20 ;
   unsigned dx =  ((unsigned)cxClient - (4 * gapx)) / 3 ;
   unsigned dy = 50 ;
   unsigned idx ;
   char bfr[81] ;

   COLORREF fgnd = GetSysColor(COLOR_WINDOWTEXT) ;
   COLORREF frame_white = RGB(192, 192, 192) ;
   COLORREF frame_black = RGB(0, 0, 0) ;
   unsigned xi = gapx ;
   unsigned yi = 2 * gapy ;

   SetBkMode(hdc, TRANSPARENT);
   int line_idx = 0 ;
   for (idx=0; idx < WCLR_NAME_CT; idx++) {
      if (idx == 1  ||  idx == 8  ||  idx == 25)
         continue;
      unsigned xf = xi + dx ;
      unsigned yf = yi + dy ;
      COLORREF bgnd = GetSysColor(idx) ;
      wsprintf(bfr, " %u: %s ", idx, wincolor_names[idx]) ;
      SolidRect(hdc, xi, yi, xf, yf, bgnd) ;
      if (bgnd == 0) {
         Box(hdc, xi, yi, xf, yf, frame_white) ;
         SetTextColor(hdc, frame_white) ;
      }
      else {
         Box(hdc, xi, yi, xf, yf, frame_black) ;
         SetTextColor(hdc, fgnd) ;
      }

      TextOut(hdc, xi+10, yi+15, bfr, strlen(bfr)) ;

      xi += (gapx + dx) ;
      if (++line_idx >= 3) {
         line_idx = 0 ;
         xi = gapx ;
         yi += (gapy + dy) ;
      }
   }

   //*****************************************************
   //  do cleanup and exit
   //*****************************************************
   ReleaseDC (hwnd, hdc) ;
}

//************************************************************************
//  return TRUE if we handled this key,
//  FALSE if we did not
//************************************************************************
bool wincolors::process_key(unsigned key_id)
{
   return FALSE ;
}


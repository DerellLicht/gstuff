#include <windows.h>

#include "gstuff.h"     //  u8, etc
#include "palettes.h"   //  24-bit palette functions
#include "gobjects.h"   //  graphics-object classes
#include "gfuncs.h"     //  graphics primitives
#include "alg_selector.h"

#define  MAX_CIRCLE_DIAM   60

//***********************************************************************
circles::circles(char *title_text) 
: graph_object(title_text) 
//  per http://www.acm.org/crossroads/xrds1-4/ovp.html
// , _v1(v1), _v2(v2), _v3(v3)
{ 
}

//************************************************************************
void circles::update_display(HWND hwnd)
{
   HBRUSH hBrush ;
   HDC    hdc ;
   int x, y, w ;

   if (pause_the_race)
      return ;

   cycle_count++ ;
   if (use_solid_pattern)
      hBrush = CreateSolidBrush (random_colorref()) ;
      // hBrush = CreateSolidBrush (random_palette_ref()) ;
   else
      hBrush = CreateHatchBrush (fill_patterns[random_int(6)], random_colorref()) ;
   hdc = GetDC (hwnd) ;
   SetBkColor(hdc, random_colorref()) ;
   SelectObject(hdc, hBrush) ;

   // FillRect (hdc, &rect, hBrush) ;
   // x = rand() % cxClient ;
   // y = rand() % cyClient ;
   x = random_int(cxClient) ;
   y = random_int(cyClient) ;
   if (!use_solid_pattern) {
      // w = rand() % 100 ;
      // r = (min(cxClient, cyClient) * 4 / 10) ;
      // r = min(r, 60) ;
      w = 1 + random_int(MAX_CIRCLE_DIAM) ;
   } else {
      w = CONST_SIZE / 2 ;
   }
   Ellipse(hdc, x-w, y-w, x+w, y+w) ;

   ReleaseDC (hwnd, hdc) ;

   DeleteObject (hBrush) ;
}

//************************************************************************
//  return TRUE if we handled this key,
//  FALSE if we did not
//************************************************************************
bool circles::process_key(unsigned key_id)
{
   return FALSE ;
}


#include <windows.h>

#include "gstuff.h"     //  u8, etc
#include "palettes.h"   //  24-bit palette functions
#include "gobjects.h"   //  graphics-object classes
#include "gfuncs.h"     //  graphics primitives
#include "demo.h"

#define  MAX_BOX_EDGE   100

//***********************************************************************
squares::squares(char *title_text) 
: graph_object(title_text) 
//  per http://www.acm.org/crossroads/xrds1-4/ovp.html
// , _v1(v1), _v2(v2), _v3(v3)
{ 
}

//************************************************************************
void squares::update_display(HWND hwnd)
{
   HBRUSH hBrush ;
   HDC    hdc ;
   RECT   rect ;
   // POINT poly[5];     /* Space to hold datapoints  */
   int xl, yl, xr, yu ;

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

   xl = random_int(cxClient) ;
   yl = random_int(cyClient) ;
   if (use_solid_pattern == 0) {
      yu = yl + 1 + random_int(MAX_BOX_EDGE) ;
      xr = xl + 1 + random_int(MAX_BOX_EDGE) ;
      // yu = random_int(cyClient) ;
      // xr = random_int(cxClient) ;
      // if (xl > xr) {
      //    itemp = xl ;
      //    xl = xr ;
      //    xr = itemp ;
      // }
      // if (yl > yu) {
      //    itemp = yl ;
      //    yl = yu ;
      //    yu = itemp ;
      // }
   } else {
      xr = xl + (int) CONST_SIZE ;
      yu = yl + (int) CONST_SIZE ;
   }

   SetRect (&rect, xl, yl, xr, yu) ;
   FillRect (hdc, &rect, hBrush) ;
   Box(hdc, xl, yl, xr, yu, (COLORREF) 0) ;
   ReleaseDC (hwnd, hdc) ;

   DeleteObject (hBrush) ;
}

//************************************************************************
//  return TRUE if we handled this key,
//  FALSE if we did not
//************************************************************************
bool squares::process_key(unsigned key_id)
{
   return FALSE ;
}


#include <windows.h>

#include "gstuff.h"     //  u8, etc
#include "palettes.h"   //  24-bit palette functions
#include "gobjects.h"   //  graphics-object classes
#include "gfuncs.h"     //  graphics primitives
#include "demo.h"

#define  DLG_X0   8
#define  DLG_Y0   8
#define  DLG_DX   5
#define  DLG_DY   6

//***********************************************************************
triangles::triangles(char *title_text) 
: graph_object(title_text) 
//  per http://www.acm.org/crossroads/xrds1-4/ovp.html
// , _v1(v1), _v2(v2), _v3(v3)
{ 
}

//************************************************************************
void triangles::update_display(HWND hwnd)
{
   HDC    hdc ;
   int j, x, y, x1, y1 ;
   double m ;
   static unsigned attr = 1 ;
   HPEN hPen ;
   POINT pts[4] ;
   unsigned max_colors = get_palette_entries() ;

   if (pause_the_race)
      return ;
   if (we_should_redraw)
      Clear_Window(hwnd, 0);
   cycle_count++ ;
   hdc = GetDC (hwnd) ;

   x = DLG_X0 ; //  x offset
   y = DLG_Y0 ; //  y offset
   x1 = maxx - DLG_X0 ;
   y1 = maxy - DLG_Y0 ;
   for (j=1; j<=20; j++) {
      hPen = CreatePen(PS_SOLID, 1, get_palette_entry(attr)) ;
      SelectObject(hdc, hPen) ;

      // MoveToEx(hdc, x, y, NULL) ;
      // LineTo  (hdc, x1, y) ;
      // LineTo  (hdc, x1, y1) ;
      // LineTo  (hdc, x, y) ;
      pts[0].x = x ;
      pts[0].y = y ;
      pts[1].x = x1 ;
      pts[1].y = y ;
      pts[2].x = x1 ;
      pts[2].y = y1 ;
      pts[3].x = x ;
      pts[3].y = y ;
      Polyline(hdc, &pts[0], 4) ;

      SelectObject(hdc, GetStockObject(BLACK_PEN)) ;  //  deselect my pen
      DeleteObject (hPen) ;

      //  update our points
      m = (double) ((double) (x1 - x + 1) 
                  / (double) (y1 - y + 1)) ;
      if (m >= 0.0) {
         x += (int) (5.0 * m) ;   //  x delta
         y += DLG_DX ;   //  y delta
      } else {
         y += (int) (5.0 * m) ;   //  x delta
         x += DLG_DX ;   //  y delta
      }
      x1 -= DLG_DX ;
      y1 -= DLG_DY ;
      if (++attr >= max_colors) {
         attr = 1 ;
      }
   }

   ReleaseDC (hwnd, hdc) ;
}

//************************************************************************
//  return TRUE if we handled this key,
//  FALSE if we did not
//************************************************************************
bool triangles::process_key(unsigned key_id)
{
   return FALSE ;
}


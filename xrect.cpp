//*******************************************************
// Demo_Rect - Tests the Solid_Rect routine              
//*******************************************************
#include <windows.h>

#include "gstuff.h"     //  u8, etc
#include "palettes.h"   //  24-bit palette functions
#include "gobjects.h"   //  graphics-object classes
#include "gfuncs.h"     //  graphics primitives
#include "demo.h"
#include "rgb_data.h"

#define  XRLAYERS    4

#define  XSCALE   90
#define  YSCALE   90

//***********************************************************************
xrect::xrect(char *title_text) 
: graph_object(title_text) 
//  per http://www.acm.org/crossroads/xrds1-4/ovp.html
// , _v1(v1), _v2(v2), _v3(v3)
{ 
}

//************************************************************************
void xrect::Solid_XRect(HDC hdc, int xl, int yu, int xr, int yl, int Color)
{
   RECT   rect ;
   SetRect (&rect, xl, yu, xr, yl) ;
   HBRUSH hBrush = CreateSolidBrush (drgb[Color].value) ;
   FillRect (hdc, &rect, hBrush) ;
   DeleteObject (hBrush) ;
}

//************************************************************************
void xrect::update_display(HWND hwnd)
{
   HDC    hdc ;
   int i,j,k,color;
   int xl, xr, yu, yl ;
   int bx, by, px, py, dx, dy ;
   int baseIdx = 136 ;

   if (!we_should_redraw) 
      return ;

   //  do computations before drawing
   bx = maxx / 13 ;
   by = maxy /  6 ;

   // px = 2 * (XRLAYERS+1) - 1 ;
   // py = 2 * (XRLAYERS+1) - 1 ;
   px = 2 * (XRLAYERS) - 1 ;
   py = 2 * (XRLAYERS) - 1 ;

   hdc = GetDC (hwnd) ;
   Clear_Window(hdc, 0);
   //  layers
   for (i = 0; i < XRLAYERS; i++) {
      //  rows
      color = baseIdx + i ;
      for (j = 0; j < 6; j++) {
         dy = (cyClient * j) / 6 ;
         //  columns
         for (k = 0; k < 13; k++) {  
            dx = (cxClient * k) / 13 ;

            xl = (i * bx) / px ;
            yu = (i * by) / py ;
            xr = ((XRLAYERS+i) * bx) / px ;
            yl = ((XRLAYERS+i) * by) / py ;

            xl = xl * XSCALE / 100;
            yu = yu * YSCALE / 100 ;
            xr = xr * XSCALE / 100 ;
            yl = yl * YSCALE / 100 ;

            xl += dx ;
            yu += dy ;
            xr += dx ;
            yl += dy ;

            Solid_XRect(hdc, xl, yu, xr, yl, color);
            color += 4 ;
         }
      }
   }

   ReleaseDC (hwnd, hdc) ;
}

//************************************************************************
//  return TRUE if we handled this key,
//  FALSE if we did not
//************************************************************************
bool xrect::process_key(unsigned key_id)
{
   return FALSE ;
}


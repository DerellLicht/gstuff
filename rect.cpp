#include <windows.h>

#include "gstuff.h"     //  u8, etc
#include "palettes.h"   //  24-bit palette functions
#include "gobjects.h"   //  graphics-object classes
#include "gfuncs.h"     //  graphics primitives
#include "demo.h"

//***********************************************************************
rect::rect(char *title_text) 
: graph_object(title_text) 
//  per http://www.acm.org/crossroads/xrds1-4/ovp.html
// , _v1(v1), _v2(v2), _v3(v3)
{ 
}

//************************************************************************
void rect::update_display(HWND hwnd)
{
   HDC hdc ;
   int i,j,k,sx,sy,x,y,color;

   if (!we_should_redraw) 
      return ;

   wsprintf(tempstr, "Color Bars, palette #%d", get_curr_palette()) ;
   SetWindowText(hwnd, tempstr) ;

   hdc = GetDC (hwnd) ;
   Clear_Window(hdc, 0);
   sx   = maxx/16;
   sy   = maxy/12;
   for (i = 0; i < 5; i++)
   for (j = 0; j < 6; j++)
   for (k = 0; k < 8; k++) {
      x   = k * maxx/8 + i * sx/5;
      y   = j * maxy/6 + i * sy/5;
      color = 16 + k + 8 * (j + 6 * i);
      Solid_Rect(hdc, x, y, x+sx-1, y+sy-1, (BYTE) color);
   }

   ReleaseDC (hwnd, hdc) ;
}

//************************************************************************
//  return TRUE if we handled this key,
//  FALSE if we did not
//************************************************************************
bool rect::process_key(unsigned key_id)
{
   return FALSE ;
}


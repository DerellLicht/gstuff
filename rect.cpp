#include <windows.h>

#include "gstuff.h"     //  u8, etc
#include "palettes.h"   //  24-bit palette functions
#include "gobjects.h"   //  graphics-object classes
#include "gfuncs.h"     //  graphics primitives
#include "alg_selector.h"

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
   int layer,row,col,sx,sy,x,y,color;

   if (!we_should_redraw) 
      return ;

   wsprintf(tempstr, "Color Bars, palette #%d", get_curr_palette()) ;
   SetWindowText(hwnd, tempstr) ;

   hdc = GetDC (hwnd) ;
   Clear_Window(hdc, 0);
   sx   = maxx/16;
   sy   = maxy/12;
   for (layer = 0; layer < 5; layer++) //  select layer
   for (row = 0; row < 6; row++) //  select row
   for (col = 0; col < 8; col++) {  //  select column
      x   = col * maxx/8 + layer * sx/5;
      y   = row * maxy/6 + layer * sy/5;
      color = 16 + col + 8 * (row + 6 * layer);
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


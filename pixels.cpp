#include <windows.h>

#include "gstuff.h"     //  for timer functions
#include "palettes.h"   //  24-bit palette functions
#include "gobjects.h"   //  graphics-object classes
#include "gfuncs.h"     //  graphics primitives
#include "demo.h"

#define  PIX_DX   3
#define  PIX_DY   3

//***********************************************************************
pixels::pixels(char *title_text) 
: graph_object(title_text) 
, dp_char_width(0)
, dp_char_height(0)
, rows(0)
, columns(0)
, color(0)
//  per http://www.acm.org/crossroads/xrds1-4/ovp.html
// , _v1(v1), _v2(v2), _v3(v3)
{ 
}

//************************************************************************
void pixels::update_display(HWND hwnd)
{
   COLORREF pcolor ;

   if (pause_the_race)
      return ;

   if (we_should_redraw) {
      color = 0 ;
      Clear_Window(hwnd, 0);
      dp_char_width  = PIX_DX ;
      dp_char_height = PIX_DY ;
      columns = (unsigned) cxClient / dp_char_width ;
      rows    = (unsigned) cyClient / dp_char_height ;
   }
   cycle_count++ ;
   HDC hdc = GetDC (hwnd) ;
   // COLORREF pcolor = random_colorref() ;
   if (use_solid_pattern) {
      pcolor = random_colorref() ;
   } else {
      pcolor = get_palette_entry(color) ;
      if (++color >= get_palette_entries())
         color = 0 ;
   }
   unsigned col = random_int(columns) ;
   unsigned row = random_int(rows) ;
   col *= dp_char_width ;
   row *= dp_char_height ;
   SolidRect(hdc, col, row, col+PIX_DX, row+PIX_DY, pcolor);
   ReleaseDC (hwnd, hdc) ;
}

//************************************************************************
//  return TRUE if we handled this key,
//  FALSE if we did not
//************************************************************************
bool pixels::process_key(unsigned key_id)
{
   return FALSE ;
}


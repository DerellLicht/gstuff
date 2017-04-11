#include <windows.h>

#include "gstuff.h"     //  u8, etc
#include "palettes.h"   //  24-bit palette functions
#include "gobjects.h"   //  graphics-object classes
#include "gfuncs.h"     //  graphics primitives
#include "demo.h"

//***********************************************************************
lines::lines(char *title_text) 
: graph_object(title_text) 
//  per http://www.acm.org/crossroads/xrds1-4/ovp.html
// , _v1(v1), _v2(v2), _v3(v3)
, orient(0)  //  0=horiz, 1=vert
{ 
}

//************************************************************************
void lines::update_display(HWND hwnd)
{
   int idx ;

   if (pause_the_race)
      return ;

   // SetWindowText(hwnd, title) ;
   cycle_count++ ;
   HDC hdc = GetDC (hwnd) ;
   // COLORREF attr = RGB(random_int(256), random_int(256), random_int(256)) ;
   orient ^= 1 ;
   if (orient) {
      //  horizontal, pick random y
      idx = random_int(cyClient) ;  /* Set the y coord of point    */
      LineCR(hdc, 0, idx, cxClient, idx, random_colorref());
   } else {
      idx = random_int(cxClient) ;  /* Set the y coord of point    */
      LineCR(hdc, idx, 0, idx, cyClient, random_colorref());
   }

   ReleaseDC (hwnd, hdc) ;
}

//************************************************************************
//  return TRUE if we handled this key,
//  FALSE if we did not
//************************************************************************
bool lines::process_key(unsigned key_id)
{
   return FALSE ;
}


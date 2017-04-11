//*******************************************************************
/*                         */
/* POLYDEMO: Display a random pattern of polygons on the screen   */
/* until the user says enough.               */
/*                         */
//*******************************************************************

#include <windows.h>

#include "gstuff.h"     //  u8, etc
#include "palettes.h"   //  24-bit palette functions
#include "gobjects.h"   //  graphics-object classes
#include "gfuncs.h"     //  graphics primitives
#include "demo.h"

#define MaxPts    6             /* Maximum # of pts in polygon  */

//***********************************************************************
polygon::polygon(char *title_text) 
: graph_object(title_text) 
//  per http://www.acm.org/crossroads/xrds1-4/ovp.html
// , _v1(v1), _v2(v2), _v3(v3)
{ 
}

//************************************************************************
void polygon::update_display(HWND hwnd)
{
   HBRUSH hBrush ;
   HDC    hdc ;
   POINT poly[MaxPts];     /* Space to hold datapoints  */
   // int color;                   /* Current drawing color  */
   int i;

   if (pause_the_race)
      return ;

   cycle_count++ ;
   // color = 1 + rand() % 256 ;  /* Get a random color # (no blk) */
   // setfillstyle(random(10), color); /* Set a random line style  */
   // setcolor(color);          /* Set the desired color  */

   //  select the random points
   for (i = 0; i < (MaxPts - 1); i++) {                      
      /* Determine a random polygon   */
      poly[i].x = random_int(cxClient) ;  /* Set the x coord of point    */
      poly[i].y = random_int(cyClient) ;  /* Set the y coord of point    */
   }
   /* last point = first point  */
   poly[i].x = random_int(cxClient) ;  /* Set the x coord of point    */
   poly[i].y = random_int(cyClient) ;  /* Set the y coord of point    */

   //***********************************************************
   // hBrush = CreateSolidBrush (RGB (random_int(256), random_int(256),
   //                                                random_int(256))) ;
   hBrush = CreateHatchBrush (fill_patterns[random_int(6)], random_colorref()) ;
   hdc = GetDC (hwnd) ;
   SetBkColor(hdc, random_colorref()) ;
   SelectObject(hdc, hBrush) ;

   // fillpoly(MaxPts, (int far *) poly); /* Draw the actual polygon     */
   Polygon(hdc, poly, MaxPts) ;

   ReleaseDC (hwnd, hdc) ;

   DeleteObject (hBrush) ;
}

//************************************************************************
//  return TRUE if we handled this key,
//  FALSE if we did not
//************************************************************************
bool polygon::process_key(unsigned key_id)
{
   return FALSE ;
}


#include <windows.h>
#include <stdio.h>   //  sprintf for %*s 

#include "gstuff.h"     //  u8, etc
#include "palettes.h"   //  24-bit palette functions
#include "gobjects.h"   //  graphics-object classes
#include "gfuncs.h"     //  graphics primitives
#include "demo.h"

#include "rgb_data.h"
#include "cheetah.def"
#include "hometown.def"

//***********************************************************************
gpalettes::gpalettes(char *title_text) 
: graph_object(title_text) 
//  per http://www.acm.org/crossroads/xrds1-4/ovp.html
// , _v1(v1), _v2(v2), _v3(v3)
{ 
}

//************************************************************************
void gpalettes::update_display(HWND hwnd)
{
   HDC hdc ;
   int xl, yu, idx ;
   // int xr, yl ;
   int maxlen, maxidx, slen, j ;
   // typedef struct tagSIZE { // siz 
   //     LONG cx; 
   //     LONG cy; 
   // } SIZE; 
   SIZE strsz ;

   if (!we_should_redraw) 
      return ;

   // SetWindowText(hwnd, title) ;
   // Clear_Screen(hdc, 0) ;
   Clear_Window(hwnd, 0);

   hdc = GetDC (hwnd) ;
   //*********************************************************
   //  find the size of the longest string, with a blank
   //  char fore and aft.
   //*********************************************************
   maxidx = 0 ;
   maxlen = 0 ;
   for (j=0; j<256; j++) {
      slen = strlen(cheetah[j].name) ;
      if (maxlen < slen) {
         maxlen = slen ;
         maxidx = j ;
      }
   }
   for (j=0; j<256; j++) {
      slen = strlen(hometown[j].name) ;
      if (maxlen < slen) {
         maxlen = slen ;
         maxidx = 256+j ;
      }
   }

   if (maxidx < 256) {
      wsprintf(tempstr, " %s ", cheetah[maxidx].name) ;
   } else {
      wsprintf(tempstr, " %s ", hometown[maxidx-256].name) ;
   }

   if (GetTextExtentPoint32(hdc, tempstr, strlen(tempstr), &strsz) == 0) {
      wsprintf(tempstr, "GetTextExtentPoint32 failed: %lu", GetLastError()) ;
      MessageBox(hwnd, tempstr, "duh...", MB_ICONEXCLAMATION) ;
   }
   // wsprintf(tempstr, "string is ( %u, %u )", strsz.cx, strsz.cy) ;
   // MessageBox(hwnd, tempstr, "string size", MB_ICONEXCLAMATION) ;

   //*********************************************************
   //  to make things fit a little better onscreen,
   //  skip all the "grayNN" entries at end of list.
   //*********************************************************

   //  loop over all available rows
   // int amode = 0 ; //  0=drawing first array, 1=drawing second array
   idx = 0 ;
   yu = 0 ;
   while (1) {
      // yl = yu + strsz.cy - 1;
      //  fit as many as fit, on this row
      xl = 0 ;
      while (1) {
         // xr = xl + strsz.cx - 1 ;

         //  draw the string
         SetTextColor(hdc, 0) ;
         // if (amode == 0) {
         if (idx < 256) {
            SetBkColor  (hdc, cheetah[idx].value) ;
            if (!(get_curr_palette() & 1)) {
               sprintf(tempstr, " %-*s ", maxlen, cheetah[idx].name) ; //  for %*s 
            } else {
               wsprintf(tempstr, " (%3d, %3d, %3d) ", 
                  cheetah[idx].red, cheetah[idx].green, cheetah[idx].blue) ;
            }
         }  //  drawing first array
         else {
            SetBkColor  (hdc, hometown[idx-256].value) ;
            // if (get_curr_palette() == 0) {
            if (!(get_curr_palette() & 1)) {
               sprintf(tempstr, " %-*s ", maxlen, hometown[idx-256].name) ; //  for %*s 
            } else {
               wsprintf(tempstr, " (%3d, %3d, %3d) ", 
                  hometown[idx-256].red, hometown[idx-256].green, hometown[idx-256].blue) ;
            }
         }  //  drawing second array
         TextOut (hdc, xl, yu, tempstr, strlen(tempstr));

         idx++ ;
         // if (idx == 256)
         //    amode = 1 ;
         if (idx == 512)
            break;
         xl += strsz.cx + 3 ;
         if ((xl + strsz.cx) > cxClient)
            break;
      }  //  draw one row
      if (idx == 512)
         break;
      yu += strsz.cy + 3 ;
      if ((yu + strsz.cy) > cyClient)
         break;
   }

   ReleaseDC (hwnd, hdc) ;
}

//************************************************************************
//  return TRUE if we handled this key,
//  FALSE if we did not
//************************************************************************
bool gpalettes::process_key(unsigned key_id)
{
   return FALSE ;
}


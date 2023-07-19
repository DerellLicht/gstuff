#include <windows.h>
#include <math.h>

#include "gstuff.h"     //  u8, etc
#include "palettes.h"   //  24-bit palette functions
#include "gobjects.h"   //  graphics-object classes
#include "gfuncs.h"     //  graphics primitives
#include "alg_selector.h"       //  pause_the_race

#define  CHANGE_ANGLE   90
// #define  CHANGE_ANGLE   45

#define  MAX_LINE_ALGORITHM   3
//***********************************************************************
line_games::line_games(char *title_text) 
: graph_object(title_text) 
//  per http://www.acm.org/crossroads/xrds1-4/ovp.html
// , _v1(v1), _v2(v2), _v3(v3)
#ifndef _lint
, start({0})
, finish({0})
#endif
, state(0)
, delay(0)
, color(0)
, line_algorithm(3)
{ 
   //lint -esym(1401, line_games::start, line_games::finish)
}

//************************************************************************
//  previously toggled by TAB key
//************************************************************************
void line_games::update_line_algorithm(void)
{
   if (++line_algorithm > MAX_LINE_ALGORITHM)
      line_algorithm = 0 ;
}

/************************************************************************/
void line_games::init_vector(vector_p vector)
{
   vector->theta = random_int(CHANGE_ANGLE) ;
   static double const degs2rads = 3.1415926535 / 180 ;
   vector->tan_theta = tan((double) vector->theta * degs2rads) ;
   vector->prev_dx = 0 ;
   vector->dx = 0.0 ;
   vector->x_changed = 0 ;
   vector->prev_dy = 0 ;
   vector->dy = 0.0 ;
   vector->y_changed = 0 ;
   // sprintf(tempstr, "theta=%u, tan=%.3f\n", vector->theta, vector->tan_theta) ;
   // OutputDebugString(tempstr) ;
}

/************************************************************************/
void line_games::move_point(vector_p vector)
{
   //  for small angles, increment x and calculate y
   if (vector->theta <= 45) {
      vector->dx++ ;
      vector->x_changed = 1 ;
      vector->dy += vector->tan_theta ;
      // sprintf(tempstr, "dy=%.3f, prev=%u, tan=%.3f\n", 
      //    vector->dy, vector->prev_dy, vector->tan_theta) ;
      // OutputDebugString(tempstr) ;
      if (vector->prev_dy < (unsigned) vector->dy) {
          vector->prev_dy = (unsigned) vector->dy ;
          vector->y_changed = 1 ;
      }
   }
   //  for large angles, increment y and calculate x
   else {
      vector->dy++ ;
      vector->y_changed = 1 ;
      vector->dx += vector->tan_theta ;
      if (vector->prev_dx < (unsigned) vector->dx) {
          vector->prev_dx = (unsigned) vector->dx ;
          vector->x_changed = 1 ;
      }
   }

   if (vector->x_changed) {
      vector->x_changed = 0 ;
      //  if x direction is INCREASE
      if (vector->x_dir == 0) {
         vector->x++ ;
         //  if X hit edge, reverse X direction and recalculate angle
         if (vector->x >= (unsigned) cxClient) {
            vector->x_dir ^= 1 ;
            vector->x-- ;
            init_vector(vector) ;
         }
      } 
      //  if X direction is DECREASE
      else {
         if (vector->x == 0) {
            vector->x_dir ^= 1 ;
            init_vector(vector) ;
         } else {
            vector->x-- ;
         }
      }
   }
   
   if (vector->y_changed) {
      vector->y_changed = 0 ;
      //  if x direction is INCREASE
      if (vector->y_dir == 0) {
         vector->y++ ;
         //  if X hit edge, reverse X direction and recalculate angle
         if (vector->y >= (unsigned) cyClient) {
            vector->y_dir ^= 1 ;
            vector->y-- ;
            init_vector(vector) ;
         }
      } 
      //  if X direction is DECREASE
      else {
         if (vector->y == 0) {
            vector->y_dir ^= 1 ;
            init_vector(vector) ;
         } else {
            vector->y-- ;
         }
      }
   }
}

//************************************************************************
void line_games::update_display(HWND hwnd)
{
   if (pause_the_race)
      return ;

   if (use_solid_pattern) {
      update_line_algorithm() ;
   }
   
   if (we_should_redraw) {
      // SetWindowText(hwnd, title) ;
      state = 0 ;
      delay = 0 ;

      if (line_algorithm == 3) {
         start.x  = random_int(cxClient) ;
         start.x_dir = start.x & 1 ;
         start.y  = random_int(cyClient) ;
         start.y_dir = start.y & 1 ;
         init_vector(&start) ;

         finish.x = random_int(cxClient) ;
         finish.x_dir = finish.x & 1 ;
         finish.y = random_int(cyClient) ;
         finish.y_dir = finish.y & 1 ;
         init_vector(&finish) ;
      } else {
         start.x = 0 ;
         start.y = 0 ;
         finish.x = cxClient ;
         finish.y = 0 ;
      }
   }
   if (++delay < 5000)
      return ;
   delay = 0 ;
   COLORREF attr = get_palette_entry(color) ;
   if (++color >= get_palette_entries())
      color = 0 ;
   HDC hdc = GetDC (hwnd) ;

   cycle_count++ ;
   LineCR(hdc, start.x, start.y, finish.x, finish.y, attr);
   switch (line_algorithm) {
   //*****************  METHOD 0  **************************
   case 0:
      switch (state) {
      case 0:  //  start = 0,0, finish = max, 0=>max
         if (++finish.y >= (unsigned) cyClient) {
            state = 1 ;
         }
         break;
      case 1:  //  start = 0=>max, 0, finish = max, max
         if (++start.x >= (unsigned) cxClient) {
            state = 2 ;
         }
         break;
      case 2:  //  start = max,0, finish = max=>0,max
         if (finish.x == 0) {
            state = 3 ;
         } else {
            finish.x-- ;
         }
         break;
      case 3:  //  start = max,0=>max, finish=0,max
         if (++start.y >= (unsigned) cyClient) {
            state = 4 ;
         }
         break;
      case 4:  //  start = max,max, finish=0,max=>0
         if (finish.y == 0) {
            state = 5 ;
         } else {
            finish.y-- ;
         }
         break;
      case 5:  //  start = max=>0,max, finish=0,0
         if (start.x == 0) {
            state = 6 ;
         } else {
            start.x-- ;
         }
         break;
      case 6:  //  start = 0,max, finish=0=>max,0
         if (++finish.x >= (unsigned) cxClient) {
            state = 7 ;
         }
         break;
      case 7:  //  start = 0,max=>, finish=max,0
         if (start.y == 0) {
            state = 0 ;
         } else {
            start.y-- ;
         }
         break;
      }  //lint !e744
      break;

   //*****************  METHOD 1  **************************
   case 1:
      switch (state) {
      case 0:
         if (++finish.y >= (unsigned) cyClient) {
            state = 1 ;
         }
         break;
      case 1:
         if (finish.x == 0) {
            state = 2 ;
         } else {
            finish.x-- ;
         }
         break;
      case 2:
         if (++start.x >= (unsigned) cxClient) {
            state = 3 ;
         }
         break;
      case 3:
         if (++start.y >= (unsigned) cyClient) {
            state = 4 ;
         }
         break;
      case 4:
         if (finish.y == 0) {
            state = 5 ;
         } else {
            finish.y-- ;
         }
         break;
      case 5:
         if (++finish.x >= (unsigned) cxClient) {
            state = 6 ;
         }
         break;
      case 6:
         if (start.x == 0) {
            state = 7 ;
         } else {
            start.x-- ;
         }
         break;
      case 7:
         if (start.y == 0) {
            state = 0 ;
         } else {
            start.y-- ;
         }
         break;
      }  //lint !e744
      break;

   //*****************  METHOD 2  **************************
   case 2:
      switch (state) {
      case 0:
         if (++finish.y >= (unsigned) cyClient) {
            state = 1 ;
         }
         break;
      case 1:
         if (++start.x >= (unsigned) cxClient) {
            state = 2 ;
            finish.x = 0 ;
         } else {
            finish.x-- ;
         }
         break;
      case 2:
         if (++start.y >= (unsigned) cyClient) {
            state = 3 ;
            finish.y = 0 ;
         } else {
            finish.y-- ;
         }
         break;
      case 3:
         if (start.x == 0) {
            state = 4 ;
         } else {
            start.x-- ;
            finish.x++ ;
         }
         break;
      case 4:
         if (start.y == 0) {
            state = 1 ;
         } else {
            start.y-- ;
            finish.y++ ;
         }
         break;
      }  //lint !e744
      break;

   //*****************  METHOD 3  ************************************
   //  This method will involve wandering of the endpoints,
   //  so it won't have the simple states of the other methods.
   //*****************************************************************
   case 3:
      move_point(&start) ;
      move_point(&finish) ;
      break;
   }  //lint !e744

   ReleaseDC (hwnd, hdc) ;
}

//************************************************************************
//  return TRUE if we handled this key,
//  FALSE if we did not
//************************************************************************
bool line_games::process_key(unsigned key_id)
{
   return FALSE ;
}


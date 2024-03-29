//**************************************************************************
//  Copyright (c) 2014 by Daniel D Miller
//  This algorithm is freeware
//  
//  This routine is ported directly from:
// Acid Warp V4.2 By Noah Spurrier
// This is Acid Warp for DOS. It's an eye-candy program. 
// 
//  Notes from Dan Miller:
//  Unfortunately, it is not easy to do palette rotation in a Windows/GDI
//  program, so all this does is render the original images.
//**************************************************************************

#include <windows.h>
#include <stdlib.h>

#include "gstuff.h"				  //  u8, etc
#include "palettes.h"			  //  24-bit palette functions
#include "gobjects.h"			  //  graphics-object classes
#include "gfuncs.h"				  //  graphics primitives
#include "alg_selector.h"
#include "lut.h"

static const int MAX_IMAGE_FUNC = 40 ;

//***********************************************************************
acidwarp::acidwarp (char *title_text)
:graph_object (title_text)
//  per http://www.acm.org/crossroads/xrds1-4/ovp.html
// , _v1(v1), _v2(v2), _v3(v3)
{
}

//***********************************************************************
//  private member function for class
//***********************************************************************
int acidwarp::generate_image (int imageFuncNum, HDC hdc,
	int xcenter, int ycenter, int xmax, int ymax, int colormax)
{
   //   UCHAR *buf_graf_unused  originall passed as argument

	long /* int */ x, y, dx, dy, dist, angle;
   int color;

	/* Some general purpose random angles and offsets. Not all functions use them. */
   // long a1, a2, a3, a4, 
   long x1, x2, x3, x4, y1, y2, y3, y4;

   x1 = random_int (40) - 20;
   x2 = random_int (40) - 20;
   x3 = random_int (40) - 20;
   x4 = random_int (40) - 20;
   y1 = random_int (40) - 20;
   y2 = random_int (40) - 20;
   y3 = random_int (40) - 20;
   y4 = random_int (40) - 20;

   // a1 = random_int (ANGLE_UNIT);
   // a2 = random_int (ANGLE_UNIT);
   // a3 = random_int (ANGLE_UNIT);
   // a4 = random_int (ANGLE_UNIT);

	for (y = 0; y < ymax; ++y) {

		for (x = 0; x < xmax; ++x) {
			dx = x - xcenter;
			dy = y - ycenter;

			dist = lut_dist (dx, dy);
			angle = lut_angle (dx, dy);

			/* select function. Could be made a separate function, but since this function is
			   evaluated over a large iteration of values I am afraid that it might slow
			   things down even more to have a separate function.                         */

			switch (imageFuncNum) {
            // case -1:    Eight Arm Star -- produces weird discontinuity
            //    color = dist+ lut_sin(angle * (200 - dist)) / 32;
            //    break;

         case 0:             /* Rays plus 2D Waves */
            color = angle + lut_sin (dist * 10) / 64 +
                            lut_cos (x * ANGLE_UNIT / xmax * 2) / 32 +
                            lut_cos (y * ANGLE_UNIT / ymax * 2) / 32;
            break;

         case 1:             /* Rays plus 2D Waves */
            color = angle + lut_sin (dist * 10) / 16 +
               lut_cos (x * ANGLE_UNIT / xmax * 2) / 8 +
               lut_cos (y * ANGLE_UNIT / ymax * 2) / 8;
            break;

         case 2:
            color = lut_sin (lut_dist (dx + x1, dy + y1) * 4) / 32 +
               lut_sin (lut_dist (dx + x2, dy + y2) * 8) / 32 +
               lut_sin (lut_dist (dx + x3, dy + y3) * 16) / 32 +
               lut_sin (lut_dist (dx + x4, dy + y4) * 32) / 32;
            break;

         case 3:             /* Peacock */
            color = angle + lut_sin (lut_dist (dx + 20, dy) * 10) / 32 +
               angle + lut_sin (lut_dist (dx - 20, dy) * 10) / 32;
            break;

         case 4:
            color = lut_sin (dist) / 16;
            break;

         case 5:             /* 2D Wave + Spiral */
            color = lut_cos (x * ANGLE_UNIT / xmax) / 8 +
               lut_cos (y * ANGLE_UNIT / ymax) / 8 +
               angle + lut_sin (dist) / 32;
            break;

         case 6:             /* Peacock, three centers */
            color = lut_sin (lut_dist (dx, dy - 20) * 4) / 32 +
               lut_sin (lut_dist (dx + 20, dy + 20) * 4) / 32 +
               lut_sin (lut_dist (dx - 20, dy + 20) * 4) / 32;
            break;

         case 7:             /* Peacock, three centers */
            color = angle +
               lut_sin (lut_dist (dx, dy - 20) * 8) / 32 +
               lut_sin (lut_dist (dx + 20, dy + 20) * 8) / 32 +
               lut_sin (lut_dist (dx - 20, dy + 20) * 8) / 32;
            break;

         case 8:             /* Peacock, three centers */
            color = lut_sin (lut_dist (dx, dy - 20) * 12) / 32 +
               lut_sin (lut_dist (dx + 20, dy + 20) * 12) / 32 +
               lut_sin (lut_dist (dx - 20, dy + 20) * 12) / 32;
            break;

         case 9:             /* Five Arm Star */
            color = dist + lut_sin (5 * angle) / 64;
            break;

         case 10:         /* 2D Wave */
            color = lut_cos (x * ANGLE_UNIT / xmax * 2) / 4 +
               lut_cos (y * ANGLE_UNIT / ymax * 2) / 4;
            break;

         case 11:         /* 2D Wave */
            color = lut_cos (x * ANGLE_UNIT / xmax) / 8 +
               lut_cos (y * ANGLE_UNIT / ymax) / 8;
            break;

         case 12:         /* Simple Concentric Rings */
            color = dist;
            break;

         case 13:         /* Simple Rays */
            color = angle;
            break;

         case 14:         /* Toothed Spiral Sharp */
            color = angle + lut_sin (dist * 8) / 32;
            break;

         case 15:         /* Rings with sine */
            color = lut_sin (dist * 4) / 32;
            break;

         case 16:         /* Rings with sine with sliding inner Rings */
            color = dist + lut_sin (dist * 4) / 32;
            break;

         case 17:
            color =
               lut_sin (lut_cos (2 * x * ANGLE_UNIT / xmax)) / (20 +
               dist) +
               lut_sin (lut_cos (2 * y * ANGLE_UNIT / ymax)) / (20 + dist);
            break;

         case 18:         /* 2D Wave */
            color = lut_cos (7 * x * ANGLE_UNIT / xmax) / (20 + dist) +
               lut_cos (7 * y * ANGLE_UNIT / ymax) / (20 + dist);
            break;

         case 19:         /* 2D Wave */
            color = lut_cos (17 * x * ANGLE_UNIT / xmax) / (20 + dist) +
               lut_cos (17 * y * ANGLE_UNIT / ymax) / (20 + dist);
            break;

         case 20:         /* 2D Wave Interference */
            color = lut_cos (17 * x * ANGLE_UNIT / xmax) / 32 +
               lut_cos (17 * y * ANGLE_UNIT / ymax) / 32 + dist + angle;
            break;

         case 21:         /* 2D Wave Interference */
            color = lut_cos (7 * x * ANGLE_UNIT / xmax) / 32 +
               lut_cos (7 * y * ANGLE_UNIT / ymax) / 32 + dist;
            break;

         case 22:         /* 2D Wave Interference */
            color = lut_cos (7 * x * ANGLE_UNIT / xmax) / 32 +
               lut_cos (7 * y * ANGLE_UNIT / ymax) / 32 +
               lut_cos (11 * x * ANGLE_UNIT / xmax) / 32 +
               lut_cos (11 * y * ANGLE_UNIT / ymax) / 32;
            break;

         case 23:
            color = lut_sin (angle * 7) / 32;
            break;

         case 24:
            color = lut_sin (lut_dist (dx + x1, dy + y1) * 2) / 12 +
               lut_sin (lut_dist (dx + x2, dy + y2) * 4) / 12 +
               lut_sin (lut_dist (dx + x3, dy + y3) * 6) / 12 +
               lut_sin (lut_dist (dx + x4, dy + y4) * 8) / 12;
            break;

         case 25:
            color =
               angle + lut_sin (lut_dist (dx + x1,
                  dy + y1) * 2) / 16 + angle + lut_sin (lut_dist (dx + x2,
                  dy + y2) * 4) / 16 + lut_sin (lut_dist (dx + x3,
                  dy + y3) * 6) / 8 + lut_sin (lut_dist (dx + x4,
                  dy + y4) * 8) / 8;
            break;

         case 26:
            color =
               angle + lut_sin (lut_dist (dx + x1, dy + y1) * 2) / 12 + 
               angle + lut_sin (lut_dist (dx + x2, dy + y2) * 4) / 12 + 
               angle + lut_sin (lut_dist (dx + x3, dy + y3) * 6) / 12 + 
               angle + lut_sin (lut_dist (dx + x4, dy + y4) * 8) / 12;
            break;

         case 27:
            color = lut_sin (lut_dist (dx + x1, dy + y1) * 2) / 32 +
                    lut_sin (lut_dist (dx + x2, dy + y2) * 4) / 32 +
                    lut_sin (lut_dist (dx + x3, dy + y3) * 6) / 32 +
                    lut_sin (lut_dist (dx + x4, dy + y4) * 8) / 32;
            break;

         case 28:         /* Random Curtain of Rain (in strong wind) */
            if (y == 0 || x == 0) {
               color = random_int (16);
            }
            else {
               // color = (*(buf_graf + (xmax * y) + (x - 1))
               //       +  *(buf_graf + (xmax * (y - 1)) + x)) / 2
               //       + random_int (16) - 8;
               COLORREF p1 = GetPixel(hdc, (x - 1), y) ;
               COLORREF p2 = GetPixel(hdc, x, (y - 1)) ;
               color = ((p1 + p2) / 2) + random_int (16) - 8;   //lint !e737
            }
            break;

         case 29:
            if (y == 0 || x == 0) {
               color = random_int (1024);
            }
            else {
               // color = dist / 6 + (*(buf_graf + (xmax * y) + (x - 1))
               //       + *(buf_graf + (xmax * (y - 1)) + x)) / 2
               //       + random_int (16) - 8;
               COLORREF p1 = GetPixel(hdc, (x - 1), y) ;
               COLORREF p2 = GetPixel(hdc, x, (y - 1)) ;
               color = dist / 6 + ((p1 + p2) / 2)
                     //  (*(buf_graf + (xmax * y) + (x - 1))
                     // + *(buf_graf + (xmax * (y - 1)) + x)) / 2
                     + random_int (16) - 8;   //lint !e737
            }
            break;

         case 30:
            color = lut_sin (lut_dist (dx, dy - 20) * 4) / 32 ^
                    lut_sin (lut_dist (dx + 20, dy + 20) * 4) / 32 ^
                    lut_sin (lut_dist (dx - 20, dy + 20) * 4) / 32;
            break;

         case 31:
            color = (angle % (ANGLE_UNIT / 4)) ^ dist;
            break;

         case 32:
            color = dy ^ dx;
            break;

         case 33:         /* Variation on Rain */
            if (y == 0 || x == 0) {
               color = random_int (16);
            }
            else {
               // color = (*(buf_graf + (xmax * y) + (x - 1))
               //       +  *(buf_graf + (xmax * (y - 1)) + x)) / 2;
               COLORREF p1 = GetPixel(hdc, (x - 1), y) ;
               COLORREF p2 = GetPixel(hdc, x, (y - 1)) ;
               color = ((p1 + p2) / 2) ;
            }

            color += random_int (2) - 1;

            if (color < 64)
               color += random_int (16) - 8;
            else
               color = color;
            break;

         case 34:         /* Variation on Rain */
            if (y == 0 || x == 0) {
               color = random_int (16);
            }
            else {
               // color = (*(buf_graf + (xmax * y) + (x - 1))
               //       +  *(buf_graf + (xmax * (y - 1)) + x)) / 2;
               COLORREF p1 = GetPixel(hdc, (x - 1), y) ;
               COLORREF p2 = GetPixel(hdc, x, (y - 1)) ;
               color = ((p1 + p2) / 2) ;
            }

            if (color < 100)
               color += random_int (16) - 8;
            break;

         case 35:
            color = angle + lut_sin (dist * 8) / 32;
            dx = x - xcenter;
            dy = (y - ycenter) * 2;
            dist = lut_dist (dx, dy);
            angle = lut_angle (dx, dy);
            color = (color + angle + lut_sin (dist * 8) / 32) / 2;
            break;

         case 36:
            color = angle + lut_sin (dist * 10) / 16 +
               lut_cos (x * ANGLE_UNIT / xmax * 2) / 8 +
               lut_cos (y * ANGLE_UNIT / ymax * 2) / 8;
            dx = x - xcenter;
            dy = (y - ycenter) * 2;
            dist = lut_dist (dx, dy);
            angle = lut_angle (dx, dy);
            color = (color + angle + lut_sin (dist * 8) / 32) / 2;
            break;

         case 37:
            color = angle + lut_sin (dist * 10) / 16 +
               lut_cos (x * ANGLE_UNIT / xmax * 2) / 8 +
               lut_cos (y * ANGLE_UNIT / ymax * 2) / 8;
            dx = x - xcenter;
            dy = (y - ycenter) * 2;
            dist = lut_dist (dx, dy);
            angle = lut_angle (dx, dy);
            color = (color + angle + lut_sin (dist * 10) / 16 +
               lut_cos (x * ANGLE_UNIT / xmax * 2) / 8 +
               lut_cos (y * ANGLE_UNIT / ymax * 2) / 8) / 2;
            break;

         case 38:
            if (dy % 2) {
               dy *= 2;
               dist = lut_dist (dx, dy);
               angle = lut_angle (dx, dy);
            }
            color = angle + lut_sin (dist * 8) / 32;
            break;

         case 39:
            color = (angle % (ANGLE_UNIT / 4)) ^ dist;
            dx = x - xcenter;
            dy = (y - ycenter) * 2;
            dist = lut_dist (dx, dy);
            angle = lut_angle (dx, dy);
            color = (color + ((angle % (ANGLE_UNIT / 4)) ^ dist)) / 2;
            break;

         case 40:
            color = dy ^ dx;
            dx = x - xcenter;
            dy = (y - ycenter) * 2;
            color = (color + (dy ^ dx)) / 2;
            break;

         default:
            color = random_int (colormax - 1) + 1;
            break;
			}

         /* Fit color value into the palette range using modulo.  
          * It seems that the Turbo-C MOD function does not behave the way I expect. 
          * It gives negative values for the MOD of a negative number. 
          * I expect MOD to function as it does on my HP-28S.
          * 
          * 01/08/14 DDM - and ALL these assumptions change on a 
			 */

         color = color % (colormax - 1);
         if (color < 0)
           color += (colormax - 1);
         ++color;                 /* color 0 is never used, so all colors are from 1 through 255 */

         //  01/07/14  for this implementation, this buffer-write operation
         //  will need to turn into a pixel-write operation...
         // *(buf_graf + (xmax * y) + x) = (UCHAR) color;  /* Store the color in the buffer */
         SetPixel(hdc, x, y, get_palette_entry(color)) ;
		}
		/* end for (y = 0; y < ymax; ++y)   */
	}
	/* end for (x = 0; x < xmax; ++x)   */

// #if 0   /* For diagnosis, put palette display line at top of new image */
//   for (x = 0; x < xmax; ++x) {
//      color = (x <= 255) ? x : 0;
// 
//      for (y = 0; y < 3; ++y)
//         *(buf_graf + (xmax * y) + x) = (UCHAR) color;
//   }
// #endif

	return (0);
}


//************************************************************************
void acidwarp::update_display (HWND hwnd)
{
	HDC hdc;
   int colormax = 256 ;
   static int ImageFuncNum = 0 ;

	if (!we_should_redraw)
		return;

	set_DAC_table (0);
	hdc = GetDC (hwnd);
	Clear_Window (hdc, 0);

   generate_image (ImageFuncNum, hdc, maxx/2, maxy/2, maxx, maxy, colormax) ;

   if (++ImageFuncNum > MAX_IMAGE_FUNC)
      ImageFuncNum = 0 ;

   //*****************************************************
	//  do cleanup and exit
	//*****************************************************
	ReleaseDC (hwnd, hdc);
}

//************************************************************************
//  return TRUE if we handled this key,
//  FALSE if we did not
//************************************************************************
bool acidwarp::process_key (unsigned key_id)
{
	return FALSE;
}

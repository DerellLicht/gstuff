//************************************************************************
//  flame_pal.cpp
//  Unused function that accessed a *huge* palette file 
//  that wasn't really worth the trouble of the huge file size.
//************************************************************************


#ifndef SKIP_FLAME_PALETTES
/************************************************************************/
#define  TOTAL_F_PALETTE_ENTRIES   701
#define  LAST_F_PALETTE_ENTRY    (TOTAL_F_PALETTE_ENTRIES - 1)
extern palette_record_t f_palette_records[TOTAL_F_PALETTE_ENTRIES] ;
static int flame_page_change = 0 ;
#define  FLAME_BAR_COUNT   20
   
static void draw_flame_color_bars(HWND hwnd)
{
   HDC hdc ;
   HBRUSH hBrush ;
   RECT   rect ;
   // int red, green, blue ;
   static unsigned top_index = 0 ;
   static unsigned bottom_index = FLAME_BAR_COUNT - 1 ;

   if (!we_should_redraw) 
      return ;
   if (flame_page_change < 0) {  //  PgUp
      flame_page_change = 0 ; //  reset the change flag
      if (top_index == 0)
         return ;
      top_index = (top_index < FLAME_BAR_COUNT) ? 0 : (top_index - FLAME_BAR_COUNT) ;
      bottom_index = top_index + FLAME_BAR_COUNT - 1 ;
   } else if (flame_page_change > 0) { //  PgDn
      flame_page_change = 0 ; //  reset the change flag
      if (bottom_index == LAST_F_PALETTE_ENTRY) 
         return ;
      bottom_index += FLAME_BAR_COUNT ;
      if (bottom_index > LAST_F_PALETTE_ENTRY) 
          bottom_index = LAST_F_PALETTE_ENTRY ;
      top_index = bottom_index - FLAME_BAR_COUNT + 1 ;
   }

   //  xn = number of squares in x direction
   //  xg = size of gap between squares in X
   //  xo = size of gapes outside squares
   //  xs = size of square in X direction (calculated)
   //  xt = intermediate value: space not consumed by gaps,
   //                           i.e., space used by squares.
   //  xx = number of pixels left over
   //  xthold = ratio of extra pixels to number of squares
   //         = number of squares which will get an extra pixel
   //  
   //  if it works for x, it works for y
   int yn, yg, yo, ys, yt, yx ;
   int xn, xg, xo, xs, xt, xx ;
   int rowIdx, colIdx, idx ;
   int xl, yu, xr, yl ;
   double xthold, xrun, ythold, yrun ;
   int xruni, xprev, yruni, yprev ;
   unsigned max_colors = 256;

   //  calc vertical layout
   yn = FLAME_BAR_COUNT ;   //  number of vertical squares
   yo = 10 ;
   yg =  5 ;
   yt = (cyClient - ((yn-1)*yg) - (2 * yo)) ;
   ys = yt / yn ;
   yx = yt % yn ;
   ythold = (double) yx / (double) yn ;

   //*****************************************************
   //  now, draw some squares!!
   //*****************************************************
   idx = 0 ;
   yu = yo ;
   yl = yu + ys ;

   //  initialize the fill vars
   yrun = 0.0 ;
   yprev = 0 ;
   yruni = 0 ;

   wsprintf(tempstr, "Flame bars %u = %s", top_index, get_fpalette_name(top_index));
   SetWindowText(hwnd, tempstr) ;
   hdc = GetDC (hwnd) ;
   Clear_Window(hdc, 0);

   //*********************************************************
   //                  do my computations                     
   //                                                         
   //  First model is a 16x16 matrix of squares, with gaps    
   //  between squares, 256 shades of one color.              
   //  Modifying CreateSolidBrush allowed observing           
   //  transitions from one primary color to another.         
   //*********************************************************

   for (rowIdx=0; rowIdx<yn; rowIdx++) {
      set_fDAC_table(top_index+rowIdx) ;
      max_colors = get_fpalette_entries(top_index+rowIdx) ;
      // wsprintf(tempstr, "row %u: max_colors=%u\n", rowIdx, max_colors) ;
      // OutputDebugString(tempstr) ;

      //  calc horizontal layout
      xn = max_colors ;   //  number of horiz squares
      xo = 10 ;   //  outside (boundary) gaps
      xg =  0 ;   //  inside gaps
      xt = (cxClient - ((xn-1)*xg) - (2 * xo)) ;
      xs = xt / xn ; //  object space
      xx = xt % xn ; //  leftover space
      // xx = cxClient / xx ; //  extra pixel every xx
      xthold = (double) xx / (double) xn ;

      xrun = 0.0 ;
      xprev = 0 ;
      xruni = 0 ;
      xl = xo ;
      xr = xl + xs ;
      for (colIdx=0; colIdx<xn; colIdx++) {
         hBrush = CreateSolidBrush (get_fpalette_entry(colIdx)) ;
         SetRect (&rect, xl, yu, xr, yl) ;
         FillRect (hdc, &rect, hBrush) ;
         DeleteObject (hBrush) ;

         xl = xr ;
         xr = xl + xs ;
         // if (colIdx != 0  &&  (colIdx % xx) == 0) 
         //    xr++ ;

         //  update the fill vars
         xrun += xthold ;
         xruni = (int) xrun ;
         if (xruni > xprev)
            xr++ ;
         xprev = xruni ;

         // idx++ ;
      }  //  loop thru all columns on one bar
      yu = yl + yg ;
      yl = yu + ys ;

      //  update the fill vars
      yrun += ythold ;
      yruni = (int) yrun ;
      if (yruni > yprev)
         yl++ ;
      yprev = yruni ;
   }  //  loop over all horizontal bars

   //*****************************************************
   //  do cleanup and exit
   //*****************************************************
   // set_DAC_table(0) ;
   ReleaseDC (hwnd, hdc) ;
}

//***********************************************************************
void update_flame_page(unsigned dir)
{
   flame_page_change = (dir == 0) ? -1 : 1 ;
}
#endif



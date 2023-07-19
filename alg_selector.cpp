//***************************************************************************************
//  alg_selector.cpp 
//  Present onscreen menu of program options, and display the selected option.
//  Possibly, a better name for this file/header might have been menu.cpp/menu.h
//***************************************************************************************
//  Original file info:
//  File: DEMO.C
//  This is a main program which is used to illustrate use of drawing routines 
//  from the book "Advanced Programmer's Guide to SuperVGA".
//***************************************************************************************

#include <windows.h>
#include <stdio.h>   //  sprintf, for %*s syntax
#include <math.h>

#include "gstuff.h"     //  u8, etc
#include "palettes.h"   //  24-bit palette functions
#include "gobjects.h"   //  graphics-object classes
#include "gfuncs.h"     //  graphics primitives
#include "keywin32.h"
#include "alg_selector.h"       //  demo declarations
#include "ezfont.h"

int demo_state = 0 ;
int we_should_redraw = 1 ;
int pause_the_race = 0 ;
unsigned use_solid_pattern = 0 ;

unsigned cycle_count = 0 ;
static u64 ti = 0 ;

//***********************************************************************
static void ShowMenu(HWND hwnd);

//***********************************************************************
//  instantiate each of the classes
//***********************************************************************
static circles circles0("Psychedelic Raindrops") ;
static squares squares0("Boxing Lessons") ;
static polygon polygon0("Temporal Lightning") ;
static rect rect0("Palette Boxes") ;
static pixels pixels0("Pixel-packing") ;
static colorbars colorbars0("Color Bars") ;
static xpalette xpalette0("XWindows palette") ;

static sglass sglass0("Stained Glass") ;
static flames flames0("Fire tricks") ;
static face_trap faces0("Face traps") ;
static rainbow rainbow0("Rainbow !!") ;
ascii ascii0("ASCII Table") ;
static rcolors rcolors0("rcolors32") ;
static line_games lgames0("Line Games") ;
static lines lines0("Lines") ;
static gpalettes gpalettes0("more palettes") ;
static xnpalette xnpalette0("Named XWindows palette") ;
static xrect xrect0("XWindows Palette Boxes") ;
static bitblt bitblt0("BitBlt demo and options") ;
static triangles triangles0("line triangle") ;
static wincolors wincolors0("Windows Colors") ;
static acidwarp do_acidwarp("AcidWarp simulation") ;

/************************************************************************/
typedef struct menu_items_s {
   unsigned menu_attr ;
   graph_object *go ;
   RECT     region ;
   char     *menu_text ;
   unsigned selection_key ;
   char     *title ;
   void (*draw_func)(HWND hwnd);
} menu_items_t, *menu_items_p ;

/************************************************************************/
static menu_items_t menu_items[] = {
{ WIN_BLUE,  0,           { 0, 0, 0, 0 }, "  ",                           0,  "Graphics demos",         ShowMenu },
{ WIN_BLUE,  &circles0,   { 0, 0, 0, 0 }, "a: Raindrops ",                ka, "Psychedelic Raindrops",  0 },
{ WIN_BLUE,  &squares0,   { 0, 0, 0, 0 }, "b: Boxes ",                    kb, "Boxing Lessons",         0 },
{ WIN_BLUE,  &polygon0,   { 0, 0, 0, 0 }, "c: Lightning ",                kc, "Temporal Lightning",     0 },
{ WIN_BLUE,  &rect0,      { 0, 0, 0, 0 }, "d: 3D Boxes ",                 kd, "Palette Boxes",          0 },
// { WIN_BLUE,  "e: Line Star ",                ke, "Color Fans",             Demo_Line },
{ WIN_BLUE,  &pixels0,    { 0, 0, 0, 0 }, "e: Pixels ",                   ke, "Pixel-packing",          0 },
{ WIN_BLUE,  &colorbars0, { 0, 0, 0, 0 }, "f: Color Bars ",               kf, "Color Bars",             0 },
{ WIN_BLUE,  &xpalette0,  { 0, 0, 0, 0 }, "g: Observe XWindows Palette ", kg, "XWindows Palette",       0 },
{ WIN_BLUE,  &bitblt0,    { 0, 0, 0, 0 }, "h: bitblt demo ",              kh, "BitBlt demo",            0 },
{ WIN_BLUE,  &xnpalette0, { 0, 0, 0, 0 }, "i: Named XWindows Palette ",   ki, "Named XWindows Palette", 0 },
{ WIN_BLUE,  &xrect0,     { 0, 0, 0, 0 }, "j: 3D XWindows Boxes ",        kj, "XWindows Palette Boxes", 0 },
{ WIN_BLUE,  &gpalettes0, { 0, 0, 0, 0 }, "k: more palettes ",            kk, "more palettes",          0 },
{ WIN_BLUE,  &triangles0, { 0, 0, 0, 0 }, "l: line stuff ",               kl, "line triangle",          0 },
{ WIN_BLUE,  &rainbow0,   { 0, 0, 0, 0 }, "m: Rainbow !!",                km, "Rainbow !!",             0 },
{ WIN_BLUE,  &lines0,     { 0, 0, 0, 0 }, "n: Lines",                     kn, "Lines",                  0 },
{ WIN_BLUE,  &lgames0,    { 0, 0, 0, 0 }, "o: Line Games",                ko, "Lines Games",            0 },
{ WIN_BLUE,  &rcolors0,   { 0, 0, 0, 0 }, "p: rcolors32",                 kp, "rcolors32",              0 },
{ WIN_BLUE,  &flames0,    { 0, 0, 0, 0 }, "q: Fire tricks",               kq, "Fire tricks",            0 },
{ WIN_BLUE,  &faces0,     { 0, 0, 0, 0 }, "r: Face Traps",                kr, "Face traps",             0 },
{ WIN_BLUE,  &ascii0,     { 0, 0, 0, 0 }, "s: ASCII table",               ks, "ASCII table",            0 },
{ WIN_BLUE,  &sglass0,    { 0, 0, 0, 0 }, "t: Stained Glass",             kt, "Stained Glass",          0 },
{ WIN_BLUE,  &wincolors0, { 0, 0, 0, 0 }, "u: Windows Colors",            ku, "Windows Colors",         0 },
{ WIN_BLUE,  &do_acidwarp,{ 0, 0, 0, 0 }, "v: AcidWarp",                  kv, "AcidWarp",               0 },
#ifndef SKIP_FLAME_PALETTES
{ WIN_BLUE,  0, { 0, 0, 0, 0 }, "r: Flame Bars ", kr, "Flame Bars", draw_flame_color_bars },
#endif
{ WIN_BLUE,  0, { 0, 0, 0, 0 }, " ",                            0, 0, 0 },
{ WIN_BROWN, 0, { 0, 0, 0, 0 }, "TAB toggles palette, SPACE clears the storm. ",        0, 0, 0 },
{ WIN_BROWN, 0, { 0, 0, 0, 0 }, "Control-C or Escape ends it all... ",    0, 0, 0 },
{ 0, 0, { 0, 0, 0, 0 }, 0, 0, 0, 0 }} ;

/************************************************************************/
int check_for_state_change(HWND hwnd, unsigned inchr)
{
   unsigned j ;
   for (j=0; menu_items[j].menu_text != 0; j++) {
      menu_items_p miptr = &menu_items[j] ;
      if (miptr->selection_key == inchr) {
         //  we only want to update the title bar when we change state.
         //  If we do the title draw in the graphics object,
         //  then objects which update display on every cycle,
         //  will repeatedly draw the title bar.
         SetWindowText(hwnd, miptr->title) ;
         cycle_count = 0 ;
         ti = proc_time ();
         demo_state = (inchr - ka) + 1 ;
      }
   }
   return -1;
}

/************************************************************************/
static unsigned find_index_of_longest_menu_string(void)
{
   unsigned j ;
   unsigned max_menu_str_len = 0 ;
   unsigned max_menu_str_index = 0 ;
   for (j=0; menu_items[j].menu_text != 0; j++) {
      menu_items_p miptr = &menu_items[j] ;
      unsigned slen = strlen(miptr->menu_text) ;
      if (slen > max_menu_str_len) {
         max_menu_str_len = slen ;
         max_menu_str_index = j ;
      }
   }
   return max_menu_str_index ;
}

/************************************************************************/
#define  MENU_BASE_LEFT    100

static unsigned menu_size_computed = 0 ;
static SIZE menusz ;
// static unsigned menu_left = 0 ;
// static unsigned menu_right = 0 ;
static int hilited_menu_item = -1 ;

/************************************************************************/
static void hilite_menu_item(HDC hdc, unsigned menu_item)
{
   menu_items_p miptr = &menu_items[menu_item] ;
   // BitBlt(hdc, destX, destY, width, height, hdc, srcX, srcY, Color) ;
   BitBlt(hdc, miptr->region.left, miptr->region.top, menusz.cx, menusz.cy, hdc, 1, 1, ROP_XOR) ;
}

/************************************************************************/
static unsigned active_menu_item(int xPos, int yPos)
{
   unsigned j ;

   for (j=0; menu_items[j].menu_text != 0; j++) {
      menu_items_p miptr = &menu_items[j] ;
      if (miptr->selection_key == 0)
         continue;
      if (xPos < miptr->region.left  ||  xPos > miptr->region.right)
         return 0;
      if (yPos > miptr->region.top  &&  yPos < miptr->region.bottom) {
         // wsprintf(tempstr, "found menu item %d\n", j) ;
         // OutputDebugString(tempstr) ;
         return j ;
      }
   }
   return 0;
}

/************************************************************************/
void update_menu_item(HWND hwnd, int xPos, int yPos)
{
   // wsprintf(tempstr, "cursor: x=%d, y=%d, mscomp=%u\n", xPos, yPos, menu_size_computed) ;
   // OutputDebugString(tempstr) ;
   if (demo_state != 0) {
      return ;
   }
   HDC hdc = GetDC (hwnd) ;
   if (hilited_menu_item >= 0) {
      hilite_menu_item(hdc, hilited_menu_item) ;
      hilited_menu_item = -1 ;
   }
   unsigned mitem = active_menu_item(xPos, yPos) ;
   if (mitem != 0  &&  menu_items[mitem].selection_key != 0) {
      hilite_menu_item(hdc, mitem) ;
      hilited_menu_item = mitem ;
   }

   ReleaseDC (hwnd, hdc) ;
}

/************************************************************************/
void process_keystroke(HWND hwnd, unsigned inchr);

void select_menu_item(HWND hwnd, int xPos, int yPos)
{
   HDC hdc = GetDC (hwnd) ;
   if (hilited_menu_item >= 0) {
      hilite_menu_item(hdc, hilited_menu_item) ;
      hilited_menu_item = -1 ;
   }
   ReleaseDC (hwnd, hdc) ;
   unsigned mitem = active_menu_item(xPos, yPos) ;
   if (mitem != 0  &&  menu_items[mitem].selection_key != 0) {
      process_keystroke(hwnd, menu_items[mitem].selection_key);
      // menu_items_p miptr = &menu_items[mitem] ;
      // SetWindowText(hwnd, miptr->title) ;
      // cycle_count = 0 ;
      // demo_state = mitem ;
      // display_current_operation(hwnd);
   }
}

/************************************************************************/
static void ShowMenu(HWND hwnd)
{
   // RECT rect;
   HDC hdc;
   TEXTMETRIC tm ;
   int baseX, baseY, diffY, j ;

   if (!we_should_redraw) 
      return ;

   // menu_items_p miptr = &menu_items[0] ;
   SetWindowText(hwnd, "Graphics demos") ;

   hdc = GetDC (hwnd) ;
   Clear_Screen(hdc) ;

   //****************************************************
   //  draw logo
   //****************************************************
   HFONT hfont = build_font("CaligulaA", 25, 0, 0, 1, 0) ;
   SelectObject (hdc, hfont) ;
   GetTextMetrics(hdc, &tm) ;
   diffY = tm.tmHeight + tm.tmExternalLeading ;

   baseY = 20 ;
   baseX = MENU_BASE_LEFT ;
   dprints(hdc, baseX, baseY, 0x1C, "       Main Menu      ") ; 
   baseY += diffY ;
   baseY += 3 ;  //  make a little more gap below menu

   //****************************************************
   //  draw other menu lines
   //****************************************************
   hfont = build_font("Bodacious-Normal", 17, 0, 0, 0, 0) ;
   SelectObject (hdc, hfont) ;
   GetTextMetrics(hdc, &tm) ;
   diffY = tm.tmHeight + tm.tmExternalLeading ;
   if (!menu_size_computed) {
      menu_size_computed = 1 ;
      unsigned lms_index = find_index_of_longest_menu_string() ;
      GetTextExtentPoint32(hdc, menu_items[lms_index].menu_text, strlen(menu_items[lms_index].menu_text), &menusz) ;
   }

   for (j=0; menu_items[j].menu_text != 0; j++) {
      dprints(hdc, baseX, baseY, menu_items[j].menu_attr, menu_items[j].menu_text) ;
      menu_items[j].region.left   = baseX ;
      menu_items[j].region.top    = baseY ;
      menu_items[j].region.right  = baseX + menusz.cx ;
      menu_items[j].region.bottom = baseY + menusz.cy ;
      baseY +=   diffY ;
   }
   // then put in the cycle counter
   baseY +=   diffY ;
   unsigned tf = (unsigned) (proc_time () - ti) / get_clocks_per_msec ();
   if (tf == 0)
      tf= 1 ;
   //  for some reason, 21226/1121=0 ... I don't understand...
   unsigned cycles_per_msec = cycle_count * 1000 / tf ;   
   wsprintf(tempstr, "cycle_count=%u, %u msec, %u cycles/sec", 
      cycle_count, tf, cycles_per_msec) ;
   dprints(hdc, baseX, baseY, 0, tempstr) ;

   DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT)));
   ReleaseDC (hwnd, hdc) ;
}

/************************************************************************/
//lint -esym(765, get_system_message)
char *get_system_message(void)
{
   static char msg[261] ;
   int slen ;

   LPVOID lpMsgBuf;
   FormatMessage( 
      FORMAT_MESSAGE_ALLOCATE_BUFFER | 
      FORMAT_MESSAGE_FROM_SYSTEM | 
      FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      GetLastError(),
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
      (LPTSTR) &lpMsgBuf,
      0, 0);
   // Process any inserts in lpMsgBuf.
   // ...
   // Display the string.
   strncpy(msg, (char *) lpMsgBuf, 260) ;

   // Free the buffer.
   LocalFree( lpMsgBuf );

   //  trim the newline off the message before copying it...
   slen = strlen(msg) ;
   if (msg[slen-1] == 10  ||  msg[slen-1] == 10) {
      msg[slen-1] = 0 ;
   }

   return msg;
}

//***********************************************************************
void display_current_operation(HWND hwnd)
{
   if (cxClient == 0 || cyClient == 0)
        return ;

   menu_items_p miptr = &menu_items[demo_state] ;
   if (miptr->go != 0) {
      miptr->go->update_display(hwnd) ;
   } else {
      (miptr->draw_func)(hwnd) ;
   }
   we_should_redraw = 0 ;
}


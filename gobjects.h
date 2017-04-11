//********************************************************************
//  gobjects - a class to manage graphics objects for my
//  gstuff program.
//
//  Written by:   Daniel D Miller
//********************************************************************

//lint -e1704  Constructor has private access specification

class graph_object {
private:   
   //  disable copy and assignment operators
   //  for classes with pointer members
   graph_object& operator=(const graph_object &src) ;
   graph_object(const graph_object&);

//  make this field protected, rather than private,
//  so derived classes can access it.
protected:
   //  actually, having the title within the graphics object isn't that practical.
   //  We only want the title redrawn when the entire window is redrawn (WM_PAINT),
   //  and the graphic object doesn't know about Windows messages, that concept
   //  is external to the class.
   //  I'm leaving it in for now, because it's my only example of 
   //  how derived classes can conditionally call the base-class constructor.
   //  (Also, I don't feel like going back and editing 20 different files...)
   char *title ;

public:
   graph_object(char *title) ;
   virtual ~graph_object() {
      if (title != NULL) {
         delete[] title ;
         title = 0 ;
      }
      } ;
   virtual void update_display(HWND hwnd) = 0 ;
   virtual bool process_key(unsigned key_id) = 0 ;
} ;

//*******************************************************
class sglass: public graph_object {
private:
   int max_col, max_row, x, y;
   int distfact ;    /* multiplier for distance */
   int size ;        /* max size to grow to     */
   int osize ;       /* same                    */
   int limit ;       /* min. size of box - one row/col */
   int in_size ;     /* starting size                */
   int o_size ;      /* same                         */
   int unoo ;        /* one - changed from + to -    */
   int onoo ;        /* same, for other drawing      */

   void box_box (HDC hdc, int col_inpt, int row_inpt, int siz);
   void box_point (HDC hdc, int ccol, int rrow, int ssiz);

   //  disable copy and assignment operators
   //  for classes with pointer members
   sglass& operator=(sglass const &src) ;
   sglass(const sglass&);

public:
   sglass(char *title_text) ;
   virtual ~sglass() {} ;
   void update_display(HWND hwnd) ;
   bool process_key(unsigned key_id) ;
} ;

//*******************************************************
class flames: public graph_object {
private:
   rgb_t fire_palette[256] ;
   unsigned fire_palette_init ;
   u8 *fire_palette_record ;
   unsigned fire_char_width ;
   unsigned fire_char_height ;
   unsigned fire_rows ;
   unsigned fire_cols ;

   //  private functions
   void dump_fire_palette(void);
   void set_fire_palette(unsigned index, u8 red, u8 green, u8 blue);
   unsigned get_record_index(unsigned x, unsigned y);
   void set_palette_index(unsigned x, unsigned y, u8 color_idx);
   unsigned get_palette_index(unsigned x, unsigned y);
   void update_global_palette28(void);
   void init_fire_palette(void);
   COLORREF get_fire_palette(unsigned index);
   void draw_fire_element(HDC hdc, unsigned x, unsigned y, unsigned color);

   //  disable copy and assignment operators
   //  for classes with pointer members
   flames &operator=(const flames &src) ;
   flames(const flames&);

public:
   flames(char *title_text) ;
   // ~flames() {} ;
   void update_display(HWND hwnd) ;
   bool process_key(unsigned key_id) ;
} ;

//*******************************************************
typedef struct face_s {
   u8  fchar ;
   COLORREF  attr ;
   unsigned  dir ;   //  0-7 representing one of 8 linear directions
   unsigned  row ;
   unsigned  col ;
} face_t, *face_p ;

#define  FACE_COUNT  30

class face_trap: public graph_object {
private:
   char *busy_bfr ;
   unsigned char_width ;
   unsigned char_height ;
   unsigned dft_columns ;
   unsigned dft_rows ;
   face_t faces[FACE_COUNT] ;

   //  private functions
   void move_a_face(HDC hdc, face_p fp);
   void redraw_face_traps(HDC hdc);
   unsigned pick_new_dir(unsigned free_flags, unsigned free_count);
   unsigned get_free_count(unsigned busy_flags);
   unsigned get_free_flags(int x, int y);
   unsigned is_cell_free(int column, int row);
   void dputc(HDC hdc, unsigned x, unsigned y, char outchr, COLORREF attr);
   unsigned max_char_width(HDC hdc);

   //  disable copy and assignment operators
   //  for classes with pointer members
   face_trap &operator=(const face_trap &src) ;
   face_trap(const face_trap&);

public:
   face_trap(char *title_text) ;
   // ~faces() {} ;
   void update_display(HWND hwnd) ;
   bool process_key(unsigned key_id) ;
} ;


//*******************************************************
class rainbow: public graph_object {
private:
   double X, Y, B ;
   double thold_limit ;
   unsigned xbase, xdiff, ybase, ydiff ;

   //  private functions
   void rainbow_plot_pixel(HDC hdc, int pcolor, double thold_angle, unsigned primary);
   void update_gtimer(HDC hdc);

   //  disable copy and assignment operators
   //  for classes with pointer members
   rainbow &operator=(const rainbow &src) ;
   rainbow(const rainbow&);

public:
   rainbow(char *title_text) ;
   // ~faces() {} ;
   void update_display(HWND hwnd) ;
   bool process_key(unsigned key_id) ;
   void update_boundaries(unsigned xClient, unsigned yClient);
} ;

//*******************************************************
class pixels: public graph_object {
private:
   unsigned dp_char_width ;
   unsigned dp_char_height ;
   unsigned rows ;
   unsigned columns ;
   unsigned color ;

   //  disable copy and assignment operators
   //  for classes with pointer members
   pixels &operator=(const pixels &src) ;
   pixels(const pixels&);

public:
   pixels(char *title_text) ;
   void update_display(HWND hwnd) ;
   bool process_key(unsigned key_id) ;
} ;

//*******************************************************
class ascii: public graph_object {
private:

   //  disable copy and assignment operators
   //  for classes with pointer members
   ascii &operator=(const ascii &src) ;
   ascii(const ascii&);

public:
   ascii(char *title_text) ;
   void update_display(HWND hwnd) ;
   bool process_key(unsigned key_id) ;
   void set_font_name(char *new_font_name);
   char *get_font_name(void);
} ;

//*******************************************************
class rcolors: public graph_object {
private:
   unsigned char_width ;
   unsigned char_height ;
   unsigned rows ;
   unsigned columns ;

   //  disable copy and assignment operators
   //  for classes with pointer members
   rcolors &operator=(const rcolors &src) ;
   rcolors(const rcolors&);

public:
   rcolors(char *title_text) ;
   void update_display(HWND hwnd) ;
   bool process_key(unsigned key_id) ;
} ;

/************************************************************************/
typedef struct vector_s {
   unsigned x ;
   unsigned x_dir ;
   unsigned y ;
   unsigned y_dir ;
   //  mode3 angle management vars
   unsigned theta ;
   double tan_theta ;
   double dx ;
   unsigned prev_dx ;
   unsigned x_changed ;
   double dy ;
   unsigned prev_dy ;
   unsigned y_changed ;
} vector_t, *vector_p ;

//*******************************************************
class line_games: public graph_object {
private:
   vector_t start ;
   vector_t finish ;
   unsigned state ;
   unsigned delay ;
   unsigned color ;
   unsigned line_algorithm ;

   //  private functions
   void move_point(vector_p vector);
   void init_vector(vector_p vector);

   //  disable copy and assignment operators
   //  for classes with pointer members
   line_games &operator=(const line_games &src) ;
   line_games(const line_games&);

public:
   line_games(char *title_text) ;
   void update_display(HWND hwnd) ;
   bool process_key(unsigned key_id) ;
   void update_line_algorithm(void);
} ;

//*******************************************************
class lines: public graph_object {
private:
   unsigned orient ;  //  0=horiz, 1=vert

   //  disable copy and assignment operators
   //  for classes with pointer members
   lines &operator=(const lines &src) ;
   lines(const lines&);

public:
   lines(char *title_text) ;
   void update_display(HWND hwnd) ;
   bool process_key(unsigned key_id) ;
} ;

//*******************************************************
class gpalettes: public graph_object {
private:

   //  disable copy and assignment operators
   //  for classes with pointer members
   gpalettes &operator=(const gpalettes &src) ;
   gpalettes(const gpalettes&);

public:
   gpalettes(char *title_text) ;
   void update_display(HWND hwnd) ;
   bool process_key(unsigned key_id) ;
} ;

//*******************************************************
class xnpalette: public graph_object {
private:

   //  disable copy and assignment operators
   //  for classes with pointer members
   xnpalette &operator=(const xnpalette &src) ;
   xnpalette(const xnpalette&);

public:
   xnpalette(char *title_text) ;
   void update_display(HWND hwnd) ;
   bool process_key(unsigned key_id) ;
} ;

//*******************************************************
class xpalette: public graph_object {
private:

   //  disable copy and assignment operators
   //  for classes with pointer members
   xpalette &operator=(const xpalette &src) ;
   xpalette(const xpalette&);

public:
   xpalette(char *title_text) ;
   void update_display(HWND hwnd) ;
   bool process_key(unsigned key_id) ;
} ;

//*******************************************************
class xrect: public graph_object {
private:

   //  private functions
   void Solid_XRect(HDC hdc, int xl, int yu, int xr, int yl, int Color);

   //  disable copy and assignment operators
   //  for classes with pointer members
   xrect &operator=(const xrect &src) ;
   xrect(const xrect&);

public:
   xrect(char *title_text) ;
   void update_display(HWND hwnd) ;
   bool process_key(unsigned key_id) ;
} ;

//*******************************************************
class rect: public graph_object {
private:

   //  disable copy and assignment operators
   //  for classes with pointer members
   rect &operator=(const rect &src) ;
   rect(const rect&);

public:
   rect(char *title_text) ;
   void update_display(HWND hwnd) ;
   bool process_key(unsigned key_id) ;
} ;

//*******************************************************
class polygon: public graph_object {
private:

   //  disable copy and assignment operators
   //  for classes with pointer members
   polygon &operator=(const polygon &src) ;
   polygon(const polygon&);

public:
   polygon(char *title_text) ;
   void update_display(HWND hwnd) ;
   bool process_key(unsigned key_id) ;
} ;

//*******************************************************
class squares: public graph_object {
private:

   //  disable copy and assignment operators
   //  for classes with pointer members
   squares &operator=(const squares &src) ;
   squares(const squares&);

public:
   squares(char *title_text) ;
   void update_display(HWND hwnd) ;
   bool process_key(unsigned key_id) ;
} ;

//*******************************************************
class circles: public graph_object {
private:

   //  disable copy and assignment operators
   //  for classes with pointer members
   circles &operator=(const circles &src) ;
   circles(const circles&);

public:
   circles(char *title_text) ;
   void update_display(HWND hwnd) ;
   bool process_key(unsigned key_id) ;
} ;

//*******************************************************
class colorbars: public graph_object {
private:

   //  disable copy and assignment operators
   //  for classes with pointer members
   colorbars &operator=(const colorbars &src) ;
   colorbars(const colorbars&);

public:
   colorbars(char *title_text) ;
   void update_display(HWND hwnd) ;
   bool process_key(unsigned key_id) ;
} ;

//*******************************************************
class bitblt: public graph_object {
private:

   //  private functions
   void Concentric_Rect(HDC hdc, int l, int t, int width, int height);

   //  disable copy and assignment operators
   //  for classes with pointer members
   bitblt &operator=(const bitblt &src) ;
   bitblt(const bitblt&);

public:
   bitblt(char *title_text) ;
   void update_display(HWND hwnd) ;
   bool process_key(unsigned key_id) ;
} ;

//*******************************************************
class triangles: public graph_object {
private:

   //  disable copy and assignment operators
   //  for classes with pointer members
   triangles &operator=(const triangles &src) ;
   triangles(const triangles&);

public:
   triangles(char *title_text) ;
   void update_display(HWND hwnd) ;
   bool process_key(unsigned key_id) ;
} ;

//*******************************************************
class wincolors: public graph_object {
private:

   //  disable copy and assignment operators
   //  for classes with pointer members
   wincolors &operator=(const wincolors &src) ;
   wincolors(const wincolors&);

public:
   wincolors(char *title_text) ;
   void update_display(HWND hwnd) ;
   bool process_key(unsigned key_id) ;
} ;


//*******************************************************
class acidwarp: public graph_object {
private:

   //  private functions
   // int generate_image(int imageFuncNum, UCHAR *buf_graf, int xcenter, int ycenter,
   //           int xmax, int ymax, int colormax);
   int generate_image (int imageFuncNum, HDC hdc,
                       int xcenter, int ycenter, int xmax, int ymax, int colormax);

   //  disable copy and assignment operators
   //  for classes with pointer members
   acidwarp &operator=(const acidwarp &src) ;
   acidwarp(const acidwarp&);

public:
   acidwarp(char *title_text) ;
   void update_display(HWND hwnd) ;
   bool process_key(unsigned key_id) ;
} ;


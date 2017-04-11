//  Win32 definitions

typedef unsigned char u8 ;
typedef unsigned long long  u64 ;

extern unsigned cycle_count ;

//  Windows-object IDs
#define  GSTUFF         303
#define  IDC_STATIC      -1
#define  IDC_FSELECT    400
#define  IDC_FCOMBO     401

#define  FONT_HEIGHT    10

//********************************************************
//  Windows timer-function wrappers
//  timer usage:
//    u64 tf, ti = proc_time ();
//    tf = (proc_time () - ti) / get_clocks_per_second ();
//********************************************************
u64 get_clocks_per_second(void);
u64 get_clocks_per_msec(void);
u64 proc_time(void);


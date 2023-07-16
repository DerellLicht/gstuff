//****************************************************************************
//  File: DEMO.H - exports graphic drawing functions to the 
//  Windows-interface module.
//****************************************************************************

#define  CONST_SIZE     50

extern int demo_state ;
extern int we_should_redraw ;
extern int pause_the_race ;
extern unsigned use_solid_pattern ;

//***********************************************************************
//  demo function prototypes
//***********************************************************************
char *get_system_message(void);
int check_for_state_change(HWND hwnd, unsigned inchr);
void display_current_operation(HWND hwnd);


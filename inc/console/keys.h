// KEY DEFINITIONS
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef KEYS_H
#define KEYS_H

// standard and movement
#define KB_ENTER	    13
#define KB_RIGHT	    2087
#define KB_LEFT		    2085
#define KB_UP		    2086
#define KB_DOWN		    2088
#define KB_ESC		    27
#define KB_PGUP		    2081
#define KB_PGDN		    2082
#define KB_HOME		    2084
#define KB_END		    2083
#define KB_INS		    2093
#define KB_DEL		    2094
#define KB_TAB		    9
#define KB_BACKTAB	    15
#define KB_BKSPACE	    8
#define KB_PLUS	    	    43    
#define KB_MINUS    	    45    
#define KB_SPACE	    32
#define KB_ALT_MINUS	    365
#define KB_ALT_PLUS	    363
#define KB_CTRL_MINUS	    621
#define KB_CTRL_PLUS	    619
#define KB_CTRL_ENTER	    525

// keypad arrows (NOT SAME!)
#define KB_KP_RIGHT	    4135
#define KB_KP_LEFT	    4133
#define KB_KP_UP	    4134
#define KB_KP_DOWN	    4136
#define KB_KP_HOME	    4132
#define KB_KP_END	    4131
#define KB_KP_PGUP	    4129
#define KB_KP_PGDN	    4130
#define KB_KP_CENTER	    4108
#define KB_KP_INS	    4141
#define KB_KP_DEL	    4142

// standard characters which I will not type them all in (they correspond to ASCII)
#define KB_A		    65		// A, B, C, ...
#define KB_a		    97		// a, b, c, ...
#define KB_0		    48		// zero, 1, 2, 3, ...

// function keys
#define KB_F1		    4208
#define KB_F2		    (KB_F1 + 1)
#define KB_F3		    (KB_F1 + 2)
#define KB_F4		    (KB_F1 + 3)
#define KB_F5		    (KB_F1 + 4)
#define KB_F6		    (KB_F1 + 5)
#define KB_F7		    (KB_F1 + 6)
#define KB_F8		    (KB_F1 + 7)
#define KB_F9		    (KB_F1 + 8)
#define KB_F10		    (KB_F1 + 9)
#define KB_F11		    (KB_F1 + 10)
#define KB_F12		    (KB_F1 + 11)

// control-function keys
#define KB_CTRL_F1	    624
#define KB_CTRL_F2	    (KB_CTRL_F1 + 1)
#define KB_CTRL_F3	    (KB_CTRL_F1 + 2)
#define KB_CTRL_F4	    (KB_CTRL_F1 + 3)
#define KB_CTRL_F5	    (KB_CTRL_F1 + 4)
#define KB_CTRL_F6	    (KB_CTRL_F1 + 5)
#define KB_CTRL_F7	    (KB_CTRL_F1 + 6)
#define KB_CTRL_F8	    (KB_CTRL_F1 + 7)
#define KB_CTRL_F9	    (KB_CTRL_F1 + 8)
#define KB_CTRL_F10	    (KB_CTRL_F1 + 9)
#define KB_CTRL_F11	    (KB_CTRL_F1 + 10)
#define KB_CTRL_F12	    (KB_CTRL_F1 + 11)

// alt-function keys
#define KB_ALT_F1	    368
#define KB_ALT_F2	    (KB_ALT_F1 + 1)
#define KB_ALT_F3	    (KB_ALT_F1 + 2)
#define KB_ALT_F4	    (KB_ALT_F1 + 3)
#define KB_ALT_F5	    (KB_ALT_F1 + 4)
#define KB_ALT_F6	    (KB_ALT_F1 + 5)
#define KB_ALT_F7	    (KB_ALT_F1 + 6)
#define KB_ALT_F8	    (KB_ALT_F1 + 7)
#define KB_ALT_F9	    (KB_ALT_F1 + 8)
#define KB_ALT_F10	    (KB_ALT_F1 + 9)
#define KB_ALT_F11	    (KB_ALT_F1 + 10)
#define KB_ALT_F12	    (KB_ALT_F1 + 11)

// control keys
#define KB_CTRL_A	    577
#define KB_CTRL_B	    (KB_CTRL_A + 1)
#define KB_CTRL_C	    (KB_CTRL_A + 2)
#define KB_CTRL_D	    (KB_CTRL_A + 3)
#define KB_CTRL_E	    (KB_CTRL_A + 4)
#define KB_CTRL_F	    (KB_CTRL_A + 5)
#define KB_CTRL_G	    (KB_CTRL_A + 6)
#define KB_CTRL_H	    (KB_CTRL_A + 7)
#define KB_CTRL_I	    (KB_CTRL_A + 8)
#define KB_CTRL_J	    (KB_CTRL_A + 9)
#define KB_CTRL_K	    (KB_CTRL_A + 10)
#define KB_CTRL_L	    (KB_CTRL_A + 11)
#define KB_CTRL_M	    (KB_CTRL_A + 12)
#define KB_CTRL_N	    (KB_CTRL_A + 13)
#define KB_CTRL_O	    (KB_CTRL_A + 14)
#define KB_CTRL_P	    (KB_CTRL_A + 15)
#define KB_CTRL_Q	    (KB_CTRL_A + 16)
#define KB_CTRL_R	    (KB_CTRL_A + 17)
#define KB_CTRL_S	    (KB_CTRL_A + 18)
#define KB_CTRL_T	    (KB_CTRL_A + 19)
#define KB_CTRL_U	    (KB_CTRL_A + 20)
#define KB_CTRL_V	    (KB_CTRL_A + 21)
#define KB_CTRL_W	    (KB_CTRL_A + 22)
#define KB_CTRL_X	    (KB_CTRL_A + 23)
#define KB_CTRL_Y	    (KB_CTRL_A + 24)
#define KB_CTRL_Z	    (KB_CTRL_A + 25)

// control keys (numbers)
#define KB_CTRL_0	    560	    // control-zero
#define KB_CTRL_1	    (KB_CTRL_0 + 1)
#define KB_CTRL_2	    (KB_CTRL_0 + 2)
#define KB_CTRL_3	    (KB_CTRL_0 + 3)
#define KB_CTRL_4	    (KB_CTRL_0 + 4)
#define KB_CTRL_5	    (KB_CTRL_0 + 5)
#define KB_CTRL_6	    (KB_CTRL_0 + 6)
#define KB_CTRL_7	    (KB_CTRL_0 + 7)
#define KB_CTRL_8	    (KB_CTRL_0 + 8)
#define KB_CTRL_9	    (KB_CTRL_0 + 9)

// Alt keys
#define KB_ALT_A	    321
#define KB_ALT_B	    (KB_ALT_A + 1)
#define KB_ALT_C	    (KB_ALT_A + 2)
#define KB_ALT_D	    (KB_ALT_A + 3)
#define KB_ALT_E	    (KB_ALT_A + 4)
#define KB_ALT_F	    (KB_ALT_A + 5)
#define KB_ALT_G	    (KB_ALT_A + 6)
#define KB_ALT_H	    (KB_ALT_A + 7)
#define KB_ALT_I	    (KB_ALT_A + 8)
#define KB_ALT_J	    (KB_ALT_A + 9)
#define KB_ALT_K	    (KB_ALT_A + 10)
#define KB_ALT_L	    (KB_ALT_A + 11)
#define KB_ALT_M	    (KB_ALT_A + 12)
#define KB_ALT_N	    (KB_ALT_A + 13)
#define KB_ALT_O	    (KB_ALT_A + 14)
#define KB_ALT_P	    (KB_ALT_A + 15)
#define KB_ALT_Q	    (KB_ALT_A + 16)
#define KB_ALT_R	    (KB_ALT_A + 17)
#define KB_ALT_S	    (KB_ALT_A + 18)
#define KB_ALT_T	    (KB_ALT_A + 19)
#define KB_ALT_U	    (KB_ALT_A + 20)
#define KB_ALT_V	    (KB_ALT_A + 21)
#define KB_ALT_W	    (KB_ALT_A + 22)
#define KB_ALT_X	    (KB_ALT_A + 23)
#define KB_ALT_Y	    (KB_ALT_A + 24)
#define KB_ALT_Z	    (KB_ALT_A + 25)

// Alt keys (numbers)
#define KB_ALT_0	    304	    // alt-zero
#define KB_ALT_1	    (KB_ALT_0 + 1)
#define KB_ALT_2	    (KB_ALT_0 + 2)
#define KB_ALT_3	    (KB_ALT_0 + 3)
#define KB_ALT_4	    (KB_ALT_0 + 4)
#define KB_ALT_5	    (KB_ALT_0 + 5)
#define KB_ALT_6	    (KB_ALT_0 + 6)
#define KB_ALT_7	    (KB_ALT_0 + 7)
#define KB_ALT_8	    (KB_ALT_0 + 8)
#define KB_ALT_9	    (KB_ALT_0 + 9)

// MOUSE STUFF - event.data is coordinates of mouse

#define MS_FLAG		    0x00010000	    // indicates a mouse event

#define MS_ALT		    0x00010001	    // indicates ALT is pressed
#define MS_CTRL		    0x00010002	    // indicates CTRL is pressed
#define MS_SHIFT	    0x00010004	    // indactes SHIFT is pressed

#define MS_LEFTBUTTON	    0x00010010	    // indicates left button is down
#define MS_MIDDLEBUTTON	    0x00010020	    // indicates middle button is down
#define MS_RIGHTBUTTON	    0x00010040	    // indicates right button is down

#define MS_DBLCLICK	    0x00010100	    // indicates second click of a double-click
#define MS_MOVED	    0x00010200	    // indicates mouse position changed
#define MS_BUTTON	    0x00010400	    // indicates button pressed or released

// CONSOLE WINDOW RESIZE

#define CW_RESIZE	    0x00011000	    // window was resized - event.data is dimension

#endif

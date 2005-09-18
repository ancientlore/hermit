// EVENTS
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef EVENTS_H
#define EVENTS_H

#define EV_QUIT	    -1		// Used to end the run() loop

#define EV_FIRST    0x00020000	    // First non-key event

#define EV_STATUS_MSG	    EV_FIRST	    // add msg to status line, event.data is a string
#define EV_STATUS_EXTRA	    (EV_FIRST + 1)  // add extra msg to status line, event.data is a string

#define EV_CURSOR_CHANGE    (EV_FIRST + 2)  // A scroller's cursor has changed location, event.data is numeric location

#define EV_TITLE_MSG	    (EV_FIRST + 3)  // add msg to title bar, event.data is a string
#define EV_TITLE_EXTRA	    (EV_FIRST + 4)  // add extra msg to title bar, event.data is a string

#define EV_RUN_SINGLE_COMMAND	(EV_FIRST + 5)	// run a single command, event.data is command key
#define EV_RUN_MULTI_COMMAND	(EV_FIRST + 6)	// run a single command, event.data is command key

#define EV_REDRAW	    (EV_FIRST + 7)	// redraw window, event.data is nothing

#define EV_COLOR_CHANGE	    (EV_FIRST + 8)	// new colors!

#endif

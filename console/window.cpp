// Window classes
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "console/screen.h"


Window::Window (Screen& screen) : myScreen (screen)
{
    myScreen.addWindow (this);
}

Window::~Window ()
{
    myScreen.removeWindow (this);
}

// End

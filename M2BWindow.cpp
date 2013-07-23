/*
	M2BWindow.cpp	- displays an M2BView 
	
		Copyright 1996,1997,1998 Bill Thibault, All Rights Reversed.
*/

#include <Application.h>
#include "M2BWindow.h"

M2BWindow::M2BWindow ( BRect frame )
			: BWindow ( frame, "MidiMonitor", B_TITLED_WINDOW,
			/* B_NOT_RESIZABLE | B_NOT_ZOOMABLE */ 0, 0 )
{
	SetPulseRate ( 50000 ); // 50 ms
}

bool M2BWindow::QuitRequested()
{
	be_app->PostMessage ( B_QUIT_REQUESTED );
	return ( TRUE );
}


void
M2BWindow::SaveRequested ( entry_ref dir, const char *filename )
{
}
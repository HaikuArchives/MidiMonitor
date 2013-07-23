//
// main.cpp - midi monitor app
//	Copyright 1998 Bill Thibault, All Rights Reversed.
//


#include <MidiKit.h>
#include <Entry.h>
#include <Path.h>
#include <Application.h>
#include <Alert.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <string.h>
#include "MidiMonitor.h"
#include "M2BWindow.h"
#include "M2BView.h"




class MidiMonitorApp : public BApplication {
	public:
				MidiMonitorApp ( void );
				~MidiMonitorApp ( void );
		virtual void MessageReceived ( BMessage *msg );
	private:
		int numPorts;
		BMidiPort	*midiPort;
		M2BMidiMonitor	*theMidiMonitor;
};

#define MSG_MIDIMONITOR_MIDI_PORT 'BUmp'

static bool isPortOpen = FALSE;

MidiMonitorApp::MidiMonitorApp ( void ) 
	: BApplication ("application/x-vnd.tebo-midimonitor")
{
	BRect aRect;
	BList windows;
	const float menuHeight = 20;
	int	initPort = 0; // set this from prefs instead...archive?
	bool isPortOpen = FALSE;
	char str[80], portname[132];
		

	midiPort = new BMidiPort;
	numPorts = midiPort->CountDevices();
	midiPort->GetDeviceName ( initPort, portname );
	if ( midiPort->Open (portname) != B_NO_ERROR ) {
		sprintf ( str, "can't open %s", portname );
		BAlert *b = new BAlert ("", str, "ug" );
		b->Go();
	} else {
//		sprintf ( str, "opened %s", portNames[initPort] );
//		BAlert *b = new BAlert ("", str, "okay!" );
//		b->Go();
		isPortOpen = TRUE;
	}
		
	theMidiMonitor = new M2BMidiMonitor ();
	theMidiMonitor->Start();
	
	midiPort->Connect ( theMidiMonitor );
	if ( isPortOpen )
		midiPort->Start();

	aRect.Set(20, 100, 340, 400);
	BWindow *aWindow = (BWindow *) new M2BWindow(aRect);
	aRect.OffsetTo(B_ORIGIN);
	BView *aView = (BView *) new M2BView(aRect, "M2BView", 
					(M2BMidiMonitor*)theMidiMonitor, menuHeight);
	aWindow->AddChild(aView);
	aWindow->Show();
	windows.AddItem ( aWindow );


	

		aRect.Set ( 0,0,1000,menuHeight);
		BMenuBar	*menubar = new BMenuBar ( aRect, "menubar" );
		menubar->SetBorder ( B_BORDER_FRAME );
	
		BMenuItem *item; 
		BMenu *menu = new BMenu("File"); 
   
		item = new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED), 'Q'); 
		item->SetTarget(be_app); 
		menu->AddItem(item); 
	
		menubar->AddItem ( menu );
		
		// add the port selection menu
		menu = new BMenu ( "Port" );
		for ( int i = 0; i < numPorts; i++ ) {
			midiPort->GetDeviceName ( i, portname );
			item = new BMenuItem ( portname, 
									new BMessage ( MSG_MIDIMONITOR_MIDI_PORT ) );
			item->Message()->AddString ( "port", portname );
			menu->AddItem ( item );
		}
		menu->SetRadioMode ( TRUE );
		menu->ItemAt(initPort)->SetMarked ( TRUE );
		menu->SetTargetForItems ( be_app );	
		menubar->AddItem ( menu );


		aView->AddChild ( menubar );
	
	
}


MidiMonitorApp::~MidiMonitorApp ( void )
{
	theMidiMonitor->Stop();
	delete theMidiMonitor;
}



void
MidiMonitorApp::MessageReceived ( BMessage *msg )
{
	switch ( msg->what ) {
		case MSG_MIDIMONITOR_MIDI_PORT:
			midiPort->Stop();
			midiPort->Close();
			if ( midiPort->Open ( msg->FindString ("port") ) != B_NO_ERROR ) {
				char str[80];
				sprintf ( str, "can't open %s", msg->FindString ("port") );
				BAlert *bug = new BAlert ( "", str, "sigh" );
				bug->Go();
				isPortOpen = false;
			} else {
				isPortOpen = true;
				midiPort->Start();
			}
			break;
		default:
			BApplication::MessageReceived ( msg );
			break;
	}
}





main ( ) {
	MidiMonitorApp	app;
	app.Run();
}
	
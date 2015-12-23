/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "MainWindow.h"

#include <Application.h>
#include <LayoutBuilder.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <Messenger.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>

#include <private/interface/AboutWindow.h>

enum
{
	SHOW_ABOUT = 'swat',
	UPDATE_TIP = 'uptp'
};


MainWindow::MainWindow(void)
	: BWindow(BRect(100,100,700,300), "Tipster", B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS)
{
	BuildLayout();
}


void
MainWindow::BuildLayout(void)
{
	BRect r(Bounds());
	r.bottom = 20;
	
	BMenuBar *fMenuBar = new BMenuBar(r, "menubar");
	BMenu *fSettingsMenu = new BMenu("Settings");
	
	fSettingsMenu->AddItem(new BMenuItem("About", new BMessage(SHOW_ABOUT),
		'A', B_COMMAND_KEY));
	
	fMenuBar->AddItem(fSettingsMenu);
	
	fTipsterViewContainer = new BView(BRect(0,0,700,300),
		"tipster_container", B_FOLLOW_ALL, B_WILL_DRAW);
	
	fTipsterView = new Tipster(fTipsterViewContainer->Frame());
	fTipsterViewContainer->AddChild(fTipsterView);
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fMenuBar)
		.Add(fTipsterViewContainer);
}


bool
MainWindow::QuitRequested(void)
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}


void
MainWindow::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
		case SHOW_ABOUT:
		{
			BAboutWindow *about = new BAboutWindow("Tipster",
				"application/x-vnd.tipster");
			
			about->AddDescription("An application to show usability tips for Haiku");
			about->AddCopyright(2015, "Vale Tolpegin");
			
			about->Show();
			
			break;
		}
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}

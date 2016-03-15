/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "MainWindow.h"

#include <Application.h>
#include <Catalog.h>
#include <ControlLook.h>
#include <Dragger.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <LayoutBuilder.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <TranslationUtils.h>

#include <private/interface/AboutWindow.h>

#include <stdio.h>

enum
{
	SHOW_ABOUT = 'swat',
	NEXT_TIP = 'nxtp',
	PREVIOUS_TIP = 'pvtp',
	DELAY = 'dely',
	MSG_QUIT = 'quit'
};


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "MainWindow"

MainWindow::MainWindow()
	:
	BWindow(BRect(100, 100, 740, 190), B_TRANSLATE_SYSTEM_NAME("Tipster"),
		B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_NOT_V_RESIZABLE)
{
	BuildLayout();
}


void
MainWindow::BuildLayout()
{
	BMenuBar* fMenuBar = new BMenuBar("menubar");

	BMenu* fTipsterMenu = new BMenu(B_TRANSLATE_SYSTEM_NAME("Tipster"));
	BMenu* fTipMenu = new BMenu(B_TRANSLATE("Tip"));

	BMenu* fDelaySubMenu = new BMenu(B_TRANSLATE("Delay"));

	fTipsterMenu->AddItem(new BMenuItem(B_TRANSLATE("About"),
		new BMessage(SHOW_ABOUT)));
	fTipsterMenu->AddItem(new BMenuItem(B_TRANSLATE("Quit"),
		new BMessage(MSG_QUIT)));

	fTipMenu->AddItem(new BMenuItem(B_TRANSLATE("Previous tip"),
		new BMessage(PREVIOUS_TIP)));
	fTipMenu->AddItem(new BMenuItem(B_TRANSLATE("Next tip"),
		new BMessage(NEXT_TIP)));
	
	BMessage* delay30s_message = new BMessage(DELAY);
	delay30s_message->AddInt32("delay", 30000000);
	fDelaySubMenu->AddItem(new BMenuItem(B_TRANSLATE("30 seconds"),
		delay30s_message));
	
	BMessage* delay1m_message = new BMessage(DELAY);
	delay1m_message->AddInt32("delay", 60000000);
	fDelaySubMenu->AddItem(new BMenuItem(B_TRANSLATE("1 minute"),
		delay1m_message));
		
	BMessage* delay2m_message = new BMessage(DELAY);
	delay2m_message->AddInt32("delay", 120000000);
	fDelaySubMenu->AddItem(new BMenuItem(B_TRANSLATE("2 minutes"),
		delay2m_message));
	
	BMessage* delay5m_message = new BMessage(DELAY);
	delay5m_message->AddInt32("delay", 300000000);
	fDelaySubMenu->AddItem(new BMenuItem(B_TRANSLATE("5 minutes"),
		delay5m_message));
	
	fTipMenu->AddItem(fDelaySubMenu);

	fMenuBar->AddItem(fTipsterMenu);
	fMenuBar->AddItem(fTipMenu);

	fTipsterView = new Tipster();

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fMenuBar)
		.Add(fTipsterView)
		.AddGlue();
}


bool
MainWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "About"

void
MainWindow::MessageReceived(BMessage* message)
{
	switch(message->what)
	{
		case SHOW_ABOUT:
		{
			BAboutWindow* about = new BAboutWindow(B_TRANSLATE_SYSTEM_NAME("Tipster"),
				"application/x-vnd.tipster");
			about->AddDescription(B_TRANSLATE("An application to show "
						"usability tips for Haiku."));
			about->AddCopyright(2015, "Vale Tolpegin");
			about->Show();
			break;
		}
		case NEXT_TIP:
		{
			fTipsterView->UpdateTip();
			break;
		}
		case PREVIOUS_TIP:
		{
			fTipsterView->DisplayPreviousTip();
			break;
		}
		case DELAY:
		{
			int32 delay = 60000000;
			message->FindInt32("delay", &delay);
			
			fTipsterView->SetDelay(delay);
			break;
		}
		case MSG_QUIT:
		{
			be_app->PostMessage(B_QUIT_REQUESTED);
			break;
		}
		default:
			BWindow::MessageReceived(message);
			break;
	}
}

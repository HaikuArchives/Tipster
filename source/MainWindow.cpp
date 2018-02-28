/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "AboutTipster.h"
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
		B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_NOT_V_RESIZABLE
		| B_AUTO_UPDATE_SIZE_LIMITS)
{
	BuildLayout();
}


void
MainWindow::BuildLayout()
{
	BMenuBar* fMenuBar = new BMenuBar("menubar");

	BMenu* fTipsterMenu = new BMenu(B_TRANSLATE_SYSTEM_NAME("Tipster"));
	BMenu* fTipMenu = new BMenu(B_TRANSLATE("Tip"));

	fDelaySubMenu = new BMenu(B_TRANSLATE("Delay"));

	fTipsterMenu->AddItem(new BMenuItem(B_TRANSLATE("About"),
		new BMessage(SHOW_ABOUT)));
	fTipsterMenu->AddItem(new BMenuItem(B_TRANSLATE("Quit"),
		new BMessage(MSG_QUIT)));

	fTipMenu->AddItem(new BMenuItem(B_TRANSLATE("Previous tip"),
		new BMessage(PREVIOUS_TIP)));
	fTipMenu->AddItem(new BMenuItem(B_TRANSLATE("Next tip"),
		new BMessage(NEXT_TIP)));
	
	
	fTipMenu->AddItem(fDelaySubMenu);

	fMenuBar->AddItem(fTipsterMenu);
	fMenuBar->AddItem(fTipMenu);

	fTipsterView = new Tipster();

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fMenuBar)
		.Add(fTipsterView)
		.AddGlue();

	// After the creation of Tipster because we need the delay
	// time saved in the settings

	AddDelaySubMenuItem(30000000, B_TRANSLATE("30 seconds"));
	AddDelaySubMenuItem(60000000, B_TRANSLATE("1 minute"));
	AddDelaySubMenuItem(120000000, B_TRANSLATE("2 minutes"));
	AddDelaySubMenuItem(300000000, B_TRANSLATE("5 minutes"));

	fDelaySubMenu->SetRadioMode(true);

	GetLayout()->SetExplicitMinSize(BSize(740, B_SIZE_UNSET));
}


void
MainWindow::AddDelaySubMenuItem(bigtime_t delay, const char *label)
{
	BMenuItem* menuItem;
	BMessage* delayMessage = new BMessage(DELAY);
	delayMessage->AddInt64("delay", delay);
	fDelaySubMenu->AddItem(menuItem = new BMenuItem(label, delayMessage));
	menuItem->SetMarked(delay == fTipsterView->Delay());
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
			BAboutWindow* about = new AboutTipster();
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
			int64 delay = 60000000;
			message->FindInt64("delay", &delay);

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

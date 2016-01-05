/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "MainWindow.h"

#include <Application.h>
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
	DELAY_30_S = 'd30s',
	DELAY_1_M = 'd1mn',
	DELAY_2_M = 'd2mn',
	DELAY_5_M = 'd5mn',
};


MainWindow::MainWindow()
	:
	BWindow(BRect(100,100,740,200), "Tipster", B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS | B_NOT_V_RESIZABLE)
{
	BuildLayout();
}


void
MainWindow::BuildLayout()
{
	BMenuBar* fMenuBar = new BMenuBar("menubar");

	BMenu* fTipsterMenu = new BMenu("Tipster");
	BMenu* fTipMenu = new BMenu("Tip");

	BMenu* fDelaySubMenu = new BMenu("Delay");

	fTipsterMenu->AddItem(new BMenuItem("About", new BMessage(SHOW_ABOUT)));

	fTipMenu->AddItem(new BMenuItem("Previous Tip",
		new BMessage(PREVIOUS_TIP)));
	fTipMenu->AddItem(new BMenuItem("Next Tip", new BMessage(NEXT_TIP)));
	fDelaySubMenu->AddItem(new BMenuItem("30 Seconds",
		new BMessage(DELAY_30_S)));
	fDelaySubMenu->AddItem(new BMenuItem("1 Minute",
		new BMessage(DELAY_1_M)));
	fDelaySubMenu->AddItem(new BMenuItem("2 Minutes",
		new BMessage(DELAY_2_M)));
	fDelaySubMenu->AddItem(new BMenuItem("5 Minutes",
		new BMessage(DELAY_5_M)));
	fTipMenu->AddItem(fDelaySubMenu);

	fMenuBar->AddItem(fTipMenu);
	fMenuBar->AddItem(fTipsterMenu);

	fTipsterView = new Tipster();
	
	BRect rect(Bounds());
	rect.top = rect.bottom - 7;
	rect.left = rect.right - 7;
	
	BDragger* dragger = new BDragger(rect, fTipsterView,
		B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fMenuBar)
		.Add(fTipsterView)
		.Add(dragger)
		.AddGlue();
}


bool
MainWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}


void
MainWindow::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
		case SHOW_ABOUT:
		{
			BAboutWindow* about = new BAboutWindow("Tipster",
				"application/x-vnd.tipster");
			about->AddDescription("An application to show usability tips "
						"for Haiku");
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
		case DELAY_30_S:
		{
			fTipsterView->SetDelay(30000000);
			break;
		}
		case DELAY_1_M:
		{
			fTipsterView->SetDelay(60000000);
			break;
		}
		case DELAY_2_M:
		{
			fTipsterView->SetDelay(120000000);
			break;
		}
		case DELAY_5_M:
		{
			fTipsterView->SetDelay(300000000);
			break;
		}
		default:
			BWindow::MessageReceived(msg);
			break;
	}
}

/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "MainWindow.h"

#include <Application.h>
#include <Bitmap.h>
#include <ControlLook.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <IconUtils.h>
#include <LayoutBuilder.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Messenger.h>
#include <Path.h>
#include <Resources.h>
#include <TranslationUtils.h>

#include <private/interface/AboutWindow.h>

#include <stdio.h>

enum
{
	SHOW_ABOUT = 'swat',
	UPDATE_ICON = 'upin',
	OPEN_URL = 'opur',
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

	fURL = new BString();
	fResources = BApplication::AppResources();
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

	//fTipsterViewContainer = new BView("tipster_container",
	//	B_SUPPORTS_LAYOUT);

	fTipsterView = new Tipster();
	//fTipsterViewContainer->AddChild(fTipsterView);

	BGroupLayout* layout = new BGroupLayout(B_VERTICAL);
	layout->SetInsets(10,0,10,0);

	fTipsterView->SetLayout(layout);
	//fTipsterViewContainer->SetLayout(layout);

	fIcon = new BButton("icon", "", new BMessage(OPEN_URL));
	fIcon->SetFlat(true);

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fMenuBar)
		.AddGroup(B_HORIZONTAL)
			.Add(fIcon)
			//.Add(fTipsterViewContainer)
			.Add(fTipsterView)
		.End()
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
		case UPDATE_ICON:
		{
			status_t status = msg->FindString("url", fURL);

			if (status == B_OK) {
				BString artwork;
				msg->FindString("artwork", &artwork);

				size_t size;
				const uint8* iconData = (const uint8*)
					fResources->LoadResource('VICN', artwork.String(),
					&size);

				if (size > 0) {
					fIconBitmap = new BBitmap(BRect(0, 0, 64, 64), 0,
						B_RGBA32);

					status_t iconStatus = BIconUtils::GetVectorIcon(
						iconData, size, fIconBitmap);

					if (iconStatus == B_OK)
						fIcon->SetIcon(fIconBitmap);
				}
			}
			break;
		}
		case OPEN_URL:
		{
			fTipsterView->OpenURL(fURL);
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

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
#include <stdio.h>
#include <TranslationUtils.h>

#include <private/interface/AboutWindow.h>

enum
{
	SHOW_ABOUT = 'swat',
	UPDATE_ICON = 'upin',
	OPEN_URL = 'opur'
};


MainWindow::MainWindow(void)
	:
	BWindow(BRect(100,100,900,200), "Tipster", B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS)
{
	BuildLayout();
	
	fURL = new BString();
	fResources = BApplication::AppResources();
}


void
MainWindow::BuildLayout(void)
{	
	BMenuBar* fMenuBar = new BMenuBar("menubar");
	BMenu* fTipsterMenu = new BMenu("Tipster");
	
	fTipsterMenu->AddItem(new BMenuItem("About", new BMessage(SHOW_ABOUT)));
	
	fMenuBar->AddItem(fTipsterMenu);
	
	fTipsterViewContainer = new BView("tipster_container",
		B_SUPPORTS_LAYOUT);
	
	fTipsterView = new Tipster();
	fTipsterViewContainer->AddChild(fTipsterView);
	
	BGroupLayout* layout = new BGroupLayout(B_VERTICAL);
	layout->SetInsets(10,0,10,0);
	
	fTipsterViewContainer->SetLayout(layout);
	
	fIcon = new BButton("icon", "", new BMessage(OPEN_URL));
	fIcon->SetFlat(true);
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fMenuBar)
		.AddGroup(B_HORIZONTAL)
			.Add(fIcon)
			.Add(fTipsterViewContainer)
		.End()
		.AddGlue();
}


bool
MainWindow::QuitRequested(void)
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
			about->AddDescription("An application to show usability tips \
for Haiku");
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
					
					if (iconStatus == B_OK) {
						fIcon->SetIcon(fIconBitmap);
						
						delete iconData;
					} else
						printf("Icon did not load properly\n");
				} else
					printf("Icon did not load properly\n");
			}
			break;
		}
		case OPEN_URL:
		{
			fTipsterView->OpenURL(fURL);	
			break;
		}
		default:
			BWindow::MessageReceived(msg);
			break;
	}
}

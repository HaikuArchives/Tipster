/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "MainWindow.h"

#include <Application.h>
#include <ControlLook.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <LayoutBuilder.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Messenger.h>
#include <Path.h>
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
	
	url = new BString("");
}


void
MainWindow::BuildLayout(void)
{	
	BMenuBar* fMenuBar = new BMenuBar("menubar");
	BMenu* fTipsterMenu = new BMenu("Tipster");
	
	fTipsterMenu->AddItem(new BMenuItem("About", new BMessage(SHOW_ABOUT),
		'A', B_COMMAND_KEY));
	
	fMenuBar->AddItem(fTipsterMenu);
	
	fTipsterViewContainer = new BView("tipster_container",
		B_SUPPORTS_LAYOUT);
	
	fTipsterView = new Tipster();
	fTipsterViewContainer->AddChild(fTipsterView);
	
	BGroupLayout* layout = new BGroupLayout(B_VERTICAL);
	layout->SetInsets(10,0,10,0);
	
	fTipsterViewContainer->SetLayout(layout);
	
	icon = new BButton("icon", "", new BMessage(OPEN_URL));
	icon->SetFlat(true);
	
	static const float spacing = be_control_look->DefaultItemSpacing() / 2;
	fMainSplitView = 
		BLayoutBuilder::Split<>(B_HORIZONTAL)
			.AddGroup(B_VERTICAL)
				.Add(icon)
			.End()
			.AddGroup(B_VERTICAL)
				.Add(fTipsterViewContainer)
			.End()
		.View();
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fMenuBar)
		.Add(fMainSplitView);
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
			status_t status = msg->FindString("url", url);
			
			if (status == B_OK) {
				BString artwork("");
				
				msg->FindString("artwork", &artwork);
				
				BPath path("artwork");
				path.Append(artwork);
				
				icon_bitmap = BTranslationUtils::GetBitmap(path.Path());
				
				icon->SetIcon(icon_bitmap);
			}
			
			break;
		}
		case OPEN_URL:
		{
			fTipsterView->OpenURL(url);
			
			break;
		}
		
		default:
			BWindow::MessageReceived(msg);
			break;
	}
}

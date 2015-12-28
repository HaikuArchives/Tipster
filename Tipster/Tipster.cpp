/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "Tipster.h"

#include <Application.h>
#include <Catalog.h>
#include <ControlLook.h>
#include <Directory.h>
#include <Entry.h>
#include <File.h>
#include <FindDirectory.h>
#include <Messenger.h>
#include <Path.h>
#include <PathFinder.h>
#include <Roster.h>
#include <stdio.h>
#include <stdlib.h>
#include <StringList.h>
#include <TranslationUtils.h>

enum
{
	M_UPDATE_TIP = 'uptp',
	M_CHECK_TIME = 'cktm',
	UPDATE_ICON = 'upin'
};


Tipster::Tipster()
	:
	BTextView("TipView")
{
	MakeEditable(false);
	SetStylable(true);
	
	fTipsList = BStringList();
	
	SetText("");
}


bool
Tipster::QuitRequested(void)
{
	return true;
}


void
Tipster::AttachedToWindow()
{
	BMessage message(M_CHECK_TIME);
	fRunner = new BMessageRunner(this, &message, 1000000);
	
	BFont linkfont(be_plain_font);
	linkfont.SetFace(B_UNDERSCORE_FACE);
	
	linkStyle.count = 1;
	linkStyle.runs[0].offset = 0;
	linkStyle.runs[0].font = linkfont;
	linkStyle.runs[0].color = make_color(0, 0, 255);

	messenger = new BMessenger(this->Parent());
	UpdateTip();

	BTextView::AttachedToWindow();
}


void
Tipster::MessageReceived(BMessage* msg)
{
	switch (msg->what)
	{
		case M_CHECK_TIME:
		{
			if (fTime + 60000000 < system_time())
			{
				//Update the tip every 60 seconds
				
				UpdateTip();
			}
			break;
		}
		case M_UPDATE_TIP:
		{
			UpdateTip();
			break;
		}
		default:
		{
			BTextView::MessageReceived(msg);
			break;
		}
	}
}


//Written by PulkoMandy
void
Tipster::OpenURL(BString* url)
{
	char *argv[2];
	argv[0] = (char*)url->String();
	argv[1] = 0;

	status_t status = be_roster->Launch(
		"application/x-vnd.Be.URL.http", 1, argv);
}


void
Tipster::MouseDown(BPoint pt)
{
	BPoint temp;
	uint32 buttons;
	GetMouse(&temp, &buttons);
	
	if (Bounds().Contains(temp)) {
		if (buttons == 1) {
			//1 = left mouse button
			UpdateTip();
		}
	}
}


void
Tipster::UpdateTip()
{
	if (fTipsList.IsEmpty()) {
		entry_ref ref = GetTipsFile();
		LoadTips(ref);
	}
	
	SetText("");
	fTipNumber = random() % fTipsList.CountStrings();
	
	BStringList fUpdatedList;
	fTipsList.StringAt(fTipNumber).Split("\n", false, fUpdatedList);
	fUpdatedList.Remove(0);
	
	BString link = fUpdatedList.StringAt(2);

	Insert(fUpdatedList.StringAt(1));
	
	fTipsList.Remove(fTipNumber);
	
	BMessage message(UPDATE_ICON);
	message.AddString("url", link);
	message.AddString("artwork",
		GetArtworkTitle(fUpdatedList.StringAt(0)));
	messenger->SendMessage(&message);
	
	fTime = system_time();
	
	static const float spacing = be_control_look->DefaultLabelSpacing();
	TruncateString(&fUpdatedList.StringAt(1), B_TRUNCATE_END,
		Frame().Width() - 16 - spacing * 4);
}


entry_ref
Tipster::GetTipsFile()
{
	entry_ref ref;
	BStringList paths;
	
	status_t status = BPathFinder::FindPaths(B_FIND_PATH_DATA_DIRECTORY,
		"tipster-tips.txt", B_FIND_PATH_EXISTING_ONLY, paths);
	
	if (!paths.IsEmpty() && status == B_OK) {
		for (int32 i = 0; i < paths.CountStrings(); i++) {
			BEntry data_entry(paths.StringAt(i).String());
			data_entry.GetRef(&ref);
		
			return ref;
		}
	}

	BEntry entry("tipster-tips.txt");
	entry.GetRef(&ref);

	return ref;
}


void
Tipster::LoadTips(entry_ref ref)
{
	BFile file(&ref, B_READ_ONLY);
	if (file.InitCheck() != B_OK)
		return;
	
	fTipsList.MakeEmpty();
	
	BString fTips;
	off_t size = 0;
	file.GetSize(&size);
	
	char* buf = fTips.LockBuffer(size);
	file.Read(buf, size);
	fTips.UnlockBuffer(size);

	fTips.Split("\n%\n", false, fTipsList);
}


const char * 
Tipster::GetArtworkTitle(BString category)
{
	if (category == "GUI")
		return "Misc_Deskbar_Group";
	else if (category == "Terminal")
		return "App_Terminal";
	else if (category == "Preferences")
		return "App_Deskbar";
	else if (category == "Application")
		return "App_Generic_2";
	else if (category == "Miscellaneous")
		return "Alert_Idea";
}

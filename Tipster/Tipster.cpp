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
#include <StringList.h>
#include <TranslationUtils.h>

#include <stdio.h>
#include <stdlib.h>

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
	fCurrentTip = new BString("");
	fDelay = 60000000;

	SetText("");
}


void
Tipster::SetDelay(bigtime_t delay)
{
	fDelay = delay;
	fTime = system_time();
}


bool
Tipster::QuitRequested()
{
	return true;
}


void
Tipster::AttachedToWindow()
{
	BMessage message(M_CHECK_TIME);
	fRunner = new BMessageRunner(this, message, 1000000);
	fMessenger = new BMessenger(this->Parent());

	AddBeginningTip();

	BTextView::AttachedToWindow();
}


void
Tipster::AddBeginningTip()
{
	entry_ref ref = GetTipsFile();
	LoadTips(ref);

	BStringList introductionTipList;
	fTipsList.StringAt(0).Split("\n", false, introductionTipList);
	BString link = introductionTipList.StringAt(2);

	Insert(introductionTipList.StringAt(1));

	BString additionalTip("%\n");
	additionalTip.Append(introductionTipList.StringAt(0).String());
	additionalTip.Append("\n");
	additionalTip.Append(introductionTipList.StringAt(1).String());
	additionalTip.Append("\n");
	additionalTip.Append(link);

	fTipsList.Remove(0);

	BMessage message(UPDATE_ICON);
	message.AddString("url", link);
	message.AddString("artwork",
		GetArtworkTitle(introductionTipList.StringAt(0)));
	fMessenger->SendMessage(&message);

	fTime = system_time();
}


void
Tipster::MessageReceived(BMessage* msg)
{
	switch (msg->what)
	{
		case M_CHECK_TIME:
		{
			if (fTime + fDelay < system_time())
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
		if (buttons == 1)	// 1 = left mouse button
			UpdateTip();
	}
}


void
Tipster::UpdateTip()
{
	if (fTipsList.IsEmpty()) {
		entry_ref ref = GetTipsFile();
		LoadTips(ref);

		fTipsList.Remove(0);
	}

	fPreviousTip = new BString(fCurrentTip->String());

	SetText("");
	fTipNumber = random() % fTipsList.CountStrings();

	BStringList tipInfoList;
	fTipsList.StringAt(fTipNumber).Split("\n", false, tipInfoList);
	tipInfoList.Remove(0);

	BString link = tipInfoList.StringAt(2);

	Insert(tipInfoList.StringAt(1));

	fCurrentTip = new BString(fTipsList.StringAt(fTipNumber));
	fTipsList.Remove(fTipNumber);

	BMessage message(UPDATE_ICON);
	message.AddString("url", link);
	message.AddString("artwork",
		GetArtworkTitle(tipInfoList.StringAt(0)));
	fMessenger->SendMessage(&message);

	fTime = system_time();
}


void
Tipster::DisplayPreviousTip()
{
	if (fPreviousTip != NULL) {
		BStringList tipInfoList;
		BString(fPreviousTip->String()).Split("\n", false, tipInfoList);
		tipInfoList.Remove(0);

		BString link = tipInfoList.StringAt(2);

		SetText("");
		Insert(tipInfoList.StringAt(1));

		BMessage message(UPDATE_ICON);
		message.AddString("url", link);
		message.AddString("artwork",
			GetArtworkTitle(tipInfoList.StringAt(0)));
		fMessenger->SendMessage(&message);

		fTime = system_time();
	}
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
		return "GUI";
	else if (category == "Terminal")
		return "Terminal";
	else if (category == "Preferences")
		return "Preferences";
	else if (category == "Application")
		return "Application";

	return "Miscellaneous";
}

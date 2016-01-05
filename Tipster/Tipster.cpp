/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "Tipster.h"

#include <Application.h>
#include <Bitmap.h>
#include <Catalog.h>
#include <ControlLook.h>
#include <Directory.h>
#include <Dragger.h>
#include <Entry.h>
#include <File.h>
#include <FindDirectory.h>
#include <GroupLayout.h>
#include <IconUtils.h>
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
	OPEN_URL = 'opur',
	M_UPDATE_TIP = 'uptp',
	M_CHECK_TIME = 'cktm',
	UPDATE_ICON = 'upin'
};


Tipster::Tipster()
	:
	BGroupView("TipView")
{
	fTipsList = BStringList();
	fCurrentTip = new BString("");
	fDelay = 10000000;
	fReplicated = false;
	fURL = new BString("");
	fArtworkTitle = new BString("");

	fTipsterTextView = new TipsterText();
	fIcon = new BButton("icon", "", new BMessage(OPEN_URL));
	fIcon->SetFlat(true);

	AddChild(fIcon);
	AddChild(fTipsterTextView);

	BRect rect(Bounds());
	rect.top = rect.bottom - 7;
	rect.left = rect.right - 7;
	
	BDragger* dragger = new BDragger(rect, this,
		B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	AddChild(dragger);
}


Tipster::Tipster(BMessage* archive)
	:
	BGroupView(archive)
{
	fReplicated = true;
	fTipsList = BStringList();
	fIconBitmap = new BBitmap(BRect(0, 0, 64, 64), 0, B_RGBA32);
	fCurrentTip = new BString("");
	fURL = new BString("");
	fArtworkTitle = new BString("");
	fDelay = 10000000;

	if (archive->FindString("Tipster::text", fCurrentTip) != B_OK)
		printf("error finding text...\n");

	if (archive->FindInt64("Tipster::delay", fDelay) != B_OK)
		printf("error finding delay...\n");

	if (archive->FindString("Tipster::url", fURL) != B_OK)
		printf("error finding url...\n");

	if (archive->FindString("Tipster::artwork", fArtworkTitle) != B_OK)
		printf("error finding artwork...\n");

	fTipsterTextView = 
		dynamic_cast<TipsterText*>(BGroupView::FindView("TipsterTextView"));
	fIcon = dynamic_cast<BButton*>(BGroupView::FindView("icon"));
}


status_t
Tipster::Archive(BMessage* data, bool deep) const
{
	status_t status = BGroupView::Archive(data, deep);
	if (status != B_OK) {
		printf("Could not archive\n");

		return status;
	}

	status = data->AddString("add_on", "application/x-vnd.tipster");
	if (status != B_OK) {
		printf("Could not add APP_SIG\n");

		return status;
	}

	status = data->AddInt64("Tipster::delay", fDelay);
	if (status != B_OK) {
		printf("Could not save the delay\n");

		return status;
	}

	status = data->AddString("Tipster::url", fURL->String());
	if (status != B_OK) {
		printf("Could not save the url\n");

		return status;
	}

	status = data->AddString("Tipster::text", fCurrentTip->String());
	if (status != B_OK) {
		printf("Could not save the current tip's text\n");

		return status;
	}

	status = data->AddString("Tipster::artwork", fArtworkTitle->String());
	if (status != B_OK) {
		printf("Could not save the artwork title\n");

		return status;
	}

	data->AddString("class", "Tipster");

	return B_OK;
}


BArchivable*
Tipster::Instantiate(BMessage *data)
{
	return new Tipster(data);
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
	fResources = BApplication::AppResources();

	if (!fReplicated) {
		fIcon->SetTarget(this);
		
		AddBeginningTip();
	} else {
		UpdateIcon(fArtworkTitle->String(), fURL->String());
		DisplayTip(fCurrentTip);
	}
	
	BGroupView::AttachedToWindow();
}


void
Tipster::AddBeginningTip()
{
	entry_ref ref = GetTipsFile();
	LoadTips(ref);

	BStringList introductionTipList;
	fTipsList.StringAt(0).Split("\n", false, introductionTipList);
	BString link = introductionTipList.StringAt(2);

	fTipsterTextView->Insert(introductionTipList.StringAt(1));

	fTipsList.Remove(0);

	GetArtworkTitle(introductionTipList.StringAt(0));
	UpdateIcon(BString(fArtworkTitle->String()), link);

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
		case OPEN_URL:
		{
			printf("IN OPENURL");

			OpenURL(fURL);
			break;
		}
		case M_UPDATE_TIP:
		{
			UpdateTip();
			break;
		}
		default:
		{
			BView::MessageReceived(msg);
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
Tipster::UpdateIcon(BString artwork, BString url)
{
	size_t size;
	const uint8* iconData = (const uint8*)
		fResources->LoadResource('VICN', artwork.String(), &size);

	if (size > 0) {
		fIconBitmap = new BBitmap(BRect(0, 0, 64, 64), 0, B_RGBA32);

		status_t iconStatus = BIconUtils::GetVectorIcon(
			iconData, size, fIconBitmap);

		if (iconStatus == B_OK)
			fIcon->SetIcon(fIconBitmap);
	}

	fURL = new BString(url.String());
}


void
Tipster::UpdateTip()
{
	if (fTipsList.IsEmpty()) {
		printf("fTipsList is empty...Getting tips\n");
		entry_ref ref = GetTipsFile();
		LoadTips(ref);

		fTipsList.Remove(0);
	}

	printf("Getting tip & displaying\n");
	
	printf("String list size: %i\n", fTipsList.CountStrings());
	
	fTipNumber = random() % fTipsList.CountStrings();

	DisplayTip(new BString(fTipsList.StringAt(fTipNumber)));
	
	printf("Displayed tips...\n");

	fPreviousTip = new BString(fCurrentTip->String());
	fCurrentTip = new BString(fTipsList.StringAt(fTipNumber));
	fTipsList.Remove(fTipNumber);
	
	printf("Updating tip information...\n");

	fTime = system_time();
	
	printf("Got time...\n");
}


void
Tipster::DisplayTip(BString* tip)
{
	BStringList tipInfoList;
	BString(tip->String()).Split("\n", false, tipInfoList);
	tipInfoList.Remove(0);

	BString link = tipInfoList.StringAt(2);

	fTipsterTextView->SetText("");
	fTipsterTextView->Insert(tipInfoList.StringAt(1));

	GetArtworkTitle(tipInfoList.StringAt(0));
	UpdateIcon(BString(fArtworkTitle->String()), link);
}


void
Tipster::DisplayPreviousTip()
{
	if (fPreviousTip != NULL) {
		DisplayTip(fPreviousTip);

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


void
Tipster::GetArtworkTitle(BString category)
{
	if (category == "GUI")
		fArtworkTitle->SetTo("GUI");
	else if (category == "Terminal")
		fArtworkTitle->SetTo("Terminal");
	else if (category == "Preferences")
		fArtworkTitle->SetTo("Preferences");
	else if (category == "Application")
		fArtworkTitle->SetTo("Application");
	else
		fArtworkTitle->SetTo("Miscellaneous");
}

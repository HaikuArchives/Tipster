/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "AboutTipster.h"
#include "Tipster.h"
#include "Shuffle.h"

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
#include <LayoutBuilder.h>
#include <LocaleRoster.h>
#include <Messenger.h>
#include <Path.h>
#include <PathFinder.h>
#include <Roster.h>
#include <StringList.h>
#include <TranslationUtils.h>

#include <private/interface/AboutWindow.h>

#include <stdio.h>
#include <stdlib.h>


enum
{
	OPEN_URL = 'opur',
	UPDATE_TIP = 'uptp',
	CHECK_TIME = 'cktm',
	UPDATE_ICON = 'upin',
	SAVE_SETTINGS = 'svse'
};

const float kDraggerSize = 7.0f;


Tipster::Tipster()
	:
	BGroupView("Tipster")
{
	fTipsList = BStringList();
	fTipsLength = 0;
	fTipIndex = -1;
	fCurrentTip = new BString("");
	fDelay = 60000000;
	fReplicated = false;
	fURL = new BString("");
	fArtworkTitle = new BString("");
	fPreviousTip = new BString("");

	fTipsterTextView = new TipsterText();
	fIcon = new BButton("iconview", "", new BMessage(OPEN_URL));
	fIcon->SetFlat(true);

	BDragger* dragger = new BDragger(this);

	BLayoutBuilder::Group<>(this, B_HORIZONTAL, 0)
			.SetInsets(-2, -2, 0, -2)
			.Add(fIcon)
			.AddStrut(be_control_look->DefaultItemSpacing())
			.AddGroup(B_VERTICAL,0)
				.Add(fTipsterTextView)
				.AddGroup(B_HORIZONTAL, 0)
					.AddGlue()
					.Add(dragger)
					.End()
				.End()
		.End();
	dragger->SetExplicitMinSize(BSize(kDraggerSize, kDraggerSize));
	dragger->SetExplicitMaxSize(BSize(kDraggerSize, kDraggerSize));
	_LoadSettings();
}


Tipster::Tipster(BMessage* archive)
	:
	BGroupView(archive)
{
	fReplicated = true;
	fTipsList = BStringList();
	fIconBitmap = new BBitmap(BRect(0, 0, 64, 64), 0, B_RGBA32);
	fCurrentTip = new BString("");
	fPreviousTip = new BString("");
	fURL = new BString("");
	fArtworkTitle = new BString("");
	fDelay = 60000000;

	if (archive->FindString("Tipster::text", fCurrentTip) != B_OK)
		printf("error finding text...\n");

	if (archive->FindInt64("Tipster::delay", fDelay) != B_OK)
		printf("error finding delay...\n");

	if (archive->FindString("Tipster::url", fURL) != B_OK)
		printf("error finding url...\n");

	if (archive->FindString("Tipster::artwork", fArtworkTitle) != B_OK)
		printf("error finding artwork...\n");
}


Tipster::~Tipster()
{
	delete fPreviousTip;
	delete fCurrentTip;

	delete fTipsterTextView;
	delete fIcon;
	delete fURL;
	delete fIconBitmap;
	delete fArtworkTitle;

	delete fResources;

	delete fRunner;
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
	if (!validate_instantiation(data, "Tipster"))
		return NULL;

	return new Tipster(data);
}


void
Tipster::SetDelay(bigtime_t delay)
{
	fDelay = delay;
	fTime = system_time();

	_SaveSettings();
}


bool
Tipster::QuitRequested()
{
	return true;
}


void
Tipster::AttachedToWindow()
{
	BMessage message(CHECK_TIME);
	fRunner = new BMessageRunner(this, message, 1000000);
	fResources = new BResources();
	fResources->SetToImage((void *)&Tipster::Instantiate);

	if (!fReplicated) {
		fIcon->SetTarget(this);

		AddBeginningTip();
	} else {
		fTipsterTextView =
			static_cast<TipsterText*>(BGroupView::FindView("TipsterTextView"));
		fIcon = static_cast<BButton*>(BGroupView::FindView("iconview"));
		fIcon->SetTarget(this);
		fIcon->SetFlat(true);
		UpdateIcon(fArtworkTitle->String(), fURL->String());
		//DisplayTip(fCurrentTip);
		AddBeginningTip();
	}

	BGroupView::AttachedToWindow();
}

status_t
Tipster::_SaveSettings()
{
	BPath path;
	BFile file;
	BMessage message(SAVE_SETTINGS);

	if (find_directory(B_USER_SETTINGS_DIRECTORY, &path) != B_OK)
		return B_ERROR;
	path.Append("Tipster");

	file.SetTo(path.Path(), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
	if (file.InitCheck() != B_OK)
		return B_ERROR;

	status_t status = message.AddInt64("delay", fDelay);
	if (status != B_OK)
		return B_ERROR;

	if (message.Flatten(&file) != B_OK)
		return B_ERROR;

	return B_OK;
}


status_t
Tipster::_LoadSettings()
{
	BPath path;
	BFile file;
	BMessage message(SAVE_SETTINGS);

	if (find_directory(B_USER_SETTINGS_DIRECTORY, &path) != B_OK)
		return B_ERROR;
	path.Append("Tipster");

	file.SetTo(path.Path(), B_READ_ONLY);
	if (file.InitCheck() != B_OK)
		return B_ERROR;

	if (message.Unflatten(&file) != B_OK)
		return B_ERROR;

	if (message.FindInt64("delay", &fDelay) != B_OK)
		return B_ERROR;

	fTime = system_time();

	return B_OK;
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
	fTipsLength = fTipsList.CountStrings();

	CreateRandomSeq(fRandomSeq1, fTipsLength);
	CreateRandomSeq(fRandomSeq2, fTipsLength);

	GetArtworkTitle(introductionTipList.StringAt(0));
	UpdateIcon(BString(fArtworkTitle->String()), link);

	fTime = system_time();
}


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "About"

void
Tipster::MessageReceived(BMessage* message)
{
	switch (message->what)
	{
		case CHECK_TIME:
		{
			if (fTime + fDelay < system_time()) {
				//Update the tip every 60 seconds
				UpdateTip();
			}
			break;
		}
		case OPEN_URL:
		{
			OpenURL(fURL);
			break;
		}
		case UPDATE_TIP:
		{
			UpdateTip();
			break;
		}
		case B_ABOUT_REQUESTED:
		{
			BAboutWindow* about = new AboutTipster();
			about->Show();
			break;
		}
		default:
		{
			BView::MessageReceived(message);
			break;
		}
	}
}


//Written by PulkoMandy
void
Tipster::OpenURL(BString* url)
{
	if (url->IsEmpty())
		return;

	char *argv[2];
	argv[0] = (char*)url->String();
	argv[1] = 0;

	status_t status = be_roster->Launch(
		"application/x-vnd.Be.URL.http", 1, argv);
}


void
Tipster::MouseDown(BPoint point)
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
	fTipIndex++;
	if (fTipIndex == 2 * fTipsLength) {
		fRandomSeq1 = fRandomSeq2;
		CreateRandomSeq(fRandomSeq2, fTipsLength);
		fTipIndex = fTipsLength;
	}

	if (fTipIndex >= fTipsLength)
		DisplayTip(new BString(fTipsList.StringAt(fRandomSeq2[fTipIndex % fTipsLength])));
	else
		DisplayTip(new BString(fTipsList.StringAt(fRandomSeq1[fTipIndex])));
	fTime = system_time();
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
	if (fTipIndex-1 != -1) {
		fTipIndex--;
		if (fTipIndex >= fTipsLength)
			DisplayTip(new BString(fTipsList.StringAt(fRandomSeq2[fTipIndex % fTipsLength])));
		else
			DisplayTip(new BString(fTipsList.StringAt(fRandomSeq1[fTipIndex])));

		fTime = system_time();
	}
}


static bool
getLocalTipsFile(entry_ref &ref, const char *language = "en")
{
	BStringList paths;
	BString localTipsFile("Tipster/tips-");

	printf("Preferred language(s): %s\n", language);

	localTipsFile += language;
	localTipsFile += ".txt";

	status_t status = BPathFinder::FindPaths(B_FIND_PATH_DATA_DIRECTORY,
		localTipsFile, B_FIND_PATH_EXISTING_ONLY, paths);

	if (!paths.IsEmpty() && status == B_OK) {
		BEntry data_entry(paths.StringAt(0).String());
		data_entry.GetRef(&ref);

		return true;
	}

	BEntry entry(localTipsFile);
	entry.GetRef(&ref);

	return entry.Exists();
}


entry_ref
Tipster::GetTipsFile()
{
	entry_ref ref;
	BMessage message;
	BLocaleRoster *roster = BLocaleRoster::Default();

	if (roster->GetPreferredLanguages(&message) == B_OK) {
		const char *language;

		for (int32 i = 0; (language = message.GetString("language", i, NULL)) != NULL; i++)
			if (getLocalTipsFile(ref, language))
				return ref;
	}
	getLocalTipsFile(ref);

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

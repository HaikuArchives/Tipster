/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "Tipster.h"

#include <Application.h>
#include <Catalog.h>
#include <Directory.h>
#include <Entry.h>
#include <File.h>
#include <FindDirectory.h>
#include <Path.h>
#include <PathFinder.h>
#include <stdio.h>
#include <stdlib.h>
#include <StringList.h>
#include <TranslationUtils.h>

enum
{
	M_UPDATE_TIP = 'uptp',
	M_CHECK_TIME = 'cktm'
};


Tipster::Tipster()
	:
	BTextView("TipView")
{
	fTipsList = BStringList();
	
	SetText("");
	UpdateTip();
	
	MakeEditable(false);
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
	entry_ref ref = GetTipsFile();
	LoadTips(ref);
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
	fTipNumber = random() % fTipsList.CountStrings();
	
	BStringList fUpdatedList;
	fTipsList.StringAt(fTipNumber).Split("\n", false, fUpdatedList);
	fUpdatedList.Remove(0);
	
	BString text(fUpdatedList.Join("\n"));
	SetText(text.String());
	
	fTime = system_time();
}

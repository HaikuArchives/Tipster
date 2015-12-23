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


Tipster::Tipster(BRect frame)
	: BTextView(frame, "TipView", B_FOLLOW_ALL, B_WILL_DRAW)
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
Tipster::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case M_CHECK_TIME:
		{
			if (time + 60000000 < system_time())
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


void Tipster::MouseDown(BPoint pt)
{
	BPoint temp;
	uint32 buttons;
	GetMouse(&temp, &buttons);
	
	if (Bounds().Contains(temp))
	{
		if (buttons == 1)
		{
			//1 = left mouse button
			UpdateTip();
		}
	}
}


void
Tipster::UpdateTip(void)
{
	entry_ref ref = GetTipsFile();
	LoadTips(ref);
}


entry_ref
Tipster::GetTipsFile(void)
{
	BStringList paths;
	status_t status = BPathFinder::FindPaths(B_FIND_PATH_DATA_DIRECTORY,
		"tipster-tips.txt", paths);
	
	if (paths.IsEmpty() || status != B_OK)
	{	
		BEntry entry("tipster-tips.txt");
		entry_ref ref;
		entry.GetRef(&ref);

		return ref;
	}
	
	BPath fullPath;
	for (int32 i = 0; i < paths.CountStrings(); i++)
	{
		BDirectory directory(paths.StringAt(i).String());
		if (directory.InitCheck() != B_OK)
			continue;
		
		fullPath = paths.StringAt(i).String();
		fullPath.Append("tipster-tips.txt");
		
		BEntry entry(fullPath.Path());
		entry_ref ref;
		entry.GetRef(&ref);
		
		return ref;
	}
	
	BEntry entry("tipster-tips.txt");
	entry_ref ref;
	entry.GetRef(&ref);

	return ref;
}


void
Tipster::LoadTips(entry_ref ref)
{
	BFile file(&ref, B_READ_ONLY);
	if (file.InitCheck() != B_OK)
		return;
	
	BString fTips;
	off_t size = 0;
	file.GetSize(&size);
	
	char *buf = fTips.LockBuffer(size);
	file.Read(buf, size);
	fTips.UnlockBuffer(size);
	
	fTips.Split("\n", true, fTipsList);
	fTipNumber = (random() % (fTipsList.CountStrings() / 4)) * 4;
	
	BString *text = new BString(fTipsList.StringAt(fTipNumber));
	text->Append("\n");
	text->Append(fTipsList.StringAt(fTipNumber + 1));
	text->Append("\n");
	text->Append(fTipsList.StringAt(fTipNumber + 2));
	SetText(text->String());
	
	time = system_time();
}

/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "Tipster.h"

#include <Entry.h>
#include <File.h>
#include <stdio.h>
#include <stdlib.h>
#include <TranslationUtils.h>


Tipster::Tipster(BRect frame)
	: BTextView(frame, "TipView", B_FOLLOW_ALL, B_WILL_DRAW)
{
	fTipsList = BStringList();
	
	SetText("TEST");
	MakeEditable(false);
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
	
	BEntry entry("tips.txt");
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
}

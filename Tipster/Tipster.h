/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef TIPSTER_H
#define TIPSTER_H

#include "TipsterText.h"

#include <Button.h>
#include <MessageRunner.h>
#include <Resources.h>
#include <String.h>
#include <StringList.h>
#include <TextView.h>
#include <GroupView.h>


class Tipster : public BGroupView
{
public:
	Tipster();
	Tipster(BMessage* archive);

	bool			QuitRequested();

	void 			MouseDown(BPoint pt);
	void 			MessageReceived(BMessage* msg);
	void 			AttachedToWindow();

	void			OpenURL(BString* url);

	void			SetDelay(bigtime_t delay);
	void			UpdateTip();
	void			DisplayPreviousTip();

	static			BArchivable* Instantiate(BMessage* data);
	status_t		Archive(BMessage* data, bool deep = true) const;

private:
	void			LoadTips(entry_ref ref);
	void			AddBeginningTip();
	void			DisplayTip(BString* tip);
	void			UpdateIcon(BString artwork, BString url);

	entry_ref		GetTipsFile();
	void			GetArtworkTitle(BString category);

	uint32			fTipNumber;
	BStringList 	fTipsList;
	int32			fTipsLength;

	BString*		fRandomTip;
	BString*		fPreviousTip;
	BString*		fCurrentTip;

	TipsterText*	fTipsterTextView;
	BButton*		fIcon;
	BString*		fURL;
	BBitmap*		fIconBitmap;
	BString*		fArtworkTitle;

	BResources*		fResources;

	bigtime_t		fTime;
	bigtime_t		fDelay;
	BMessageRunner* fRunner;

	bool			fReplicated;
};

#endif

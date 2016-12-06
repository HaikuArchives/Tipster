/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef TIPSTER_H
#define TIPSTER_H

#include "TipsterText.h"

#include <Button.h>
#include <GroupView.h>
#include <MessageRunner.h>
#include <Resources.h>
#include <String.h>
#include <StringList.h>
#include <TextView.h>


class Tipster : public BGroupView
{
public:
					Tipster();
					Tipster(BMessage* archive);
					~Tipster();

	bool			QuitRequested();

	void 			MouseDown(BPoint point);
	void 			MessageReceived(BMessage* msg);
	void 			AttachedToWindow();

	void			OpenURL(BString* url);

	void			SetDelay(bigtime_t delay);
	void			UpdateTip();
	void			DisplayPreviousTip();
	int				tipindex;

	static			BArchivable* Instantiate(BMessage* data);
	status_t		Archive(BMessage* data, bool deep = true) const;

private:
	void			LoadTips(entry_ref ref);
	void			AddBeginningTip();
	void			DisplayTip(BString* tip);
	void			UpdateIcon(BString artwork, BString url);

	entry_ref		GetTipsFile();
	void			GetArtworkTitle(BString category);

	status_t		_SaveSettings();
	status_t		_LoadSettings();

	uint32			fTipNumber;
	BStringList 	fTipsList;
	int32			fTipsLength;

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

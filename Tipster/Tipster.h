/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef TIPSTER_H
#define TIPSTER_H

#include <MessageRunner.h>
#include <String.h>
#include <StringList.h>
#include <TextView.h>
#include <View.h>


class Tipster : public BView
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
	void			_Init(BMessage* settings);
	
	BTextView*		fTipsterTextView;

	entry_ref		GetTipsFile();
	const char*		GetArtworkTitle(BString category);

	uint32			fTipNumber;
	BStringList 	fTipsList;
	int32			fTipsLength;

	BString*		fRandomTip;
	BString*		fPreviousTip;
	BString*		fCurrentTip;

	bigtime_t		fTime;
	bigtime_t		fDelay;
	BMessageRunner* fRunner;
	BMessenger*		fMessenger;

	bool			fReplicated;
};

#endif

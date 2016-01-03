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


class Tipster : public BTextView
{
public:
	Tipster();

	bool			QuitRequested();

	void 			MouseDown(BPoint pt);
	void 			MessageReceived(BMessage* msg);
	void 			AttachedToWindow();

	void			OpenURL(BString* url);

	void			SetDelay(bigtime_t delay);
	void			UpdateTip();
	void			DisplayPreviousTip();

private:
	void			LoadTips(entry_ref ref);
	void			AddBeginningTip();
	void			DisplayTip(BString* tip);
	void			_Init();

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

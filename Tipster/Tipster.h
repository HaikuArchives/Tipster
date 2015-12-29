/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef TIPSTER_H
#define TIPSTER_H

#include <MessageRunner.h>
#include <TextView.h>
#include <String.h>
#include <StringList.h>


class Tipster : public BTextView
{
	public:
		Tipster();

		bool			QuitRequested();

		void 			MouseDown(BPoint pt);
		void 			MessageReceived(BMessage* msg);
		void 			AttachedToWindow();

		void			OpenURL(BString* url);

	private:		
		void			UpdateTip();
		void			LoadTips(entry_ref ref);
		void			AddBeginningTip();

		entry_ref		GetTipsFile();
		const char*		GetArtworkTitle(BString category);

		uint32			fTipNumber;
		BStringList 	fTipsList;
		int32			fTipsLength;

		BString*		fRandomTip;

		bigtime_t		fTime;
		BMessageRunner* fRunner;

		BMessenger*		fMessenger;
};

#endif

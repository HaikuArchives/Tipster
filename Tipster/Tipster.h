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
		Tipster(BRect frame);
		
		void MouseDown(BPoint pt);
		void MessageReceived(BMessage *msg);
		void AttachedToWindow();
	
	private:
		void UpdateTip(void);
		void LoadTips(entry_ref ref);

		entry_ref GetTipsFile(void);

		uint32 fTipNumber;
		BStringList fTipsList;
		
		BString *fRandomTip;
		
		bigtime_t time;
		BMessageRunner *fRunner;
};

#endif

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
		Tipster(void);
		
		bool QuitRequested(void);
		
		void MouseDown(BPoint pt);
		void MessageReceived(BMessage *msg);
		void AttachedToWindow();
	
	private:
		struct tLink {
			const int32 linkoffset;
			const int32 linklen;
			const BString target;
			
			tLink(int32 off, int32 len, BString trg)
				:
				linkoffset(off),
				linklen(len),
				target(trg)
			{
			}
		};
		
		void UpdateTip(void);
		void LoadTips(entry_ref ref);
		void OpenURL(BString url);
		
		entry_ref GetTipsFile(void);
		tLink* GetLinkAt(BPoint point);
		
		BList links;
		BList fTipsUsed;

		uint32 fTipNumber;
		BStringList fTipsList;
		int32 fTipsLength;
		
		BString *fRandomTip;
		
		bigtime_t fTime;
		BMessageRunner *fRunner;
		
		static text_run_array linkStyle;
};

#endif

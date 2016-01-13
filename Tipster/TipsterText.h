/*
 * Copyright 2015 Your Name <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef TIPSTER_TEXT_H
#define TIPSTER_TEXT_H

#include <Messenger.h>
#include <TextView.h>


class TipsterText : public BTextView
{
public:
	TipsterText();
	TipsterText(BMessage* data);

	void			MouseDown(BPoint pt);
	void 			AttachedToWindow();
	
	static			BArchivable* Instantiate(BMessage* data);
	
private:
	BMessenger*		fMessenger;
};


#endif // _H

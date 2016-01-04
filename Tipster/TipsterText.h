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

	void			MouseDown(BPoint pt);
	void 			AttachedToWindow();
	
private:
	BMessenger*		fMessenger;
};


#endif // _H

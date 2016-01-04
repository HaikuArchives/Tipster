/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "TipsterText.h"

#include <Messenger.h>

#include <stdio.h>
#include <stdlib.h>

enum
{
	M_UPDATE_TIP = 'uptp'
};


TipsterText::TipsterText()
	:
	BTextView("TipsterTextView")
{
	SetText("");
	
	MakeEditable(false);
	SetStylable(true);
}


void
TipsterText::AttachedToWindow()
{
	fMessenger = new BMessenger(this->Parent());
	
	SetViewColor(Parent()->ViewColor());

	BTextView::AttachedToWindow();
}


void
TipsterText::MouseDown(BPoint pt)
{
	BPoint temp;
	uint32 buttons;
	GetMouse(&temp, &buttons);

	if (Bounds().Contains(temp)) {
		BMessage message(M_UPDATE_TIP);
		
		fMessenger->SendMessage(&message);
	}
}

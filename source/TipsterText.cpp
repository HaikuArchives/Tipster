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
	UPDATE_TIP = 'uptp',
	NEXT_TIP = 'nxtp',
	PREVIOUS_TIP = 'pvtp'
};


TipsterText::TipsterText()
	:
	BTextView("TipsterTextView")
{
	SetText("");

	MakeEditable(false);
	SetStylable(true);
}


TipsterText::TipsterText(BMessage* data)
	:
	BTextView(data)
{
	SetText("");

	MakeEditable(false);
	SetStylable(true);
}


BArchivable*
TipsterText::Instantiate(BMessage *data)
{
	if (!validate_instantiation(data, "TipsterText")) {
		printf("Could not complete instantiation...\n");

		return NULL;
	}

	printf("Instantiation validated...\n");

	return new TipsterText(data);
}


void
TipsterText::AttachedToWindow()
{
	fMessenger = new BMessenger(this->Parent());

	SetViewColor(Parent()->ViewColor());

	BTextView::AttachedToWindow();
}


void
TipsterText::MouseDown(BPoint point)
{
	BPoint temp;
	uint32 buttons;
	GetMouse(&temp, &buttons);

	if (Bounds().Contains(temp)) {
		if (buttons == 1)  //left mouse button
			fMessenger->SendMessage(NEXT_TIP);
		if (buttons == 2) //right mouse button
			fMessenger->SendMessage(PREVIOUS_TIP);

	}
}

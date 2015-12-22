/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "Tipster.h"

Tipster::Tipster(BRect frame)
	: BTextView(frame, "TipView", B_FOLLOW_ALL, B_WILL_DRAW)
{
	SetText("TEST");
	
	MakeEditable(false);
}

void Tipster::UpdateTip(void)
{
}

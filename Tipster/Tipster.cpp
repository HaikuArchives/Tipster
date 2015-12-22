/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "Tipster.h"

#include <stdio.h>

Tipster::Tipster(BRect frame)
	: BTextView(frame, "TipView", B_FOLLOW_ALL, B_WILL_DRAW)
{
	SetText("TEST");
	
	MakeEditable(false);
}

void Tipster::MouseDown(BPoint pt)
{
	BPoint temp;
	uint32 buttons;
	GetMouse(&temp, &buttons);
	
	if (Bounds().Contains(temp))
	{
		//1 = left mouse button
		if (buttons == 1)
		{
			printf("Left button pressed\n");
			UpdateTip();
		}
	}
}

void Tipster::UpdateTip(void)
{
}

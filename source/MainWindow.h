/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Tipster.h"

#include <Window.h>


class MainWindow : public BWindow
{
public:
				MainWindow();

	bool		QuitRequested();
	void		MessageReceived(BMessage* message);

private:
	void		BuildLayout();

	Tipster*	fTipsterView;
};

#endif

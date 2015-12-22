/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "MainWindow.h"

MainWindow::MainWindow(void)
	: BWindow(BRect(100,100,500,500), "Tipster", B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS)
{
	BuildLayout();
}

MainWindow::BuildLayout(void)
{
}

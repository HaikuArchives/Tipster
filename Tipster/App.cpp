/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "MainWindow.h"

#include <Application.h>


class App : public BApplication
{
	public:
		App(void);
};

App::App(void)
	: BApplication("application/x-vnd.tipster")
{
	MainWindow *win = new MainWindow();
	
	win->Show();
}

int main(void)
{
	App app;
	app.Run();
	return 0;
}

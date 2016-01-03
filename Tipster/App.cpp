/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "MainWindow.h"

#include <Application.h>


class App : public BApplication
{
	public:
		App();
};


App::App()
	:
	BApplication("application/x-vnd.tipster")
{
	MainWindow* win = new MainWindow();
	win->Show();
}


int
main()
{
	App app;
	app.Run();
	return 0;
}

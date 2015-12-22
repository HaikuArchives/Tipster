/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

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
	App app = new App();
	app.Run();
	return 0;
}

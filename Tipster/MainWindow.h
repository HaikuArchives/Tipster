/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "TipsterView.h"

#include <Window.h>
#include <View.h>


class MainWindow : public BWindow
{
	public:
		MainWindow(void);
		
	private:
		void BuildLayout(void);
		
		BView *f_tipsterViewContainer;

		TipsterView *f_tipsterView;
};

#endif

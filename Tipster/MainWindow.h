/*
 * Copyright 2015 Vale Tolpegin <valetolpegin@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Tipster.h"

#include <Button.h>
#include <Resources.h>
#include <SplitView.h>
#include <View.h>
#include <Window.h>


class MainWindow : public BWindow
{
	public:
		MainWindow(void);
		
		bool QuitRequested(void);
		void MessageReceived(BMessage* msg);

	private:
		void BuildLayout(void);
		
		BView* fTipsterViewContainer;
		BSplitView* fMainSplitView;

		Tipster* fTipsterView;
		
		BButton* fIcon;
		BString* fURL;
		BBitmap* fIconBitmap;
		
		BResources* fResources;
};

#endif

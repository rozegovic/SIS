//
//  Created by Izudin Dzafic on 16/12/2023.
//  Copyright © 2023 IDz. All rights reserved.
//
#pragma once
#include "Class.h"
#include <gui/TabView.h>

class DynTabView : public gui::TabView
{
private:
	Default _backgroundView;
	CanvasView _userBackView;

protected:
public:
	DynTabView()
		: gui::TabView(gui::TabHeader::Type::FitToText, 8, 64)
	{
		//setBackgroundView(&_backgroundView);
		//setBackgroundView(&_userBackView);

	}

	void reset() {
		//_backgroundView.disable();
		setBackgroundView(&_userBackView);
	};
};

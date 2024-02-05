//
//  Created by Izudin Dzafic on 16/12/2023.
//  Copyright © 2023 IDz. All rights reserved.
//
#pragma once

#include "BackView.h"
#include "CanvasNotifications.h"
#include "CanvasETF.h"
#include <gui/TabView.h>
#include <gui/SplitterLayout.h>
#include "Globals.h"

class DynTabView : public gui::TabView
{
private:
	BackView _backgroundView;
	Notifications _notificationsView;
	ETFBackground _etf;
	gui::SplitterLayout _splitter;

protected:
public:
    DynTabView()
	: gui::TabView(gui::TabHeader::Type::FitToText, 8, 64)
	, _splitter(gui::SplitterLayout::Orientation::Horizontal, gui::SplitterLayout::AuxiliaryCell::Second)

    {
		/*if (Globals::_currentUserID == 6)
		{
			setBackgroundView(&_etf);
		}
		else
		{*/
			_splitter.setContent(_backgroundView, _notificationsView);
			setLayout(&_splitter);
		//}
    }
};

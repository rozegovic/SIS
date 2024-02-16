#pragma once

#include "BackView.h"
#include "CanvasNotifications.h"
#include "CanvasETF.h"
#include <gui/TabView.h>
#include <gui/SplitterLayout.h>
#include "Globals.h"

class CanvasView : public gui::View
{
private:
	BackView _backgroundView;
	Notifications _notificationsView;
	ETFBackground _etf;
	gui::SplitterLayout _splitter;

protected:
public:
	CanvasView()
		: _splitter(gui::SplitterLayout::Orientation::Horizontal, gui::SplitterLayout::AuxiliaryCell::Second)
	{
		_splitter.setContent(_backgroundView, _notificationsView);    // pokusaj i ovo implementirati preko funkcija 
		setLayout(&_splitter);
	}
	void reset() {

		_backgroundView.reset();
		_notificationsView.getView().reset();


		//_splitter.disable();    // da li ista radi???
	}
};


class Default : public gui::View
{
private:
protected:
	gui::Image _etf;
	gui::ImageView _imgView;
	gui::VerticalLayout _vl;
public:
	Default()
		: _etf(":ETF")
		, _vl(3)
	{
		_vl.appendSpacer();
		_imgView.setImage(&_etf);
		_vl.append(_imgView);
		_vl.appendSpacer();
		setLayout(&_vl);
	}
};
#pragma once
#include <gui/View.h>
#include <gui/VerticalLayout.h>
#include <gui/Image.h>
#include <gui/ImageView.h>

//-----------------------Klasa za grupu 2
//-----------------------Predmeti za studenta
//-------------------izbrisati sliku etf (tu samo da se nesto prikazuje)!!!

class SubjectShow : public gui::Canvas
{
private:
protected:
    gui::Image _etf;

public:
    SubjectShow() : _etf(":ETF")
    {

    }

    void onDraw(const gui::Rect& rect) override {
        gui::Size sz;
        getSize(sz);
        gui::Point cp(sz.width / 2, sz.height / 2);
        td::INT4 x = cp.x;
        td::INT4 y = cp.y;

        gui::Rect imgRect(x - 15 - x / 4, y - 15 - y / 4, x + 15 + x / 4, y + 15 + y / 4);
        _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center); //no


    };
};
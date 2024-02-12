#pragma once
#include <gui/View.h>
#include <gui/VerticalLayout.h>
#include <gui/Image.h>
#include <gui/ImageView.h>
#include <gui/DrawableString.h>
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>
#include <dp/IDataSetDelegate.h>
#include "Globals.h"
#include <vector>

//-----------------------Klasa za grupu 2
//-----------------------Predmeti za studenta
//-------------------izbrisati sliku etf (tu samo da se nesto prikazuje)!!!

class SubjectShow : public gui::Canvas
{
private:
protected:
    gui::Image _etf;
    std::vector<gui::DrawableString> _subjects;

public:
    bool createStrings() {
        dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select a.Naziv_Predmeta AS Naziv FROM Predmet a, UpisPredmeta b WHERE b.ID_Studenta = ? AND  b.ID_Predmeta = a.ID_Predmeta");
        dp::Params pParams(pSelect->allocParams());
        pParams << Globals::_currentUserID;
        dp::Columns pCols = pSelect->allocBindColumns(1);
        td::String name;
        pCols << "Naziv" << name;
        if (!pSelect->execute())
            return false;

        while (pSelect->moveNext()) {
            _subjects.push_back(name);
        }
            return false;
    }
    SubjectShow() : _etf(":ETF")
    {
        createStrings();
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
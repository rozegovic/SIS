#pragma once
#include <gui/View.h>
#include <gui/Image.h>
#include <gui/ImageView.h>
#include <gui/SplitterLayout.h>

//-------------------Klasa za obavjestenja (sve grupe rade zajedno)
//-------------------dodati provjeru ko je prijavljen u onDraw i sa tim dalje prikazati obavjestenja
//-------------------izbrisati sliku etf (tu samo da se nesto prikazuje)!!!
class Notifications : public gui::Canvas
{
private:
protected:
    gui::Image _etf;

public:
    Notifications()
        : _etf(":ETF")
    {
 
    }

    void onDraw(const gui::Rect& rect) override {
        gui::Size sz;
        getSize(sz);

        //problem je sto se ne mogu skrolat poruke vec treba promijeniti size prozora da se sve vidi 
        //dodati provjeru da se ne ispisuje obavijest za ispit ako je rok za prijavu istog istekao
       
            dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT a.Subject, a.Poruke "
                "  FROM Messages a "
              "  JOIN MsgReceivers mr ON mr.MsgID = a.ID "
             "   WHERE mr.UserID = ? ");
            dp::Params parDS(pSelect->allocParams());
            parDS << Globals::_currentUserID;
            dp::Columns pCols = pSelect->allocBindColumns(2);
            td::String Subject, Poruke;
            pCols << "Subject" << Subject << "Poruke" << Poruke;
            if (!pSelect->execute()) {
                Subject= "Greska";
                Poruke = "Greska";
            }
            gui::Rect imgRect(0, 0, sz.width, 60);
            gui::Point pt(10, 0);
            gui::Point pt2(10, 30);
            while (pSelect->moveNext())
            {
                gui::Shape::drawRect(imgRect, td::ColorID::LightGray, td::ColorID::MidnightBlue, 2, td::LinePattern::Solid);
                gui::DrawableString tekst = Subject;
                gui::DrawableString tekst2 = Poruke;
                tekst.draw(pt, gui::Font::ID::SystemLargerBold, td::ColorID::Navy);
                tekst2.draw(pt2, gui::Font::ID::SystemNormal, td::ColorID::Navy);
                pt.translate(0, 70);
                pt2.translate(0, 70);
                imgRect.translate(0, 70);

            }

    };
};
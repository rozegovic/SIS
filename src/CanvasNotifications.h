#pragma once
#include <gui/View.h>
#include <gui/Image.h>
#include <gui/ImageView.h>
#include <gui/ViewScroller.h>
#include <gui/SplitterLayout.h>
#include "GlobalsCanvas.h"


//-------------------Klasa za obavjestenja (sve grupe rade zajedno)
//-------------------dodati provjeru ko je prijavljen u onDraw i sa tim dalje prikazati obavjestenja
//-------------------izbrisati sliku etf (tu samo da se nesto prikazuje)!!!
class Notifications1 : public gui::Canvas
{
private:
protected:
    gui::Image _etf;

    friend class Notifications;
public:
    Notifications1()
        : Canvas({ gui::InputDevice::Event::CursorDrag })
        , _etf(":ETF")
    {

    }

    void onDraw(const gui::Rect& rect) override {
        if (Globals::_currentUserID == 1 || Globals::_currentUserID == 2 || Globals::_currentUserID == 3 || Globals::_currentUserID == 4 || Globals::_currentUserID == 5 || Globals::_currentUserID == 6) {
            gui::Size sz;
            getSize(sz);


            //problem je sto se ne mogu skrolat poruke vec treba promijeniti size prozora da se sve vidi --> RJESENO
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
                Subject = "Greska";
                Poruke = "Greska";
            }
            gui::Rect imgRect(0, 0, sz.width, 60);
            gui::Point pt(10, 0);
            gui::Point pt2(10, 30);


            gui::Rect rectt(0, 0, sz.width, 60);//

            gui::Shape rrect;//

            GlobalsCanvas::brObavijesti = 0;

            while (pSelect->moveNext())
            {

                //gui::Shape::drawRect(imgRect, td::ColorID::LightGray, td::ColorID::MidnightBlue, 2, td::LinePattern::Solid);

                rrect.createRoundedRect(rectt, 20);//
                rrect.drawFill(td::ColorID::Silver);//

                gui::DrawableString tekst = Subject;
                gui::DrawableString tekst2 = Poruke;
                /*  tekst.draw(pt, gui::Font::ID::SystemLargerBold, td::ColorID::Navy);
                  tekst2.draw(pt2, gui::Font::ID::SystemNormal, td::ColorID::Navy);*/
                tekst.draw(pt, gui::Font::ID::SystemLargerBold, td::ColorID::Black);//
                tekst2.draw(pt2, gui::Font::ID::SystemNormal, td::ColorID::Black);//
                pt.translate(0, 70);
                pt2.translate(0, 70);
                //imgRect.translate(0, 70);
                rectt.translate(0, 70);//

                GlobalsCanvas::brObavijesti++;
            }

            if (GlobalsCanvas::brObavijesti == 0)
            {

                gui::DrawableString str = tr("noNotifs");//

                rrect.createRoundedRect(rectt, 20);//
                rrect.drawFill(td::ColorID::Silver);//

                pt.translate(sz.width / 2 - 75, 20);

                str.draw(pt, gui::Font::ID::SystemLargerBold, td::ColorID::Black);//

            }
        }
    };
    void reset() {
        reDraw();
    };

    bool getModelSize(gui::Size& modelSize) const override
    {
        modelSize.width = 1300;
        modelSize.height = GlobalsCanvas::brObavijesti * 70;
        //showAlert("", std::to_string(brObavijesti));
        return true;
    };
    void measure(gui::CellInfo& ci) override
    {
        gui::Size sz;
        getSize(sz);
        ci.minHor = 230;
        /*        ci.minVer = 1000;
                ci.nResHor = 0;
                ci.nResVer = 0;*/
    }
    void reMeasure(gui::CellInfo& ci) override
    {
        gui::Size sz;
        getSize(sz);
    }
};




class Notifications : public gui::ViewScroller
{
private:
protected:
    Notifications1 _canvas;
protected:

public:
    Notifications()
        : gui::ViewScroller(gui::ViewScroller::Type::NoScroll, gui::ViewScroller::Type::ScrollerAlwaysVisible)
    {
        setContentView(&_canvas);
    }

    Notifications1& getView()
    {
        return _canvas;
    }
    
};


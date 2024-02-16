#pragma once
#include <gui/View.h>
#include <gui/Image.h>
#include <gui/ImageView.h>
#include <gui/SplitterLayout.h>
#include "MiddleCanvas.h"

//-------------------izbrisati sliku etf (tu samo da se nesto prikazuje)!!!
class FarLeftCanvas : public gui::Canvas
{
private:
protected:
    gui::Image _etf;
    MiddleCanvas* _middleCanvas;
    gui::DrawableString _str;
    gui::DrawableString _request;
    td::INT4 _status;

    int i;
    gui::Shape _shapeCircle1;
    std::vector<std::pair<td::String, td::INT4>> users;

    td::INT4 _brojChat; // varijabla koja definise broj pravougaonika
    td::INT4 _visinaChata;  // varijabla d iz gui::Rect imgRect(a,b,c,d)
public:
    FarLeftCanvas( MiddleCanvas* canvas)
        : Canvas({ gui::InputDevice::Event::PrimaryClicks })
        ,_etf(":ETF")
        , _brojChat(3)
        , _visinaChata(100)
    {
        _middleCanvas = canvas;
    }

    void onDraw(const gui::Rect& rect) override {
        const bool check = false;
        // pogled za profesora i asistenta ------ grupa 3
        if (Globals::isProfessor || Globals::isAssistant)
            return;




        // pogled za SAO ------ grupa 1
        else if (Globals::isSAO) {
            gui::Size sz;
            getSize(sz);

            //CITANJE IMENA IZ BAZE
            dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT k.Ime , k.Prezime FROM Korisnici k, SAOStudentTicket p WHERE p.Indeks=k.Indeks;");
            dp::Columns pCols = pSelect->allocBindColumns(2);
            td::String name,sname;
            pCols << "Ime" << name << "Prezime" << sname;
            pSelect->execute();


            int i = 0;

            //CITANJE PORUKE IZ BAZE

            dp::IStatementPtr pSelect1 = dp::getMainDatabase()->createStatement("SELECT Req_Title from SAOStudentTicket;");
            dp::Columns pCols1 = pSelect1->allocBindColumns(1);
            td::String request;
            pCols1 << "Req_Title" << request;
            pSelect1->execute();

            //DA LI JE OBRADJENO

            dp::IStatementPtr pSelect2 = dp::getMainDatabase()->createStatement("SELECT Status_ID from SAOStudentTicket;");
            dp::Columns pCols2 = pSelect2->allocBindColumns(1);
            td::INT4 status;
            pCols2 << "Status_ID" << status;
            pSelect2->execute();


            gui::Rect imgRect(0, 0, sz.width, 100);

            while (pSelect->moveNext() && pSelect1->moveNext() && pSelect2->moveNext())
            {
                name.append(" ");
                name.append(sname);
                _str = name;
                _request = request;
                _status = status;


                if (_status == 1)
                {
                    gui::Shape::drawRect(imgRect, td::ColorID::LightGray, td::ColorID::Red, 4, td::LinePattern::Solid);

                }
                else
                {
                    gui::Shape::drawRect(imgRect, td::ColorID::LightGray, td::ColorID::Green, 4, td::LinePattern::Solid);
                }

                gui::Point pt(i+5, i * 110);
                // gui::Rect textRect(15, 0, sz.width, 100);
                _str.draw(pt, gui::Font::ID::SystemBold, td::ColorID::Black);


                gui::Point pt1(i+5, 30 + i * 110);
              //  gui::Rect textRect1(15, 0, sz.width, 100);
                _request.draw(pt1, gui::Font::ID::SystemNormal, td::ColorID::Black);

                i++;

                imgRect.translate(0, 110);

            }

        }



            // pogled za studenta ------ grupa 2
        else if (Globals::_currentUserID == 5) {
            gui::Size sz;
            getSize(sz);
            gui::Point cp(sz.width / 2, sz.height / 2);
            td::INT4 x = cp.x;
            td::INT4 y = cp.y;

            gui::Rect imgRect(x - 15 - x / 4, y - 15 - y / 4, x + 15 + x / 4, y + 15 + y / 4);
            _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center);
        }
        // kada nema ulogovane osobe
       /* else if(Globals::isLogged==check){

        }*/
        else {

        }
        
    };

    // ideja za implementaciju otvaranja chata
    // treba dodati funkciju za klik
    // vjerovatno treba slati neki parametar i napraviti novu funkciju u middlecanvasu
    //void openMiddleCanvas() {
    //    _middleCanvas->reset();
    //};
   /* void measure(CellInfo& ci) 
    {
        gui::Size imgSize;
        _image.getSize(imgSize);
        ci.minHor = imgSize.width;
        ci.minVer = imgSize.height;
        ci.nResHor = 0;
        ci.nResVer = 0;
    }
    void reMeasure(CellInfo& ci) 
    {
        gui::Size imgSize;
        _image.getSize(imgSize);
        ci.minHor = imgSize.width;
        ci.minVer = imgSize.height;
        ci.nResHor = 0;
        ci.nResVer = 0;
    }*/


    void onPrimaryButtonPressed(const gui::InputDevice& InputDevice) override{

        if (Globals::isSAO)
        {

            gui::Size sz;
            getSize(sz);
  
            const gui::Point& klik = InputDevice.getFramePoint();

            if (klik.x > sz.width + 5)
                return;

            td::INT4 rbrPoruke = klik.y;

            rbrPoruke = rbrPoruke / 100;

            //showAlert("", std::to_string(rbrPoruke));

            _middleCanvas->SetMessageNumSAO(rbrPoruke);

        }
    }


    void reset() {
        reDraw();
    };

    bool getModelSize(gui::Size& modelSize) const override
    {
        modelSize.width = 1300;
        modelSize.height = 550;
        return true;
    }

};
#pragma once
#include "MainWindow.h"
#include <gui/View.h>
#include <gui/Image.h>
#include <gui/ImageView.h>
#include <gui/SplitterLayout.h>
#include "MiddleCanvas.h"
#include <vector>
#include <gui/DrawableString.h>
#include <gui/Window.h>


//-------------------izbrisati sliku etf (tu samo da se nesto prikazuje)!!!
class FarLeftCanvas : public gui::Canvas
{
private:
protected:
    gui::Image _etf;
    MiddleCanvas* _middleCanvas;
    
    //Grupa 2
    gui::Shape _rect;
    td::INT4 _noOfSubjects;
    std::vector<td::INT4> subjects;
   
  
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

    
    //Grupa2
    //bool createStrings() {
    //    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select a.Naziv_Predmeta AS Naziv FROM Predmet a, UpisPredmeta b WHERE b.ID_Studenta = ? AND  b.ID_Predmeta = a.ID_Predmeta");
    //    dp::Params pParams(pSelect->allocParams());
    //    pParams << Globals::_currentUserID;
    //    dp::Columns pCols = pSelect->allocBindColumns(1);
    //    td::String name;
    //   // gui::DrawableString drawableName;
    //    pCols << "Naziv" << name;
    //    if (!pSelect->execute())
    //        return false;
    //    _subjects.resize(0);
    //    while (pSelect->moveNext()) {
    //        gui::DrawableString drawableName = name;
    //        _subjects.push_back(drawableName);
    //    }
    //        return false;
    //}
    void onDraw(const gui::Rect& rect) override {
        const bool check = false;
        // pogled za profesora i asistenta ------ grupa 3
        if (Globals::_currentUserID == 1 || Globals::_currentUserID == 3) {
            gui::Size sz;
            getSize(sz);
            //gui::Point cp(sz.width / 2, sz.height / 2);
            //td::INT4 x = cp.x;
            //td::INT4 y = cp.y

            //gui::Rect imgRect(x - 15 - x / 4, y - 15 - y / 4, x + 15 + x / 4, y + 15 + y / 4);
            //_etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center); //no

            //--------------------------------------naredna linija zakomentarisana
            //_pDS = dp::getMainDatabase()->createDataSet("SELECT Korisnici.ID as IDUser, Korisnici.Ime as nameUser, Korisnici.Prezime as surnameUser, Pozicija.ID as positionID, Pozicija.Naziv as roleUser, Korisnici.JMBG as jmbgUser, Korisnici.DatumUpisa as dateEUser, Korisnici.Adresa as addressUser, Korisnici.DatumRodjenja as dateBUser, Korisnici.Indeks as indeksUser FROM Korisnici, Pozicija WHERE Korisnici.PozicijaID = Pozicija.ID AND Korisnici.ID>0", dp::IDataSet::Execution::EX_MULT);
            //ovo treba da se popuni unutrasnjost pravougaonika imenom.

            
            gui::Rect imgRect(0, 0, sz.width, 100);
            _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Left); //no
            _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Right); //no
            gui::Shape::drawRect(imgRect, td::ColorID::Blue, 2, td::LinePattern::Dot);

            for (int i = 0; i < 3; i++) {
                imgRect.translate(0, 110);
                _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Left);
                gui::Shape::drawRect(imgRect, td::ColorID::Red, 5, td::LinePattern::DashEq);
            }
        }
        // pogled za SAO ------ grupa 1
        else if (Globals::_currentUserID == 4) {
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
            subjects.clear();
         //  createStrings();
            //std::vector<td::INT4> SubjFrames;// rbr predmeta
            //_noOfSubjects  = 0 ;
            gui::Size sz;
            getSize(sz);
            //createStrings();
            gui::Rect r(0, 0, sz.width, 50);
            float lw = 5;
           // gui::Rect imgRect(450, 50, 650, 150);
            /*for (int i = 0; i < _subjects.size(); i++) {
                gui::Shape::drawRect(r,td::ColorID::WhiteSmoke, td::ColorID::Navy, 4, td::LinePattern::Solid);
                _subjects[i].draw(pt, gui::Font::ID::SystemLargerBold, td::ColorID::Navy);
                pt.translate(0,50);
                r.translate(0, 50);
            }*/

            dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select a.Naziv_Predmeta AS Naziv, a.ID_Predmeta as ID FROM Predmet a, UpisPredmeta b WHERE b.ID_Studenta = ? AND  b.ID_Predmeta = a.ID_Predmeta");
            dp::Params pParams(pSelect->allocParams());
            pParams << Globals::_currentUserID;
            dp::Columns pCols = pSelect->allocBindColumns(2);
            td::String name;
            td::INT4 ID;
            // gui::DrawableString drawableName;
            pCols << "Naziv" << name << "ID" << ID;
            if (!pSelect->execute())
                return ;
            gui::Point pt(0, 25);
            while (pSelect->moveNext()) {
                gui::DrawableString subject = name;
                gui::Shape::drawRect(r, td::ColorID::DimGray, td::ColorID::Black, 0.5, td::LinePattern::Solid);
                subject.draw(r, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Gainsboro, td::TextAlignment::Center, td::VAlignment::Center);
                //pt.translate(0, 50);
                r.translate(0, 50);
                pt.translate(0,50);
                subjects.push_back(ID);

            }
            _middleCanvas->setSubjectID(0);    //
//            gui::Size sz;
//            getSize(sz);
//            gui::Point cp(sz.width / 2, sz.height / 2);
//            td::INT4 x = cp.x;
//            td::INT4 y = cp.y;
//
//            gui::Rect imgRect(x - 15 - x / 4, y - 15 - y / 4, x + 15 + x / 4, y + 15 + y / 4);
//            _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center);
            
        }
        // kada nema ulogovane osobe
       /* else if(Globals::isLogged==check){

        }*/
        else {

        }
    };
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override//zasto se ne poziva
    {
        gui::Size sz;
        getSize(sz);
        const gui::Point& modelPoint = inputDevice.getFramePoint();
        td::INT4 rbr = modelPoint.y/50; //visina svakog pravougaonika je 50, pa ce ovo vratiti rbr (pocevsi od 0)?
        //vektor subjects cuva sve IDs predmeta koji su ispisani, i to redom kako su ispisani. 
        // Da dobijete ID kliknutog predmeta ide subjects.at(rbr), kao u alertu ispod
        std::cout<<subjects.size();
        if(rbr > subjects.size())
            return;
        td::INT4 subjectID = subjects.at(rbr);
        _middleCanvas->setSubjectID(subjectID);
        
        //_middleCanvas->onDraw();
       // showAlert("", std::to_string(subjects.at(rbr)));
                
           
    }


    void reset() {
        if(Globals::isStudent){
            subjects.clear();
        }
        reDraw();
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
    
};

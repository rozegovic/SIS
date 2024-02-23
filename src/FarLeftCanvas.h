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



<<<<<<< Updated upstream
//-------------------izbrisati sliku etf (tu samo da se nesto prikazuje)!!!
=======
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
    td::INT4 _visinaChata;  // varijabla d iz gui::Rect imgRect(a,b,c,d)
=======
    td::INT4 _visinaChata;  // varijabla d iz gui::Rect imgRect(a,b,c,d
    
    //Grupa 2
    gui::Shape _rect;
    td::INT4 _noOfSubjects;
    std::vector<std::pair<td::String, td::INT4>> SubjFrames;//naziv predmeta + y koord

    td::INT4 numOfTickets;
    
>>>>>>> Stashed changes
public:
    FarLeftCanvas( MiddleCanvas* canvas)
        : Canvas({ gui::InputDevice::Event::PrimaryClicks })
        ,_etf(":ETF")
        , _brojChat(3)
        , _visinaChata(100)
<<<<<<< Updated upstream
    {
        _middleCanvas = canvas;
    }
=======
        ,numOfTickets(0)
        {
            _middleCanvas = canvas;
        }
>>>>>>> Stashed changes

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

            numOfTickets = 0;

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

                numOfTickets++;

            }
            
            GlobalsCanvas::visinaLeftSAO = i*100;

            sz.height = GlobalsCanvas::visinaLeftSAO;

            getScroller()->setContentSize(sz);

        }



            // pogled za studenta ------ grupa 2
        else if (Globals::_currentUserID == 5) {

    
    //Grupa 2
    gui::Shape _rect;
    td::INT4 _noOfSubjects;
    std::vector<std::pair<td::String, td::INT4>> SubjFrames;//naziv predmeta + y koord
   

<<<<<<< Updated upstream
    
    
public:
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        gui::Size sz;
        getSize(sz);
        const gui::Point& modelPoint = inputDevice.getModelPoint();
        std::vector<std::pair<gui::DrawableString, gui::Rect>> SubjRects;//stvori niz rectova za sve predmete
        for(int i = 0; i < SubjFrames.size(); i++){
            td::INT4 p2 = SubjFrames.at(i).second;
            gui::Rect temp(0, p2, sz.width, 50);
            SubjRects.at(i).second = temp;
            SubjRects.at(i).first = SubjFrames.at(i).first;
        }
        for(int i = 0; i < SubjRects.size(); i++){
            if(SubjRects.at(i).second.contains(modelPoint)){
               gui::DrawableString subject1 = SubjRects.at(i).first;
                gui::Window* a = getParentWindow();
                /*MainWindow* mw = a;//??
                 a->showTimeSlotView(5);//Funkcija iz MainWindow da otvori ovaj View???
                 
                 */
//                gui::Rect r1(0, 50 * _noOfSubjects, 20, 20);
//                gui::Shape::drawRect(r1, td::ColorID::WhiteSmoke, td::ColorID::Navy, 4, td::LinePattern::Solid);
//                subject1.draw(r1, gui::Font::ID::SystemLargerBold, td::ColorID::Navy);
                
                
            }
        }
    }
    FarLeftCanvas( MiddleCanvas* canvas)
        : _etf(":ETF")
     //   , _middleCanvas(canvas)
    {
       
        _middleCanvas = canvas;
        
        //Grupa 2
//        gui::Rect r(50, 50, 200, 150);
//        float lw = 5;
//        _rect.createRect(r, lw);
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
            gui::Point cp(sz.width / 2, sz.height / 2);
            td::INT4 x = cp.x;
            td::INT4 y = cp.y;

            gui::Rect imgRect(x - 15 - x / 4, y - 15 - y / 4, x + 15 + x / 4, y + 15 + y / 4);
            _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center);
        }
        // pogled za studenta ------ grupa 2
        else if (Globals::_currentUserID == 5) {
         //  createStrings();
            std::vector<std::pair<gui::DrawableString, td::INT4>> SubjFrames;//naziv predmeta + y koord
            _noOfSubjects  = 0 ;
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
            dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select a.Naziv_Predmeta AS Naziv FROM Predmet a, UpisPredmeta b WHERE b.ID_Studenta = ? AND  b.ID_Predmeta = a.ID_Predmeta");
            dp::Params pParams(pSelect->allocParams());
            pParams << Globals::_currentUserID;
            dp::Columns pCols = pSelect->allocBindColumns(1);
            td::String name;
            // gui::DrawableString drawableName;
            pCols << "Naziv" << name;
            if (!pSelect->execute())
                return ;
            while (pSelect->moveNext()) {
                gui::DrawableString subject = name;
                gui::Shape::drawRect(r, td::ColorID::WhiteSmoke, td::ColorID::Navy, 4, td::LinePattern::Solid);
                subject.draw(r, gui::Font::ID::SystemLargerBold, td::ColorID::Navy);
                //pt.translate(0, 50);
                r.translate(0, 50);
//                td::INT4 tempNo = 50*_noOfSubjects;
//                SubjFrames.push_back(std::pair<gui::DrawableString, td::INT4>(subject, tempNo));
//                _noOfSubjects++;
            }
            
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

    void reset() {
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

=======
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override {
        if(Globals::_currentUserID == 1 || Globals::_currentUserID == 3){
        
        double tempk = 0;

        int a = 0;
        td::INT4 IdUserChat;

        for (int i = 0; i < users.size(); i++) {

            // if (tempk < int(inputDevice.getFramePoint().y) && inputDevice.getFramePoint().y < (tempk + _visinaChata)) {
            if (tempk < int(inputDevice.getModelPoint().y) && inputDevice.getModelPoint().y < (tempk + _visinaChata)) {
                a = i;
                IdUserChat = users[i].second; 
                //IdUserChat = i; // ovo se poslije brise
                _middleCanvas->Reset(IdUserChat);
            }
            tempk = tempk + _visinaChata+10;
        }

        // openMiddleCanvas();
        if (inputDevice.getType() == gui::InputDevice::Type::Mouse && inputDevice.getButton() == gui::InputDevice::Button::Primary) {
            // openMiddleCanvas();
        }
        }
        else if (Globals::isSAO) {


                gui::Size sz;
                getSize(sz);

                const gui::Point& klik = inputDevice.getModelPoint();

                td::INT4 rbrPoruke = klik.y;

                rbrPoruke = rbrPoruke / 110;


                if (klik.x > sz.width + 5 || rbrPoruke>numOfTickets-1)
                    return;

                //showAlert("", std::to_string(rbrPoruke));

                _middleCanvas->SetMessageNumSAO(rbrPoruke);

            
>>>>>>> Stashed changes
        }
    }


    void reset() {
        reDraw();
    };

    bool getModelSize(gui::Size& modelSize) const override
    {
<<<<<<< Updated upstream
        modelSize.width = 1300;
        modelSize.height = 550;
=======

        if (Globals::isSAO)
        {
            modelSize.width = 230;
            modelSize.height = GlobalsCanvas::visinaLeftSAO;
            return true;
        }

        modelSize.width = 230;
        modelSize.height = (_brojChat) * 110;
>>>>>>> Stashed changes
        return true;
    }

};


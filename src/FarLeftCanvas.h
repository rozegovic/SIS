#pragma once

#include "MainWindow.h"

#include <gui/View.h>
#include <gui/Image.h>
#include <gui/ImageView.h>
#include <gui/SplitterLayout.h>
#include "MiddleCanvas.h"
#include <algorithm>
#include <gui/Label.h>
#include <vector>
#include <gui/DrawableString.h>
#include <gui/Window.h>

#include "GlobalsCanvas.h"
#include <utility>


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
    td::INT4 _visinaChata;  // varijabla d iz gui::Rect imgRect(a,b,c,d

    //Grupa 2
    gui::Shape _rect;
    td::INT4 _noOfSubjects;
    std::vector<std::pair<td::String, td::INT4>> SubjFrames;//naziv predmeta + y koord
    std::vector<td::INT4> subjects;
    std::vector<td::String> _subjectsName;


    td::INT4 numOfTickets;

    //---------------------------------
    std::vector<std::pair<td::INT4, td::INT4>> _Poruke;
    int BrojPozivaOnDraw=0;
    int kontrola = 0;
    dp::IDatabase* _db=nullptr;
    dp::IDataSetPtr _pDS;
    gui::Image img2;


  
    

public:
    FarLeftCanvas(MiddleCanvas* canvas)
        : _etf(":defaultUuser")
        , gui::Canvas({ gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::Keyboard })
        , _brojChat(50)  // --------------------------problem sa ovim 
        , _visinaChata(100)
        , numOfTickets(0)
        , _db(dp::getMainDatabase())
        , img2(":add")
    {
        _middleCanvas = canvas;
     
    }

    //Grupa2
    //bool createStrings() {
    //    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select a.Naziv_Predmeta AS Naziv, a.ID_Predmeta as ID FROM Predmet a, UpisPredmeta b WHERE b.ID_Studenta = ? AND  b.ID_Predmeta = a.ID_Predmeta");
    //    dp::Params pParams(pSelect->allocParams());
    //    pParams << Globals::_currentUserID;
    //    dp::Columns pCols = pSelect->allocBindColumns(2);
    //    td::String name;
    //    td::INT4 ID;
    //    // gui::DrawableString drawableName;
    //    pCols << "Naziv" << name << "ID" << ID;
    //    if (!pSelect->execute())
    //        return false;
    //    _subjectsName.resize(0);
    //    subjects.resize(0);
    //    while (pSelect->moveNext()) {
    //        //gui::DrawableString drawableName = name;
    //        _subjectsName.push_back(name);
    //        subjects.push_back(ID);

    //  
    //    }
    //    return true;
    //}


    

    
    //Grupa2
    bool createStrings() {
        dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select a.Naziv_Predmeta AS Naziv, a.ID_Predmeta as ID FROM Predmet a, UpisPredmeta b WHERE b.ID_Studenta = ? AND  b.ID_Predmeta = a.ID_Predmeta");        
        dp::Params pParams(pSelect->allocParams());
        pParams << Globals::_currentUserID;
        dp::Columns pCols = pSelect->allocBindColumns(2);        
        td::String name;
        td::INT4 ID;
       // gui::DrawableString drawableName;
        pCols << "Naziv" << name << "ID" << ID;
        if (!pSelect->execute())
            return false;
        _subjectsName.resize(0);
        subjects.resize(0);
        while (pSelect->moveNext()) {
            //gui::DrawableString drawableName = name;
            _subjectsName.push_back(name);
            subjects.push_back(ID);
        }
        return true;
    }
    void onDraw(const gui::Rect& rect) override {
        const bool check = false;
        // pogled za profesora i asistenta ------ grupa 3
        if ((Globals::_currentUserRole == 1 || Globals::_currentUserRole == 3)) {
            users.resize(0);
            gui::Size sz;
            getSize(sz);
            dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT ID, Prezime, Ime FROM Korisnici");
            dp::Columns pCols = pSelect->allocBindColumns(3);

            td::String username, userlastname;
            td::INT4 id;
            pCols << "ID" << id << "Ime" << username << "Prezime" << userlastname;
            if (!pSelect->execute()) {
                username = "Greska";
                userlastname = "Greska";
            }
            while (pSelect->moveNext()) {
                std::string fullname = userlastname.c_str();
                fullname += ' ';
                fullname += username.c_str();

                users.push_back(std::make_pair(fullname, id));
            }

            //brisanje sistema iz korisnika
            auto it = users.begin();
            while (it != users.end()) {
                if (it->second == (-1)) {
                    it = users.erase(it);
                    break;
                }
                else {
                    ++it;
                }
            }
            //abecedno sortiranje po prezimenu
           /* std::sort(users.begin(), users.end(), [](const auto& a, const auto& b) {
                return a.first < b.first;
                });*/
            std::sort(users.begin(), users.end(), [](const auto& a, const auto& b) {
                std::string lowerStr1, lowerStr2;
                td::String str1 = a.first, str2 = b.first;
                for (char c : str1) {
                    lowerStr1 += std::tolower(c);
                }
                for (char c : str2) {
                    lowerStr2 += std::tolower(c);
                }
                return lowerStr1 < lowerStr2;
                });

            _brojChat = users.size();
            std::sort(users.begin(), users.end(), [&](const auto& a, const auto& b) {
                if (a.second == Globals::_currentUserID)
                    return true;
                if (b.second == Globals::_currentUserID)
                    return false;
                return false;
                });

            gui::DrawableString _user;
            _user = users[0].first;
            //-------------INDIKATOR ZA PORUKE------------------
            dp::IStatementPtr pSelect1 = dp::getMainDatabase()->createStatement("select mr.UserID, mr.MsgID, m.AuthorID, m.Procitano from MsgReceivers mr, Messages m where mr.MsgID = m.id and m.Subject like 'chat' and mr.UserID = ?");
            dp::Columns pCols1 = pSelect1->allocBindColumns(4);
            dp::Params parDS1(pSelect1->allocParams());
            parDS1 << Globals::_currentUserID;

            td::INT4 idKoSalje, idKoPrima, idPoruke, procitano; // 0 procitana, 1 ne procitana poruka
            pCols1 << "UserID" << idKoPrima << "MsgID" << idPoruke << "AuthorID" << idKoSalje << "Procitano" << procitano;

            if (!pSelect1->execute()) {
                pSelect1 = nullptr;
            }
            _Poruke.resize(0);
            while (pSelect1->moveNext()) {
                if (procitano == 1){
                    _Poruke.push_back(std::make_pair(idKoPrima, idKoSalje));
                }
            }
            //-------------------------------------------------
            gui::Rect imgRect(0, 0, sz.width, _visinaChata);
            gui::Shape::drawRect(imgRect, td::ColorID::LightGray , td::ColorID::LightBlue, 5, td::LinePattern::Solid);
            _user.draw(imgRect, gui::Font::ID::SystemLargerBold, td::ColorID::Black, td::TextAlignment::Right, td::VAlignment::Center, td::TextEllipsize::End);//no
            _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Left);
            imgRect.translate(0, 110);
            /* _user.draw(imgRect, gui::Font::ID fntID, td::ColorID clrID, td::TextAlignment hAlign = td::TextAlignment::Left, td::VAlignment vAlign = td::VAlignment::Top, td::TextEllipsize ellips = td::TextEllipsize::End); *///no
            td::ColorID boja(td::ColorID::LightBlue);
            gui::Rect imgRect2(45, 45, 3, 3);
            imgRect2.translate(0, 110);
            //--------------------------------------------
            //************************************************************************************
                auto poruke = _Poruke;
                int pozicijaK = 1;
           
            //************************************************************************************
                //-------------------------------------------
                for (auto x : users) {
                    if (x.second == Globals::_currentUserID)continue;

                    if (static_cast<td::ColorID>(static_cast<int>(boja) + 1) == td::ColorID::White)
                        boja = td::ColorID::LightBlue;
                    boja = static_cast<td::ColorID>(static_cast<int>(boja) + 1);

                    td::String a = x.first;
                    gui::DrawableString others = a;
                    gui::Shape::drawRect(imgRect, td::ColorID::LightGray, boja, 5, td::LinePattern::Solid);
                    others.draw(imgRect, gui::Font::ID::SystemLarger, td::ColorID::Black, td::TextAlignment::Right, td::VAlignment::Center, td::TextEllipsize::End);
                    _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Left);
               
                    //-----------------------------------------------------
                    int w = poruke.size();

                    for (int i = 0; i < poruke.size(); i++) {
                        if (poruke[i].first == Globals::_currentUserID) {
                            int xx = poruke[i].second;
                            int yy = x.second;
                            gui::Circle c(150, 100 + _visinaChata * (pozicijaK), 10);
                            float lw = 10;
                            if (poruke[i].second == x.second) {
                              /*  _shapeCircle1.createCircle(c, lw, td::LinePattern::Solid);
                                _shapeCircle1.drawFill(td::ColorID::RoyalBlue);*/
                                gui::Shape::drawRect(imgRect2,td::ColorID::Transparent, 1, td::LinePattern::Solid);
                   
                               
                                img2.draw(imgRect2, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center);
                                
                            }
                        }
                        //else {
                        //    gui::Circle c(0, 0, 0);
                        //    float lw = 10;
                        //  /*  _shapeCircle1.createCircle(c, lw, td::LinePattern::Solid);
                        //    _shapeCircle1.drawFill(td::ColorID::RoyalBlue);*/
                        //  
                        // 
                        //    img2.draw(imgRect2, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center);
                        // 
                        //}
                    }

                    pozicijaK++;
                    //--------------------------------------------------------------
                    imgRect.translate(0, 110);
                    imgRect2.translate(0, 110);
                }
            getScroller()->setContentSize(sz);

        }

        // pogled za SAO ------ grupa 1
        else if (Globals::isSAO) {
            gui::Size sz;
            getSize(sz);

            //CITANJE IMENA IZ BAZE
            dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT k.Ime , k.Prezime FROM Korisnici k, SAOStudentTicket p WHERE p.Indeks=k.Indeks;");
            dp::Columns pCols = pSelect->allocBindColumns(2);
            td::String name, sname;
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

                gui::Point pt(i + 5, i * 110);
                // gui::Rect textRect(15, 0, sz.width, 100);
                _str.draw(pt, gui::Font::ID::SystemBold, td::ColorID::Black);


                gui::Point pt1(i + 5, 30 + i * 110);
                //  gui::Rect textRect1(15, 0, sz.width, 100);
                _request.draw(pt1, gui::Font::ID::SystemNormal, td::ColorID::Black);

                i++;

                imgRect.translate(0, 110);

                numOfTickets++;

            }

            GlobalsCanvas::visinaLeftSAO = i * 100;


            sz.height = GlobalsCanvas::visinaLeftSAO;

            getScroller()->setContentSize(sz);

        }



       
        // pogled za studenta ------ grupa 2
        else if (Globals::_currentUserID == 5) {
            // subjects.clear();
            createStrings();
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

             //dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select a.Naziv_Predmeta AS Naziv, a.ID_Predmeta as ID FROM Predmet a, UpisPredmeta b WHERE b.ID_Studenta = ? AND  b.ID_Predmeta = a.ID_Predmeta");
             //dp::Params pParams(pSelect->allocParams());
             //pParams << Globals::_currentUserID;
             //dp::Columns pCols = pSelect->allocBindColumns(2);
             //td::String name;
             //td::INT4 ID;
             //// gui::DrawableString drawableName;
             //pCols << "Naziv" << name << "ID" << ID;
             //if (!pSelect->execute())
             //    return ;
            gui::Point pt(0, 25);
            for (auto& name : _subjectsName) {
                gui::DrawableString subject = name;
                gui::Shape::drawRect(r, td::ColorID::DimGray, td::ColorID::Black, 0.5, td::LinePattern::Solid);
                subject.draw(r, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Gainsboro, td::TextAlignment::Center, td::VAlignment::Center);
                //pt.translate(0, 50);
                r.translate(0, 50);
                pt.translate(0, 50);

            }
            //_middleCanvas->setSubjectID(0);    //
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
        //if(Globals::isStudent){
        //    subjects.clear();
        //}
        //createStrings();

        kontrola = 1;
        reDraw();
    };

    // ideja za implementaciju otvaranja chata
    // treba dodati funkciju za klik
    // vjerovatno treba slati neki parametar i napraviti novu funkciju u middlecanvasu
    void openMiddleCanvas() {
        _middleCanvas->reset();
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
        ci.minHor = 100 + sz.width;
    }

    std::vector<std::pair<td::INT4, td::INT4>> accessMiddleCanvas(MiddleCanvas* canvas) {
        //sta ako je prazan?
       
    }


    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override {
        if (Globals::_currentUserRole == 1 || Globals::_currentUserRole == 3) {

            double tempk = 0;

            int a = 0;
            td::INT4 IdUserChat;

            for (int i = 0; i < users.size(); i++) {

                // if (tempk < int(inputDevice.getFramePoint().y) && inputDevice.getFramePoint().y < (tempk + _visinaChata)) {
                if (tempk < inputDevice.getModelPoint().y && inputDevice.getModelPoint().y < (tempk + _visinaChata)) {
                    a = i;
                    IdUserChat = users[i].second;
                    //IdUserChat = i; // ovo se poslije brise
                    _middleCanvas->Reset(IdUserChat, users[i].first);

                   //auto mess = accessMiddleCanvas(_middleCanvas);
                    td::INT4 firstfield = Globals::_currentUserID;
                    td::INT4 secondfield = IdUserChat;
                    td::INT4 idA = 0;
                    dp::IStatementPtr pUpdateGrade(_db->createStatement("UPDATE Messages SET Procitano=0 WHERE AuthorID=?"));
                    dp::Params pParams2(pUpdateGrade->allocParams());
                    pParams2 << IdUserChat;

                    if (!pUpdateGrade->execute())
                    {
                        return;
                    }
                    reset();

                   //brisemo iz paira sve poruke gdje je prvi field = currentuserid i gdje je drugi field = chatuserid

                     //OVDJE SE TREBA DODATI DA JE PORUKA PROCITANA, TJ NEKA NESTANE CRVENI KRUZIC UKOLIKO GA JE BILO I UKLONI SE PORUKA/E IZ RECIMO ONOG MULTIPAIRA STA VEC
                     //  KOJI CUVA SVE NAPROCITANE KADA SE POSALJU I KOME
                }
                tempk = tempk + _visinaChata + 10;
            }

            


             // openMiddleCanvas();
             if (inputDevice.getType() == gui::InputDevice::Type::Mouse && inputDevice.getButton() == gui::InputDevice::Button::Primary) {
                 // openMiddleCanvas();
             }
         }
         else if (Globals::isSAO) {


             reDraw();

             gui::Size sz;
             getSize(sz);

             const gui::Point& klik = inputDevice.getModelPoint();

             td::INT4 rbrPoruke = klik.y;

             rbrPoruke = rbrPoruke / 110;



             if (klik.x > sz.width + 5 || rbrPoruke > numOfTickets - 1)
                 return;

             _middleCanvas->SetMessageNumSAO(rbrPoruke, this);

         }





         else if (Globals::isStudent)
         {
             gui::Size sz;
             getSize(sz);
             const gui::Point& modelPoint = inputDevice.getFramePoint();
             td::INT4 rbr = modelPoint.y / 50; //visina svakog pravougaonika je 50, pa ce ovo vratiti rbr (pocevsi od 0)?
             //vektor subjects cuva sve IDs predmeta koji su ispisani, i to redom kako su ispisani. 
             // Da dobijete ID kliknutog predmeta ide subjects.at(rbr), kao u alertu ispod
             std::cout << subjects.size();
             if (rbr >= subjects.size())
                 return;
             td::INT4 subjectID = subjects.at(rbr);
             td::String name = _subjectsName.at(rbr);
             _middleCanvas->setSubjectID(subjectID);
             _middleCanvas->setSubjectName(name);


             _middleCanvas->reset();
             //showAlert("", std::to_string(subjects.at(rbr)));

             //_middleCanvas->onDraw();
            // showAlert("", std::to_string(subjects.at(rbr)));


         }
     }
    bool getModelSize(gui::Size& modelSize) const override
    {


        if (Globals::isSAO)
        {
            modelSize.width = 230;
            modelSize.height = GlobalsCanvas::visinaLeftSAO;
            return true;
        }

        modelSize.width = 230;
        modelSize.height = (_brojChat) * 110;
        return true;
    }




};

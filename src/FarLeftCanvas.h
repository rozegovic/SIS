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
    
public:
    FarLeftCanvas(MiddleCanvas* canvas)
        : _etf(":defaultUuser")
        , gui::Canvas({ gui::InputDevice::Event::PrimaryClicks })
        , _brojChat(50)  // --------------------------problem sa ovim 
        , _visinaChata(100)
        {
            _middleCanvas = canvas;
        }

    void onDraw(const gui::Rect& rect) override {
        const bool check = false;
        // pogled za profesora i asistenta ------ grupa 3
        if ((Globals::_currentUserID == 1 || Globals::_currentUserID == 3)) {
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
            std::sort(users.begin(), users.end(), [](const auto& a, const auto& b) {
                return a.first < b.first;
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


            gui::Rect imgRect(0, 0, sz.width, _visinaChata);
            _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Left);
            _user.draw(imgRect, gui::Font::ID::SystemBold, td::ColorID::Black, td::TextAlignment::Right, td::VAlignment::Center, td::TextEllipsize::End); //no
            gui::Shape::drawRect(imgRect, td::ColorID::Red, 5, td::LinePattern::Solid);
            imgRect.translate(0, 110);
            /* _user.draw(imgRect, gui::Font::ID fntID, td::ColorID clrID, td::TextAlignment hAlign = td::TextAlignment::Left, td::VAlignment vAlign = td::VAlignment::Top, td::TextEllipsize ellips = td::TextEllipsize::End); *///no
            td::ColorID boja(td::ColorID::Maroon);

            for (auto x : users) {
                if (x.second == Globals::_currentUserID)continue;
                _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Left);
                if (static_cast<td::ColorID>(static_cast<int>(boja) + 1) == td::ColorID::White)
                    boja = td::ColorID::Maroon;
                boja = static_cast<td::ColorID>(static_cast<int>(boja) + 1);
                td::String a = x.first;
                gui::DrawableString others = a;
                others.draw(imgRect, gui::Font::ID::SystemNormal, td::ColorID::Black, td::TextAlignment::Right, td::VAlignment::Center, td::TextEllipsize::End);
                gui::Shape::drawRect(imgRect, boja, 5, td::LinePattern::Solid);
                imgRect.translate(0, 110);
            }
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

            }

        }

        // pogled za studenta ------ grupa 2
        else if (Globals::_currentUserID == 5) {
            //  createStrings();
            std::vector<std::pair<gui::DrawableString, td::INT4>> SubjFrames;//naziv predmeta + y koord
            _noOfSubjects = 0;
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
                return;
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
        }
    };
    void reset() {
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

    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override {
        if(Globals::_currentUserID == 1 || Globals::_currentUserID == 3){
        
        double tempk = 0;

        int a = 0;
        td::INT4 IdUserChat;

        for (int i = 0; i < users.size(); i++) {

            // if (tempk < int(inputDevice.getFramePoint().y) && inputDevice.getFramePoint().y < (tempk + _visinaChata)) {
            if (tempk < inputDevice.getModelPoint().y && inputDevice.getModelPoint().y < (tempk + _visinaChata)) {
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
    }

    bool getModelSize(gui::Size& modelSize) const override
    {
        modelSize.width = 230;
        modelSize.height = (_brojChat) * 110;
        return true;
    }
};


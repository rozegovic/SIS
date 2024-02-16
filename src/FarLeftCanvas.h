#pragma once
#include <gui/View.h>
#include <gui/Image.h>
#include <gui/ImageView.h>
#include <gui/SplitterLayout.h>
#include "MiddleCanvas.h"
#include <algorithm>
#include <gui/Label.h>


class FarLeftCanvas : public gui::Canvas
{
private:
protected:
    gui::Image _etf;
    MiddleCanvas* _middleCanvas;
    gui::Shape _shapeCircle1;
    std::vector<std::pair<td::String, td::INT4>> users;

    td::INT4 _brojChat; // varijabla koja definise broj pravougaonika
    td::INT4 _visinaChata;  // varijabla d iz gui::Rect imgRect(a,b,c,d
public:
    FarLeftCanvas(MiddleCanvas* canvas)
        : _etf(":defaultUuser")
        , gui::Canvas({ gui::InputDevice::Event::PrimaryClicks })
        , _brojChat(5)
        , _visinaChata(100)
    {
        _middleCanvas = canvas;


    }


    void onDraw(const gui::Rect& rect) override {
        const bool check = false;
        // pogled za profesora i asistenta ------ grupa 3
        if ((Globals::_currentUserID == 1 || Globals::_currentUserID == 3) ) {
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
            while(pSelect->moveNext()){
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
            gui::DrawableString _user;
            for (auto a : users) {
                if(a.second==Globals::_currentUserID)
                    _user=a.first;
            }

            gui::Rect imgRect(0, 0, sz.width, _visinaChata);
            _user.draw(imgRect, gui::Font::ID::SystemBold, td::ColorID::Black, td::TextAlignment::Center, td::VAlignment::Center, td::TextEllipsize::End); //no
            _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Left);
            gui::Shape::drawRect(imgRect, td::ColorID::Red, 5, td::LinePattern::Solid);
            imgRect.translate(0, 110);
            /* _user.draw(imgRect, gui::Font::ID fntID, td::ColorID clrID, td::TextAlignment hAlign = td::TextAlignment::Left, td::VAlignment vAlign = td::VAlignment::Top, td::TextEllipsize ellips = td::TextEllipsize::End); *///no
            td::ColorID boja(td::ColorID::Maroon);

            for (auto x:users) {
                if (x.second == Globals::_currentUserID)continue;
                _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Left);
                if (static_cast<td::ColorID>(static_cast<int>(boja) + 1) == td::ColorID::White)
                    boja = td::ColorID::Maroon;
                boja = static_cast<td::ColorID>(static_cast<int>(boja) + 1);
                td::String a = x.first;
                gui::DrawableString others = a;
                others.draw(imgRect, gui::Font::ID::SystemNormal, td::ColorID::Black, td::TextAlignment::Center, td::VAlignment::Center, td::TextEllipsize::End);
                gui::Shape::drawRect(imgRect, boja, 5, td::LinePattern::Solid);
                imgRect.translate(0, 110);
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

    void reset() {

        reDraw();
    };


    // ideja za implementaciju otvaranja chata
    // treba dodati funkciju za klik
    // vjerovatno treba slati neki parametar i napraviti novu funkciju u middlecanvasu
    void openMiddleCanvas() {
        _middleCanvas->reset();
    };
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


    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override {
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

    bool getModelSize(gui::Size& modelSize) const override
    {
        modelSize.width = 500;
        modelSize.height = (_brojChat) * 110;
        return true;
    }
};

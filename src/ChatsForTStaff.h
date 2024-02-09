#pragma once
#include <gui/View.h>
#include <gui/Image.h>
#include <gui/ImageView.h>
#include <gui/SplitterLayout.h>

//-------------------Klasa za grupu 3
//-------------------svi chatovi za profesora/asistenta
//-------------------izbrisati sliku etf (tu samo da se nesto prikazuje)!!!
class ChatsTS : public gui::Canvas
{
private:
protected:
    gui::Image _etf;

public:
    ChatsTS()
        : _etf(":ETF")
    {

    }

    void onDraw(const gui::Rect& rect) override {
        gui::Size sz;
        getSize(sz);
        //gui::Point cp(sz.width / 2, sz.height / 2);
        //td::INT4 x = cp.x;
        //td::INT4 y = cp.y

        //gui::Rect imgRect(x - 15 - x / 4, y - 15 - y / 4, x + 15 + x / 4, y + 15 + y / 4);
        //_etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center); //no


        _pDS = dp::getMainDatabase()->createDataSet("SELECT Korisnici.ID as IDUser, Korisnici.Ime as nameUser, Korisnici.Prezime as surnameUser, Pozicija.ID as positionID, Pozicija.Naziv as roleUser, Korisnici.JMBG as jmbgUser, Korisnici.DatumUpisa as dateEUser, Korisnici.Adresa as addressUser, Korisnici.DatumRodjenja as dateBUser, Korisnici.Indeks as indeksUser FROM Korisnici, Pozicija WHERE Korisnici.PozicijaID = Pozicija.ID AND Korisnici.ID>0", dp::IDataSet::Execution::EX_MULT);
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


    };
};
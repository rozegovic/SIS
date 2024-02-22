#pragma once
#include <gui/View.h>
#include <gui/Image.h>
#include <gui/ImageView.h>
#include <gui/SplitterLayout.h>
#include <gui/DrawableString.h>

class MiddleCanvas : public gui::Canvas
{
private:
   td::INT4 predmetID = 0; // grupa 2 
protected:
    gui::Image _etf;

public:
    MiddleCanvas()
        : _etf(":ETF")
    {

    }

    void setSubjectID(td::INT4 id) { //grupa 2 - potreban subjectID
       predmetID = id;  //linije 53, 54
        reset();//da bi se nakon klika refreshao middle canvas
    }

    void onDraw(const gui::Rect& rect) override {
        const bool check = false;
        // pogled za profesora i asistenta ------ grupa 3
        if (Globals::_currentUserID == 1 || Globals::_currentUserID == 3) {
            gui::Size sz;
            getSize(sz);
            gui::Point cp(sz.width / 2, sz.height / 2);
            td::INT4 x = cp.x;
            td::INT4 y = cp.y;

            gui::Rect imgRect(x - 15 - x / 4, y - 15 - y / 4, x + 15 + x / 4, y + 15 + y / 4);
            _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center);
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
        // pogled za studenta ------ grupa 2 ------ ako ne koristite mozete samo ostaviti da bude nacrtan etf znak
        else if (Globals::_currentUserID == 5) {

            td::INT4 broj = predmetID;  //uzima random vrijednost, a u funkciji setSubjectID dobro ocita
          //  td::INT4 broj = 1;  //  1 za provjeru select-a

            gui::Size sz;
            getSize(sz);
            gui::Point cp(sz.width /2, sz.height / 2);
            cp.x = 10 ;
            cp.y = 10 ;
            td::String text, text1, text2, text3;
            text = "Detalji o odabranom predmetu "; 
            
            /* funkcije:
            static void draw(const td::String& txt, const gui::Rect& r, gui::Font::ID fntID, td::ColorID clrID, td::TextAlignment hAlign = td::TextAlignment::Left, td::VAlignment vAlign = td::VAlignment::Top,  td::TextEllipsize ellips = td::TextEllipsize::End);
            static void draw(const td::String& txt, const gui::Point& pt, gui::Font::ID fntID, td::ColorID clrID);
            */
            if (predmetID == -1) { //ovaj text2 bi trebao da piše čim se student uloguje, prije nego odabere neki predmet
                gui::DrawableString text1 = "Odaberite predmet da biste vidjeli detalje ";
                text1.draw(cp, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Gainsboro);
            }
            else {
                gui::DrawableString text2 = "Predmet: ";
                text2.draw(cp, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Gainsboro);
                int razmak = 40;
                cp.y = cp.y + razmak; // trebalo bi jos + visina slova prethodnog
                // + sta ako tekst zauzima npr 5 redova(teoretski )...
                text = "Predmet: ";
                gui::DrawableString::draw(text, cp, gui::Font::ID::SystemLargerBold, td::ColorID::Coral);
                dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("Select Naziv_Predmeta AS Naziv FROM Predmet WHERE ID_Predmeta = ?");
                dp::Params pParams(pSelect->allocParams());
                pParams << broj;
                dp::Columns pCols = pSelect->allocBindColumns(1);
                pCols << "Naziv" << text1;
                if (!pSelect->execute())
                    return;
                while (pSelect->moveNext());                                            
                cp.x = cp.x + text.length() + 55;
                gui::DrawableString::draw(text1, cp, gui::Font::ID::SystemLargerBold, td::ColorID::Gold);
            
                razmak += 5;
                cp.y += razmak;
                cp.x = 10;
                text = "Odgovorni nastavni ansambl: ";
                gui::DrawableString::draw(text, cp, gui::Font::ID::SystemLargerBold, td::ColorID::Gainsboro);
                dp::IStatementPtr pSelect1 = dp::getMainDatabase()->createStatement("Select a.Ime AS ime, a.Prezime AS prezime FROM Korisnici a, PredmetStaff b WHERE b.ID_Korisnika = a.ID AND  b.ID_Predmeta = ?");
                dp::Params pParams1(pSelect1->allocParams());
                pParams1 << broj;
                dp::Columns pCols1 = pSelect1->allocBindColumns(2);
                td::String a, b;
                pCols1 << "prezime" << a << "ime" << b;
                text1 = a;
               // text1 += b;
                if (!pSelect1->execute())
                    return;
                while (pSelect1->moveNext())
                {
                    cp.x = cp.x + 55; //+ text.length()
                    cp.y += 45;
                    gui::DrawableString Surname = surname;
                    Surname.draw(cp, gui::Font::ID::SystemLargerBold, td::ColorID::Gold);
                    cp.x = cp.x + surname.length() + 55;
                    gui::DrawableString Name = name;
                    Name.draw(cp, gui::Font::ID::SystemLargerBold, td::ColorID::Gold);
                    razmak += 10;
                    cp.y += razmak;
                    cp.x = 10;
                }
                text = "Aktivnosti na predmetu: ";
                gui::DrawableString::draw(text, cp, gui::Font::ID::SystemLargerBold, td::ColorID::Gainsboro);
                dp::IStatementPtr pSelect2 = dp::getMainDatabase()->createStatement("SELECT Naziv_Aktivnosti AS Naziv FROM Aktivnosti WHERE ID_Predmeta = ?");
                dp::Params pParams2(pSelect2->allocParams());
                pParams2 << broj;
                dp::Columns pCols2 = pSelect2->allocBindColumns(1);
                pCols2 << "Naziv" << text3;
                if (!pSelect2->execute())
                    return;
                cp.x = cp.x + 75;
                while (pSelect2->moveNext()) {
                    cp.y += 20;
                    gui::DrawableString::draw(text3, cp, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Gold);
                }
                razmak += 10;
                cp.y += razmak;
                cp.x = 10;
                td::INT4 id = Globals::_currentUserID;
                text = "Ocijenjeno: ";
                gui::DrawableString::draw(text, cp, gui::Font::ID::SystemLargerBold, td::ColorID::Gainsboro);
                dp::IStatementPtr pSelect3 = dp::getMainDatabase()->createStatement("Select a.Ocjena as ocjena, d.Naziv as tip FROM OcjeneIspita a, Aktivnosti c, VrstaAktivnosti d WHERE c.ID_Aktivnosti=a.ID_Aktivnosti and d.ID=a.ID_Aktivnosti and a.ID_Korisnika=? AND c.ID_Predmeta=?");
                dp::Params pParams3(pSelect3->allocParams());
              pParams3 << id << predmetID; //krahira ovdje
                dp::Columns pCols3 = pSelect3->allocBindColumns(2);
               td::String ocjena ;
                td::String tip;
                pCols3 << "ocjena" << ocjena << "tip" << tip;
               // text4=std::to_string(ocjena);
                if (!pSelect3->execute())
                    return;
               cp.x = cp.x + 75;
          
                while (pSelect3->moveNext()) {
                    cp.y += 20;
                    gui::DrawableString text4 = tip;
                    text4.draw( cp, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Gold);
                    cp.x = cp.x + tip.length() + 55;
                    gui::DrawableString text5 = ocjena;
                    text5.draw(cp, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Gold);
                    cp.x= cp.x-tip.length() - 55;
                }
            }
        }
        // kada nema ulogovane osobe
       /* else if(Globals::isLogged==check){

        }*/
        //samo srednji canvas crta etf znak
        else {
            gui::Size sz;
            getSize(sz);
            gui::Point cp(sz.width / 2, sz.height / 2);
            td::INT4 x = cp.x;
            td::INT4 y = cp.y;

            gui::Rect imgRect(x - 15 - x / 4, y - 15 - y / 4, x + 15 + x / 4, y + 15 + y / 4);
            _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center);
        }

    };

    void reset() {
        reDraw();
    };

};

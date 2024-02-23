#pragma once
#include <gui/View.h>
#include <gui/Image.h>
#include <gui/ImageView.h>
#include <gui/SplitterLayout.h>
#include <gui/DrawableString.h>
#include <gui/Transformation.h>

class MiddleCanvas : public gui::Canvas
{
private:
   td::INT4 predmetID = -1; // grupa 2 
   td::String _subjectname;

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
    void setSubjectName(td::String ime) {
        _subjectname = ime;
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

            //td::INT4 broj = predmetID;  //uzima random vrijednost, a u funkciji setSubjectID dobro ocita
            //td::INT4 broj = 1;  //  1 za provjeru select-a
            gui::Size sz;
            getSize(sz);
            gui::Point cp(sz.width /2, sz.height / 2);
            cp.x = 10 ;
            cp.y = 10 ;
            gui::Point cp2;
            cp2.x = 65;
            td::String text, text1, text2, text3, text4;
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
/*                gui::DrawableString::draw(text, cp, gui::Font::ID::SystemLargerBold, td::ColorID::Coral);
                dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("Select Naziv_Predmeta AS Naziv FROM Predmet WHERE ID_Predmeta = ?");
                dp::Params pParams(pSelect->allocParams());
                pParams << broj;
                dp::Columns pCols = pSelect->allocBindColumns(1);
                pCols << "Naziv" << text1;
                if (!pSelect->execute())
                    return;
                while (pSelect->moveNext());    */
                cp.x = 65;
                gui::DrawableString::draw(_subjectname, cp, gui::Font::ID::SystemLargerBold, td::ColorID::Gold);

                razmak += 5;
                cp.y += razmak;
                cp.x = 10;
                text = "Odgovorni nastavni ansambl: ";
                gui::DrawableString::draw(text, cp, gui::Font::ID::SystemLargerBold, td::ColorID::Gainsboro);
                dp::IStatementPtr pSelect1 = dp::getMainDatabase()->createStatement("Select a.Ime AS ime, a.Prezime AS prezime, Pozicija.Naziv as Pozicija FROM Korisnici a, PredmetStaff b, Pozicija WHERE b.ID_Korisnika = a.ID AND a.PozicijaID=Pozicija.ID and b.ID_Predmeta = ?");
                dp::Params pParams1(pSelect1->allocParams());
                pParams1 << predmetID;
                dp::Columns pCols1 = pSelect1->allocBindColumns(3);
                td::String name, surname, pozicija;
                pCols1 << "ime" << name << "prezime" << surname << "Pozicija" << pozicija;
                // text1 += b;
                if (!pSelect1->execute())
                    return;
                while (pSelect1->moveNext());
                {   cp.x = 10; //+ text.length()
                cp.y += 25;
                pozicija += " : ";
                pozicija += surname;
                pozicija += " ";
                pozicija += name;
                gui::DrawableString Pozicija = pozicija;
                Pozicija.draw(cp, gui::Font::ID::SystemLargerBold, td::ColorID::Gold);
                }
                razmak += 10;
                cp.y += razmak;
                td::INT4 Ypos = cp.y;
                cp.y += 50;
                cp.x = 10;
                text = "Aktivnosti na predmetu: ";
                gui::DrawableString::draw(text, cp, gui::Font::ID::SystemLargerBold, td::ColorID::Gainsboro);
                dp::IStatementPtr pSelect2 = dp::getMainDatabase()->createStatement("SELECT Naziv_Aktivnosti AS Naziv FROM Aktivnosti WHERE ID_Predmeta = ?");
                dp::Params pParams2(pSelect2->allocParams());
                pParams2 << predmetID;
                dp::Columns pCols2 = pSelect2->allocBindColumns(1);
                td::String naziv;
                pCols2 << "Naziv" << naziv;
                if (!pSelect2->execute())
                    return;
                cp.x = cp.x + 75;
                while (pSelect2->moveNext()) {
                    cp.y += 20;
                    gui::DrawableString text3 = naziv;
                    text3.draw(cp, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Gold);
                }
                razmak += 10;
                cp.y += razmak;
                cp.x = 10;
                td::INT4 id = Globals::_currentUserID;;
                text = "Ocijenjeno: ";
                gui::DrawableString::draw(text, cp, gui::Font::ID::SystemLargerBold, td::ColorID::Gainsboro);
                dp::IStatementPtr pSelect3 = dp::getMainDatabase()->createStatement("Select a.Ocjena as ocjena, d.Naziv as tip FROM OcjeneIspita a, Aktivnosti c, VrstaAktivnosti d WHERE c.ID_Aktivnosti=a.ID_Aktivnosti and d.ID=a.ID_Aktivnosti and a.ID_Korisnika=? AND c.ID_Predmeta=?");
                //select radi (npr id korisnika = 5 i id predmeta = 1
                dp::Params pParams3(pSelect3->allocParams());
                pParams3 << id << predmetID;
                dp::Columns pCols3 = pSelect3->allocBindColumns(2);
                td::String ocjena;
                td::String tip;
                pCols3 << "ocjena" << ocjena << "tip" << tip;
                if (!pSelect3->execute())
                    return;
                cp.x = cp.x + 75;
                td::INT4 prosjek = 0.; int brojac = 0; std::vector<td::INT4> Ocjene;
                while (pSelect3->moveNext()) {
                    cp.y += 20;
                    Ocjene.push_back(ocjena.toINT4());
                    gui::DrawableString text4 = tip;
                    text4.draw(cp, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Gold);
                    cp.x = cp.x + tip.length() + 55;
                    gui::DrawableString text5 = ocjena;
                    text5.draw(cp, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Gold);
                    cp.x = cp.x - tip.length() - 55;
                }
                for (int i = 0; i < Ocjene.size(); i++)
                {
                    prosjek += Ocjene[i] * 20; //skalirano
                }
                if (Ocjene.size() != 0)
                    prosjek = prosjek / Ocjene.size();


                {  //Progres bar
                    gui::Point prog;

                    gui::Size sz2;
                    getSize(sz2);
                    prog.x = sz.width / 2 - 100;
                    prog.y = Ypos - 25;
                    gui::DrawableString osvojeno = "Osvojili ste...";
                    osvojeno.draw(prog, gui::Font::ID::SystemBoldItalic, td::ColorID::Gainsboro);
                    prog.y = Ypos + 25;
                    gui::DrawableString moguce = "Od ukupno mogućih 100 bodova.";
                    moguce.draw(prog, gui::Font::ID::SystemBoldItalic, td::ColorID::Gainsboro);
                    gui::Rect prazan(0, 0, 200, 20);
                    gui::Rect fill(0, 0, prosjek, 20);
                    gui::DrawableString postotak;
                    prazan.translate(sz2.width / 2 - 100, Ypos);
                    fill.translate(sz2.width / 2 - 100, Ypos);
                    gui::Shape::drawRect(prazan, td::ColorID::White, td::ColorID::Black, 0.5, td::LinePattern::Solid);
                    if (prosjek <= 108) //zato sto je skalirano 
                        gui::Shape::drawRect(fill, td::ColorID::Red, td::ColorID::Black, 0.5, td::LinePattern::Solid);
                    else if (prosjek <= 148)
                        gui::Shape::drawRect(fill, td::ColorID::Yellow, td::ColorID::Black, 0.5, td::LinePattern::Solid);
                    else
                        gui::Shape::drawRect(fill, td::ColorID::Green, td::ColorID::Black, 0.5, td::LinePattern::Solid);

                    prosjek /= 2;
                    td::String pom = std::to_string(prosjek);
                    pom += "%";
                    //std::string pom1 = std::to_string(prosjek);
                    postotak = pom;
                    postotak.draw(prazan, gui::Font::ID::SystemBoldItalic, td::ColorID::Black, td::TextAlignment::Center, td::VAlignment::Center);
                }

                {   // TABELA PRISUSTVA

                    gui::Transformation zoomTr;
                    double zoom = 1.; //experiment with this and see impact
                    double scrollX = 0;
                    double scrollY = cp.y + 70;
                    zoomTr.translate(scrollX, scrollY);
                    zoomTr.scale(zoom);
                    zoomTr.appendToContext(); //this is transformationfor all objects

                    gui::Point privremena;
                    privremena.x = 0;
                    privremena.y = -25;
                    gui::DrawableString prisustvolabovima = "Prisustvo lab. vjezbama: ";
                    prisustvolabovima.draw(privremena, gui::Font::ID::SystemBoldItalic, td::ColorID::Gainsboro);


                    int duzina = 100;
                    int visina = 30;
                    gui::Rect sedmica(0, 0, duzina, visina);
                    gui::Rect vrijeme(0, 0, duzina, visina);
                    gui::Rect prisutan(0, 0, duzina, visina);
                    gui::DrawableString sedm = "Sedmica";
                    gui::DrawableString time = "Vrijeme";
                    gui::DrawableString prisustvo = "Prisutan";
                    vrijeme.translate(0, visina);
                    prisutan.translate(0, 2 * visina);
                    gui::Shape::drawRect(sedmica, td::ColorID::DimGray, td::ColorID::Black, 0.5, td::LinePattern::Solid);
                    gui::Shape::drawRect(vrijeme, td::ColorID::DimGray, td::ColorID::Black, 0.5, td::LinePattern::Solid);
                    gui::Shape::drawRect(prisutan, td::ColorID::DimGray, td::ColorID::Black, 0.5, td::LinePattern::Solid);
                    sedm.draw(sedmica, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Gainsboro, td::TextAlignment::Center, td::VAlignment::Center);
                    time.draw(vrijeme, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Gainsboro, td::TextAlignment::Center, td::VAlignment::Center);
                    prisustvo.draw(prisutan, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Gainsboro, td::TextAlignment::Center, td::VAlignment::Center);

                    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select Br_sedmice AS brsedmice, Vrijeme as vrijeme FROM Prisustvo, TerminiStudenti, Termini WHERE TerminiStudenti.ID_Studenta = ? AND  TerminiStudenti.ID_Termina = Prisustvo.ID_termina AND TerminiStudenti.TipPredavanjaID = 3");
                    dp::Params pParams(pSelect->allocParams());
                    pParams << Globals::_currentUserID;
                    dp::Columns pCols = pSelect->allocBindColumns(2);
                    td::INT4 brsedmice;
                    td::Time Vrijeme;
                    pCols << "brsedmice" << brsedmice << "Vrijeme" << Vrijeme;
                    std::vector<td::INT4> prisutnesedmice;
                    if (pSelect->execute()) {
                        prisutnesedmice.resize(0);
                        while (pSelect->moveNext()) {
                            //gui::DrawableString drawableName = name;
                            prisutnesedmice.push_back(brsedmice);
                        }
                    }

                    for (int i = 0; i < 15; i++) {
                        int novaduzina = 50;
                        gui::Rect red_sedmica(0, 0, novaduzina, visina);
                        td::String broj = std::to_string(i + 1);
                        gui::DrawableString rednibroj = broj;
                        red_sedmica.translate(i * novaduzina + duzina, 0);
                        gui::Shape::drawRect(red_sedmica, td::ColorID::White, td::ColorID::Black, 0.5, td::LinePattern::Solid);
                        rednibroj.draw(red_sedmica, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Black, td::TextAlignment::Center, td::VAlignment::Center);

                        gui::Rect time1(0, 0, novaduzina, visina);
                        time1.translate(i * novaduzina + duzina, visina);
                        td::String temp = std::to_string(Vrijeme.getHour()); 
                        temp += ":";
                        temp+= std::to_string(Vrijeme.getMinute());
                        gui::DrawableString vrijemezaispis = temp;
                        gui::Shape::drawRect(time1, td::ColorID::White, td::ColorID::Black, 0.5, td::LinePattern::Solid);
                        vrijemezaispis.draw(time1, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Black, td::TextAlignment::Center, td::VAlignment::Center);

                        gui::Rect prisutan(0, 0, novaduzina, visina);
                        prisutan.translate(i * novaduzina + duzina, 2*visina);
                        bool jeprisutan = false;
                        for (auto x : prisutnesedmice) {
                            if (x == i + 1) {
                                jeprisutan = true;
                                break;
                            }
                        }
                        if (jeprisutan) {
                            gui::DrawableString jestprisutan = "DA";
                            gui::Shape::drawRect(prisutan, td::ColorID::Green, td::ColorID::Black, 0.5, td::LinePattern::Solid);
                            jestprisutan.draw(prisutan, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Black, td::TextAlignment::Center, td::VAlignment::Center);
                        }
                        else {
                            gui::DrawableString jestprisutan = "NE";
                            gui::Shape::drawRect(prisutan, td::ColorID::Red, td::ColorID::Black, 0.5, td::LinePattern::Solid);
                            jestprisutan.draw(prisutan, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Black, td::TextAlignment::Center, td::VAlignment::Center);
                        }
                    }
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

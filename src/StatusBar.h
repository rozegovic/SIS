#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/TextEdit.h>
#include <gui/LineEdit.h>
#include <gui/HorizontalLayout.h>
#include "Globals.h"
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>
#include <functional>


class StatusBar : public gui::View
{
protected:
    gui::Label _lblKorisnik;
    gui::Label _korisnik;
    
    gui::Label _lblUloga;
    gui::Label _uloga;
    
    gui::HorizontalLayout _hl;
    std::function<void(td::INT1, int, int)> _fnUpdateStatusBar;
    
public:
    StatusBar():
    _lblKorisnik(tr("StatUser"))
    ,_lblUloga(tr("StatRole"))
    ,_korisnik(" ")
    ,_uloga(" ")
    ,_hl(5)
    {
        setMargins(4, 0, 4, 4);
        _hl << _lblKorisnik << _korisnik;
        _hl.appendSpacer();
        _hl << _lblUloga << _uloga;
        setLayout(&_hl);
    }
    void UpdateStatusBar(){
        dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT Ime, Prezime FROM Korisnici WHERE ID = ?");
        dp::Params parDS(pSelect->allocParams());
        //d::INT4 IDPredmeta = Globals::_IDSubjectSelection;
        parDS << Globals::_currentUserID;
        dp::Columns pCols = pSelect->allocBindColumns(2);
        td::String Ime, Prezime;
        pCols << "Ime" << Ime << "Prezime" << Prezime;
        if(!pSelect->execute()){
            Ime = "Problem";
            Prezime = "Problem";
        }
        while (pSelect->moveNext())
        {
            /*td::Variant val1, val2, kor;
            val1 = Ime;
            val2 = Prezime;
            kor = val1 + val2;*/
            
            Ime += td::String(" ") += Prezime;
            _korisnik.setTitle(Ime);
            

        }
        
        dp::IStatementPtr pSelect1 = dp::getMainDatabase()->createStatement("SELECT Naziv FROM Pozicija WHERE ID = ?");
        dp::Params parDS1(pSelect1->allocParams());
        parDS1 << Globals::_currentUserRole;
        dp::Columns pCols1 = pSelect1 ->allocBindColumns(1);
        td::String Uloga;
        pCols1 << "Naziv" << Uloga;
        if(!pSelect1->execute()){
            Uloga = "Problem";
        }
        while (pSelect1->moveNext())
        {
            //td::Variant val;
            //val  = Uloga;
            _uloga.setTitle(Uloga);
       

        }
    }
};



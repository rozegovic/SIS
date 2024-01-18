#pragma once
#include <gui/Dialog.h>
#include "ViewChooseSubject.h"
#include "Globals.h"

class  DialogChooseSubject : public gui::Dialog
{
protected:
    ViewChooseSubject _viewAttendance;

    bool onClick(Dialog::Button::ID btnID, gui::Button* pButton) override
    {
        if(btnID == Dialog::Button::ID::OK){
          //  Globals::_IDSubjectSelection = _viewAttendance.getSubjectID();
            return true;
        }
        else closeModal(btnID);
        return false;
    }

public:
    td::INT4 getSubjectID() {
        return  _viewAttendance.getSubjectID();
    }  

    DialogChooseSubject(gui::Frame* pView)
        : gui::Dialog(pView, { {gui::Dialog::Button::ID::OK, tr("Ok"), gui::Button::Type::Default},
                                {gui::Dialog::Button::ID::Cancel, tr("Cancel")} }, gui::Size(300, 100))
    {
        setCentralView(&_viewAttendance);
        loadComboBox("select a.Naziv_Predmeta AS Naziv, a.ID_Predmeta AS ID from Predmet a, PredmetStaff b WHERE a.ID_Predmeta = b.ID_Predmeta AND b.ID_Korisnika = ?", _viewAttendance.getComboBox());
    }

    bool loadComboBox(td::String select, gui::DBComboBox& combo) {
        dp::IStatementPtr pSelect;
        if (Globals::_currentUserRole == 6) {
            pSelect = dp::getMainDatabase()->createStatement("Select Naziv_Predmeta AS Naziv, ID_Predmeta AS ID FROM Predmet");
        }
        else {
             pSelect = dp::getMainDatabase()->createStatement(select.c_str());
                 dp::Params pParams(pSelect->allocParams());
                 pParams << Globals::_currentUserID;
        }
        dp::Columns pCols = pSelect->allocBindColumns(2);
        td::String name;
        td::INT4 id;
        pCols << "ID" << id << "Naziv" << name;
        if (!pSelect->execute())
            return false;

        while (pSelect->moveNext())
        {
            combo.addItem(name, id);
        }
        if (combo.getNoOfItems() > 0)
        combo.selectIndex(0);
        return true;

    }
    gui::DBComboBox& getComboBox()
    {
        return _viewAttendance.getComboBox();

    }
    ~DialogChooseSubject() {}

};

#pragma once
#include <gui/Dialog.h>
#include "ViewChooseSubject.h"
#include "Globals.h"
#include <dp/IStatement.h>
#include <dp/IDatabase.h>

class  DialogChooseAllSubjects : public gui::Dialog
{
protected:
    ViewChooseSubject _viewAllSubjects;

    bool onClick(Dialog::Button::ID btnID, gui::Button* pButton) override
    {
        if (btnID == Dialog::Button::ID::OK) {
            //  Globals::_IDSubjectSelection = _viewAttendance.getSubjectID();
            return true;
        }
        return false;
    }

public:
    td::INT4 getSubjectID() {
        return  _viewAllSubjects.getSubjectID();
    }
    DialogChooseAllSubjects(gui::Frame* pView)
        : gui::Dialog(pView, { {gui::Dialog::Button::ID::OK, tr("Ok"), gui::Button::Type::Default},
                                {gui::Dialog::Button::ID::Cancel, tr("Cancel")} }, gui::Size(300, 100))
    {
        setCentralView(&_viewAllSubjects);
        loadComboBox("select Naziv_Predmeta AS Naziv, ID_Predmeta AS ID from Predmet", _viewAllSubjects.getComboBox());
    }

    bool loadComboBox(td::String select, gui::DBComboBox& combo) {
        dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement(select.c_str());
       // dp::Params pParams(pSelect->allocParams());
       // pParams << Globals::_currentUserID;

        dp::Columns pCols = pSelect->allocBindColumns(2);
        td::String name;
        td::INT4 id;
        pCols << "Naziv" << name << "ID" << id ;
        if (!pSelect->execute())
            return false;

        while (pSelect->moveNext())
        {
            combo.addItem(name, id);
        }
        combo.selectIndex(0);
        return true;

    }
    ~DialogChooseAllSubjects() {}

};

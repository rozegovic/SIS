
#pragma once
#include <gui/Dialog.h>
#include "ViewCurriculumDialog.h"
#include <dp/IDatabase.h>
#include "Globals.h"

class  DialogCurriculum : public gui::Dialog
{

protected:
    ViewCurriculumDialog _viewCurriculum;

    bool onClick(Dialog::Button::ID btnID, gui::Button* pButton) override
    {
        if (btnID == Dialog::Button::ID::OK) {
            //  Globals::_IDSubjectSelection = _viewAttendance.getSubjectID();
            if (!(_viewCurriculum.canAdd()))
                return false;
            return true;
        }
        return false;
    }
public:
    td::INT4 getSemester() {
        return  _viewCurriculum.getSemester();
    }
    td::INT4 getDepartmentID() {
        return  _viewCurriculum.getDepartmentID();
    }

    DialogCurriculum(gui::Frame* pView)
        : gui::Dialog(pView, { {gui::Dialog::Button::ID::OK, tr("Ok"), gui::Button::Type::Default},
                                {gui::Dialog::Button::ID::Cancel, tr("Cancel")} }, gui::Size(300, 100))
    {
        
        setCentralView(&_viewCurriculum);
        loadComboBox("select ID_Smjera as ID, Naziv_Smjera as Name from Smjer", _viewCurriculum.getCombobox());
        
    }
    bool loadComboBox(td::String select, gui::DBComboBox& combo)
    {
        dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement(select.c_str());
        dp::Columns pCols = pSelect->allocBindColumns(2);
        td::String name;
        td::INT4 id;
        pCols << "ID" << id << "Name" << name;
        if (!pSelect->execute())
            return false;

        while (pSelect->moveNext())
        {
            combo.addItem(name, id);
        }
        combo.selectIndex(0);
        return true;
    }
    gui::DBComboBox& getComboBox()
    {
        return _viewCurriculum.getCombobox();

    }
    ~DialogCurriculum() {}
};

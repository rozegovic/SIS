
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
        _viewCurriculum.getDepartment();
        
    }
    ~DialogCurriculum() {}
};

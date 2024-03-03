#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/PasswordEdit.h>
#include <gui/GridLayout.h>
#include <gui/NumericEdit.h>
#include <gui/ComboBox.h>
#include <gui/DBComboBox.h>
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>
#include <dp/IDataSetDelegate.h>

class ViewCurriculumDialog : public gui::View
{
private:
protected:
    gui::Label _lblDepartment;
    gui::DBComboBox _department;
    gui::Label _lblSemester;
    gui::NumericEdit _semester;
    gui::GridLayout _gl;

public:
    ViewCurriculumDialog()
        : _lblDepartment(tr("Department"))
        , _department(td::int4)
        , _lblSemester(tr("Semester"))
        , _semester(td::int4)
        , _gl(2, 2)
    {

        // _department.setToolTip(tr("Enter department:"));
        _gl.insert(0, 0, _lblDepartment);
        _gl.insert(0, 1, _department);
        _gl.insert(1, 0, _lblSemester);
        _gl.insert(1, 1, _semester);
        _semester.setValue(1);
        gui::View::setLayout(&_gl);
    }


    gui::DBComboBox& getCombobox() {
        return _department;
    }

  /*  void getDepartment()
    {
        loadComboBox("select ID_Smjera as ID, Naziv_Smjera as Name from Smjer", _department);
    }*/

    td::INT4 getSemester() const
    {
        td::Variant semestar;
        _semester.getValue(semestar);
        return semestar.i4Val();
    }

    td::INT4 getDepartmentID()
    {
        td::Variant val;
        _department.getValue(val);
        return val.i4Val();

    }
    bool canAdd()
    {
        td::Variant pom = _semester.getValue();
        if (pom < 1) {
            showAlert(tr("alert"), tr("alertCRSS"));
            return false;
        }
        return true;
    }
};

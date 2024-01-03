#pragma once
#include <algorithm>
#include <string>
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/LineEdit.h>
#include <gui/TextEdit.h>
#include <gui/VerticalLayout.h>
#include <gui/HorizontalLayout.h>
#include <gui/GridLayout.h>
#include <gui/Button.h>
#include <gui/NumericEdit.h>
#include <gui/ComboBox.h>
#include <gui/DBComboBox.h>
#include <gui/CheckBox.h>
#include <gui/Slider.h>
#include <gui/ProgressIndicator.h>
#include <gui/GridComposer.h>
#include <gui/DateEdit.h>
#include <gui/TimeEdit.h>
#include <gui/ColorPicker.h>
#include <gui/TableEdit.h>
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>
#include <dp/IDataSetDelegate.h>
#include <fo/FileOperations.h>
#include <gui/ImageView.h>
#include <rnd/MinMax.h>
#include "ViewUsers.h"


class ViewCourseEnroll : public gui::View
{
private:
protected:

    // first row
    gui::Label _departlbl;
    gui::DBComboBox _department;
    gui::Label _semlbl;
    gui::NumericEdit _semester;
    gui::Label _courselbl;
    gui::DBComboBox _course;
    gui::Label _namelbl;
    gui::LineEdit _name;
    gui::Label _surnamelbl;
    gui::LineEdit _surname;
    gui::Label _indexlbl;
    gui::LineEdit _index;
    gui::TableEdit _tableStudents;
    gui::TableEdit _tableEnrolled;
    gui::ComboBox _semesterCombo;
    gui::Label _enrolled;
    gui::Label _toBeEnrolled;

    //labels for the bottom row
    gui::HorizontalLayout _hlBtnsDB;
    gui::Button _btnSave;
    gui::Button _btnReload;
    gui::Button _btnWithdraw;
    gui::Button _btnEnroll;
    gui::GridLayout _gl;
    dp::IDatabase* _db;
    dp::IDataSetPtr _pDS;
    dp::IDataSetPtr _pDS2;


public:
    ViewCourseEnroll();

protected:
    void populateDepartmentCombo(gui::DBComboBox& combo);
    void populateCourseCombo(gui::DBComboBox& combo, td::String smjer="0", td::String semestar="0");
    void populateSemesterCombo(gui::ComboBox& combo);
    void populateDataForStudents();
    void populateDataForEnrolledStudents();
    bool onChangedSelection(gui::TableEdit* pTE) override;
    bool onChangedSelection(gui::ComboBox* pCmb) override;
   bool onChangedSelection(gui::DBComboBox* pCmb) override;
    void initTableEnrolledStudents();
    void initTableStudents();
    bool onClick(gui::Button* pBtn);
    bool onAnswer(td::UINT4 questionID, gui::Alert::Answer answer);
    void populateDSRow(dp::IDataSet::Row& row);
    bool saveEnrolls();
    void UpdateEnrollDataSet();
    void UpdateStudentDataSet();
};
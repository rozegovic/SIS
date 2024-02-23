#pragma once
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
#include <gui/Color.h>
#include "SendMessage.h"


class ViewFinalGrade : public gui::View {
protected:
    gui::Label _lblname;
    gui::LineEdit _name;
    gui::Label _lblsname;
    gui::LineEdit _sname;
    gui::Label _lblpoints;
    gui::LineEdit _points;
    gui::Label _lblGrade;
    gui::NumericEdit _grade;
    gui::Label _lblindex;
    gui::LineEdit _index;

    gui::Label _lblCName;
    gui::LineEdit _cName;

    gui::HorizontalLayout _hlBtns;

    gui::Button _btnDelete;
    gui::Button _btnAdd;

    dp::IDatabase* _db;
    gui::GridLayout _gl;
    dp::IDataSetPtr _pDS;
    td::INT4 _SubjectID;
    td::INT4 _UserID;
    gui::TableEdit _table;

    td::INT4 moguciBodovi;

    std::vector<std::pair<int,td::INT4>> userIds;




public:
    ViewFinalGrade(td::INT4 SubjectID);   
protected:
    void populateData();   
    bool onChangedSelection(gui::TableEdit* pTE);     
    bool onClick(gui::Button* pBtn); 
    void SetCurrentSubject();     
    void initTable();
    void reloadTable();

};
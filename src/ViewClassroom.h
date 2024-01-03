#pragma once
#include <algorithm>
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

//enum class QuestionID : td::UINT2 { Save };
class ViewClassroom : public gui::View
{
private:
protected:
    // first row
    gui::Label _lblName;
    gui::LineEdit _name;
    gui::Label _lblLocation;
    gui::LineEdit _location;
    //gui::Label _lblTip;
   // gui::LineEdit _tip;
    gui::Label _lblType;
    gui::DBComboBox _type;
    gui::Label _lblNumSeats;
    gui::NumericEdit _numSeats;
    gui::TableEdit _table;

    //labels for the bottom row
    gui::HorizontalLayout _hlBtnsDB;
    gui::Button _btnSave;
    gui::Button _btnReload;
    gui::Button _btnDelete;
    gui::Button _btnPushBack;
    gui::Button _btnUpdate;
    gui::GridLayout _gl;
   // dp::IDatabasePtr _db;
    dp::IDataSetPtr _pDS;

public:
    ViewClassroom();
protected:
    void populateTypeCombo(gui::DBComboBox& combo);
    void populateData();
    bool onChangedSelection(gui::TableEdit* pTE);
    void populateDSRow(dp::IDataSet::Row& row);
    bool doesIDexist(td::Variant name);
    bool onClick(gui::Button* pBtn);
    void saveData();
    //bool onAnswer(td::UINT4 questionID, gui::Alert::Answer answer);
};

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
#include "Globals.h"

//enum class QuestionIDD : td::UINT2 { Savee };


class ViewDepartment : public gui::View
{
protected:
    gui::Label _lblID;
    gui::NumericEdit _id;
    gui::Label _lblName;
    gui::LineEdit _name;
    gui::LineEdit _shName;
    gui::Label _lblShName;

    gui::TableEdit _table;
    gui::HorizontalLayout _hlBtns;
    gui::Button _btnAdd;
    gui::Button _btnSave;
    gui::Button _btnReload;
  
    gui::Button _btnDelete;
    gui::Button _btnUpdate;
    /*gui::Button _btnReload;
    gui::Button _btnRemoveAll;
    /*gui::Button _btnReload;
    gui::Button _btnRemoveAll;
    gui::Button _btnDelete;
    gui::Button _btnUpdate;
    gui::Button _btnInsert;
    gui::Button _btnPushBack;*/
    gui::GridLayout _gl;

    dp::IDatabase* _db;
    dp::IDataSetPtr _pDS = nullptr;
    td::INT4 _paramFrom;  //mozda kasnije potrebni
    td::INT4 _paramTo;    //mozda kasnije potrebni

    std::vector<td::INT4> _depsToDelete, _depsToInsert, _depsToUpdate;    

public:
    ViewDepartment();
    ~ViewDepartment();   

protected:
    void initTable();
    void populateData();
    bool loadComboBox(td::String select, gui::DBComboBox& combo);
    virtual bool onChangedSelection(gui::TableEdit* pTE);
    void populateDSRow(dp::IDataSet::Row& row);
    bool canAdd();
    td::INT4 getIDfromTable(int rowID);
    bool canUpdateDepartment(int iRow);
    bool existsInDb(td::INT4 id);
    bool canDelete(int iRow);
    bool eraseDepartment();
    bool insertDepartment();
    bool updateDepartment();
    bool saveData();
    virtual bool onClick(gui::Button* pBtn);
    bool onAnswer(td::UINT4 questionID, gui::Alert::Answer answer);

};


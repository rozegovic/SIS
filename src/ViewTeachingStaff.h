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
//#include <td/String.h>

//enum class QuestionIDDDD : td::UINT2 { Saveeee };


class ViewTeachingStaff : public gui::View
{
protected:
    gui::Label _lblCode;
    gui::LineEdit _code;
    gui::Label _lblCourseID;
    gui::NumericEdit _courseID;
    gui::Label _lblCourseName;
    gui::LineEdit _courseName;

    gui::Label _lblUserID;
    gui::NumericEdit _userID;
    gui::Label _lblUserName;
    gui::LineEdit _userName;
    gui::Label _lblUserLN;
    gui::LineEdit _userLName;

    gui::TableEdit _table;
    gui::HorizontalLayout _hlBtns;
    gui::Button _btnAdd;
    gui::Button _btnSave;
    gui::Button _btnReload;
    gui::Button _btnRemoveAll;
    gui::Button _btnDelete;
    gui::Button _btnInsert;
    gui::GridLayout _gl;

    dp::IDatabase* _db;
    dp::IDataSetPtr _pDS = nullptr;

    bool insertPositionInDSRow(dp::IDataSet::Row& row);
    td::INT4 _SubjectID;


    //-------------primarni kljuc tabele PredmetStaff se sastoji od ID_Korisnika i ID_Predmeta sto znaci 
    // ---------------da moramo imati dvije kljucne vrijednosti koje treba pratiti (jedna se moze preklapati)
    std::vector<std::pair<td::INT4, td::INT4>> _itemsToDelete, _itemsToInsert;
public:
    ViewTeachingStaff(td::INT4 SubjectID);
    ~ViewTeachingStaff();

protected:
    void initTable(int type);
    void populateData();
    bool loadComboBox(td::String select, gui::DBComboBox& combo);
    bool onFinishEdit(gui::LineEdit* pCtrl) override;
    virtual bool onChangedSelection(gui::TableEdit* pTE);
    void populateDSRow(dp::IDataSet::Row& row);
    bool canAdd();
    std::pair<td::INT4, td::INT4> getIDfromTable(int rowID);
    bool existsInDb(td::INT4 id);
    bool canDelete(int iRow);
    bool eraseStaff();
    bool insertStaff();
    bool saveData();
    virtual bool onClick(gui::Button* pBtn);
    bool onAnswer(td::UINT4 questionID, gui::Alert::Answer answer);
    void SetCurrentSubject();
};
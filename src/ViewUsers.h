#pragma once
#include <algorithm>
#include <vector>
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
#include <td/Types.h>
#include "Globals.h"

//enum class QuestionID : td::UINT2 { Save };
class ViewUsers : public gui::View
{
private:
protected:
    // first row
    gui::Label _lblID;
    gui::NumericEdit _id;
    gui::Label _lblName;
    gui::LineEdit _name;
    gui::Label _lblSurname;
    gui::LineEdit _surname;
    gui::Label _lblEnrollDate;
    gui::DateEdit _enrollDate;
    gui::Label _lblJmbg;
    gui::LineEdit _jmbg;
    gui::Label _lblAddress;
    gui::LineEdit _address;
    gui::Label _lblDateofbirth;
    gui::DateEdit _dateofbirth;
    gui::Label _lblRole;
    gui::DBComboBox _role;
    gui::Label _lblIndex;
    gui::LineEdit _index;
    gui::TableEdit _table;
    td::String _noChngIndex;

    td::Date _dateEnroll;

    //labels for the bottom row
    gui::HorizontalLayout _hlBtnsDB;
    gui::Button _btnSave;
    gui::Button _btnReload;
    gui::Button _btnDelete;
    gui::Button _btnUpdate;
    gui::Button _btnPushBack;
    gui::GridLayout _gl;
    dp::IDataSetPtr _pDS;
    
    //Vector used for logins
    std::vector<td::INT4> InitialUserIds;


public:
    ViewUsers();
protected:
    void populateRoleCombo(gui::DBComboBox& combo);
    void populateData();
    bool onChangedSelection(gui::TableEdit* pTE);
    void populateDSRow(dp::IDataSet::Row& row);
    bool doesIDexist(td::INT4 id);
    bool doesIndexExist(td::Variant index);
    bool doesJMBGExist(td::Variant jmbg);
    bool onClick(gui::Button* pBtn);
    bool saveUsers();
    bool saveUserRoles();
    bool onAnswer(td::UINT4 questionID, gui::Alert::Answer answer);
    bool onChangedSelection(gui::DBComboBox* pCmb);
    void initTable();
    //New functions for creating UserLogins
    bool CreateUserLogin(td::String surname, td::INT4 UserID);
    bool DeleteUserLogins(std::vector<td::INT4> &UserIds);
    bool DoesLoginExist(td::INT4 UserID);
    void GetInitialIds( std::vector<td::INT4> &Ids);

};

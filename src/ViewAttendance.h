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
#include "ViewSubject.h"

class ViewAttendance : public gui::View
{
private:
protected:
    gui::Label _LblSubjName;
    gui::LineEdit _Subject;
    
    gui::LineEdit _day;
     
    gui::Label _LblDay;
    gui::ComboBox _dayCombo;
  
    gui::Label _LblTime;
    gui::TimeEdit _time;
    
    td::Time _Time;
    
    gui::Label _LblType;
    gui::DBComboBox _type;
    
    gui::Label _LblMaxNum;
    gui::NumericEdit _maxNum;
   
    
    gui::HorizontalLayout _hlBtnsDB;
    gui::Button _btnAdd;
    gui::Button _btnDelete;
    gui::Button _btnUpdate;
    gui::Button _btnReport;

   // gui::Button _btnSave;
    
    gui::GridLayout _gl;
    dp::IDataSetPtr _pDS;
    
    td::INT4 _SubjectID;
    td::Time LastMsgTime;
    td::Date LastMsgDate;
    
    gui::TableEdit _table;
    
    gui::Image _imgClassAtt;
    ViewSubject* _subject;

public:
    ViewAttendance(td::INT4 SubjectID, ViewSubject* subject);
    void AttendanceReport(const gui::Image* pImage);
protected:
    void populateRoleCombo(gui::DBComboBox &combo);
    void populateDayCombo(gui::ComboBox& combo);
    void populateData();
    bool onChangedSelection(gui::TableEdit* pTE);
    bool onChangedSelection(gui::ComboBox* pCmb) override;
    void populateDSRow(dp::IDataSet::Row& row);
    bool doesItDexist(td::Variant day, td::Time time);
    bool onClick(gui::Button* pBtn);
    void saveData();
    void saveDataUpdate();
    void SetCurrentSubject();
    //void SendAddMsg();
    void SendMsg(td::INT4 MsgType);
    bool CheckTime();
};



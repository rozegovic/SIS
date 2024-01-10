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

class ViewTimeSlot : public gui::View
{
private:
protected:
    gui::Label _LblSubjName;
    gui::LineEdit _Subject;

   // gui::Label _LblType;
   // gui::DBComboBox _type;

    gui::HorizontalLayout _hlBtnsDB;
    gui::Button _btnEnroll;
    gui::Button _btnDEnroll;
    gui::Button _btnReload;

    gui::GridLayout _gl;
    dp::IDatabase* _db;
    dp::IDataSetPtr _pDS = nullptr;
    dp::IDataSetPtr _pDSpos;
    td::INT4 _SubjectID;

    gui::TableEdit _table;
public:
    ViewTimeSlot(td::INT4 SubjectID);
    ~ViewTimeSlot();

protected:
    void initTable();
  //  virtual bool onClick(gui::Button* pBtn);
    void populateDataForTable();
    void getSubjectName();
    bool IsEnrolled(td::INT4 ID_stud, td::INT4 ID_Pred);
    bool saveData1();
    bool saveData2();
    virtual bool onClick(gui::Button* pBtn);
  //  virtual bool onChangedSelection(gui::TableEdit* pTE);

};
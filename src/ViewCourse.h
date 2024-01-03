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


enum class QuestionIDDD : td::UINT2 { Saveee };

class ViewCourse: public gui::View
{
protected:

    gui::Label _lblName;
    gui::LineEdit _name;
    gui::Label _lblIDCode;
    gui::LineEdit _idCode;
    gui::Label _lblIDDEP;
    gui::DBComboBox _idDep;
    gui::Label _lblSemester;
    gui::NumericEdit _semester;
    gui::Label _lblEcts;
    gui::NumericEdit _ects;
    gui::Label _lblElective;
    gui::DBComboBox _elective;      //----------1  izborni, 0 obavezni

    gui::Label _lblMaxNum;
    gui::NumericEdit _maxNum;

    gui::TableEdit _table;
    gui::HorizontalLayout _hlBtns;
    gui::Button _btnSave;
    gui::Button _btnAdd;
    gui::Button _btnReload;
    //gui::Button _btnRemoveAll;
    gui::Button _btnDelete;
    gui::Button _btnUpdate;
    gui::Button _btnInsert;
	
    gui::GridLayout _gl;

    dp::IDatabase* _db;
    dp::IDataSetPtr _pDS=nullptr;
    td::String _sifra;

    std::vector<td::INT4> _itemsToDelete, _itemsToInsert, _itemsToUpdate;
    td::INT4 findMaxID();  // funkcija za trazenje slobodnog id, nema potrebe da iko osim klase ima pristup njoj
public:
    ViewCourse();
    ~ViewCourse();     

protected:
    void initTable(int type);
    td::INT4 getIDfromTable(int rowID);
    bool canDelete(int iRow);
    bool eraseCourse();
    bool saveData();
    bool canAdd();
    bool existsInCourse(td::INT4 id);
    bool canUpdate(int iRow);  
    void populateData();
	bool updateCourse();
    bool loadComboBox(td::String select, gui::DBComboBox& combo);
    virtual bool onChangedSelection(gui::TableEdit* pTE);
    void populateDSRow(dp::IDataSet::Row& row, td::INT4 i);
    virtual bool onClick(gui::Button * pBtn);
    bool insertCourse();
    bool onAnswer(td::UINT4 questionID, gui::Alert::Answer answer);
};

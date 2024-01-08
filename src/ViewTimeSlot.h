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
    gui::Label _LblDate;
    gui::DateEdit _date;

    gui::Label _LblTime;
    gui::TimeEdit _time;

    gui::Label _LblType;
    gui::DBComboBox _type;

    gui::HorizontalLayout _hlBtnsDB;
    gui::Button _btnAdd;
    gui::Button _btnDelete;
    // gui::Button _btnSave;

    gui::GridLayout _gl;
    dp::IDataSetPtr _pDS;

    td::INT4 _SubjectID;

    gui::TableEdit _table;
public:
    ViewTimeSlot(td::INT4 SubjectID);
protected:
};
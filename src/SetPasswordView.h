//
//  SetPasswordView.h
//  appSIS
//
//  Created by Amina Hajrić on 15.01.2024..
//
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
#include <td/Types.h>
#include <gui/PasswordEdit.h>

class SetPasswordView : public gui::View
{
    
    gui::Label _header;
    
    gui::Label _lblPwrd;
    gui::PasswordEdit _pwrd;
    gui::Label _lblRepeatPwrd;
    gui::PasswordEdit _repeatPwrd;
    
    gui::Button _btnSave;
    
    gui::HorizontalLayout _headerLayout;
    gui::GridLayout _gl;
    
    dp::IDatabase* _db;
public :
    SetPasswordView();
    ~SetPasswordView();
protected:
    bool onClick(gui::Button* pBtn);
    bool SaveData();
    bool CompareText();
    
};


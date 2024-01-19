//
//  SetPasswordView.cpp
//  appSIS
//
//  Created by Amina Hajrić on 15.01.2024..
//

#pragma once
#include "SetPasswordView.h"
#include "Globals.h"
#include <gui/Window.h>

SetPasswordView::SetPasswordView()
:_lblPwrd(tr("NewPwrd"))
,_lblRepeatPwrd(tr("RepeatPwrd"))
,_header(tr("HNewPwrd"))
,_btnSave(tr("save"))
, _gl(5, 4)
, _headerLayout(3)
,_db(dp::getMainDatabase())
{
    _header.setAsReadOnly();
    _header.setBold();
    
    _btnSave.setType(gui::Button::Type::Constructive);
    
    
    _headerLayout.appendSpacer();
    _headerLayout.append(_header);
    _headerLayout.appendSpacer();
    
    gui::GridComposer gc(_gl);
    
    gc.appendRow(_headerLayout, 0);
    

    gc.appendRow(_lblPwrd);
    gc.appendCol(_pwrd);
    
    gc.appendRow(_lblRepeatPwrd);
    gc.appendCol(_repeatPwrd);
    
    gc.appendRow(_btnSave,0);
    gui::View::setLayout(&_gl);
    
}
SetPasswordView::~SetPasswordView(){
    
}
bool SetPasswordView::onClick(gui::Button* pBtn){
    if(pBtn == &_btnSave){
        td::String p1, p2;
        p1 = _pwrd.getText();
        p2 = _repeatPwrd.getText();
        if(p1 == td::String ("")){
            gui::NatObject::showAlert(tr("alert"), tr("NoPwrd"));
            return false;
            
        }
        if(p1 == td::String ("default")){
            gui::NatObject::showAlert(tr("alert"), tr("DefaultPwrd"));
            return false;
        }
        if(p1 == p2){
            SaveData();
            gui::NatObject::showAlert(tr("PwrdOK"), tr("CloseWnd"));
       auto wnd = getParentWindow();
        wnd->close();
            return true;
        }
        else
            gui::NatObject::showAlert(tr("alert"), tr("PwrdErr"));
    
    }
    return false;
}
bool SetPasswordView::CompareText(){
    return true;
}
bool SetPasswordView::SaveData(){
    td::String pass = _pwrd.getText();
    dp::IStatementPtr pInsStat(dp::getMainDatabase()->createStatement("UPDATE UserLogin SET Password = ? WHERE ID = ?"));
    dp::Params parDS(pInsStat->allocParams());
    parDS << dp::toNCh(pass, 30) << Globals::_currentUserID;
    dp::Transaction tr(dp::getMainDatabase());
    
    if(!pInsStat->execute())
        return false;
    if(!tr.commit())
        tr.rollBack();
    return true;
}

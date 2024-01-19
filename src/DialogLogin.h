
#pragma once
#include <gui/Dialog.h>
#include "ViewLogin.h"
#include <dp/IDatabase.h>
#include "Globals.h"
#include "ViewIDs.h"

#include "SetPasswordWindow.h"
#include "WindowMessages.h"
class  DialogLogin : public gui::Dialog
{
    
protected:
    ViewLogin _viewLogin;
    
    bool onClick(Dialog::Button::ID btnID, gui::Button* pButton) override
    {

        if (btnID == Dialog::Button::ID::OK)
        {
            td::String username = _viewLogin.getUserName();
            td::String pwrd = _viewLogin.getPassword();
            td::INT4 userID;
            td::String passDB;

            dp::IStatementPtr pSel(dp::getMainDatabase()->createStatement("select ID, Password from UserLogin a where a.Username = ?"));
            dp::Params pParams(pSel->allocParams());
            pParams << dp::toNCh(username, USERNAME_LEN);

            dp::Columns pCols = pSel->allocBindColumns(2);
            pCols << "ID" << userID << "Password" << passDB;
            if (!pSel->execute())
            {
                showAlert(tr("WrongPass"), tr("EnterCorrPass"));
                return false;

            }
            if (!pSel->moveNext())
            {
                showAlert(tr("WrongPass"), tr("EnterCorrPass"));
                return false;
            }
            if (pwrd != passDB)
            {
                showAlert(tr("WrongPass"), tr("EnterCorrPass"));
                return false;
            }

            Globals::_currentUserID = userID;
            td::INT4 userRole;
            dp::IStatementPtr pSel1(dp::getMainDatabase()->createStatement("select PozicijaID as RoleID from Korisnici a where a.ID = ?"));
            dp::Params pParams1(pSel1->allocParams());
            pParams1 << userID;
            dp::Columns pCols1 = pSel1->allocBindColumns(1);
            pCols1 << "RoleID" << userRole;
            if (!pSel1->execute())
            {
                showAlert(tr("RoleErr"), tr("RoleErrTxt"));
                return false;

            }
            if (!pSel1->moveNext())
            {
                showAlert(tr("alert"), tr("RoleErr"));
                return false;
            }
            //Otvaranje prozora za prvi sign in
            if(pwrd == td::String("default")){
                gui::Window* pParentWnd = getParentWindow();
                auto pWnd = new SetPasswordWindow (pParentWnd, 2);
                pWnd->keepOnTopOfParent();
                pWnd->open();


            }
            Globals::_currentUserRole = userRole;

            if (Globals::_currentUserRole == 6) {
                Globals::isAdmin = true;
             
            }
            else  if (Globals::_currentUserRole == 1) {
              
                Globals::isProfessor = true;
            }
            else if (Globals::_currentUserRole == 3) {
              
                Globals::isAssistant = true;
           
            }
            else if (Globals::_currentUserRole == 4) {
              
                Globals::isSAO = true;
            }
            else if (Globals::_currentUserRole == 5) {
             
                Globals::isStudent = true;
               
            }

        }
      
        return true;
    }
public:
    DialogLogin(gui::Frame* pView)
        : gui::Dialog(pView, { {gui::Dialog::Button::ID::OK, tr("Ok"), gui::Button::Type::Default},
                                {gui::Dialog::Button::ID::Cancel, tr("Cancel")} }, gui::Size(300, 100))
    {
        setCentralView(&_viewLogin);
    }
    ~DialogLogin(){}
};

#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/PasswordEdit.h>
#include <gui/GridLayout.h>

class ViewLogin : public gui::View
{
private:
protected:
    gui::Label _lblUn;
    gui::LineEdit _editUN;
    gui::Label _lblPwd;
    gui::PasswordEdit _editPwd;
    gui::GridLayout _gl;
public:
    ViewLogin()
        : _lblUn(tr("username"))
        , _editUN(gui::LineEdit::Messages::Send)
        , _lblPwd(tr("password"))
        , _editPwd(gui::LineEdit::Messages::Send)
        , _gl(2, 2)
    {

        _editPwd.setToolTip(tr("Enter password:"));
        _gl.insert(0, 0, _lblUn);
        _gl.insert(0, 1, _editUN);
        _gl.insert(1, 0, _lblPwd);
        _gl.insert(1, 1, _editPwd);

        gui::View::setLayout(&_gl);
    }

    td::String getUserName() const
    {
        return _editUN.getText();
    }

    td::String getPassword() const
    {
        return _editPwd.getText();
    }
};

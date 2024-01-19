//
//  SetPasswordWindow.h
//  appSIS
//
//  Created by Amina Hajrić on 15.01.2024..
//
#pragma once
#include <gui/Window.h>
#include "SetPasswordView.h"



class SetPasswordWindow : public gui::Window
{
protected:
    SetPasswordView _viewPwrd;
public:
    SetPasswordWindow(gui::Window* parentWnd, td::UINT4 wndID = 0)
        : gui::Window(gui::Size(800, 600), parentWnd, wndID)
    {
        setTitle(tr("SetPassword"));
        setCentralView(&_viewPwrd);
    }

    ~SetPasswordWindow()
   {
        
    }
};

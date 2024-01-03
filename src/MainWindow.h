//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "DynTabView.h"
#include "Globals.h"
#include "NavigatorView.h"

class MainWindow : public gui::Window
{
private:
protected:
    MenuBar _mainMenuBar;
    DynTabView _mainView;
    gui::Image _imgUsers;
    gui::Image _imgDepartment;
    gui::Image _imgCourse;
    gui::Image _imgClassroom;
    gui::Image _imgTStaff;
    gui::Image _imgActivity;
    gui::Image _imgEnroll;
    gui::Image _imgAttendance;
    gui::Image _imgCurr;
    gui::Image _imgExamAtt;
    gui::Image _imgTicket;
    gui::Image _imgCourseenr;



protected:
    void onInitialAppearance() override
    {
        showLogin();
    }

    
public:
    MainWindow();
public:
    void showLogin();
    bool showSubjectChoose();
    bool showMySubjectChoose();
    bool showAllSubjectChoose();
    bool showSubjectChooseActivty();


protected:
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override;
    bool focusOnViewPositionWithID(td::UINT4 viewID);
    bool showUsersView();
    bool showDepartmentsView();
    bool showCoursesView();
    bool showClassroomView();
    bool showTStaffView(td::INT4 SubjectID);
    bool showActivityView();
    bool showActivityView(td::INT4 SubjectID);
    bool showEnrollView();
    bool showAttendanceView(td::INT4 SubjectID);
    bool showCurriculumView();
    bool showExamSignUpView();
    bool showTicketView();
    bool showCourseEnrollView();
    //bool onClick(gui::Dialog* pDlg, td::UINT4 dlgID) override;



};

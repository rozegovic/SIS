#pragma once
#include <gui/ViewSwitcher.h>
#include "ViewAttendance.h"
#include "ViewSubject.h"


class SwitcherView : public gui::ViewSwitcher
{
private:
protected:
    ViewAttendance _viewAttendance;
    ViewSubject _viewSubject;

public:
    SwitcherView(td::UINT4 subjectid)
        : gui::ViewSwitcher(2),
        _viewAttendance(subjectid, &_viewSubject),
        _viewSubject(subjectid)

    {
        addView(&_viewAttendance, true);
        addView(&_viewSubject);

    }
    //void loadData()
    //{
      //  _viewAttendance.loadData();
    //}
};


#pragma once
#include <gui/ViewSwitcher.h>
#include "ViewAttendance.h"
#include "ViewSubject.h"
#include "ViewTimeSlot.h"


class SwitcherView : public gui::ViewSwitcher
{
private:
protected:
    ViewAttendance _viewAttendance;
    ViewSubject _viewSubject;
  //  ViewTimeSlot _viewTimeSlot;

public:
    SwitcherView(td::UINT4 subjectid)
        : gui::ViewSwitcher(3),
        _viewAttendance(subjectid),
        _viewSubject(subjectid)
     //   _viewTimeSlot(subjectid)

    {
        addView(&_viewAttendance, true);
        addView(&_viewSubject);
    //    addView(&_viewTimeSlot);

    }
    //void loadData()
    //{
      //  _viewAttendance.loadData();
    //}
};


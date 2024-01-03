#pragma once
#include <gui/ViewSwitcher.h>
#include "ViewActivity.h"
#include "ViewSubject.h"


class SwitcherViewActivity : public gui::ViewSwitcher
{
private:
protected:
    ViewActivity _viewActivity;
    ViewSubject _viewDateTime;

public:
    SwitcherViewActivity(td::UINT4 subjectid)
        : gui::ViewSwitcher(2)
        , _viewActivity(subjectid)

    {
        addView(&_viewActivity, true);
        addView(&_viewDateTime);

    }
};


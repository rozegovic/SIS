#pragma once
#include <gui/ViewSwitcher.h>
#include "ViewActivity.h"
#include "ViewSubject.h"
#include "ViewDateTimeActivity.h"


class SwitcherViewActivity : public gui::ViewSwitcher
{
private:
protected:
    ViewActivity _viewActivity;
    ViewDateTimeActivity _viewDateTime;

public:
    SwitcherViewActivity(td::UINT4 subjectid)
        : gui::ViewSwitcher(1)
        , _viewActivity(subjectid)

    {
        addView(&_viewActivity, true);
        addView(&_viewDateTime);

    }
};


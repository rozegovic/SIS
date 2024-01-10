#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/GridLayout.h>
#include <gui/DBComboBox.h>




class ViewChooseSubjectForTimeSlot : public gui::View
{
private:
protected:

    gui::Label _lblSubj;
    gui::DBComboBox _subject;
    gui::GridLayout _gl;


public:
    ViewChooseSubjectForTimeSlot()
        : _lblSubj(tr("Subject"))
        , _subject(td::int4)
        , _gl(1, 2)
    {


        _gl.insert(0, 0, _lblSubj);
        _gl.insert(0, 1, _subject);


        gui::View::setLayout(&_gl);
    }
    gui::DBComboBox& getComboBox()
    {
        return _subject;

    }
    td::INT4& getSubjectID()
    {
        td::Variant val;
        _subject.getValue(val);
        return val.i4Val();

    }

};

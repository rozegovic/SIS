#include "ViewTimeSlot.h"
#include "Globals.h"
#include "ViewIDs.h"
#include <td/StringConverter.h>

ViewTimeSlot::ViewTimeSlot(td::INT4 SubjectID) :
    _LblSubjName(tr("AttSubj"))

    , _LblType(tr("AttType")),
    _hlBtnsDB(5)
    , _btnDEnroll(tr("DEnroll"))
    , _btnReload(tr("Reload"))
    , _btnEnroll(tr("Enroll"))
    
    , _type(td::int4)
    , _gl(4, 4)
    , _SubjectID(SubjectID)
{


    _hlBtnsDB.appendSpacer();

    _hlBtnsDB.append(_btnDEnroll);
    _hlBtnsDB.append(_btnReload);
    _hlBtnsDB.append(_btnEnroll);
    _hlBtnsDB.appendSpacer();

    _btnDEnroll.setType(gui::Button::Type::Default);
    _btnEnroll.setType(gui::Button::Type::Constructive);

    _Subject.setAsReadOnly();

    gui::GridComposer gc(_gl);


    gc.appendRow(_LblSubjName);
    gc.appendCol(_Subject);

    gc.appendRow(_LblType);
    gc.appendCol(_type);

    gc.appendRow(_table, 0);
    gc.appendRow(_hlBtnsDB, 0);

    gui::View::setLayout(&_gl);

  //  populateRoleCombo(_type);
  //  populateData();

  /*_table.init(_pDS, {0, 1, 2});
    if (_pDS->getNumberOfRows())
    {
        _table.selectRow(0, true);
    }*/
}
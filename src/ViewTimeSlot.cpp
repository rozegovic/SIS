#include "ViewTimeSlot.h"
#include "Globals.h"
#include "ViewIDs.h"
#include <td/StringConverter.h>

ViewTimeSlot::ViewTimeSlot(td::INT4 SubjectID) :
    _LblSubjName(tr("AttSubj")),
    _LblDate(tr("AttDate")),
    _LblTime(tr("AttTime"))
    , _LblType(tr("AttType"))
    , _btnAdd(tr("add"), tr("AddTT"))
    , _btnDelete(tr("Delete"), tr("DeleteTT"))
    //,_btnSave(tr("Save"), tr("SaveTT"))
    , _type(td::int4)
    , _hlBtnsDB(5)
    , _gl(6, 2)
    , _SubjectID(SubjectID)
{


    _hlBtnsDB.appendSpacer();
    _hlBtnsDB.append(_btnDelete);
    //   _hlBtnsDB.append(_btnSave);
    _hlBtnsDB.append(_btnAdd);
    _hlBtnsDB.appendSpacer();

    //_btnSave.setType(gui::Button::Type::Default);
    _btnDelete.setType(gui::Button::Type::Destructive);
    _btnAdd.setType(gui::Button::Type::Constructive);

   // SetCurrentSubject();
    _Subject.setAsReadOnly();

    gui::GridComposer gc(_gl);


    gc.appendRow(_LblSubjName);
    gc.appendCol(_Subject);

    gc.appendRow(_LblDate);
    gc.appendCol(_date);

    gc.appendRow(_LblTime);
    gc.appendCol(_time);

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
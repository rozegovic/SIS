#pragma once
#include "ViewClassroom.h"
#include "ViewIDs.h"


ViewClassroom::ViewClassroom()
    : _lblName(tr("name"))
    , _lblLocation(tr("Location"))
    , _lblType(tr("Type"))
    , _type(td::int4)
    //, _lblTip(tr("Type"))
    , _lblNumSeats(tr("NumSeats"))
    , _numSeats(td::int4)
    , _hlBtnsDB(5)
    , _btnSave(tr("Save"), tr("SaveTT"))
    , _btnReload(tr("Reload"), tr("ReloadTT"))
    , _btnDelete(tr("Delete"), tr("DeleteTT"))
    , _btnUpdate(tr("Update"), tr("UpdateTT"))
    , _btnPushBack(tr("PushBack"), tr("PushBackTT"))
    , _gl(6, 2)
    //, dp::getMainDatabase()(dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3))
{
    setVisualID(View_CLASSROOM);
    // buttons in last row
   // _hlBtnsDB.append(_btnSave);
    _hlBtnsDB.appendSpacer();
    //_hlBtnsDB.append(_btnReload);
    //_hlBtnsDB.appendSpace(20);
    _hlBtnsDB.append(_btnDelete);
    _hlBtnsDB.append(_btnUpdate);
    _hlBtnsDB.append(_btnPushBack);
    _hlBtnsDB.appendSpacer();
    _btnUpdate.setType(gui::Button::Type::Default);
    _btnDelete.setType(gui::Button::Type::Destructive);
    _btnPushBack.setType(gui::Button::Type::Constructive);

    gui::GridComposer gc(_gl);
    gc.appendRow(_lblName);
    gc.appendCol(_name);

    gc.appendRow(_lblLocation);
    gc.appendCol(_location);

    gc.appendRow(_lblNumSeats);
    gc.appendCol(_numSeats);

    gc.appendRow(_lblType);
    gc.appendCol(_type);

    gc.appendRow(_table, 0);
    gc.appendRow(_hlBtnsDB, 0);

    gui::View::setLayout(&_gl);

    // connect to database 
    //fo::fs::path homePath;
    //mu::getHomePath(homePath);
    //fo::fs::path testDBPath = (homePath / "Work/CPProjects/v2/Baza2.db");
    //if (!dp::getMainDatabase()->connect(testDBPath.string().c_str()))
    //{
    //    assert(false);
    //}

     //populate role combo
    populateTypeCombo(_type);
    populateData();

    _table.init(_pDS, { 0,1,3,4 });
    if (_pDS->getNumberOfRows())
    {
        _table.selectRow(0, true);
    }
}

void ViewClassroom::populateTypeCombo(gui::DBComboBox &combo)
{
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT ID, Type FROM ClassRoles");
    dp::Columns pCols = pSelect->allocBindColumns(2);
    td::String tip;
    td::INT4 id;
    pCols << "ID" << id << "Type" << tip;
    pSelect->execute();

    while (pSelect->moveNext())
    {
        combo.addItem(tip, id);
    }
    combo.selectIndex(0);
}

void ViewClassroom::populateData()
{
   auto pDB = dp::getMainDatabase();
      _pDS = pDB->createDataSet("SELECT  Name , Location , ClassRoles.ID as ClassID, ClassRoles.Type as Type, NumSeats FROM Classrooms, ClassRoles where Classrooms.Type =ClassRoles.ID", dp::IDataSet::Execution::EX_MULT);
    dp::DSColumns cols(_pDS->allocBindColumns(5));
    cols << "Name" << td::string8 << "Location" << td::string8 << "ClassID" << td::int4 << "Type" << td::string8 << "NumSeats" << td::int4;

    if (!_pDS->execute())
    {
        _pDS = nullptr;
        return;
    }
}
bool ViewClassroom::onChangedSelection(gui::TableEdit* pTE) {
    if (pTE == &_table) {
        int iRow = _table.getFirstSelectedRow();
        if (iRow < 0) {
            return true;
        }
        td::Variant val;
        dp::IDataSet* pDS = _table.getDataSet();
        auto& row = pDS->getRow(iRow);

        val = row[0];
        _name.setValue(val);

        val = row[1];
        _location.setValue(val);

        val = row[2];
        _type.setValue(val);

        val = row[4];
        _numSeats.setValue(val);

        return true;
    }
    return false;
}
void ViewClassroom::populateDSRow(dp::IDataSet::Row& row)
{
    td::Variant val;

    _name.getValue(val);
    row[0].setValue(val);

    _location.getValue(val);
    row[1].setValue(val);

    _numSeats.getValue(val);
    row[4].setValue(val);

    _type.getValue(val);
    row[2] = val.i4Val();
    row[3].setValue(_type.getSelectedText());

}
void ViewClassroom::saveData()
{
     dp::IStatementPtr pInsStat(dp::getMainDatabase()->createStatement("INSERT INTO Classrooms (Name, Location, Type, NumSeats) VALUES(?,?,?,?)"));
    dp::Params parDS(pInsStat->allocParams());
    td::INT4 numSeats, type;
    td::String name, location;
    parDS << dp::toNCh(name, 30) << dp::toNCh(location, 30) << type << numSeats;
    dp::Transaction tr(dp::getMainDatabase());

    dp::IStatementPtr pDel(dp::getMainDatabase()->createStatement("DELETE FROM Classrooms"));
    if (!pDel->execute())
        return;
    size_t nRows = _pDS->getNumberOfRows();
    for (size_t i = 0; i < nRows; ++i)
    {
        auto row = _pDS->getRow(i);
        name = row[0];
        location = row[1];
        type = row[2].i4Val();
        numSeats = row[4].i4Val();

        if (!pInsStat->execute())
            return;
    }
    tr.commit();
    return;
}

bool ViewClassroom::onClick(gui::Button* pBtn)
{
    if (pBtn == &_btnReload)
    {
        _table.reload();
        _table.selectRow(0, true);
        return true;
    }

    if (pBtn == &_btnDelete)
    {
        int iRow = _table.getFirstSelectedRow();
        if (iRow < 0)
            return true;
        _table.beginUpdate();
        _table.removeRow(iRow);
        _table.endUpdate();
        saveData();
        _table.reload();
        _table.selectRow(0, true);
        return true;
    }
    if (pBtn == &_btnUpdate)
    {
        td::Variant val;
        _name.getValue(val);
        if (!doesIDexist(val))
        {
            showAlert(tr("alert"), tr("alertU"));
            return true;
        }
        int iRow = _table.getFirstSelectedRow();
        if (iRow < 0)
            return true;
        _table.beginUpdate();
        auto& row = _table.getCurrentRow();
        populateDSRow(row);
        _table.updateRow(iRow);
        _table.endUpdate();
        saveData();
        _table.reload();
        _table.selectRow(0, true);
        return true;
    }

    if (pBtn == &_btnPushBack)
    {
        td::Variant val;
        _name.getValue(val);
        if (doesIDexist(val))
        {
            showAlert(tr("alert"), tr("alertPB"));
            return true;
        }
        _table.beginUpdate();
        auto& row = _table.getEmptyRow();
        populateDSRow(row);
        _table.push_back();
        _table.endUpdate();
        saveData();
        _table.reload();
        return true;
    }

    if (pBtn == &_btnSave)
    {

        //showYesNoQuestionAsync(QuestionID::Save, this, tr("alert"), tr("saveSure"), tr("Yes"), tr("No"));

        showYesNoQuestionAsync(tr("alert"), tr("saveSure"), tr("Yes"), tr("No"), [this](gui::Alert::Answer answer)
            {
                if (answer == gui::Alert::Answer::Yes)
                    saveData();
            });
        //return true;
    }

    return false;
}
//bool ViewClassroom::onAnswer(td::UINT4 questionID, gui::Alert::Answer answer)
//{
//    if ((QuestionID)questionID == QuestionID::Save)
//    {
//        if (answer == gui::Alert::Answer::Yes)
//            saveData();
//        return true;
//    }
//    return false;
//}
bool ViewClassroom::doesIDexist(td::Variant name)
{
    size_t nRows = _pDS->getNumberOfRows();
    for (size_t i = 0; i < nRows; ++i)
    {
        auto row = _pDS->getRow(i);
        if (row[0] == name)
            return true;
    }
    return false;
}
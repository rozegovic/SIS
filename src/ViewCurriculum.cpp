#pragma once
#include "ViewCurriculum.h"
#include "ViewIDs.h"


ViewCurriculum::ViewCurriculum() : _db(dp::getMainDatabase())
    , _lblDepartment(tr("Department"))
    , _lblSemester(tr("Semester"))
    , _semester(td::int4)
    , _lblCourse(tr("Course"))
    , _lblShortname(tr("Shortname"))
    , _lblECTS(tr("ECTS"))
    , _ECTS(td::int4)
    , _department(td::int4)
    , _course(td::int4)
    , _shortname(td::int4)
    , _hlBtnsDB(8)
    , _btnSave(tr("Save"), tr("SaveTT"))
    , _btnDelete(tr("Delete"), tr("DeleteTT"))
    , _btnUpdate(tr("Update"), tr("UpdateTT"))
    , _btnPushBack(tr("PushBack"), tr("PushBackTT"))
    , _gl(5, 4)
{
    setVisualID(View_CURRICULUM);
    // buttons in last row
    _hlBtnsDB.append(_btnSave);
    _hlBtnsDB.appendSpacer();
    _hlBtnsDB.appendSpace(2);
    _hlBtnsDB.append(_btnDelete);
    _hlBtnsDB.append(_btnUpdate);
    _hlBtnsDB.append(_btnPushBack);
    //_btnSave.setType(gui::Button::Type::Default);
    //_btnDelete.setType(gui::Button::Type::Destructive);
    //_btnPushBack.setType(gui::Button::Type::Constructive);

    gui::GridComposer gc(_gl);
    gc.appendRow(_lblDepartment);
    gc.appendCol(_department);
    gc.appendCol(_lblSemester);
    gc.appendCol(_semester);

    gc.appendRow(_lblCourse);
    gc.appendCol(_course);
    gc.appendCol(_lblShortname);
    gc.appendCol(_shortname);

    gc.appendRow(_lblECTS);
    gc.appendCol(_ECTS);

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

    // populate role combo
   // populateRoleCombo(_type);
    populateData();

    loadComboBox("select ID_Smjera as ID, Naziv_Smjera as Name from Smjer", _department);
    loadComboBox("select ID_Predmeta as ID, Naziv_Predmeta as Name from Predmet", _course);
    loadComboBox("select ID_Predmeta as ID, Sifra_Predmeta as Name from Predmet", _shortname);
}


ViewCurriculum::~ViewCurriculum()
{
    if (_pDS)
        _pDS->release();
}
td::INT4 ViewCurriculum::getIDfromTable(int rowID)
{
    return td::INT4();
}
bool ViewCurriculum::canDelete(int iRow)
{
    return false;
}
bool ViewCurriculum::eraseCurriculum()
{
    td::INT4 id;
    dp::IStatementPtr pDeleteItem(_db->createStatement("delete from Predmet where ID_Predmeta = ?"));
    dp::Params pParams2(pDeleteItem->allocParams());
    pParams2 << id;

    for (auto itd : _itemsToDelete)
    {
        id = itd;
        if (!pDeleteItem->execute())
        {
            return false;
        }
    }
    return true;
}

bool  ViewCurriculum::saveData()
{
    dp::Transaction tran(_db);
    if (!eraseCurriculum())
        return false;

    if (!insertCurriculum())
        return false;

    if (tran.commit())
    {
        _itemsToDelete.clear();
        _itemsToInsert.clear();
        _itemsToUpdate.clear();
    }
    return true;
}

bool ViewCurriculum::existsInDepartment(td::INT4 id)
{
    //KOD ZA EXISTS IN TRANSACTION POTREBNO PREPRAVITI ZA EXISTS IN DEPARTMENT (DA LI COURSE POSTOJI VEC)


    dp::IStatementPtr pSel(_db->createStatement("select count(1) as Cnt from Predmet a where a.ID_Predmeta = ?"));
    dp::Params pParams(pSel->allocParams());
    pParams << id;

    dp::Columns pCols = pSel->allocBindColumns(1);
    td::INT4 cnt;
    pCols << "Cnt" << cnt;
    if (!pSel->execute())
        return false;
    pSel->moveNext();
    return (cnt > 0);
    return false;
}

bool ViewCurriculum::canUpdate(int iRow)
{
    return false;
}

void ViewCurriculum::populateData()
{

    _pDS = _db->createDataSet("select n.ID_Smjera a.Naziv_Smjera, n.Naziv_Predmeta, n.Sifra_Predmeta, n.ECTS_bodovi from Predmet n, Smjer a where n.ID_Smjera = a.ID_Smjera ", dp::IDataSet::Execution::EX_MULT);
   // dp::Params params(_pDS->allocParams());
   // params << _paramFrom << _paramTo;

    //specify columns to obtain from the data provider
    dp::DSColumns cols(_pDS->allocBindColumns(5));
    cols << "ID_Smjera" << td::int4 << "Naziv_Smjera" << td::string8 << "Naziv_Predmeta" << td::string8 << "Sifra_Predmeta" << td::string8
        << "ECTS" << td::int4;

    if (!_pDS->execute())
    {
        //show log
        _pDS = nullptr;
        return;
    }

    _table.init(_pDS, 0);
}

bool ViewCurriculum::updateCourse()
{
    return false;
}


bool ViewCurriculum::loadComboBox(td::String select, gui::DBComboBox& combo)
{
    dp::IStatementPtr pSelect = _db->createStatement(select.c_str());
    dp::Columns pCols = pSelect->allocBindColumns(2);
    td::String name;
    td::INT4 id;
    pCols << "ID" << id << "Name" << name;
    if (!pSelect->execute())
        return false;

    while (pSelect->moveNext())
    {
        combo.addItem(name, id);
    }
    combo.selectIndex(0);
    return true;
}


bool ViewCurriculum::onChangedSelection(gui::TableEdit* pTE)
{
    if (pTE == &_table)
    {
        int iRow = _table.getFirstSelectedRow();
        if (iRow < 0)
        {
            _department.toZero();
            _semester.toZero();
            _course.toZero();
            _shortname.toZero();
            _ECTS.toZero();
            return true;
        }
        /*
        rows moraju odgovarati

        dp::IDataSet* pDS = _table.getDataSet();
        auto& row = pDS->getRow(iRow);
        _id.setValue(row[0]);
        _name.setValue(row[1]);
        _idCode.setValue(row[6]);
        _year.setValue(row[2].i4Val());
        _ects.setValue(row[3].i4Val());
        _teachingStaff.setValue();*/


        return true;
    }
    return false;
}

void ViewCurriculum::populateDSRow(dp::IDataSet::Row& row)
{
    td::Variant val;
    _department.getValue(val);
    row[0].setValue(val);
    _semester.getValue(val);
    row[1].setValue(val);
    _course.getValue(val);
    row[2].setValue(val);
    _shortname.getValue(val);
    row[3].setValue(val);
    _ECTS.getValue(val);
    row[4].setValue(val);
}

bool ViewCurriculum::onClick(gui::Button* pBtn)
{
    if (pBtn == &_btnDelete)
    {
        int iRow = _table.getFirstSelectedRow();
        if (iRow < 0)
            return true;
        _table.beginUpdate();
        _table.removeRow(iRow);
        _table.endUpdate();
        return true;
    }
    if (pBtn == &_btnUpdate)
    {
        td::Variant val;
        _semester.getValue(val);
        if (!doesIDexist(val.i4Val()))
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
        return true;
    }
    //dodaj ispod u tabelu
    if (pBtn == &_btnPushBack)
    {
        td::Variant val;
        _semester.getValue(val);
        if (doesIDexist(val.i4Val()))
        {
            showAlert(tr("alert"), tr("alertPB"));
            return true;
        }
        _table.beginUpdate();
        auto& row = _table.getEmptyRow();
        populateDSRow(row);
        _table.push_back();
        _table.endUpdate();
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

bool ViewCurriculum::insertCurriculum()
{
    return false;
}



bool ViewCurriculum::canAdd() {
    return true;
}

/*bool ViewCurriculum::onClick(gui::Button* pBtn)
{
    if (pBtn == &_btnAdd)
    {
        td::INT4 itemid = _id.getValue().i4Val();
        if (!canAdd())
            return true;
        _table.beginUpdate();
        auto& row = _table.getEmptyRow();
        populateDSRow(row);
        _table.push_back();
        _table.endUpdate();


        _itemsToUpdate.erase(std::remove(_itemsToUpdate.begin(), _itemsToUpdate.end(), itemid), _itemsToUpdate.end());
        _itemsToInsert.push_back(itemid);
        //neka ostaje za brisanje, mozda je pobrisanoriginalni artikal sa sifrom itemid, i sada dodajemo novi sa istom sifrom
        return true;
    }
    if (pBtn == &_btnSave)
    {
        saveData();
        return true;
    }
    return false;
}


bool ViewCourse::insertCourse()
{
    dp::IStatementPtr pInsertCourseP(_db->createStatement("insert into Predmet(ID_Predmeta, Naziv_Predmeta, Sifra_Predmeta, ID_Smjera, Godina_Studija, ECTS_bodovi, Izborni, Max_Broj_Polaznika, Ciklus) values(?,?,?,?,?,?,?,?,?)"));

    // ubacivanje u tabelu Predmet
    dp::Params pParams2(pInsertCourseP->allocParams());
    td::INT4 id, idCode, idDep, year, elective, maxNum, ciklus, ects;
    td::String name, idPredmeta;

    pParams2 << id << dp::toNCh(name, 100) << dp::toNCh(idPredmeta, 100) << idDep << year << ects << elective << maxNum << ciklus;

    dp::IDataSet* pDS = _table.getDataSet();
    auto rowCnt = pDS->getNumberOfRows();
    for (size_t iRow = 0; iRow < rowCnt; ++iRow)
    {
        // pod pretpostavkom da populateData ucitava podatke analogno redoslijedu u bazi podataka
        auto& row = pDS->getRow(iRow);
        id = row[0].i4Val();
        if (std::find(_itemsToInsert.begin(), _itemsToInsert.end(), id) == _itemsToInsert.end())//this item is not marked to insert, go next
            continue;
        name = row[1];
        idPredmeta = row[2];
        idDep = row[3].i4Val();
        year = row[4].i4Val();
        ects = row[5].i4Val();
        elective = row[6].i4Val();
        maxNum = row[7].i4Val();
        ciklus = row[8].i4Val();

        if (!pInsertCourseP->execute())
        {
            return false;
        }
    }

    // ubacivanje u tabelu PredmetStaff
    dp::IStatementPtr pInsertCoursePS(_db->createStatement("insert into PredmetStaff(ID_Korisnika, ID_predmeta) values(?,?)"));
    dp::Params pParams(pInsertCoursePS->allocParams());
    td::INT4 idk, idp;
    pParams << idk << idp;

    dp::IDataSet* pDS1 = _table.getDataSet();
    auto rowCnt1 = pDS1->getNumberOfRows();
    for (size_t iRow = 0; iRow < rowCnt1; ++iRow)
    {
        // pod pretpostavkom da populateData ucitava podatke analogno redoslijedu u bazi podataka
        auto& row = pDS1->getRow(iRow);
        idk = row[0].i4Val();
        if (std::find(_itemsToInsert.begin(), _itemsToInsert.end(), id) == _itemsToInsert.end())//this item is not marked to insert, go next
            continue;
        idp = row[1].i4Val();

        if (!pInsertCoursePS->execute())
        {
            return false;
        }
    }

    return true;
}



*/
bool ViewCurriculum::doesIDexist(td::INT4 id)
{
    size_t nRows = _pDS->getNumberOfRows();
    for (size_t i = 0; i < nRows; ++i)
    {
        auto row = _pDS->getRow(i);
        if (row[0].i4Val() == id)
            return true;
    }
    return false;
}
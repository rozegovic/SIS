#pragma once
#include "ViewCurriculum.h"
#include "ViewIDs.h"
#include "ViewCurriculumDialog.h"


ViewCurriculum::ViewCurriculum(td::INT4 _departmentID, td::INT4 _semesterID) : _db(dp::getMainDatabase())
, _lblDepartment(tr("Department"))
, _lblSemester(tr("Semester"))
, _semester(td::int4)
, _lblCourse(tr("Course"))
, _lblShortname(tr("Shortname"))
, _lblECTS(tr("ECTS"))
, _ECTS(td::int4)
, _course(td::int4)
, _shortname(td::int4)
, _hlBtnsDB(8)
, _btnSave(tr("Save"), tr("SaveTT"))
, _btnDelete(tr("Delete"), tr("DeleteTT"))
, _btnUpdate(tr("Update"), tr("UpdateTT"))
, _btnPushBack(tr("PushBack"), tr("PushBackTT"))
, _departmentID(_departmentID)
, _semesterID(_semesterID)
, _gl(5, 4)
{
    setVisualID(View_CURRICULUM);
    _hlBtnsDB.append(_btnSave);
    _hlBtnsDB.appendSpacer();
    _hlBtnsDB.appendSpace(2);
    _hlBtnsDB.append(_btnDelete);
    _hlBtnsDB.append(_btnUpdate);
    _hlBtnsDB.append(_btnPushBack);

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

    populateData();

    SetCurrentDepartment();
    SetCurrentSemester();/*
    SetCurrentECTS();*/
    _department.setAsReadOnly();
    _semester.setAsReadOnly();
    loadComboBox("select ID_Predmeta as ID, Naziv_Predmeta as Name from Predmet where ID_Smjera=?", _course);
    loadComboBox("select ID_Predmeta as ID, Sifra_Predmeta as Name from Predmet where ID_Smjera=?", _shortname);
    _table.init(_pDS, { 0, 1, 2 });
    if (_pDS->getNumberOfRows())
    {
        _table.selectRow(0, true);
    }
}


ViewCurriculum::~ViewCurriculum()
{
}
td::INT4 ViewCurriculum::getIDfromTable(int rowID)
{
    return td::INT4();
}
bool ViewCurriculum::canDelete(int iRow)
{
    return false;
}

bool ViewCurriculum::saveData()
{
    dp::IStatementPtr pInsStat(dp::getMainDatabase()->createStatement("INSERT INTO Curriculum (ID_Smjera,  ID_Predmeta, Shortname, ECTS) VALUES(?,?,?,?)"));
    dp::Params parDS(pInsStat->allocParams());
    dp::Transaction tr(dp::getMainDatabase());

    td::INT4 predmet, ects, smjer;
    td::String shortname;
    td::Variant val;
    parDS << smjer << predmet << dp::toNCh(shortname, MESSAGE_HEADER_LEN) << ects;
    dp::IStatementPtr pDel(dp::getMainDatabase()->createStatement("DELETE FROM Curriculum"));
    if (!pDel->execute())
        return false;
    size_t nRows = _pDS->getNumberOfRows();
    for (size_t i = 0; i < nRows; ++i)
    {
        auto row = _pDS->getRow(i);
        predmet = row[0].i4Val();
        shortname = row[1];
        ects = row[2].i4Val();
        smjer = _departmentID;


        if (!pInsStat->execute())
            return false;
        ++smjer;
    }
    tr.commit();
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


void ViewCurriculum::populateData()
{
    _pDS = _db->createDataSet("SELECT Predmet.ID_Predmeta, Curriculum.Shortname, Curriculum.ECTS from Curriculum, Predmet WHERE Curriculum.ID_Predmeta=Predmet.ID_Predmeta", dp::IDataSet::Execution::EX_MULT);


    //specify columns to obtain from the data provider
    dp::DSColumns cols(_pDS->allocBindColumns(3));
    cols << "ID_Predmeta" << td::int4 << "Shortname" << td::string8
        << "ECTS" << td::int4;

    if (!_pDS->execute())
    {
        //show log
        _pDS = nullptr;
        return;
    }
}


bool ViewCurriculum::loadComboBox(td::String select, gui::DBComboBox& combo)
{
    dp::IStatementPtr pSelect = _db->createStatement(select.c_str());
    dp::Params parDS(pSelect->allocParams());
    //d::INT4 IDPredmeta = Globals::_IDSubjectSelection;
    parDS << _departmentID;
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
    if (pTE == &_table) {
        int iRow = _table.getFirstSelectedRow();
        if (iRow < 0) {
            return true;
        }
        td::Variant val;
        dp::IDataSet* pDS = _table.getDataSet();
        auto& row = _pDS->getRow(iRow);
        val = row[0];
        _course.setValue(val);
         _shortname.setValue(val);
        val = row[2];
        _ECTS.setValue(val);
        return true;
    }
    return false;
}

void ViewCurriculum::populateDSRow(dp::IDataSet::Row& row)
{
    td::Variant val;

    _course.getValue(val);
    row[0] = val.i4Val();

    _shortname.getValue(val);
    row[1].setValue(_shortname.getSelectedText());

    _ECTS.getValue(val);
    row[2].setValue(val);
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
        int iRow = _table.getFirstSelectedRow();
        if (iRow < 0)
            return true;
        if (!canUpdate(iRow))
            return true;

        _table.beginUpdate();
        auto& row = _table.getCurrentRow();
        populateDSRow(row);
        _table.updateRow(iRow);
        int sRow = iRow + 1;
        _table.selectRow(sRow);
        _table.selectRow(iRow);
        _table.endUpdate();

        return true;
    }
    //dodaj ispod u tabelu
    if (pBtn == &_btnPushBack)
    {
        if (!canAdd())
            return true;
        _table.beginUpdate();
        auto& row = _table.getEmptyRow();
        populateDSRow(row);
        _table.push_back();
        _table.endUpdate();
        return true;

    }

    if (pBtn == &_btnSave)
    {


        showYesNoQuestionAsync(tr("alert"), tr("saveSure"), tr("Yes"), tr("No"), [this](gui::Alert::Answer answer)
            {
                if (answer == gui::Alert::Answer::Yes)
                    saveData();
                showAlert(tr("succes"), tr("succesEE"));
            });
        return true;
    }

    return false;
}


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


void ViewCurriculum::SetCurrentDepartment() {
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT Naziv_Smjera FROM Smjer WHERE ID_Smjera = ?");
    dp::Params parDS(pSelect->allocParams());
    //d::INT4 IDPredmeta = Globals::_IDSubjectSelection;
    parDS << _departmentID;
    dp::Columns pCols = pSelect->allocBindColumns(1);
    td::String Smjer;
    pCols << "Naziv_Smjera" << Smjer;
    if (!pSelect->execute()) {
        Smjer = "Greska";
    }
    while (pSelect->moveNext())
    {
        td::Variant val;
        val = Smjer;
        _department.setValue(val);

    }

}

void ViewCurriculum::SetCurrentECTS() {
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT ECTS_bodovi FROM Predmet WHERE ID_Predmeta = ?");
    dp::Params parDS(pSelect->allocParams());
    ////d::INT4 IDPredmeta = Globals::_IDSubjectSelection;
    td::Variant val(td::int4);
    _course.getValue(val);
    td::INT4 id =val.i4Val();
    parDS << id;
    dp::Columns pCols = pSelect->allocBindColumns(1);
    td::INT4 ects;
    pCols << "ECTS_bodovi" << ects;
    if (!pSelect->execute()) {
        ects = 0;
    }
    while (pSelect->moveNext())
    {
        td::Variant val;
        val = ects;
        _ECTS.setValue(val);

    }

}

void ViewCurriculum::SetCurrentSemester() {
        td::Variant val;
        val = _semesterID;
        _semester.setValue(val);
}

bool ViewCurriculum::canAdd()
{
    td::Variant pom = _ECTS.getValue();
    if (pom < 1) {
        showAlert(tr("alert"), tr("alertCRSE"));
        return false;
    }
    return true;
}

bool ViewCurriculum::canUpdate(int iRow)
{
    //-----------ne vrsi se promjena id
    td::Variant pom = _ECTS.getValue();
    if (pom < 1) {
        showAlert(tr("alert"), tr("alertCRSE"));
        return false;
    }
    return true;
}

bool ViewCurriculum::onChangedSelection(gui::DBComboBox* pCmb) {
    if (pCmb == &_course) {
        SetCurrentECTS();
        _shortname.selectIndex(_course.getSelectedIndex());
        return true;
    }
    return false;
}
#pragma once
#include "ViewEnroll.h"
#include "ViewIDs.h"


ViewEnroll::ViewEnroll() :
    _departlbl(tr("departmentUserL")),
    _department(td::int4),
    _semlbl(tr("semesterUserL")),
    _idlbl(tr("IDUserL")),
    _id(td::int4),
    _semester(td::int4),
    _namelbl(tr("nameUserL")),
    _surnamelbl(tr("surnameUserL")),
    _indexlbl(tr("indexUserL")),
    _enrolled(tr("enrolled")),
    _toBeEnrolled(tr("tobeenrolled")),
    _hlBtnsDB(8)
    , _btnSave(tr("Save"), tr("SaveTT"))
    , _btnReload(tr("Reload"), tr("ReloadTT"))
    , _btnEnroll(tr("Enroll"), tr("EnrollTT"))
    , _btnWithdraw(tr("Withdraw"), tr("WithdrawTT"))
    , _gl(7, 6)
    , _db(dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3))
{

    _hlBtnsDB.append(_btnSave);
    _hlBtnsDB.appendSpacer();
    _hlBtnsDB.append(_btnReload);
    _hlBtnsDB.appendSpace(20);
    _hlBtnsDB.append(_btnWithdraw);
    _hlBtnsDB.append(_btnEnroll);
    _btnSave.setType(gui::Button::Type::Default);
    _btnEnroll.setType(gui::Button::Type::Constructive);
    _btnWithdraw.setType(gui::Button::Type::Destructive);


    gui::GridComposer gc(_gl);

    gc.appendRow(_idlbl);
    gc.appendCol(_id);
    gc.appendCol(_namelbl);
    gc.appendCol(_name);
    gc.appendCol(_surnamelbl);
    gc.appendCol(_surname);

    gc.appendRow(_indexlbl);
    gc.appendCol(_index);
    gc.appendCol(_departlbl);
    gc.appendCol(_department);
    gc.appendCol(_semlbl);
    gc.appendCol(_semesterCombo);

    gc.appendRow(_toBeEnrolled, 0);
    gc.appendRow(_tableStudents, 0);

    gc.appendRow(_enrolled, 0);
    gc.appendRow(_tableEnrolled, 0);
    gc.appendRow(_hlBtnsDB, 0);

    gui::View::setLayout(&_gl);

    _db = dp::getMainDatabase();

    populateDataForStudents();
    populateDataForEnrolledStudents();

    populateDepartmentCombo(_department);
    populateSemesterCombo(_semesterCombo);

    _id.setAsReadOnly();
    _name.setAsReadOnly();
    _surname.setAsReadOnly();
    _index.setAsReadOnly();

    _enrolled.setBold();
    _toBeEnrolled.setBold();
}

void ViewEnroll::initTableEnrolledStudents()
{
    gui::Columns visCols(_tableEnrolled.allocBindColumns(6));
    visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, tr("IDUser"), "", 305) << gui::Header(3, tr("indexUser"), "", 305) << gui::Header(1, tr("nameUser"), "", 305) << gui::Header(2, tr("surnameUser"), "", 305) << gui::Header(4, tr("departmentUser"), "", 305) << gui::Header(6, tr("semesterUser"), "", 305);
    _tableEnrolled.init(_pDS2);
    if (_pDS2->getNumberOfRows()) {
        _tableEnrolled.selectRow(0, true);
    }
}

void ViewEnroll::initTableStudents()
{
    gui::Columns visCols(_tableStudents.allocBindColumns(4));
    visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, tr("IDUser"), "", 465) << gui::Header(3, tr("indexUser"), "", 465) << gui::Header(1, tr("nameUser"), "", 465) << gui::Header(2, tr("surnameUser"), "", 465);
    _tableStudents.init(_pDS);
    if (_pDS->getNumberOfRows()) {
        _tableStudents.selectRow(0, true);
    }
}

void ViewEnroll::populateDepartmentCombo(gui::DBComboBox& combo)
{
        dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT ID_Smjera,Naziv_Smjera FROM Smjer");
        dp::Columns pCols = pSelect->allocBindColumns(2);
        td::String name;
        td::INT4 id;
        pCols << "ID_Smjera" << id << "Naziv_Smjera" << name;
        pSelect->execute();

        while (pSelect->moveNext())
        {
            combo.addItem(name, id);
        }

        combo.selectIndex(0);
}

void ViewEnroll::populateSemesterCombo(gui::ComboBox& combo)
{
    combo.addItem("I");
    combo.addItem("II");
    combo.addItem("III");
    combo.addItem("IV");
    combo.addItem("V");
    combo.addItem("VI");

    combo.selectIndex(0);

}

void ViewEnroll::populateDataForEnrolledStudents() {
    _pDS2 = dp::getMainDatabase()->createDataSet("SELECT Korisnici.ID as IDUser, Korisnici.Ime as nameUser, Korisnici.Prezime as surnameUser, Korisnici.Indeks as indexUser, Smjer.Naziv_Smjera as departmentUser, Smjer.ID_Smjera as departID, Upis.Semestar as semesterUser FROM Korisnici, Smjer, Upis WHERE Upis.ID_Smjera = Smjer.ID_Smjera AND Korisnici.indeks = Upis.Indeks and Korisnici.ID != 0 AND Korisnici.ID != -1", dp::IDataSet::Execution::EX_MULT);
    dp::DSColumns cols(_pDS2->allocBindColumns(7));
    cols << "IDUser" << td::int4 << "nameUser" << td::string8 << "surnameUser" << td::string8 << "indexUser" << td::string8 << "departmentUser" << td::string8 << "departID" << td::int4 << "semesterUser" << td::int4;
    if (!_pDS2->execute())
    {
        _pDS2 = nullptr;
        return;
    }
    initTableEnrolledStudents();
}

void ViewEnroll::populateDataForStudents()
{
    _pDS = dp::getMainDatabase()->createDataSet("SELECT K.ID as IDUser, K.Ime as nameUser, K.Prezime as surnameUser, K.Indeks as indexUser FROM Korisnici K LEFT JOIN Upis U ON K.Indeks = U.Indeks WHERE U.ID IS NULL AND K.Indeks != 0", dp::IDataSet::Execution::EX_MULT);
    dp::DSColumns cols(_pDS->allocBindColumns(4));
    cols << "IDUser" << td::int4 << "nameUser" << td::string8 << "surnameUser" << td::string8 << "indexUser" << td::string8;

    if (!_pDS->execute())
    {
        _pDS = nullptr;
        return;
    }
    initTableStudents();
}




bool ViewEnroll::onChangedSelection(gui::TableEdit* pTE) {
    if (pTE == &_tableStudents) {
        int iRow = _tableStudents.getFirstSelectedRow();
        if (iRow < 0) {
            return true;
        }
        td::Variant val;
        dp::IDataSet* pDS = _tableStudents.getDataSet();
        auto& row = pDS->getRow(iRow);
        val = row[0];
        _id.setValue(val);

        val = row[1];
        _name.setValue(val);

        val = row[2];
        _surname.setValue(val);

        val = row[3];
        _index.setValue(val);

        return true;
    }

    /*if (pTE == &_tableEnrolled) {
        int iRow = _tableEnrolled.getFirstSelectedRow();
        if (iRow < 0) {
            return true;
        }
        td::Variant val;
        dp::IDataSet* pDS = _tableEnrolled.getDataSet();
        auto& row = pDS->getRow(iRow);
        val = row[0];
        _id.setValue(val);

        val = row[1];
        _name.setValue(val);

        val = row[2];
        _surname.setValue(val);

        val = row[3];
        _index.setValue(val);

        val = row[6];
        _semesterCombo.selectIndex(val.i4Val() - 1);
        _semester.setValue(val);

        val = row[5];
        _department.setValue(val);

        return true;
    }*/
    return false;
}

bool ViewEnroll::onChangedSelection(gui::ComboBox* pCmb)
{
    if (pCmb == &_semesterCombo) {

        int semester = _semesterCombo.getSelectedIndex();

        switch (semester) {
        case 0: _semester.setValue(1); break;
        case 1: _semester.setValue(2); break;
        case 2: _semester.setValue(3); break;
        case 3: _semester.setValue(4); break;
        case 4: _semester.setValue(5); break;
        case 5: _semester.setValue(6); break;
        }

        return true;
    }

    return false;
}

void ViewEnroll::populateDSRow(dp::IDataSet::Row& row)
{
    td::Variant val, val1;
    _id.getValue(val);
    row[0].setValue(val);

    _name.getValue(val);
    row[1].setValue(val);

    _surname.getValue(val);
    row[2].setValue(val);

    _semester.getValue(val1);
    row[6].setValue(val1);

    _department.getValue(val);
    row[5] = val.i4Val();
    row[4].setValue(_department.getSelectedText());

}

bool ViewEnroll::onClick(gui::Button* pBtn)
{
    if (pBtn == &_btnReload)
    {
        _tableStudents.reload();
        _tableStudents.selectRow(0, true);
        _tableEnrolled.reload();
        _tableEnrolled.selectRow(0, true);
        return true;
    }

    if (pBtn == &_btnWithdraw)   //ako admin napravi gresku da moze azurirati
    {
        int iRow = _tableEnrolled.getFirstSelectedRow();
        if (iRow < 0)
            return true;
        _tableEnrolled.beginUpdate();
        _tableEnrolled.removeRow(iRow);
        _tableEnrolled.endUpdate();

        return true;
    }

    if (pBtn == &_btnEnroll)
    {
        int iRow = _tableStudents.getFirstSelectedRow();
        auto& rowToBeDel = _tableStudents.getCurrentRow();
        if (iRow < 0)
            return true;
        if (iRow >= 0) {
            _tableEnrolled.beginUpdate();
            auto& row = _tableEnrolled.getEmptyRow();
            populateDSRow(row);
            row[3] = rowToBeDel[3];
            _tableEnrolled.push_back();

            _tableEnrolled.endUpdate();
            _tableStudents.removeRow(iRow);
            return true;
        }
    }
    if (pBtn == &_btnSave)
    {
        showYesNoQuestionAsync(QuestionID::Save, this, tr("alert"), tr("saveSure"), tr("Yes"), tr("No"));
        return true;
    }

    return false;
}
bool ViewEnroll::saveEnrolls()
{
    //ovo je kada su svi updateovani
    dp::IStatementPtr pInsStat(_db->createStatement("INSERT INTO Upis (ID, Indeks, ID_smjera, Semestar) VALUES (?,?,?,?) "));
    dp::Params parDS(pInsStat->allocParams());
    td::INT4 IDSmjer, semestar, id = 1;
    td::String index;
    parDS << id << dp::toNCh(index, 30) << IDSmjer << semestar;

    dp::IStatementPtr pDel(_db->createStatement("DELETE FROM Upis"));
    if (!pDel->execute())
        return false;

    dp::Transaction tr(_db);

    size_t nRows = _pDS2->getNumberOfRows();
    for (size_t i = 0; i < nRows; i++) {
        auto row = _pDS2->getRow(i);
        index = row[3];
        IDSmjer = row[5].i4Val();
        semestar = row[6].i4Val();
        if (!pInsStat->execute())
            return false;
        ++id;
    }
    tr.commit();
    return true;
}

bool ViewEnroll::onAnswer(td::UINT4 questionID, gui::Alert::Answer answer)
{
    if ((QuestionID)questionID == QuestionID::Save)
    {
        if (answer == gui::Alert::Answer::Yes) {
            saveEnrolls();
            showAlert(tr("succes"), tr("succesEE"));
        }
        return true;
    }
    return false;
}
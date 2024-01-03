#pragma once
#include "ViewUsers.h"
#include "ViewIDs.h"

ViewUsers::ViewUsers()
    : _lblID(tr("IDUserL"))
    , _id(td::int4)
    , _lblName(tr("nameUserL"))
    , _lblSurname(tr("surnameUserL"))
    , _lblEnrollDate(tr("dateEUserL"))
    , _lblJmbg(tr("jmbgUserL"))
    , _lblAddress(tr("addressUserL"))
    , _lblDateofbirth(tr("dateBUserL"))
    , _lblRole(tr("roleUserL"))
    , _role(td::int4)
    , _lblIndex(tr("indexUserL"))
    , _hlBtnsDB(7)
    , _btnSave(tr("Save"), tr("SaveTT"))
    , _btnReload(tr("Reload"), tr("ReloadTT"))
    , _btnDelete(tr("Delete"), tr("DeleteTT"))
    , _btnUpdate(tr("Update"), tr("UpdateTT"))
    , _btnPushBack(tr("PushBack"), tr("PushBackTT"))
    , _gl(5, 6)
{
    setVisualID(View_USERS);
    // buttons in last row
    _hlBtnsDB.append(_btnSave);
    _hlBtnsDB.appendSpacer();
    _hlBtnsDB.append(_btnReload);
    _hlBtnsDB.appendSpace(20);
    _hlBtnsDB.append(_btnDelete);
    _hlBtnsDB.append(_btnUpdate);
    _hlBtnsDB.append(_btnPushBack);
    _btnSave.setType(gui::Button::Type::Default);
    _btnDelete.setType(gui::Button::Type::Destructive);
    _btnPushBack.setType(gui::Button::Type::Constructive);

    gui::GridComposer gc(_gl);
    gc.appendRow(_lblID);
    gc.appendCol(_id);
    gc.appendCol(_lblName);
    gc.appendCol(_name);
    gc.appendCol(_lblSurname);
    gc.appendCol(_surname);

    gc.appendRow(_lblDateofbirth);
    gc.appendCol(_dateofbirth);
    gc.appendCol(_lblJmbg);
    gc.appendCol(_jmbg);
    gc.appendCol(_lblAddress);
    gc.appendCol(_address);

    gc.appendRow(_lblRole);
    gc.appendCol(_role);
    gc.appendCol(_lblIndex);
    gc.appendCol(_index);

    gc.appendRow(_table, 0);
    gc.appendRow(_hlBtnsDB, 0);

    gui::View::setLayout(&_gl);

    _dateEnroll.now();
    _noChngIndex = "0";
    
    populateRoleCombo(_role);
    populateData();
}

void ViewUsers::initTable()
{
    gui::Columns visCols(_table.allocBindColumns(9));
    visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, tr("IDUser"), "", 200) << gui::Header(4, tr("roleUser"), "", 200) << gui::Header(1, tr("nameUser"), "", 200) << gui::Header(2, tr("surnameUser"), "", 200) << gui::Header(8, tr("dateBUser"), "", 200) << gui::Header(5, tr("jmbgUser"), "", 200) << gui::Header(7, tr("addressUser"), "", 200) << gui::Header(9, tr("indexUser"), "", 200) << gui::Header(6, tr("dateEUser"), "", 200);
    _table.init(_pDS);
    if (_pDS->getNumberOfRows()) {
        _table.selectRow(0, true);
    }
}

void ViewUsers::populateRoleCombo(gui::DBComboBox& combo)
{
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT ID, Naziv FROM Pozicija");
    dp::Columns pCols = pSelect->allocBindColumns(2);
    td::String name;
    td::INT4 id;
    pCols << "ID" << id << "Naziv" << name;
    pSelect->execute();

    while (pSelect->moveNext())
    {
        combo.addItem(name, id);
    }
    combo.selectIndex(0);
}

void ViewUsers::populateData()
{
        _pDS = dp::getMainDatabase()->createDataSet("SELECT Korisnici.ID as IDUser, Korisnici.Ime as nameUser, Korisnici.Prezime as surnameUser, Pozicija.ID as positionID, Pozicija.Naziv as roleUser, Korisnici.JMBG as jmbgUser, Korisnici.DatumUpisa as dateEUser, Korisnici.Adresa as addressUser, Korisnici.DatumRodjenja as dateBUser, Korisnici.Indeks as indeksUser FROM Korisnici, Pozicija WHERE Korisnici.PozicijaID = Pozicija.ID AND Korisnici.ID>0", dp::IDataSet::Execution::EX_MULT);
        dp::DSColumns cols(_pDS->allocBindColumns(10));
        cols << "IDUser" << td::int4 << "nameUser" << td::string8 << "surnameUser" << td::string8 << "positionID" << td::int4 << "roleUser" << td::string8
            << "jmbgUser" << td::string8 << "dateEUser" << td::date << "addressUser" << td::string8 << "dateBUser" << td::date << "indeksUser" << td::string8;

    
        if (!_pDS->execute())
        {
            _pDS = nullptr;
            return;
        }

        initTable();
}

bool ViewUsers::onChangedSelection(gui::TableEdit* pTE) {
    if (pTE == &_table) {
        int iRow = _table.getFirstSelectedRow();
        if (iRow < 0) {
            return true;
        }
        td::Variant val;
        dp::IDataSet* pDS = _table.getDataSet();
        auto& row = pDS->getRow(iRow);
        val = row[0];
        _id.setValue(val);

        val = row[1];
        _name.setValue(val);

        val = row[2];
        _surname.setValue(val);

        val = row[5];
        _jmbg.setValue(val);

        val = row[6];
        _enrollDate.setValue(_dateEnroll);

        val = row[3];
        _role.setValue(val);

        val = row[7];
        _address.setValue(val);

        val = row[8];
        _dateofbirth.setValue(val);

        val = row[9];
        _index.setValue(val);
        
        return true;
    }
    return false;
}

void ViewUsers::populateDSRow(dp::IDataSet::Row& row)
{
    td::Variant val;
    _id.getValue(val);
    row[0].setValue(val);

    _name.getValue(val);
    row[1].setValue(val);

    _surname.getValue(val);
    row[2].setValue(val);

    _jmbg.getValue(val);
    row[5].setValue(val);

    td::Variant date = _dateEnroll;
    row[6].setValue(date);

    _address.getValue(val);
    row[7].setValue(val);

    _dateofbirth.getValue(val);
    row[8].setValue(val);
    
    _role.getValue(val);
    row[3] = val.i4Val();
    row[4].setValue(_role.getSelectedText());

    td::Variant i;
    _index.getValue(i);
    if (_role.getSelectedIndex() == USER_STUDENT) {
        row[9].setValue(i);
    }
    else {
        row[9].setValue(_noChngIndex);
    }
}

bool ViewUsers::saveUserRoles()
{
    td::INT4 id, kId, pId;
    dp::IStatementPtr pStat(dp::getMainDatabase()->createStatement("INSERT INTO KorisniciPozicija (ID, IDKorisnici, IDPozicija) VALUES(?,?,?)"));
    dp::Params par(pStat->allocParams());
    par << id << kId << pId;
    dp::Transaction tr(dp::getMainDatabase());

    dp::IStatementPtr pDel(dp::getMainDatabase()->createStatement("DELETE FROM KorisniciPozicija"));
    if (!pDel->execute())
        return false;

    size_t nRows = _pDS->getNumberOfRows();
    for (size_t i = 0; i < nRows; ++i)
    {
        auto row = _pDS->getRow(i);
        kId = row[0].i4Val();
        pId = row[3].i4Val();
        id = kId * 10;

        if (!pStat->execute())
            return false;
    }

    if (tr.commit())
        return true;
}

bool ViewUsers::saveUsers()
{
    dp::IStatementPtr pInsStat(dp::getMainDatabase()->createStatement("INSERT INTO Korisnici (ID, Prezime, Ime, DatumUpisa, PozicijaID, JMBG, Adresa, DatumRodjenja, Indeks) VALUES(?,?,?,?,?,?,?,?,?)"));
    dp::Params parDS(pInsStat->allocParams());
    td::INT4 id=0, pID;
    td::Date dateE, dateB;
    td::String name, surname, jmbg, address, index;
    parDS << id << dp::toNCh(surname, 30) << dp::toNCh(name, 30) << dateE << pID << dp::toNCh(jmbg, 30) << dp::toNCh(address, 30) << dateB << dp::toNCh(index, 20);
    
    dp::Transaction tr(dp::getMainDatabase());

    dp::IStatementPtr pDel(dp::getMainDatabase()->createStatement("DELETE FROM Korisnici"));
    if (!pDel->execute())
        return false;
    size_t nRows = _pDS->getNumberOfRows();
    for (size_t i = 0; i < nRows; ++i)
    {
        auto row = _pDS->getRow(i);
        id = row[0].i4Val();
        name = row[1];
        surname = row[2];
        pID = row[3].i4Val();
        jmbg = row[5];
        dateE = row[6];
        address = row[7];
        dateB = row[8];
        index = row[9];
        if (!pInsStat->execute())
            return false;
    }
    
    tr.commit();
    return true;
}

bool ViewUsers::onClick(gui::Button* pBtn)
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
        return true;
    }

    if (pBtn == &_btnUpdate)
    {
        int iRow = _table.getFirstSelectedRow();
        if (iRow < 0)
            return true;
        if (iRow >= 0) {
            _table.beginUpdate();
            auto& row = _table.getCurrentRow();
            populateDSRow(row);
            _table.updateRow(iRow);
            _table.endUpdate();
            int sRow = iRow + 1;
            _table.selectRow(sRow);
            _table.selectRow(iRow);
            return true;
        }
    }

    if (pBtn == &_btnPushBack)
    {
        td::Variant val, str, ind, v;
        _id.getValue(val);
        if (doesIDexist(val.i4Val()))
        {
            showAlert(tr("error"), tr("alertPB")); // Vec postoji korisnik 
            return true;
        }

        _index.getValue(ind);
        if (doesIndexExist(ind)) {
            showAlert(tr("error"), tr("alertI")); // Vec postoji index
            return true;
        }
        
        int i = _role.getSelectedIndex();
        if (i == USER_STUDENT && ind == _noChngIndex) {
            showAlert(tr("error"), tr("alertIND")); // Nije moguce registrovati studenta sa indeksom nula
            return true;
        }

        _jmbg.getValue(str);
        td::String b;
        str.getValue(b);
        if (doesJMBGExist(str) || b.length() != 13) {
            showAlert(tr("error"), tr("alertMB")); // Neispravan maticni broj ili je vec registrovan
            return true;
        }

        _dateofbirth.getValue(v);
        td::Date dt;
        dt = v;
        if (dt > _dateEnroll) {
            showAlert(tr("error"), tr("alertDR")); // Neispravan datum rodjenja
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
        showYesNoQuestionAsync(QuestionID::Save, this, tr("alert"), tr("saveSure"), tr("Yes"), tr("No"));
        return true;
    }

    return false;
}

bool ViewUsers::onAnswer(td::UINT4 questionID, gui::Alert::Answer answer)
{
    if ((QuestionID)questionID == QuestionID::Save)
    {
        if (answer == gui::Alert::Answer::Yes) {
            saveUsers();
            saveUserRoles();
            showAlert(tr("succes" ), tr("succesEE"));
        }  
        return true;
    }
    return false;
}

bool ViewUsers::doesIDexist(td::INT4 id)
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

bool ViewUsers::doesJMBGExist(td::Variant jmbg)
{
    size_t nRows = _pDS->getNumberOfRows();
    for (size_t i = 0; i < nRows; ++i)
    {
        auto row = _pDS->getRow(i);
        if (row[5] == jmbg)
            return true;
    }
    return false;
}

bool ViewUsers::doesIndexExist(td::Variant index)
{
    size_t nRows = _pDS->getNumberOfRows();
    td::Variant ind;
    for (size_t i = 0; i < nRows; ++i)
    {
        auto row = _pDS->getRow(i);
        if ( index != _noChngIndex && row[9] == index)
            return true;
    }
    return false;
}


bool ViewUsers::onChangedSelection(gui::DBComboBox* pCmb)
{
    if (pCmb == &_role)
    {
        int ind = _role.getSelectedIndex();
        if (ind == USER_STUDENT) {
            _lblIndex.show(true);
            _index.show(true);
        }
        else {
            _index.setText(_noChngIndex);
            _lblIndex.hide(true, false);
            _index.hide(true, false);
        }
        return true;
    }
    return false;
}
#pragma once
#include "ViewDepartment.h"
#include "ViewIDs.h"
#include <td/Types.h>



ViewDepartment::ViewDepartment()
    : _db(dp::getMainDatabase())
    , _lblID(tr("id:"))
    , _id(td::int4)
    , _lblName(tr("name:"))
    , _btnAdd(tr("add"))
    , _btnSave(tr("save"))
    , _btnReload(tr("Reload"), tr("ReloadTT"))
    , _lblShName(tr("Sname"))
    //, _btnRemoveAll(tr("DelAll"), tr("DelAllTT"))
    , _btnDelete(tr("Delete"), tr("DeleteTT"))
    , _btnUpdate(tr("Update"), tr("UpdateTT"))
    , _hlBtns(6)
    , _gl(3, 6)
    , _paramFrom(0)
    , _paramTo(100)
{
    setVisualID(View_DEPARTMENT);

    _hlBtns.appendSpacer();
    _hlBtns.append(_btnSave, td::HAlignment::Right);
    _hlBtns.append(_btnReload, td::HAlignment::Right);
    // _hlBtns.append(_btnRemoveAll, td::HAlignment::Right);
    _hlBtns.append(_btnDelete, td::HAlignment::Right);
    _hlBtns.append(_btnUpdate, td::HAlignment::Right);
    _hlBtns.append(_btnAdd, td::HAlignment::Right);
    _btnAdd.setAsDefault();

    gui::GridComposer gc(_gl);
    gc.appendRow(_lblID);
    gc.appendCol(_id);
    gc.appendCol(_lblName);
    gc.appendCol(_name);
    gc.appendCol(_lblShName);
    gc.appendCol(_shName);
    gc.appendRow(_table, 0);
    gc.appendRow(_hlBtns, 0);
    gui::View::setLayout(&_gl);

    populateData();
}

ViewDepartment::~ViewDepartment()
{
   
}

void ViewDepartment::initTable() 
{
    gui::Columns visCols(_table.allocBindColumns(3));
    visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, tr("id")) << gui::Header(1, tr("name"),tr("name"),250) << gui::Header(2, tr("name"));
    _table.init(_pDS);
    
}

void ViewDepartment::populateData()
{
    _pDS = _db->createDataSet("select ID_Smjera as ID, Naziv_Smjera as Naziv, Skr_Naziv_Smjera as Skr from Smjer", dp::IDataSet::Execution::EX_MULT);
    /*dp::Params params(_pDS->allocParams());
      params << _paramFrom << _paramTo;*/
      //moze se dodati ako nekada bude potrebno

       //specify columns to obtain from the data provider
    dp::DSColumns cols(_pDS->allocBindColumns(3));
    cols << "ID" << td::int4 << "Naziv" << td::string8 << "Skr" << td::string8;

    if (!_pDS->execute())
    {
        //show log
        _pDS = nullptr;
        return;
    }

    initTable();
}

bool ViewDepartment::loadComboBox(td::String select, gui::DBComboBox& combo)
{
    // ne treba funkcija trenutno
    return false;
}

bool ViewDepartment::onChangedSelection(gui::TableEdit* pTE)
{
    if (pTE == &_table)
    {
        int iRow = _table.getFirstSelectedRow();
        if (iRow < 0)
        {
            _id.toZero();
            _name.toZero();
            return true;
        }
        td::Variant val;
        dp::IDataSet* pDS = _table.getDataSet();
        auto& row = pDS->getRow(iRow);

        val = row[0];
        _id.setValue(val);

        val = row[1];
        _name.setValue(val);

        return true;
    }
    return false;
}

void ViewDepartment::populateDSRow(dp::IDataSet::Row& row)
{
    td::Variant val;
    _id.getValue(val);
    row[0].setValue(val);
    _name.getValue(val);
    row[1].setValue(val);
    _shName.getValue(val);
    row[2].setValue(val);
    
}

bool ViewDepartment::canAdd()
{
    //provjeravamo da li postoji smjer sa istim id
    td::Variant id = _id.getValue();
    td::Variant name = _name.getValue(name);
    td::Variant sh = _shName.getValue(sh);
    if (id < 0) {
        showAlert(tr("alert"), tr("alertNEG"));
        return false;

    }

    dp::IDataSet* pDS = _table.getDataSet();
    for (size_t i = 0; i < pDS->getNumberOfRows(); ++i)
    {
        auto& row = pDS->getRow(i);
        if (row[0] == id)
        {
            showAlert(tr("alert"), tr("alertDEPE"));
            return false;
        }
        if (row[1] == name)
        {
            showAlert(tr("alert"), tr("alertDEPN"));
            return false;
        }
        if (row[2] == sh)
        {
            showAlert(tr("alert"), tr("alertDEPSH"));
            return false;
        }

    }
    td::Variant pom;
    _id.getValue(pom);
    if (pom.isZero()) {
        showAlert(tr("alert"), tr("alertNp"));
        return false;
    }
    _name.getValue(pom);
    if (pom.isZero()) {
        showAlert(tr("alert"), tr("alertNp"));
        return false;
    }
    _shName.getValue(pom);
    if (pom.isZero()) {
        showAlert(tr("alert"), tr("alertNp"));
        return false;
    }

    return true;
}

td::INT4 ViewDepartment::getIDfromTable(int rowID)
{
    dp::IDataSet* pDS = _table.getDataSet();
    auto& row = pDS->getRow(rowID);
    return row[0].i4Val();
}

bool ViewDepartment::canUpdateDepartment(int iRow)
{
    td::Variant id = _id.getValue();
    if (id < 0) {
        showAlert(tr("alert"), tr("alertNEG"));
        return false;
    }
    td::INT4 idTable = getIDfromTable(iRow);

    if (idTable == id.i4Val())
    {
        return true;
    }
    //po potrebi dodati poruku
    return false;
}

bool ViewDepartment::existsInDb(td::INT4 id)
{
    //po potrebi implementirati poslije
    return false;
}

bool ViewDepartment::canDelete(int iRow)
{
    return !existsInDb(getIDfromTable(iRow));
}

bool ViewDepartment::eraseDepartment()
{
    // kada bude potrebno...
    td::INT4 id;
    dp::IStatementPtr pDeleteItem(_db->createStatement("delete from Smjer where ID_Smjera = ?"));
    dp::Params pParams2(pDeleteItem->allocParams());
    pParams2 << id;

    for (auto dep : _depsToDelete)
    {
        id = dep;
        if (!pDeleteItem->execute())
        {
            return false;
        }
    }
    return true;
}

bool ViewDepartment::insertDepartment()
{
    // Different way of inserting data in DB
    dp::IDataSet* pDS = _table.getDataSet();
    auto rowCnt = pDS->getNumberOfRows();

    for (size_t iRow = 0; iRow < rowCnt; ++iRow)
    {
        auto& row = pDS->getRow(iRow);
        td::INT4 id = row[0].i4Val();  // Declare and initialize id within the loop
        auto name = row[1];     // type is td::Variant

        // Check if the item is marked for insertion
        if (std::find(_depsToInsert.begin(), _depsToInsert.end(), id) == _depsToInsert.end())
            continue;

        // Create new parameters for each iteration
        dp::IStatementPtr pInsertDepartment(_db->createStatement("insert into Smjer(ID_Smjera, Naziv_Smjera) values(?,?)"));
        dp::Params pParams(pInsertDepartment->allocParams());
        pParams << id << dp::toNCh(name, 100);

        if (!pInsertDepartment->execute())
        {
            return false;
        }
    }
    return true;

    // Code does not work for some reason
    /* insertovanje u bazu podataka
    dp::IStatementPtr pInsertDepartment(_db->createStatement("insert into Smjer(ID_Smjera, Naziv_Smjera) values(?,?)"));
    dp::Params pParams(pInsertDepartment->allocParams());
    td::INT4 id;
    td::String name;
    pParams << id << dp::toNCh(name, 100);

    dp::IDataSet* pDS = _table.getDataSet();
    auto rowCnt = pDS->getNumberOfRows();
    for (size_t iRow = 0; iRow < rowCnt; ++iRow)
    {
        auto& row = pDS->getRow(iRow);
        id = row[0].i4Val();
        if (std::find(_depsToInsert.begin(), _depsToInsert.end(), id) == _depsToInsert.end())
        name = row[1];
        // name = "RI";

        
        if (!pInsertDepartment->execute())
        {
            //ovdje vraca false u prvom prolasku kroz petlju
            return false;
        }

    }
    return true;*/
}

bool ViewDepartment::updateDepartment()
{
    /*

    POTREBNO PREPRAVITI KOD DA RADI ZA DEPARTMENT

    NamingType nt = NamingType::CUSTOMER;
    td::String name;
    td::INT4 id, idAdr;
    dp::IStatementPtr pChangeNaming(_db->createStatement("update Naming set Name=? where TipID=? and ID=?"));
    dp::Params pParams(pChangeNaming->allocParams());
    pParams << dp::toNCh(name, 100) << nt << id;



    dp::IStatementPtr pChangeCust(_db->createStatement("update KupacDob set AdresaID = ? where TipID=? and ID=?"));
    dp::Params pParams2(pChangeCust->allocParams());
    pParams2 << idAdr << nt << id;


    dp::IDataSet* pDS = _table.getDataSet();
    auto rowCnt = pDS->getNumberOfRows();
    for (size_t iRow = 0; iRow < rowCnt; ++iRow)
    {
        auto& row = pDS->getRow(iRow);
        id = row[0].i4Val();
        if (std::find(_itemsToUpdate.begin(), _itemsToUpdate.end(), id) == _itemsToUpdate.end()) // this item is not marked to insert, go next
            continue;

        name = row[1];
        idAdr = row[2].i4Val();

        if (!pChangeNaming->execute())
        {
            return false;
        }
        if (!pChangeCust->execute())
        {
            return false;
        }
    }
    return true;*/
    return true;

}

bool ViewDepartment::saveData()
{
    dp::IStatementPtr pInsStat(dp::getMainDatabase()->createStatement("INSERT INTO Smjer (ID_Smjera, Naziv_Smjera, Skr_Naziv_Smjera) VALUES (?,?,?)"));
    dp::Params parDS(pInsStat->allocParams());
    td::INT4 id;
    td::String name, shName;
    parDS << id << dp::toNCh(name, 30) << dp::toNCh(shName, 5);// << dateE << pID << dp::toNCh(jmbg, 30) << dp::toNCh(address, 30) << dateB << dp::toNCh(index, 20);

    dp::Transaction tr(dp::getMainDatabase());

    dp::IStatementPtr pDel(dp::getMainDatabase()->createStatement("DELETE FROM Smjer"));
    if (!pDel->execute())
        return false;
    size_t nRows = _pDS->getNumberOfRows();
    for (size_t i = 0; i < nRows; ++i)
    {
        auto row = _pDS->getRow(i);
        id = row[0].i4Val();
        name = row[1];
        shName = row[2];
        if (!pInsStat->execute())
            return false;
    }

    tr.commit();
    return true;
}

/*bool ViewDepartment::saveData()
{
    //insertDepartment();
    dp::Transaction tran(_db);
    //tr.commit();
    if (!eraseDepartment())
        return false;

    if (!insertDepartment())
        return false;
     if (!updateItems())
         return false;
         //po potrebi poslije 
    
    if (tran.commit())
    {
        _depsToDelete.clear();
        _depsToInsert.clear();
        _depsToUpdate.clear();
    }
    return true;
}
*/

bool ViewDepartment::onClick(gui::Button* pBtn)
{
    /*if (pBtn == &_btnAdd) {
        _table.beginUpdate();
        auto& row = _table.getEmptyRow();
        populateDSRow(row);
        _table.push_back();
        _table.endUpdate();
        return true;
    }*/


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

       // _depsToUpdate.erase(std::remove(_depsToUpdate.begin(), _depsToUpdate.end(), itemid), _depsToUpdate.end());
        //_depsToInsert.push_back(itemid);
        return true;
    }
    
    /*  if (pBtn == &_btnRemoveAll)
     {
         _table.clean();
         return true;
     }*/

    if (pBtn == &_btnReload)
    {
        _table.reload();
        _depsToDelete.clear();
        _depsToInsert.clear();
        _depsToUpdate.clear();
        return true;
    }
    if (pBtn == &_btnDelete)
    {
        int iRow = _table.getFirstSelectedRow();
        td::INT4 itemID = getIDfromTable(iRow);
        if (iRow < 0)
            return true;

        //pazi na canDelete
        if (!canDelete(iRow))
            return true;
        _table.beginUpdate();
        _table.removeRow(iRow);
        _table.endUpdate();

        // interaction with database
        _depsToDelete.push_back(itemID);
        //kada brisemo neki item, ako smo ga ranije dodali ili azurirali i nije spaseno u bazu, nemoj ga spasiti u bazu
        _depsToInsert.erase(std::remove(_depsToInsert.begin(), _depsToInsert.end(), itemID), _depsToInsert.end());
        _depsToUpdate.erase(std::remove(_depsToUpdate.begin(), _depsToUpdate.end(), itemID), _depsToUpdate.end());
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
            td::INT4 idTable = getIDfromTable(iRow);
            if (idTable < 0) {
                showAlert(tr("alert"), tr("alertNEG"));
                return false; 
            }
            _table.selectRow(iRow);
            return true;
        }

    }
   /* if (pBtn == &_btnUpdate)
    {
        int iRow = _table.getFirstSelectedRow();
        if (iRow < 0)
            return true;
        if (!canUpdateDepartment(iRow))
            return true;
        td::INT4 itemID = getIDfromTable(iRow);

        _table.beginUpdate();
        auto& row = _table.getCurrentRow();
        populateDSRow(row);
        _table.updateRow(iRow);
        _table.endUpdate();
        //update samo ako nije novi item
        if (std::find(_depsToInsert.begin(), _depsToInsert.end(), itemID) == _depsToInsert.end())
            _depsToUpdate.push_back(itemID);

        return true;
    }
    /*
    if (pBtn == &_btnInsert)
    {
        int iRow = _table.getFirstSelectedRow();
        if (iRow < 0)
            iRow = 0;
        if (!canAdd())
            return true;
        _table.beginUpdate();
        auto& row = _table.getEmptyRow();
        populateDSRow(row);
        _table.insertRow(iRow);
        _table.endUpdate();

        td::INT4 itemID = getIDfromTable(iRow);

        _itemsToUpdate.erase(std::remove(_itemsToUpdate.begin(), _itemsToUpdate.end(), itemID), _itemsToUpdate.end());
        _itemsToInsert.push_back(itemID);
        return true;
    }

    if (pBtn == &_btnPushBack)
    {
        td::INT4 itemID = _id.getValue().i4Val();
        if (!canAdd())
            return true;
        _table.beginUpdate();
        auto& row = _table.getEmptyRow();
        populateDSRow(row);
        _table.push_back();
        _table.endUpdate();


        _itemsToUpdate.erase(std::remove(_itemsToUpdate.begin(), _itemsToUpdate.end(), itemID), _itemsToUpdate.end());
        _itemsToInsert.push_back(itemID);
        //neka ostaje za brisanje, mozda je pobrisanoriginalni artikal sa sifrom itemid, i sada dodajemo novi sa istom sifrom
        return true;
    }
    */
    if (pBtn == &_btnSave)
    {
        showYesNoQuestionAsync(QuestionID::Save, this, tr("alert"), tr("saveSure"), tr("Yes"), tr("No"));
        return true;
        //saveData();
        //return true;
    }

    return false;

}

bool ViewDepartment::onAnswer(td::UINT4 questionID, gui::Alert::Answer answer)
{
    if ((QuestionID)questionID == QuestionID::Save)
    {
        if (answer == gui::Alert::Answer::Yes) {
            saveData();
            showAlert(tr("succes"), tr("succesEE"));
        }
        return true;
    }
    return false;
}


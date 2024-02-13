#pragma once
#include "ViewCourse.h"
#include <td/Types.h>
#include "ViewIDs.h"


td::INT4 ViewCourse::findMaxID()
{
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select ifnull(max(ID_Predmeta), 0) as maxid from Predmet");
    dp::Columns pColumns = pSelect->allocBindColumns(1);
    td::INT4 maxID;
    pColumns << "maxid" << maxID;
    if (!pSelect->execute())
        return false;
    if (!pSelect->moveNext())
        return false;
    
    auto x = std::max_element(_itemsToInsert.begin(), _itemsToInsert.end());
    auto y = std::max_element(_itemsToUpdate.begin(), _itemsToUpdate.end());
 
    //provjera da li iteratori pokazuju na neku konkretnu vrijednost
    if (x == _itemsToInsert.end() && y == _itemsToUpdate.end()) 
        return ++maxID;
    if (x != _itemsToInsert.end() && y == _itemsToUpdate.end())
        if (maxID >= *x) return ++maxID;
        else {
            auto p1 = *x;
            return ++p1;
        }
    if (x == _itemsToInsert.end() && y != _itemsToUpdate.end())
        if (maxID >= *y) return ++maxID;
        else {
            auto p2 = *y;
            return ++p2;
        }
    auto p1 = *x;
    auto p2 = *y;
    if (maxID >= *x && maxID >= *y) return ++maxID;
    if (*x >= maxID && (*x) >= (*y)) return ++p1;
    if (*y >= maxID && (*y) >= (*x)) return ++p2;

    return ++maxID;
}

ViewCourse::ViewCourse(): _db(dp::getMainDatabase())
, _lblName(tr("name"))
, _lblIDCode(tr("sifraP"))
, _lblIDDEP(tr("dep"))
, _idDep(td::int4)
, _lblSemester(tr("semestar"))
, _semester(td::int4)
, _lblEcts(tr("ects"))
, _ects(td::int4)
, _lblElective(tr("elective"))
, _elective(td::int4)
, _lblMaxNum(tr("maxatt"))
, _maxNum(td::int4)
, _btnAdd(tr("add"))
, _btnSave(tr("save"))
, _btnReload(tr("Reload"))
, _btnDelete(tr("Delete"))
, _btnUpdate(tr("Update"))
, _btnInsert(tr("Insert"))
, _hlBtns(11)
, _gl(6, 4)
{
     setVisualID(View_COURSE);
    _hlBtns.appendSpace(4);
    _hlBtns.append(_btnSave, td::HAlignment::Left);
    _hlBtns.appendSpacer();
    _hlBtns.append(_btnReload);
    _hlBtns.append(_btnDelete);
    _hlBtns.append(_btnUpdate);
    _hlBtns.append(_btnInsert);
    _hlBtns.append(_btnAdd, td::HAlignment::Right);
    _btnAdd.setAsDefault();

    gui::GridComposer gc(_gl);
    gc.appendRow(_lblName);
    gc.appendCol(_name);
    gc.appendCol(_lblIDDEP);
    gc.appendCol(_idDep);
    gc.appendRow(_lblSemester);
    gc.appendCol(_semester);
    gc.appendCol(_lblEcts);
    gc.appendCol(_ects);
    gc.appendRow(_lblElective);
    gc.appendCol(_elective);
    gc.appendCol(_lblMaxNum);
    gc.appendCol(_maxNum);
    gc.appendRow(_lblIDCode);
    gc.appendCol(_idCode);

    gc.appendRow(_table, 0);
    gc.appendRow(_hlBtns, 0);
    gui::View::setLayout(&_gl);

    _elective.addItem(tr("mnd"), 0);  //obavezni
    _elective.addItem(tr("elctv"), 1); //izborni
    _elective.selectIndex(0);

    populateData();
    loadComboBox("select ID_Smjera as ID, Naziv_Smjera as Name from Smjer", _idDep);
}

ViewCourse::~ViewCourse()
{
    
}

void ViewCourse::populateData()
{
    //--------------kako postaviti da se za status ispisuje obavezni ili izborni
    _pDS = _db->createDataSet("select n.Naziv_Predmeta as Naziv, n.ID_Smjera, a.Naziv_Smjera as Smjer, n.Semestar as Semestar, n.ECTS_bodovi as ECTS, n.Izborni as Izborni, n.Max_Broj_Polaznika as maxBroj, n.Sifra_Predmeta, n.ID_Predmeta from Predmet n, Smjer a where n.ID_Smjera = a.ID_Smjera", dp::IDataSet::Execution::EX_MULT);
    dp::DSColumns cols(_pDS->allocBindColumns(9));

    cols << "Naziv" << td::string8 << "ID_Smjera"<< td::int4 << "Smjer" << td::string8 << "Semestar" << td::int4 << "ECTS" << td::int4 
        << "Izborni" << td::int4 << "maxBroj" << td::int4 <<"Sifra_Predmeta"<< td::string8 << "ID_Predmeta" << td::int4;

    if (!_pDS->execute())
    {
        _pDS = nullptr;
        return;
    }

    //------------0 za standardni ispis, bilo koji drugi broj za prilagodeni ispis
    initTable(1);
}

void ViewCourse::initTable(int type)
{
    //-----------moguci razliciti nacini ispisa, estetski ljepse

    if (type == 0)
    {
        _table.init(_pDS, {0,2,3,4,5,6});
    }
    else
    {
        gui::Columns visCols(_table.allocBindColumns(6));
        visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, tr("naziv"), tr("nazivTT"), 200, td::HAlignment::Left)
            << gui::Header(2, tr("smjer"), tr("smjerTT"), 170, td::HAlignment::Left)
            << gui::Header(3, tr("semestar"), tr("semestarTT"), 80, td::HAlignment::Right)
            << gui::Header(4, tr("ects"), tr("ectsTT"), 80, td::HAlignment::Right)
            << gui::Header(5, tr("izborni"), tr("izborniTT"), 80, td::HAlignment::Right)
            << gui::Header(6, tr("maxBr"), tr("maxbrTT"), 150, td::HAlignment::Right);
            //promijeniti izborni u status ako se promijeni ispis sa 0/1
        _table.init(_pDS);
    }


}

 bool ViewCourse::loadComboBox(td::String select, gui::DBComboBox & combo)
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

 bool ViewCourse::onChangedSelection(gui::TableEdit* pTE)
 {
     if (pTE == &_table)
     {
         int iRow = _table.getFirstSelectedRow();
         if (iRow < 0)
         {
             _name.toZero();
             _idDep.toZero();
             _semester.toZero();
             _ects.toZero();
             _maxNum.toZero();
             return true;
         }
         td::Variant val;
         dp::IDataSet* pDS = _table.getDataSet();
         auto& row = pDS->getRow(iRow);
         val = row[0]; 
         _name.setValue(val);
         val = row[1];
         _idDep.setValue(val);
         val = row[3];
         _semester.setValue(row[4]);
         val = row[4];
         _ects.setValue(val);
         val = row[5];
         _elective.setValue(val);
         _maxNum.setValue(row[6]);
         val = row[7];
         _idCode.setValue(val);

         return true;
     }
     return false;
 }

 //------------prima dodatni parametar za id (svako novo ucitavanje stvara probleme izvedbe koda)
 void ViewCourse::populateDSRow(dp::IDataSet::Row& row, td::INT4 i)
 {
      td::Variant val;
      _name.getValue(val);
      row[0].setValue(val);
      _idDep.getValue(val);
      row[1].setValue(val);
      row[2].setValue(_idDep.getSelectedText());
      _semester.getValue(val);
      row[3].setValue(val);
      _ects.getValue(val);

      row[4].setValue(val);
      _elective.getValue(val);
      row[5].setValue(val);
      _maxNum.getValue(val);
      row[6].setValue(val);
      _idCode.getValue(val);
      row[7].setValue(val);
      td::Variant x = i;
      row[8].setValue(x);
      // -------------------------------ucitavanje row[8] kao id koji se ne ucitava ali je potreban za kod
      
 }


 bool ViewCourse::canAdd() 
 {
     //-------------bespotrebna provjera id jer se vrsi automatska dodjela (za koji se provjerava da li vec postoji u bazi)   
     td::Variant pom = _semester.getValue();
     if (pom < 1) {
         showAlert(tr("alert"), tr("alertCRSS"));
         return false;
     }
     pom = _ects.getValue();
     if (pom < 1) {
         showAlert(tr("alert"), tr("alertCRSE"));
         return false;
     }
     pom = _maxNum.getValue();
     if (pom < 1) {
         showAlert(tr("alert"), tr("alertCRSM"));
         return false;
     }
     _idCode.getValue(pom);
     if(pom.isZero()){
         showAlert(tr("alert"), tr("alertNp"));
         return false;
     }
     _name.getValue(pom);
     if (pom.isZero()) {
         showAlert(tr("alert"), tr("alertNp"));
         return false;
      }
     _semester.getValue(pom);
     if (pom.isZero()) {
         showAlert(tr("alert"), tr("alertNp"));
         return false;
     }
     _ects.getValue(pom);

     if (pom.isZero()) {
         showAlert(tr("alert"), tr("alertNp"));
         return false;
     }
     _maxNum.getValue(pom);
     if (pom.isZero()) {
         showAlert(tr("alert"), tr("alertNp"));
         return false;
     }
     return true;
 }

 bool ViewCourse::existsInCourse(td::INT4 id)
 {
     //-------------bespotrebna provjera id jer se vrsi automatska dodjela (za koji se provjerava da li vec postoji u bazi)   
     return false;
 }

 bool ViewCourse::canUpdate(int iRow)
 {
     //-----------ne vrsi se promjena id
     td::Variant pom = _semester.getValue();
     if (pom < 1) {
         showAlert(tr("alert"), tr("alertCRSS"));
         return false;
     }
     pom = _ects.getValue();
     if (pom < 1) {
         showAlert(tr("alert"), tr("alertCRSE"));
         return false;
     }
     pom = _maxNum.getValue();
     if (pom < 1) {
         showAlert(tr("alert"), tr("alertCRSM"));
         return false;
     }
     return true;
 }

 td::INT4 ViewCourse::getIDfromTable(int rowID)
 {
     dp::IDataSet* pDS = _table.getDataSet();
     auto& row = pDS->getRow(rowID);
     return row[8].i4Val();
 }

 bool ViewCourse::canDelete(int iRow)
 {
     //-------------zasto se ne bi mogao izbrisati?
     return true;
 }

 bool ViewCourse::eraseCourse()
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
            // showAlert(tr("alert"), tr("alertDR")); ??? 
            return false;
        }
    }
    return true;
}

 bool ViewCourse::insertCourse()
 {
     dp::IStatementPtr pInsertCourseP(_db->createStatement("insert into Predmet (ID_Predmeta, Naziv_Predmeta, Sifra_Predmeta, ID_Smjera, Semestar, ECTS_bodovi, Izborni, Max_Broj_Polaznika) values (?,?,?,?,?,?,?,?)"));
     dp::Params pParams2(pInsertCourseP->allocParams());
     td::INT4 id,  idDep, semester, elective, maxNum, ects;
     td::String name, idCode;

     pParams2 << id << dp::toNCh(name, 100) << dp::toNCh(idCode, 100) << idDep << semester << ects << elective << maxNum;

     dp::IDataSet* pDS = _table.getDataSet();
     auto rowCnt = pDS->getNumberOfRows();
     for (size_t iRow = 0; iRow < rowCnt; ++iRow)
     {
         auto& row = pDS->getRow(iRow);
         id = row[8].i4Val();
         if (std::find(_itemsToInsert.begin(), _itemsToInsert.end(), id) == _itemsToInsert.end())
             continue;
         name = row[0];
         idCode = row[7];
         idDep = row[1].i4Val();
         semester = row[3].i4Val();
         ects = row[4].i4Val();
         elective = row[5].i4Val();
         maxNum = row[6].i4Val();

         if (!pInsertCourseP->execute())
         {
             //showAlert(tr("alert"), tr("alertDR")); ???
             return false;
         }
     }
     return true;
 }

 bool ViewCourse::updateCourse()
 {
     dp::IStatementPtr pChangeCourse(_db->createStatement("UPDATE Predmet SET Naziv_Predmeta=?, Sifra_Predmeta=?, ID_Smjera=?, Semestar=?, ECTS_bodovi=?, Izborni=?, Max_Broj_Polaznika=? WHERE ID_Predmeta=?"));
     dp::Params pParams2(pChangeCourse->allocParams());
     td::INT4 id, idDep, semester, elective, maxNum, ects;
     td::String name, idCode;
     pParams2 << dp::toNCh(name, 100) << dp::toNCh(idCode, 100) << idDep << semester << ects << elective << maxNum << id;

     dp::IDataSet* pDS = _table.getDataSet();
     auto rowCnt = pDS->getNumberOfRows();
     for (size_t iRow = 0; iRow < rowCnt; ++iRow)
     {
         auto& row = pDS->getRow(iRow);
         id = row[8].i4Val();
         if (std::find(_itemsToUpdate.begin(), _itemsToUpdate.end(), id) == _itemsToUpdate.end())
             continue;

         name = row[0];
         idCode = row[7];
         idDep = row[1].i4Val();
         semester = row[3].i4Val();
         ects = row[4].i4Val();
         elective = row[5].i4Val();
         maxNum = row[6].i4Val();

         if (!pChangeCourse->execute())
         {
             return false;
         }
     }
     return true;
 }



bool  ViewCourse::saveData()
{
    dp::Transaction tran(_db);
    if (!eraseCourse())
        return false;

    if (!insertCourse())
        return false;

    if (!updateCourse())
       return false;

    if (tran.commit())
    {
        _itemsToDelete.clear();
        _itemsToInsert.clear();
        _itemsToUpdate.clear();
    }
    return true;
}


bool ViewCourse::onClick(gui::Button* pBtn)
{
    if (pBtn == &_btnReload)
    {
        _table.reload();
        _itemsToDelete.clear();
        _itemsToInsert.clear();
        _itemsToUpdate.clear();
        return true;
    }
    if (pBtn == &_btnDelete)
    {
        int iRow = _table.getFirstSelectedRow();
        if (iRow < 0)
            return true;
       /* if (!canDelete(iRow))
            return true;*/
        //-------------uzimamo id reda kojeg je potrebno obrisati
        td::INT4 itemid = getIDfromTable(iRow);

        _table.beginUpdate();
        _table.removeRow(iRow);
        _table.endUpdate();
        _itemsToDelete.push_back(itemid);

        //--------------ako brisemo predmet koji se nije sacuvan u bazi, ali se nalazi u vektorima obrisati ga iz njih
        _itemsToInsert.erase(std::remove(_itemsToInsert.begin(), _itemsToInsert.end(), itemid), _itemsToInsert.end());
        _itemsToUpdate.erase(std::remove(_itemsToUpdate.begin(), _itemsToUpdate.end(), itemid), _itemsToUpdate.end());

        return true;
    }
    if (pBtn == &_btnUpdate)
    {
        int iRow = _table.getFirstSelectedRow();
        if (iRow < 0)
            return true;
        if (!canUpdate(iRow))
            return true;
        td::INT4 itemid = getIDfromTable(iRow);

        _table.beginUpdate();
        auto& row = _table.getCurrentRow();
        populateDSRow(row, itemid);
        _table.updateRow(iRow);
        _table.endUpdate();

        if (std::find(_itemsToInsert.begin(), _itemsToInsert.end(), itemid) == _itemsToInsert.end())
            _itemsToUpdate.push_back(itemid);

        return true;
    }
    if (pBtn == &_btnInsert)
    {
        int iRow = _table.getFirstSelectedRow();
        if (iRow < 0)
            iRow = 0;
        if (!canAdd())
            return true;
        td::INT4 itemid = getIDfromTable(iRow);
        //-------------pozivamo funkciju za trazenje maxID+1 iz bazi i vektora za update i insert
        td::INT4 id = findMaxID();

        _table.beginUpdate();
        auto& row = _table.getEmptyRow();
        populateDSRow(row, id);
        _table.insertRow(iRow);
        _table.endUpdate();

        _itemsToUpdate.erase(std::remove(_itemsToUpdate.begin(), _itemsToUpdate.end(), id), _itemsToUpdate.end());
        _itemsToInsert.push_back(id);
        return true;
    }
    if (pBtn == &_btnAdd)
    {
        td::INT4 itemid = findMaxID();
        if (!canAdd())
            return true;
        _table.beginUpdate();
        auto& row = _table.getEmptyRow();
        populateDSRow(row, itemid);
        _table.push_back();
        _table.endUpdate();

        _itemsToUpdate.erase(std::remove(_itemsToUpdate.begin(), _itemsToUpdate.end(), itemid), _itemsToUpdate.end());
        _itemsToInsert.push_back(itemid);
        return true;
    }
    if (pBtn == &_btnSave)
    {
        showYesNoQuestionAsync(QuestionIDDD::Saveee, this, tr("alert"), tr("saveSure"), tr("Yes"), tr("No"));
        return true;
       // saveData();
        //return true;
    }
    return false;
}

bool ViewCourse::onAnswer(td::UINT4 questionID, gui::Alert::Answer answer)
{
    if ((QuestionIDDD)questionID == QuestionIDDD::Saveee)
    {
        if (answer == gui::Alert::Answer::Yes) {
            saveData();
            
            showAlert(tr("succes"), tr("succesEE"));
        }
        return true;
    }
    return false;
}



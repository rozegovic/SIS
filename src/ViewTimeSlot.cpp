#include "ViewTimeSlot.h"
#include "Globals.h"
#include "ViewIDs.h"
#include <td/StringConverter.h>

#include <dp/IDataSetDelegate.h>
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>

ViewTimeSlot::ViewTimeSlot(td::INT4 SubjectID,td::INT4 ViewID ) :
    _LblSubjName(tr("AttSubj"))
    
    //, _LblType(tr("AttType")),
    ,_hlBtnsDB(4)
    ,_lblTable2("Izabrani termin/i:")
    , _btnDEnroll(tr("DEnroll"))
   // , _btnReload(tr("Reload"))
    , _btnEnroll(tr("Enroll"))
    //, _type(td::int4)
    , _db(dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3))
    , _gl(5, 4)
    , _SubjectID(SubjectID)
{
    setVisualID(ViewID);
   _hlBtnsDB.appendSpacer();
    _hlBtnsDB.append(_btnDEnroll);
//    _hlBtnsDB.append(_btnReload);
    _hlBtnsDB.append(_btnEnroll);
    _hlBtnsDB.appendSpacer();
    _btnDEnroll.setType(gui::Button::Type::Default);
    _btnEnroll.setType(gui::Button::Type::Constructive);
    _Subject.setAsReadOnly();

    gui::GridComposer gc(_gl);
    gc.appendRow(_LblSubjName);
    gc.appendCol(_Subject);
    gc.appendRow(_table, 0);
    gc.appendRow(_lblTable2);
    gc.appendRow(_table2, 0);
    gc.appendRow(_hlBtnsDB, 0);

    gui::View::setLayout(&_gl);
    getSubjectName();

    _db = dp::getMainDatabase();

    populateDataForTable();
   // _table.setFont(gui::Font::ID::SystemSmallestBoldItalic);
   // _table.setBold();
    populateTable2();
  //  _table2.setBold();
}

ViewTimeSlot::~ViewTimeSlot() {
}

void ViewTimeSlot::initTable()
{
    gui::Columns visCols(_table.allocBindColumns(3));
   // visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, tr("Type")) << gui::Header(1, tr("Time")) << gui::Header(2, tr("Date"));
    visCols << gui::Header(0, tr("Type")) << gui::Header(1, tr("Time")) << gui::Header(2, tr("Date"));
    _table.init(_pDS); 
}

void ViewTimeSlot::initTable2()
{
    gui::Columns visCols(_table2.allocBindColumns(3));
    visCols  << gui::Header(0, tr("Type")) << gui::Header(1, tr("Time")) << gui::Header(2, tr("Date"));
    _table2.init(_pDS2);             
}

bool ViewTimeSlot::IsEnrolled(td::INT4 ID_stud, td::INT4 ID_Pred) {
    auto pDB = dp::getMainDatabase();
    
    _pDSpos = pDB->createDataSet("SELECT ID_Studenta as IDs ,TipPredavanjaID as IDp FROM TerminiStudenti", dp::IDataSet::Execution::EX_MULT);
    dp::DSColumns cols(_pDSpos->allocBindColumns(2));
    cols << "IDs" << td::int4 << "IDp" << td::int4;

    if (!_pDSpos->execute())
    {
        _pDSpos = nullptr;
        return false;
    }
    size_t nRows = _pDSpos->getNumberOfRows();
    for (size_t i = 0; i < nRows; ++i)
    {
        auto row = _pDSpos->getRow(i);
        if (row[0].i4Val() == ID_stud && row[1].i4Val() == ID_Pred)
        {
            td::INT4 pom = row[1].i4Val();
            return true;
        }
    }
    return false;

}

bool ViewTimeSlot::IsTherePlace(td::INT4 tID) {
      
     // dp::IStatementPtr _pDS3 = dp::getMainDatabase()->createStatement("SELECT Max_br_pol as max, Br_prijavljenih as trenutnih FROM Termini WHERE Termini.ID = ? ");
      _pDS3 = dp::getMainDatabase()->createStatement("SELECT Max_br_pol, Br_prijavljenih  FROM Termini WHERE ID = ? ");
      
      dp::Params pParams(_pDS3->allocParams());
      pParams << tID;

      dp::Columns pCols =_pDS3->allocBindColumns(2); ///
      td::INT4 max ;
      td::INT4 trenutnih ;
     
  //    pCols << "max " << max << "trenutnih" << trenutnih;
      pCols << "Max_br_pol" << max << "Br_prijavljenih" << trenutnih;

      if (!_pDS3->execute())
          return false;
      if (!_pDS3->moveNext())
          return false;
      
      if (trenutnih == max) return false;
      return true;
 }
 

void ViewTimeSlot::populateDataForTable()
{
    _pDS = dp::getMainDatabase()->createDataSet("SELECT b.Naziv AS tip, a.Vrijeme AS time, a.Dan AS date, a.TipPredavanjaID as idPred, a.ID as idTerm from Termini a, TipPredavanja b WHERE b.ID = a.TipPredavanjaID and a.TipPredavanjaID!=1 and a.Predmet_ID=?", dp::IDataSet::Execution::EX_MULT);
    dp::Params pParams(_pDS->allocParams());
    pParams << _SubjectID;
    dp::DSColumns cols(_pDS->allocBindColumns(5));
    cols << "tip" << td::string8 << "time" << td::time << "date" << td::string8 <<"idPred"<<td::int4<<"idTerm"<<td::int4;

    if (!_pDS->execute())
    {
        _pDS = nullptr;
        return;
    }
    initTable();
}

void ViewTimeSlot::populateTable2()
{
    auto sID = Globals::_currentUserID;
  
    _pDS2 = dp::getMainDatabase()->createDataSet("SELECT b.Naziv AS tip, a.Vrijeme AS time, a.Dan AS date, c.ID_Studenta as IDs from Termini a, TipPredavanja b, TerminiStudenti c WHERE c.ID_Termina = a.ID and c.TipPredavanjaID = b.ID and c.ID_Studenta = ? AND a.Predmet_ID = ?", dp::IDataSet::Execution::EX_MULT);
    
    dp::Params pParams(_pDS2->allocParams());
    pParams << sID << _SubjectID;
    
    dp::DSColumns cols(_pDS2->allocBindColumns(3));
    cols << "tip" << td::string8 << "time" << td::time << "date" << td::string8;
    if (!_pDS2->execute())
    {
        _pDS2 = nullptr;
        return;
    }
    initTable2();

}


void ViewTimeSlot::getSubjectName() {
    
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT Naziv_Predmeta FROM Predmet WHERE ID_Predmeta = ?");
    dp::Params parDS(pSelect->allocParams());
    parDS << _SubjectID;
    dp::Columns pCols = pSelect->allocBindColumns(1);
    td::String Predmet;
    pCols << "Naziv_Predmeta" << Predmet;
    if (!pSelect->execute()) {
        Predmet = "Haos";
    }
    while (pSelect->moveNext())
    {
        td::Variant val;
        val = Predmet;
        _Subject.setValue(val);

    }
}


bool ViewTimeSlot::saveData1() { //upis
   dp::IStatementPtr pInsStat(dp::getMainDatabase()->createStatement("INSERT INTO TerminiStudenti (ID_Studenta,ID_Termina,TipPredavanjaID) VALUES(?,?,?)"));
    dp::Params parDS(pInsStat->allocParams());
    dp::Transaction tr(dp::getMainDatabase());
    td::INT4 sID, tID,pID;
    parDS << sID << tID << pID;
    sID = Globals::_currentUserID;
    td::INT4 curRow = _pDS->getCurrentRowNo();
    auto row = _pDS->getRow(curRow);
    tID = row[4].i4Val();
    pID = row[3].i4Val();
  
    if (!pInsStat->execute()){
        tr.rollBack();
        return false;
        
    }
    tr.commit();
    return true;

}

bool ViewTimeSlot::saveData2() { //ispis 
    td::INT4 sID, tID, pID;
    sID = Globals::_currentUserID;
    td::INT4 curRow = _pDS->getCurrentRowNo();
    auto row = _pDS->getRow(curRow);
    tID = row[4].i4Val();
    pID = row[3].i4Val();
    if (IsEnrolled(sID, pID))
    {
        dp::IStatementPtr pInsStat(dp::getMainDatabase()->createStatement("Delete from TerminiStudenti where ID_Studenta=? and TipPredavanjaID=?"));
        dp::Params parDS(pInsStat->allocParams());
        parDS << sID << pID;
        if (!pInsStat->execute())
            return false;

           
    }
    return true;
}

bool ViewTimeSlot::onClick(gui::Button* pBtn)
{
   /* if (pBtn == &_btnReload)
    {
       // _table.reload();
        UpdatePresentDataSet();
  
        return true;
    }*/
    if (pBtn == &_btnEnroll)
    {
        td::INT4 tID, pID, sID;
        sID = Globals::_currentUserID;
        td::INT4 curRow = _pDS->getCurrentRowNo();
        auto row = _pDS->getRow(curRow);
        tID = row[4].i4Val();
        pID = row[3].i4Val();
        if (IsEnrolled(sID, pID))
        {
            showAlert(tr("alert"), tr("alertPr"));
            return false;
        }
    
     if (!IsTherePlace(tID)) {   //funkcija u 97.liniji
           showAlert(tr("alert"), ("Nazalost vise nema mjesta!"));
           return false;
    }
        
        saveData1();
        UpdatePresentDataSet();
        _pDS4 = dp::getMainDatabase()->createStatement("UPDATE Termini SET Br_prijavljenih = Br_prijavljenih + 1 WHERE ID = ? ");
        //ovaj update radi u bazi, ali pokazivac ?
        dp::Params pParams(_pDS4->allocParams());
        pParams << tID;
        if (!_pDS4->execute())
            return false;
        if (!_pDS4->moveNext())
            return false;
      
    //  UpdatePresentDataSet();
      //  _table2.reload();                    ///
        return true;
       
    }

    if (pBtn == &_btnDEnroll)
    {
        td::INT4 tID;
        td::INT4 curRow = _pDS->getCurrentRowNo();
        auto row = _pDS->getRow(curRow);
        tID = row[4].i4Val();

        saveData2();
        UpdatePresentDataSet();
    
    _pDS4 = dp::getMainDatabase()->createStatement("UPDATE Termini SET Br_prijavljenih = Br_prijavljenih - 1 WHERE ID = ? ");
        dp::Params pParams(_pDS4->allocParams());
        pParams << tID;
        if (!_pDS4->execute())
            return false;
        if (!_pDS4->moveNext())
            return false;
       

        _table.reload();
     //   _table2.reload();
        _table.selectRow(0, true);

    //    UpdatePresentDataSet();
        return true;

    }

    return false;

}

void ViewTimeSlot::UpdatePresentDataSet() {
   
    auto sID = Globals::_currentUserID;
    dp::IDataSetPtr pomDS = dp::getMainDatabase()->createDataSet("SELECT b.Naziv AS tip, a.Vrijeme AS time, a.Dan AS date from Termini a,  TipPredavanja b, TerminiStudenti c WHERE c.ID_Termina = a.ID and c.TipPredavanjaID = b.ID and c.ID_Studenta = ?", dp::IDataSet::Execution::EX_MULT);
    
    dp::Params pParams(pomDS->allocParams());
    pParams << Globals::_currentUserID;           

    dp::DSColumns cols(pomDS->allocBindColumns(3));
    cols << "tip" << td::string8 << "time" << td::time << "date" << td::string8;
    if (!pomDS->execute())
    {
        pomDS = nullptr;
        return;
    }

    _table2.clean();

    size_t nRows = pomDS->getNumberOfRows();
    for (size_t i = 0; i < nRows; i++) {
        _table2.beginUpdate();
        auto& rowpom = pomDS->getRow(i);
        auto& row = _table2.getEmptyRow();
        row = rowpom;
        _table2.push_back();
        _table2.endUpdate();
    }

}

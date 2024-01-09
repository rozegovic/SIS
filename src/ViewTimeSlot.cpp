#include "ViewTimeSlot.h"
#include "Globals.h"
#include "ViewIDs.h"
#include <td/StringConverter.h>

ViewTimeSlot::ViewTimeSlot(td::INT4 SubjectID) :
    _LblSubjName(tr("AttSubj"))
    //, _LblType(tr("AttType")),
    ,_hlBtnsDB(5)
    , _btnDEnroll(tr("DEnroll"))
    , _btnReload(tr("Reload"))
    , _btnEnroll(tr("Enroll"))
    //, _type(td::int4)
    , _db(dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3))
    , _gl(3, 4)
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
   // gc.appendRow(_LblType);
   // gc.appendCol(_type);
    gc.appendRow(_table, 0);
    gc.appendRow(_hlBtnsDB, 0);
    gui::View::setLayout(&_gl);
    getSubjectName();
   
 
    _db = dp::getMainDatabase();

    populateDataForTable();
    _table.setBold();
}
ViewTimeSlot::~ViewTimeSlot() {
}

void ViewTimeSlot::initTable()
{
    gui::Columns visCols(_table.allocBindColumns(3));
    visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, tr("Type")) << gui::Header(1, tr("Time")) << gui::Header(2, tr("Date")) ;
    _table.init(_pDS); 
}
void ViewTimeSlot::populateDataForTable()
{
    _pDS = dp::getMainDatabase()->createDataSet("SELECT b.Naziv AS tip, a.Vrijeme AS time, a.Datum AS date,a.TipPredavanjaID as idPred, a.ID as idTerm from Termini a, TipPredavanja b WHERE b.ID = a.TipPredavanjaID and a.TipPredavanjaID!=1  and a.Predmet_ID=?", dp::IDataSet::Execution::EX_MULT);
    dp::Params pParams(_pDS->allocParams());
    pParams << _SubjectID;
    dp::DSColumns cols(_pDS->allocBindColumns(5));
    cols << "tip" << td::string8 << "time" << td::time << "date" << td::date<<"idPred"<<td::int4<<"idTerm"<<td::int4;

    if (!_pDS->execute())
    {
        _pDS = nullptr;
        return;
    }
    initTable();
}
void ViewTimeSlot::getSubjectName() {

    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT Naziv_Predmeta FROM Predmet WHERE ID_Predmeta = ?");
    dp::Params parDS(pSelect->allocParams());
    //d::INT4 IDPredmeta = Globals::_IDSubjectSelection;
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

bool ViewTimeSlot::IsEnrolled(td::INT4 ID_stud ,td::INT4 ID_Pred) {
    auto pDB = dp::getMainDatabase();
    _pDSpos = pDB->createDataSet("SELECT ID_Studenta as IDs TipPredavanjaID as IDp  FROM TerminiStudenti", dp::IDataSet::Execution::EX_MULT);
    dp::DSColumns cols(_pDS->allocBindColumns(2));
    cols << "IDs" << td::int4  << "IDp" << td::int4;

    if (!_pDSpos->execute())
    {
        _pDSpos = nullptr;
        return false;
    }
    size_t nRows = _pDSpos->getNumberOfRows();
    for (size_t i = 0; i < nRows; ++i)
    {
        auto row = _pDSpos->getRow(i);
        if (row[0] == ID_stud && row[1] == ID_Pred)
            return true;
    }
    return false;
}


bool ViewTimeSlot::saveData1() { //upis
   dp::IStatementPtr pInsStat(dp::getMainDatabase()->createStatement("INSERT INTO TerminiStudenti (ID_Studenta,ID_Termina,TipPredavanjaID) VALUES(?,?,?)"));
    dp::Params parDS(pInsStat->allocParams());
    td::INT4 sID, tID,pID;
    parDS << sID << tID<<pID;
    sID = Globals::_currentUserID;
    td::INT4 curRow = _pDS->getCurrentRowNo();
    auto row = _pDS->getRow(curRow);
    tID = row[4].i4Val();
    pID = row[3].i4Val();
    if (IsEnrolled(sID,pID) == true)
    {
        return false;
    }
    if (!pInsStat->execute())
        return false;

    return true;

 
}

bool ViewTimeSlot::saveData2() { //ispis 
    //   
  //  dp::IStatementPtr pInsStat(dp::getMainDatabase()->createStatement("DELETE FROM TerminiStudenti (ID_studenta,ID_termina,TipPredavanjaID) VALUES(ID_stud,ID_term,ID_Pred)"));
   // if (!pInsStat->execute())
    //    return false;
    return true;
}

bool ViewTimeSlot::onClick(gui::Button* pBtn)
{//provjeru izvr�iti
  

    if (pBtn == &_btnReload)
    {
      //  _table.reload();
       // _table.selectRow(0, true);
  
        return true;
    }
    if (pBtn == &_btnEnroll)
    {
        td::INT4 tID, pID,sID;
        sID = Globals::_currentUserID;
        td::INT4 curRow = _pDS->getCurrentRowNo();
        auto row = _pDS->getRow(curRow);
        tID = row[4].i4Val();
        pID = row[3].i4Val();
        if (IsEnrolled(sID, pID) == true)
        {
showAlert(tr("alert"), tr("alertPr"));
            return true;
        }

        saveData1();
      //  _table.reload();
       // _table.selectRow(0, true);
        return true;
       
        

    }

    if (pBtn == &_btnDEnroll)
    {
        saveData2();
        _table.reload();
        _table.selectRow(0, true);
        return true;

    }

    return false;

}
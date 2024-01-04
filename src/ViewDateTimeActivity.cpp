#include "ViewDateTimeActivity.h"
#include "Globals.h"

//
//ViewDateTimeActivity::ViewDateTimeActivity(td::INT4 ActivityID) :
//    _LblActName(tr("Activity")),
//    _LblDateBegin(tr("ActivityDateB")),
//    _LblTimeBegin(tr("ActivityTimeB")), 
//    _LblDateEnd(tr("ActivityDateE")),
//    _LblTimeEnd(tr("ActivityTimeE")),
//     _btnAdd(tr("add"), tr("AddTT"))
//    //, _btnUpdate(tr("Update"), tr("UpdateTT"))
//    , _btnDelete(tr("Delete"), tr("DeleteTT"))
//    , _btnSave(tr("Save"), tr("SaveTT"))
//    , _hlBtnsDB(5)
//    , _gl(6, 2)
//    , _ActivityID(ActivityID)
//{
//
//
//    _hlBtnsDB.appendSpacer();
//    _hlBtnsDB.append(_btnDelete);
//    // _hlBtnsDB.append(_btnUpdate); 
//    _hlBtnsDB.append(_btnSave);
//    _hlBtnsDB.append(_btnAdd);
//    _hlBtnsDB.appendSpacer();
//
//
//    //  _btnUpdate.setType(gui::Button::Type::Default);
//    _btnSave.setType(gui::Button::Type::Default);
//    _btnDelete.setType(gui::Button::Type::Destructive);
//    _btnAdd.setType(gui::Button::Type::Constructive);
//
//    gui::GridComposer gc(_gl);
//
//
//  /*gc.appendRow(_LblActName);
//    gc.appendCol(_Activity);*/  
//
//    gc.appendRow(_LblDateBegin);
//    gc.appendCol(_dateB);
//
//    gc.appendRow(_LblTimeBegin);
//    gc.appendCol(_timeB);
//
//    gc.appendRow(_LblDateEnd);
//    gc.appendCol(_dateE);
//
//    gc.appendRow(_LblTimeEnd);
//    gc.appendCol(_timeE);
//
//    gc.appendRow(_table, 0);
//    gc.appendRow(_hlBtnsDB, 0);
//
//    gui::View::setLayout(&_gl);
//    populateData();
//
//    _table.init(_pDS, { 0, 1, 2 });
//    if (_pDS->getNumberOfRows())
//    {
//        _table.selectRow(0, true);
//    }
//}
//
//void ViewDateTimeActivity::populateData()
//{
//    auto pDB = dp::getMainDatabase();
//    _pDS = pDB->createDataSet("SELECT a.Datum_Pocetka AS datumb, a.Vrijeme_Pocetka AS vrijemeb, a.Datum_Kraja AS datume, a.Vrijeme_Kraja AS vrijemee, a.Vrijeme_Prijave FROM Rokovi a", dp::IDataSet::Execution::EX_MULT);
//
//    dp::Params parDS(_pDS->allocParams());
//    //td::INT4 IDPredmeta = Globals::_IDSubjectSelection;
//
//    //u parDS ce se ucitavati Globals::CurrentActivity
//    parDS << _ActivityID;
//
//    dp::DSColumns cols(_pDS->allocBindColumns(4));
//    cols << "datumb" << td::date << "vrijemeb" << td::time << "datume" << td::date << "vrijemee" << td::time << "vrijemep" << td::time;
//
//    if (!_pDS->execute())
//    {
//        _pDS = nullptr;
//        return;
//    }
//}
//
//bool ViewDateTimeActivity::onChangedSelection(gui::TableEdit* pTE) {
//    if (pTE == &_table) {
//        int iRow = _table.getFirstSelectedRow();
//        if (iRow < 0) {
//            return true;
//        }
//        td::Variant val;
//        dp::IDataSet* pDS = _table.getDataSet();
//        auto& row = pDS->getRow(iRow);
//        val = row[0];
//        _dateB.setValue(val);
//
//        val = row[1];
//        _timeB.setValue(val);
//
//        val = row[2];
//        _dateE.setValue(val);
//
//        val = row[3];
//        _timeE.setValue(val);
//
//        val = row[4];
// 
//        _timeP.setValue(val);
//
//        return true;
//    }
//    return false;
//}
//
//void ViewDateTimeActivity::populateDSRow(dp::IDataSet::Row& row)
//{
//    td::Variant val;
//    _dateB.getValue(val);
//    row[0].setValue(val);
//
//    _timeB.getValue(val);
//    row[1].setValue(val);
//
//    _dateE.getValue(val);
//    row[2].setValue(val);
//
//    _timeE.getValue(val);
//    row[3].setValue(val);
//
//}
//
//void ViewDateTimeActivity::saveData()
//{
//    dp::IStatementPtr pInsStat(dp::getMainDatabase()->createStatement("INSERT INTO  Rokovi ( ID_Aktivnosti, Datum_Pocetka, Vrijeme_Pocetka,Datum_Kraja, Vrijeme_Kraja, Vrijeme_Prijave, ID_Predmeta) VALUES(?,?,?,?,?,?,?)"));
//    dp::Params parDS(pInsStat->allocParams());
//    dp::Transaction tr(dp::getMainDatabase());
//
//    td::Date datb;
//    td::Time tb;
//    td::Date datend;
//    td::Time te;
//    td::Time tp;
//    td::INT4 predmet, aktivnost;
//    td::Variant val;
//    parDS << aktivnost << datb << tb << datend << te << tp << predmet;
//    dp::IStatementPtr pDel(dp::getMainDatabase()->createStatement("DELETE FROM Rokovi"));
//    if (!pDel->execute())
//        return;
//    size_t nRows = _pDS->getNumberOfRows();
//    for (size_t i = 0; i < nRows; ++i)
//    {
//        //auto row = _pDS->getRow(i);
//        ////row[0].getValue (val);
//        //dat = row[0];
//        //tip = row[3].i4Val();
//        //predmet = 1;
//        //t = row[1];
//
//        //if (!pInsStat->execute())
//        //    return;
//    }
//    tr.commit();
//    return;
//}
//
//void ViewDateTimeActivity::SetCurrentSubject()
//{
//   
//    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT ID_Predmeta FROM Aktivnost WHERE ID_Aktivnosti = ?");
//    dp::Params parDS(pSelect->allocParams());
//    //d::INT4 IDPredmeta = Globals::_IDSubjectSelection;
//    parDS << _ActivityID;
//    dp::Columns pCols1 = pSelect->allocBindColumns(1);
//    td::INT4 idp;
//    pCols1 << "ID_Predmeta" << idp;
//    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT Naziv_Predmeta FROM Predmet WHERE ID_Predmeta = ?");
//    dp::Params parDS1(pSelect->allocParams());
//    parDS1 << idp;
//    dp::Columns pCols2 = pSelect->allocBindColumns(1);
//    td::String Predmet;
//    pCols2 << "Naziv_Predmeta" << Predmet;
//    if (!pSelect->execute()) {
//        Predmet = "NEMA";
//    }
//    while (pSelect->moveNext())
//    {
//        td::Variant val;
//        val = Predmet;
//        _Subject.setValue(val);
//
//    }
//
//}
//
//
//bool ViewDateTimeActivity::onClick(gui::Button* pBtn)
//{
//    if (pBtn == &_btnDelete)
//    {
//        int iRow = _table.getFirstSelectedRow();
//        if (iRow < 0)
//            return true;
//        _table.beginUpdate();
//        _table.removeRow(iRow);
//        _table.endUpdate();
//        //   saveData();
//          // _table.reload();
//        return true;
//    }
//
//    if (pBtn == &_btnAdd)
//    {
//
//        td::Variant val1, val2,val3,val4;
//        _dateB.getValue(val1);
//        _timeB.getValue(val2);
//        _dateE.getValue(val3);
//        _timeE.getValue(val4);
//        if (doesItDexist(val1.dateVal(), val2.timeVal()))
//        {
//            showAlert(tr("alert"), tr("alertAttTxt"));
//            return true;
//        }
//
//        _table.beginUpdate();
//        auto& row = _table.getEmptyRow();
//        populateDSRow(row);
//        _table.push_back();
//        _table.endUpdate();
//        return true;
//    }
//    if (pBtn == &_btnSave) {
//        saveData();
//        //  _table.reload();
//    }
//
//    return false;
//}
//bool ViewDateTimeActivity::doesItDexist(td::Date d, td::Time t)
//{
//    size_t nRows = _pDS->getNumberOfRows();
//    for (size_t i = 0; i < nRows; ++i)
//    {
//        auto row = _pDS->getRow(i);
//        if (row[0].dateVal() == d && row[1].timeVal() == t)
//            return true;
//    }
//    return false;
//}
#include "ViewAttendance.h"
#include "Globals.h"


ViewAttendance::ViewAttendance(td::INT4 SubjectID):
_LblSubjName(tr("AttSubj")),
_LblDate(tr("AttDate")),
_LblTime(tr("AttTime"))
,_LblType(tr("AttType"))
, _btnAdd(tr("add"), tr("AddTT"))
//, _btnUpdate(tr("Update"), tr("UpdateTT"))
, _btnDelete(tr("Delete"), tr("DeleteTT"))
,_btnSave(tr("Save"), tr("SaveTT"))
, _type(td::int4)
, _hlBtnsDB(5)
,_gl(6, 2)
,_SubjectID(SubjectID)
{
    
    
    _hlBtnsDB.appendSpacer();
    _hlBtnsDB.append(_btnDelete);
   // _hlBtnsDB.append(_btnUpdate); 
    _hlBtnsDB.append(_btnSave);
    _hlBtnsDB.append(_btnAdd);
    _hlBtnsDB.appendSpacer();
   
    
  //  _btnUpdate.setType(gui::Button::Type::Default);
    _btnSave.setType(gui::Button::Type::Default);
    _btnDelete.setType(gui::Button::Type::Destructive);
    _btnAdd.setType(gui::Button::Type::Constructive);

    SetCurrentSubject();
    _Subject.setAsReadOnly();
    
    gui::GridComposer gc(_gl);
    
    
    gc.appendRow(_LblSubjName);
    gc.appendCol(_Subject);

    gc.appendRow(_LblDate);
    gc.appendCol(_date);
    
    gc.appendRow(_LblTime);
    gc.appendCol(_time);
  
    gc.appendRow(_LblType);
    gc.appendCol(_type);

    gc.appendRow(_table, 0);
    gc.appendRow(_hlBtnsDB, 0);

    gui::View::setLayout(&_gl);
    populateRoleCombo(_type);
    populateData();

    _table.init(_pDS, {0, 1, 2});
    if (_pDS->getNumberOfRows())
    {
        _table.selectRow(0, true);
    }
   
    
}
void ViewAttendance::SetCurrentSubject(){
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT Naziv_Predmeta FROM Predmet WHERE ID_Predmeta = ?");
    dp::Params parDS(pSelect->allocParams());
    //d::INT4 IDPredmeta = Globals::_IDSubjectSelection;
    parDS << _SubjectID;
    dp::Columns pCols = pSelect->allocBindColumns(1);
    td::String Predmet;
    pCols << "Naziv_Predmeta" << Predmet;
    if(!pSelect->execute()){
        Predmet = "Haos";
    }
    while (pSelect->moveNext())
    {
        td::Variant val;
        val = Predmet;
        _Subject.setValue(val);

    }
    
}

void ViewAttendance::populateRoleCombo(gui::DBComboBox &combo)
{
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT ID, Naziv FROM TipPredavanja");
    dp::Columns pCols = pSelect->allocBindColumns(2);
    td::String tip;
    td::INT4 id;
    pCols << "ID" << id << "Naziv" << tip;
    pSelect->execute();

    while (pSelect->moveNext())
    {
        combo.addItem(tip, id);
    }
    combo.selectIndex(0);
}
void ViewAttendance::populateData()
{
    auto pDB = dp::getMainDatabase();
    _pDS = pDB->createDataSet("SELECT a.Datum AS datum, a.Vrijeme AS vrijeme, b.Naziv AS Tip, b.ID as ID FROM Termini a, TipPredavanja b WHERE a.TipPredavanjaID = b.ID and a.Predmet_ID = ?", dp::IDataSet::Execution::EX_MULT);
    
    dp::Params parDS(_pDS->allocParams());
    //td::INT4 IDPredmeta = Globals::_IDSubjectSelection;
    
    //u parDS ce se ucitavati Globals::CurrentSubject
    parDS << _SubjectID;
    
    dp::DSColumns cols(_pDS->allocBindColumns(4));
    cols << "datum" << td::date << "vrijeme" << td::time<< "Tip" << td::string8 << "ID" << td::int4;

    if (!_pDS->execute())
    {
        _pDS = nullptr;
        return;
    }
}
void ViewAttendance::populateDSRow(dp::IDataSet::Row& row)
{
    td::Variant val;
    _date.getValue(val);
    row[0].setValue(val);

    _time.getValue(val);
    row[1].setValue(val);

    _type.getValue(val);
    row[3] = val.i4Val();
    row[2].setValue(_type.getSelectedText());

}
bool ViewAttendance::onChangedSelection(gui::TableEdit* pTE) {
    if (pTE == &_table) {
        int iRow = _table.getFirstSelectedRow();
        if (iRow < 0) {
            return true;
        }
        td::Variant val;
        dp::IDataSet* pDS = _table.getDataSet();
        auto& row = pDS->getRow(iRow);
        val = row[0];
        _date.setValue(val);

        val = row[1];
        _time.setValue(val);

        val = row[3];
        _type.setValue(val);

        return true;
    }
    return false;
}
void ViewAttendance::saveData()
{
    dp::IStatementPtr pInsStat(dp::getMainDatabase()->createStatement("INSERT INTO Termini ( Datum, TipPredavanjaID, Predmet_ID, Vrijeme) VALUES(?,?,?,?)"));
    dp::Params parDS(pInsStat->allocParams());
    dp::Transaction tr(dp::getMainDatabase());

    td::Date dat;
    td::Time t;
    td::INT4 tip, predmet;
    td::Variant val;
    parDS << dat << tip << predmet << t;
    dp::IStatementPtr pDel(dp::getMainDatabase()->createStatement("DELETE FROM Termini"));
    if (!pDel->execute())
        return;
    size_t nRows = _pDS->getNumberOfRows();
    for (size_t i = 0; i < nRows; ++i)
    {
        auto row = _pDS->getRow(i);
        //row[0].getValue (val);
        dat = row[0];
        tip = row[3].i4Val();
        predmet = 1;
        t = row[1];
        
        if (!pInsStat->execute())
            return;
    }
    tr.commit();
    return;
}
bool ViewAttendance::onClick(gui::Button* pBtn)
{
    if (pBtn == &_btnDelete)
    {
        int iRow = _table.getFirstSelectedRow();
        if (iRow < 0)
            return true;
        _table.beginUpdate();
        _table.removeRow(iRow);
        _table.endUpdate();
     //   saveData();
       // _table.reload();
        return true;
    }
   /* if (pBtn == &_btnUpdate)
    {
        td::Variant val1, val2;
        _date.getValue(val1);
        _time.getValue(val2);
        if (doesItDexist(val1.dateVal(), val2.timeVal()))
        {
            showAlert(tr("alert"), tr("alertAttTxt"));
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
    }*/

    if (pBtn == &_btnAdd)
    {
        
       td::Variant val1, val2;
       _date.getValue(val1);
        _time.getValue(val2);
        if (doesItDexist(val1.dateVal(), val2.timeVal()))
        {
            showAlert(tr("alert"), tr("alertAttTxt"));
            return true;
        }
       
        _table.beginUpdate();
        auto& row = _table.getEmptyRow();
        populateDSRow(row);
        _table.push_back();
        _table.endUpdate();
        return true;
    }
    if(pBtn == &_btnSave){
        saveData();
      //  _table.reload();
    }

    return false;
}
bool ViewAttendance::doesItDexist(td::Date d, td::Time t)
{
    size_t nRows = _pDS->getNumberOfRows();
    for (size_t i = 0; i < nRows; ++i)
    {
        auto row = _pDS->getRow(i);
        if (row[0].dateVal() == d && row[1].timeVal() == t)
            return true;
    }
    return false;
}

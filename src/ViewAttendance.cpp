#include "ViewAttendance.h"
#include "Globals.h"
#include "SendMessage.h"
#include <iostream>
#include <math.h>
#include "ReportAttData.h"
#include <rpt/IViewer.h>
#include <dp/IDataSetDelegate.h>
#include <gui/ContextMenu.h>
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>
#include <mu/IAppSettings.h>
#include <gui/Image.h>
#include <gui/Frame.h>

ViewAttendance::ViewAttendance(td::INT4 SubjectID, ViewSubject* subject):
_LblSubjName(tr("AttSubj")),
_LblDate(tr("AttDate")),
_LblTime(tr("AttTime"))
,_LblType(tr("AttType"))
, _btnAdd(tr("add"), tr("AddTT"))
, _btnUpdate(tr("Update"), tr("UpdateTT"))
, _btnDelete(tr("Delete"), tr("DeleteTT"))
,_btnReport(tr("AttReport"), tr("AttRepTT"))
//,_btnSave(tr("Save"), tr("SaveTT"))
, _type(td::int4)
, _hlBtnsDB(6)
,_gl(6, 2)
,_SubjectID(SubjectID)
,_imgClassAtt(":complex")
{
    _subject = subject;
    
    _hlBtnsDB.appendSpacer();
    _hlBtnsDB.append(_btnDelete);
    _hlBtnsDB.append(_btnUpdate);
 //   _hlBtnsDB.append(_btnSave);
    _hlBtnsDB.append(_btnAdd);
    _hlBtnsDB.appendSpacer();
    _hlBtnsDB.append(_btnReport);
   
    
    _btnUpdate.setType(gui::Button::Type::Default);
    //_btnSave.setType(gui::Button::Type::Default);
    _btnDelete.setType(gui::Button::Type::Destructive);
    _btnAdd.setType(gui::Button::Type::Constructive);
    _btnReport.setType(gui::Button::Type::Normal);
    
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
        predmet = _SubjectID;
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
        saveData();
        if(CheckTime())
            SendMsg(2);
       // _table.reload();
        return true;
    }
   if (pBtn == &_btnUpdate)
    {
        td::Variant val1, val2;
        _date.getValue(val1);
        _time.getValue(val2);
        int iRow = _table.getFirstSelectedRow();
        if (iRow < 0)
            return true;
        _table.beginUpdate();
        auto& row = _table.getCurrentRow();
        populateDSRow(row);
        _table.updateRow(iRow);
        _table.endUpdate();
        saveData();
        if(CheckTime())
            SendMsg(3);
        return true;
    }

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
        saveData();
        _subject->getDate().clean();
        _subject->populateDateCombo(_subject->getDate());
        if(CheckTime())
            SendMsg(1);
        return true;
    }
    if(pBtn == &_btnReport){
        AttendanceReport(&_imgClassAtt);
        return true;
    }
  /*  if(pBtn == &_btnSave){
        saveData();
    }*/

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

void ViewAttendance::SendMsg(td::INT4 MsgType){
    
    std::vector<td::INT4> userIDs;
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select c.ID  from Korisnici c, Upis a, Predmet b WHERE a.ID_Smjera = b.ID_Smjera AND b.Semestar = a.Semestar AND c.Indeks = a.Indeks AND b.ID_Predmeta = ?");
    dp::Params pParams(pSelect->allocParams());
    pParams << _SubjectID;
    dp::Columns pCols = pSelect->allocBindColumns(1);
    td::INT4 id;
    pCols << "ID" << id;
    if (!pSelect->execute())
        return;
    while (pSelect->moveNext())
    {
        userIDs.push_back(id);
    }
    dp::IStatementPtr pSelect1 = dp::getMainDatabase()->createStatement("select Naziv_Predmeta FROM Predmet WHERE ID_Predmeta = ?");
    dp::Params pParams1(pSelect1->allocParams());
    pParams1 << _SubjectID;
    dp::Columns pCols1 = pSelect1->allocBindColumns(1);
    td::String predmet;
    pCols1 << "Naziv_Predmeta" << predmet;
    if (!pSelect1->execute())
        return;
    if(!pSelect1->moveNext())
        return;
    //std::cout << predmet;
    td::String naslov;
    td::String poruka;
    if(MsgType == 2){
        naslov = "Uklonjen termin nastave!";
        poruka = "Uklonjen termin nastave za predmet ";
    }
    else if(MsgType == 1){
        naslov = "Novi termin nastave!";
        poruka = "Dodan je novi termin nastave za predmet ";
    }
    else if (MsgType == 3){
        naslov = "Promjena termina nastave!";
        poruka = "Promijenjen termin nastave za predmet ";
    }
    
    poruka += predmet;

    MsgSender za_poruke;
    za_poruke.sendSystemMsgtoUsers(naslov, poruka, userIDs);
    td::Time t(true);
    td::Date d(true);
    LastMsgTime = t;
    LastMsgDate = d;
}
bool ViewAttendance::CheckTime(){
    if(LastMsgTime == 0 && LastMsgDate == 0 )
        return true;
    td::Time currTime(true);
    td::Date currDate(true);
    if(currDate == LastMsgDate && std::fabs((LastMsgTime.getMinute() - currTime .getMinute())) <= 1)
        return false;
    return true;
}
void ViewAttendance::AttendanceReport(const gui::Image* pImage){
    
    dp::IDatabase* pDB = dp::getMainDatabase();

    //#ifdef REPTEST
    DocumentData docData(_SubjectID);
    CompanyData companyData;
    ObjectData objectData;

   rpt::ReportData repData(2, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());

    rpt::ReportData::iterator it(repData.begin());
    
    td::Variant temp;
    td::Date tempD;
    td::Time tempT;
    
  /*  _date.getValue(temp);
   // tempD = temp;
    tempD = td::Date (2024, 1, 7);
    _time.getValue(temp);
    //tempT = temp;
    tempT  = 649079385;
   {
        dp::IDataSet* pDP(dp::createConnectionlessDataSet(dp::IDataSet::Size::SingleRow));
        
        it << rpt::ContainerType::CNT_DocHeader << pDP; //define field and its data
        
        dp::DSColumns cols(pDP->allocBindColumns(2));
        cols << "date" << td::int4
        << "time" << td::int4;
        
        
        pDP->execute();
        
        auto& row = pDP->getRow(0);
        row[0] = tempD;
        row[1] = tempT;
    }
    
    ++it;*/
    
    {
        dp::IDataSet* pRep(pDB->createDataSet("sselect a.Ime, a.Prezime, c.Datum as Datum, c.Vrijeme as Vrijeme FROM Korisnici a, Prisustvo b, Termini c WHERE b.ID_Studenta = a.ID AND b.ID_termina = c.ID and c.Predmet_ID = ?"));
        dp::Params pParams(pRep->allocParams());
        pParams << _SubjectID;
           dp::DSColumns colsRep(pRep->allocBindColumns(4));
        colsRep << "Ime" << td::string8 << "Prezime" << td::string8 <<"Datum" << td::date <<"Vrijeme" << td::time;
           it << rpt::ContainerType::CNT_Body << pRep;
           
               
               if(!pRep->execute()){
                   return;
               }
               size_t nRows = pRep->getNumberOfRows();
               if (nRows == 0)
               {
                   //niko nije prijavljen, smislit sta treba radit
                   //return;
               }

    }
   ++it;
    it.nextLayout();
    {
        dp::IDataSet* pRep(pDB->createDataSet("SELECT a.Ime, a.Prezime FROM Korisnici a LEFT JOIN Prisustvo b ON a.ID = b.ID_studenta LEFT JOIN Termini c ON b.ID_termina= c.ID LEFT JOIN Predmet d ON c.Predmet_ID = d.ID_Predmeta WHERE b.ID_studenta IS NULL AND a.PozicijaID = 5 AND d.ID_Predmeta =  ?"));
        dp::Params pParams(pRep->allocParams());
        pParams << _SubjectID;
           dp::DSColumns colsRep(pRep->allocBindColumns(2));
        colsRep << "Ime" << td::string8 << "Prezime" << td::string8;
           it << rpt::ContainerType::CNT_Body << pRep;
           
               
               if(!pRep->execute()){
                   return;
               }
               size_t nRows = pRep->getNumberOfRows();
               if (nRows == 0)
               {
                   //niko nije prijavljen, smislit sta treba radit
                   //return;
               }
    }
   
    td::String configName("ClassAttRep");

    rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
    if (pRepView)
    {
        pRepView->show(gui::tr("ClassAttRep"), pImage);

    }
    

    
}
/*void ViewAttendance::AttendanceReport(const gui::Image* pImage){
    dp::IDatabase* pDB = dp::getMainDatabase();

    DocumentData docData(_SubjectID);
    CompanyData companyData;
    ObjectData objectData;

    //docData[RPT_TXT_DOCUMENT_TYPE] = "Prijave na termine nastave";
    //docData[RPT_TXT_DOCUMENT_ID] = "";
    
    rpt::ReportData repData(2, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());

    rpt::ReportData::iterator it(repData.begin());
    //#endif
        //Grouper
    {
        dp::IDataSet* pDP(pDB->createDataSet("select b.Datum FROM Termini b ORDER BY Datum ASC"));

        //#ifdef REPTEST
        it << rpt::ContainerType::CNT_Grouper << pDP; //define field and its data
        //#endif
        dp::DSColumns cols(pDP->allocBindColumns(1));
        cols  << "Datum" << td::date;
        if (!pDP->execute())//sto ovdje nece ne znam...
        {
            return;
        }
        //it << rpt::ContainerType::CNT_Body << pDP;

        size_t nRows = pDP->getNumberOfRows();
        if (nRows == 0)
        {
            //nema termina u tabeli

            return;
        }
    }
    ++it;
    {
        dp::IDataSet* pDP(pDB->createDataSet("select a.Ime, a.Prezime From Korisnici a, Prisustvo c Where a.ID = c.ID_Studenta"));

        //#ifdef REPTEST
        it << rpt::ContainerType::CNT_Body << pDP; //define field and its data
        //#endif
        dp::DSColumns cols(pDP->allocBindColumns(2));
        cols  << "Ime" << td::string8 << "Prezime" << td::string8;
        if (!pDP->execute())//sto ovdje nece ne znam...
        {
            return;
        }
        //it << rpt::ContainerType::CNT_Body << pDP;

        size_t nRows = pDP->getNumberOfRows();
        if (nRows == 0)
        {
            //nema termina u tabeli

            return;
        }
    }

    //InvoiceSimple
    td::String configName("ClassAttRep");

    rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
    if (pRepView)
    {
        pRepView->show(gui::tr("ClassAttRep"), pImage);

    }

    


    
}
*/

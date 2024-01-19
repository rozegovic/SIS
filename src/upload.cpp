#pragma once
#include "upload.h"
#include "ViewIDs.h"
#include <td/Types.h>



ViewUpload::ViewUpload()
    : _lblName(tr("namee:")),

    _lblSurname(tr("surname:")),
    _lblIndeks(tr("indeksUser:")),

    _lblTable1(tr("Active:")),
    _lblTable2(tr("Committed:")),
    // _toBeEnrolled(tr("tobeenrolled")),
    _hlBtnsDB(4)
    , _btnDEnroll(tr("DEnroll"))
    , _btnReload(tr("Reload"))
    , _btnEnroll(tr("Enroll"))
    , _btnSend(tr("Send"))
    //, _btnWithdraw(tr("Withdraw"), tr("WithdrawTT"))
    , _gl(6, 6)
    , _db(dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3))
{

    //_hlBtnsDB.append(_btnDEnroll);
    _hlBtnsDB.appendSpacer();
    _hlBtnsDB.append(_btnReload);
    // _hlBtnsDB.appendSpace(20);
    // _hlBtnsDB.append(_btnWithdraw);
    _hlBtnsDB.append(_btnSend);
   _btnDEnroll.setType(gui::Button::Type::Default);
    _btnEnroll.setType(gui::Button::Type::Constructive);
    // _btnWithdraw.setType(gui::Button::Type::Destructive);


    gui::GridComposer gc(_gl);

    gc.appendRow(_lblIndeks);
   gc.appendCol(_indeks);
    gc.appendCol(_lblName);
    gc.appendCol(_name);
    gc.appendCol(_lblSurname);
    gc.appendCol(_surname);

    gc.appendRow(_lblTable1, 0);
    gc.appendRow(_table1, 0);

    gc.appendRow(_lblTable2, 0);
    gc.appendRow(_table2, 0);
    gc.appendRow(_hlBtnsDB, 0);

    gui::View::setLayout(&_gl);

   _db = dp::getMainDatabase();
    populateDataForTable1();
    populateDataForTable2();


    _indeks.setValue("111111");
    _indeks.setAsReadOnly();

    _name.setValue("Mini");
    _name.setAsReadOnly();
    _surname.setValue("Mouse");
    _surname.setAsReadOnly();


    _lblTable1.setBold();
   _lblTable2.setBold();
}

ViewUpload::~ViewUpload()
{

}


void ViewUpload::populateDataForTable1()
{
    _pDS = dp::getMainDatabase()->createDataSet("SELECT P.Naziv_Predmeta, P.Sifra_Predmeta, A.Naziv_Aktivnosti, r.Datum_Kraja, r.Vrijeme_Kraja, P.ID_Predmeta FROM Predmet P, Aktivnosti A, Predaja pr, Rokovi r where P.ID_Predmeta = A.ID_Predmeta and r.ID_Predmeta = A.ID_Predmeta and (A.Tip_Aktivnosti = 2 or A.Tip_Aktivnosti = 5) and r.Datum_Kraja > 0 and pr.Predano = 0 and r.ID_Roka = pr.ID_Roka", dp::IDataSet::Execution::EX_MULT);
    dp::DSColumns cols(_pDS->allocBindColumns(6));
    cols << "Naziv_Predmeta" << td::string8 
        << "Sifra_Predmeta" << td::string8 
        << "Naziv_Aktivnosti" << td::string8 
        << "Datum_Kraja" << td::date 
        << "Vrijeme_Kraja" << td::time
        /* << "Reg_time" << td::time */ 
        << "ID_Predmeta" << td::int4;;

    if (!_pDS->execute())
    {
        _pDS = nullptr;
        return;
    }
    //initTable1();
    _table1.init(_pDS,0);
}
void ViewUpload::populateDataForTable2() // treba modifikovati select da radi kad se preda na zadnji dan da se uporedi vrijeme 
{
    _pDS2 = dp::getMainDatabase()->createDataSet("SELECT P.Naziv_Predmeta, P.Sifra_Predmeta, A.Naziv_Aktivnosti, pr.Datum_Predaje, pr.Vrijeme_Predaje, P.ID_Predmeta, pr.Datoteka FROM Predmet P, Aktivnosti A, Rokovi r, Predaja pr where  predano = 1 and P.ID_Predmeta = A.ID_Predmeta and r.ID_Predmeta = A.ID_Predmeta and (A.Tip_Aktivnosti = 2 or A.Tip_Aktivnosti = 5) and r.ID_Roka = pr.ID_Roka and r.Datum_Kraja > pr.Datum_Predaje", dp::IDataSet::Execution::EX_MULT);
    dp::DSColumns cols(_pDS2->allocBindColumns(7));
    cols << "Naziv_Predmeta" << td::string8 << "Sifra_Predmeta" << td::string8 << "Naziv_Aktivnosti" << td::string8 << "Datum_Predaje" << td::date << "Vrijeme_Predaje" << td::time << "ID_Predmeta" << td::int4  << "Datoteka" << td::string8; // treba vidjet koji tip podatka treba za datoteku

    if (!_pDS2->execute())
    {
       _pDS2 = nullptr;
        return;
   }
    //initTable2();

    _table2.init(_pDS2,0);
}


//bool ViewExamSignUp::saveData1()
//{
//    //ovo je kada su svi updateovani
//
//    dp::IStatementPtr pInsStat(_db->createStatement("UPDATE Polozeni_Predmeti SET Prijavljen = 1 WHERE ID_Studenta = 5 AND ID_Predmeta = ?"));
//
//    dp::Params parDS(pInsStat->allocParams());
//
//    parDS << ID_Pre1;
//
//
//    dp::Transaction tr(_db);
//
//
//
//    size_t nRows = _pDS2->getNumberOfRows();
//    for (size_t i = 0; i < nRows; i++) {
//        auto row = _pDS2->getRow(i);
//
//        if (!pInsStat->execute())
//            return false;
//
//    }
//    tr.commit();
//    return true;
//}
//
//bool ViewExamSignUp::saveData2()
//{
//    //ovo je kada su svi updateovani
//
//    dp::IStatementPtr pInsStat(_db->createStatement("UPDATE Polozeni_Predmeti SET Prijavljen = 0 WHERE ID_Studenta = 5 AND ID_Predmeta = ?"));
//
//    dp::Params parDS(pInsStat->allocParams());
//
//    parDS << ID_Pre2;
//
//
//    dp::Transaction tr(_db);
//
//
//
//    size_t nRows = _pDS2->getNumberOfRows();
//    for (size_t i = 0; i < nRows; i++) {
//        auto row = _pDS2->getRow(i);
//
//        if (!pInsStat->execute())
//            return false;
//
//    }
//    tr.commit();
//    return true;
//}
//
//
//
//bool ViewExamSignUp::onChangedSelection(gui::TableEdit* pTE)
//{
//    if (pTE == &_table2)
//    {
//        int iRow = _table2.getFirstSelectedRow();
//        if (iRow < 0)
//        {
//            _name.toZero();
//            return true;
//        }
//        // td::Variant val;
//        dp::IDataSet* pDS = _table2.getDataSet();
//        auto& row = pDS->getRow(iRow);
//
//        ID_Pre2 = row[6];
//
//        return true;
//    }
//
//    if (pTE == &_table1)
//    {
//        int iRow = _table1.getFirstSelectedRow();
//        if (iRow < 0)
//        {
//            _name.toZero();
//            return true;
//        }
//        // td::Variant val;
//        dp::IDataSet* pDS = _table1.getDataSet();
//        auto& row = pDS->getRow(iRow);
//
//        ID_Pre1 = row[6];
//
//        return true;
//    }
//    return false;
//}
//
//
//
//
bool ViewUpload::onClick(gui::Button* pBtn)
{
    if (pBtn == &_btnReload)
    {
        _table1.reload();
        _table1.selectRow(0, true);
        _table2.reload();
        _table2.selectRow(0, true);
        return true;
    }
    if (pBtn == &_btnSend) {

    }

//    if (pBtn == &_btnEnroll)
//    {
//        saveData1();
//        _table1.reload();
//        _table1.selectRow(0, true);
//        _table2.reload();
//        _table2.selectRow(0, true);
//        return true;
//
//    }
//
//    if (pBtn == &_btnDEnroll)
//    {
//        saveData2();
//        _table1.reload();
//        _table1.selectRow(0, true);
//        _table2.reload();
//        _table2.selectRow(0, true);
//        return true;
//
//    }

    return false;

}
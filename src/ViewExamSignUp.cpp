#pragma once
#include "ViewExamSignUp.h"
#include "ViewIDs.h"
#include <td/Types.h>



ViewExamSignUp::ViewExamSignUp()
    : _lblName(tr("namee:")),

    _lblSurname(tr("surname:")),
    _lblIndeks(tr("indeksUser:")),

    _lblTable1(tr("nenrolled:")),
    _lblTable2(tr("enrolled:")),
    // _toBeEnrolled(tr("tobeenrolled")),
    _hlBtnsDB(4)
    , _btnDEnroll(tr("DEnroll"))
    , _btnReload(tr("Reload"))
    , _btnEnroll(tr("Enroll"))
    //, _btnWithdraw(tr("Withdraw"), tr("WithdrawTT"))
    , _gl(6, 6)
    , _db(dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3))
{

    _hlBtnsDB.append(_btnDEnroll);
    _hlBtnsDB.appendSpacer();
    _hlBtnsDB.append(_btnReload);
    // _hlBtnsDB.appendSpace(20);
    // _hlBtnsDB.append(_btnWithdraw);
    _hlBtnsDB.append(_btnEnroll);
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

    ID_studenta = Globals::_currentUserID;
    SetCurrentStudentName();
    SetCurrentStudentSurname();
    SetCurrentStudentIndeks();

    _indeks.setAsReadOnly();


    _name.setAsReadOnly();

    _surname.setAsReadOnly();


    _lblTable1.setBold();
    _lblTable2.setBold();

}

ViewExamSignUp::~ViewExamSignUp()
{

}

void ViewExamSignUp::initTable1()
{
    gui::Columns visCols(_table1.allocBindColumns(8));
    visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, tr("Naz_pred")) << gui::Header(1, tr("sifra_pred")) << gui::Header(2, tr("naz_akt")) << gui::Header(3, tr("poc")) << gui::Header(4, tr("v_akt")) << gui::Header(5, tr("vr_prijave")) << gui::Header(6, tr("id_pre")) << gui::Header(7, tr("id_roka"));
    _table1.init(_pDS); //ne radi kada se stavi {0,1,2,3,4,5} kao drugi parametar

}
void ViewExamSignUp::initTable2()
{
    gui::Columns visCols(_table2.allocBindColumns(8));
    visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, tr("Naz_pred")) << gui::Header(1, tr("sifra_pred")) << gui::Header(2, tr("naz_akt")) << gui::Header(3, tr("poc")) << gui::Header(4, tr("v_akt")) << gui::Header(5, tr("vr_prijave")) << gui::Header(6, tr("id_pre")) << gui::Header(7, tr("id_roka"));
    _table2.init(_pDS2);//ne radi kada se stavi {0,1,2,3,4,5} kao drugi parametar

}


void ViewExamSignUp::populateDataForTable1()
{
    _pDS = dp::getMainDatabase()->createDataSet("SELECT P.Naziv_Predmeta as Course_name, P.Sifra_Predmeta as Course_code, Ak.Naziv_Aktivnosti as Name_of_activity, a.Datum_Pocetka as Start_date, a.Vrijeme_Pocetka as Start_time, a.Vrijeme_Prijave as Reg_time, P.ID_Predmeta as id_pre, a.ID_Roka as id_roka FROM Rokovi a INNER JOIN UpisPredmeta b ON a.ID_Predmeta = b.ID_Predmeta INNER JOIN Aktivnosti Ak ON Ak.ID_Aktivnosti = a.ID_Aktivnosti INNER JOIN Predmet P ON P.ID_Predmeta = a.ID_Predmeta LEFT JOIN Prijavljeni_ispiti c ON a.ID_Roka = c.ID_Roka AND b.ID_Studenta = c.ID_Studenta WHERE c.ID_Roka IS NULL AND b.ID_Studenta = ? AND Ak.Tip_Aktivnosti = 1;", dp::IDataSet::Execution::EX_MULT);
    dp::Params parDS(_pDS->allocParams());
    //td::INT4 id = Globals::_currentUserID;
    parDS << Globals::_currentUserID; // id;
    dp::DSColumns cols(_pDS->allocBindColumns(8));
    cols << "Course_name" << td::string8 << "Course_code" << td::string8 << "Name_of_activity" << td::string8 << "Start_date" << td::date << "Start_time" << td::time << "Reg_time" << td::time << "id_pre" << td::int4 << "id_roka" << td::int4;

    if (!_pDS->execute())
    {
        _pDS = nullptr;
        return;
    }
    initTable1();
}
void ViewExamSignUp::populateDataForTable2()
{
    _pDS2 = dp::getMainDatabase()->createDataSet("SELECT P.Naziv_Predmeta as Course_name, P.Sifra_Predmeta as Course_code, A.Naziv_Aktivnosti as Name_of_activity, R.Datum_Pocetka as Start_date, R.Vrijeme_Pocetka as Start_time, R.Vrijeme_Prijave as Reg_time, P.ID_Predmeta as id_pre, R.ID_Roka as id_roka FROM Predmet P INNER JOIN Aktivnosti A ON P.ID_Predmeta = A.ID_Predmeta INNER JOIN Rokovi R ON A.ID_Aktivnosti = R.ID_Aktivnosti INNER JOIN UpisPredmeta U ON P.ID_Predmeta = U.ID_Predmeta INNER JOIN Prijavljeni_ispiti PP ON P.ID_Predmeta = PP.ID_Predmeta AND A.ID_Predmeta = PP.ID_Predmeta AND R.ID_Roka = PP.ID_Roka AND U.ID_Studenta = PP.ID_Studenta AND U.ID_Predmeta = PP.ID_Predmeta AND PP.ID_Studenta = ? AND A.Tip_Aktivnosti = 1;", dp::IDataSet::Execution::EX_MULT);
    dp::Params parDS2(_pDS2->allocParams());
    parDS2 << Globals::_currentUserID;
    dp::DSColumns cols(_pDS2->allocBindColumns(8));
    cols << "Course_name" << td::string8 << "Course_code" << td::string8 << "Name_of_activity" << td::string8 << "Start_date" << td::date << "Start_time" << td::time << "Reg_time" << td::time << "id_pre" << td::int4 << "id_roka" << td::int4;

    if (!_pDS2->execute())
    {
        _pDS2 = nullptr;
        return;
    }
    initTable2();
}


bool ViewExamSignUp::saveData1()
{


    dp::IStatementPtr pInsStat(_db->createStatement("INSERT INTO Prijavljeni_ispiti (ID_Predmeta, ID_Studenta, ID_Roka) VALUES (?,?,?)"));

    dp::Params parDS(pInsStat->allocParams());


    int iRow = _table1.getFirstSelectedRow();


    dp::IDataSet* pDS = _table1.getDataSet();
    auto& row = pDS->getRow(iRow);

    td::INT4 ID_Pre1 = row[6].i4Val();
    td::INT4 ID_Roka = row[7].i4Val();
    parDS << ID_Pre1 << ID_studenta << ID_Roka;
    dp::Transaction tr(_db);




    if (!pInsStat->execute())
        return false;


    tr.commit();
    return true;
}

bool ViewExamSignUp::saveData2()
{

    dp::IStatementPtr pInsStat(_db->createStatement("DELETE FROM Prijavljeni_ispiti WHERE ID_Studenta=? AND ID_Predmeta=?"));

    dp::Params parDS(pInsStat->allocParams());


    int iRow = _table2.getFirstSelectedRow();

    dp::IDataSet* pDS = _table2.getDataSet();
    auto& row = pDS->getRow(iRow);

    td::INT4 ID_Pre2 = row[6].i4Val();
    parDS << ID_studenta << ID_Pre2;


    dp::Transaction tr(_db);


    if (!pInsStat->execute())
        return false;


    tr.commit();
    return true;
}



bool ViewExamSignUp::onClick(gui::Button* pBtn)
{
    if (pBtn == &_btnReload)
    {
        _table1.reload();
        _table1.selectRow(0, true);
        _table2.reload();
        _table2.selectRow(0, true);
        return true;
    }
    if (pBtn == &_btnEnroll)
    {
        saveData1();
        _table1.reload();
        _table1.selectRow(0, true);
        _table2.reload();
        _table2.selectRow(0, true);
        return true;

    }

    if (pBtn == &_btnDEnroll)
    {
        saveData2();
        _table1.reload();
        _table1.selectRow(0, true);
        _table2.reload();
        _table2.selectRow(0, true);
        return true;

    }

    return false;

}


void ViewExamSignUp::SetCurrentStudentName()
{
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT Ime FROM Korisnici WHERE ID= ?");
    dp::Params parDS(pSelect->allocParams());
    //d::INT4 IDPredmeta = Globals::_IDSubjectSelection;
    parDS << ID_studenta;
    dp::Columns pCols = pSelect->allocBindColumns(1);
    td::String Ime;
    pCols << "Ime" << Ime;
    if (!pSelect->execute()) {
        Ime = "Nema imena";
    }
    while (pSelect->moveNext())
    {
        td::Variant val;
        val = Ime;
        _name.setValue(val);

    }
}

void ViewExamSignUp::SetCurrentStudentSurname()
{
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT Prezime FROM Korisnici WHERE ID= ?");
    dp::Params parDS(pSelect->allocParams());
    //d::INT4 IDPredmeta = Globals::_IDSubjectSelection;
    parDS << ID_studenta;
    dp::Columns pCols = pSelect->allocBindColumns(1);
    td::String Prezime;
    pCols << "Prezime" << Prezime;
    if (!pSelect->execute()) {
        Prezime = "Nema prezimena";
    }
    while (pSelect->moveNext())
    {
        td::Variant val;
        val = Prezime;
        _surname.setValue(val);

    }
}

void ViewExamSignUp::SetCurrentStudentIndeks()
{
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT Indeks FROM Korisnici WHERE ID= ?");
    dp::Params parDS(pSelect->allocParams());
    //d::INT4 IDPredmeta = Globals::_IDSubjectSelection;
    parDS << ID_studenta;
    dp::Columns pCols = pSelect->allocBindColumns(1);
    td::String Indeks;
    pCols << "Indeks" << Indeks;
    if (!pSelect->execute()) {
        Indeks = "Nema indeksa";
    }
    while (pSelect->moveNext())
    {
        td::Variant val;
        val = Indeks;
        _indeks.setValue(val);

    }
}
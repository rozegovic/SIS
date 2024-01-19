#pragma once
#include "upload.h"
#include "ViewIDs.h"
#include <td/Types.h>



ViewUpload::ViewUpload()
    : _lblName(tr("namee:"))

    , _lblSurname(tr("surname:"))
    , _lblIndeks(tr("indeksUser:"))

    , _lblTable1(tr("Active:"))
    , _lblTable2(tr("Committed:"))
    , _idPredaja(td::int4)
    , _lblFile(tr("Docs:"))
    , _titleFile("")
    // _toBeEnrolled(tr("tobeenrolled")),
    , _hlBtnsDB(4)
    , _btnDEnroll(tr("DEnroll"))
    , _btnReload(tr("Reload"))
    , _btnEnroll(tr("Enroll"))
    , _btnSend(tr("Send"))
    , _btnAddFile(tr("AddFile"), tr("AddFileTT"))
    //, _btnWithdraw(tr("Withdraw"), tr("WithdrawTT"))
    , _gl(7, 6)
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

    gc.appendRow(_lblFile);
    gc.appendCol(_titleFile, 4);
    gc.appendCol(_btnAddFile, td::HAlignment::Right);

    gc.appendRow(_lblTable1, 0);
    gc.appendRow(_table1, 0);

    gc.appendRow(_lblTable2, 0);
    gc.appendRow(_table2, 0);
    gc.appendRow(_hlBtnsDB, 0);

    gui::View::setLayout(&_gl);

    _db = dp::getMainDatabase();
    populateDataForTable1();
    populateDataForTable2();

    SetCurrentData();

    _indeks.setAsReadOnly();
    _name.setAsReadOnly();
    _surname.setAsReadOnly();


    _lblTable1.setBold();
    _lblTable2.setBold();
}

ViewUpload::~ViewUpload()
{

}

void ViewUpload::SetCurrentData() {
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT Ime, Prezime, Indeks FROM Korisnici WHERE ID = ?");
    dp::Params parDS(pSelect->allocParams());
    parDS << Globals::_currentUserID;
    dp::Columns pCols = pSelect->allocBindColumns(3);
    td::String Ime, Prezime, Indeks;
    pCols << "Ime" << Ime << "Prezime" << Prezime << "Indeks" << Indeks;
    if (!pSelect->execute()) {
        Ime = "Greska";
        Prezime = "Greska";
        Indeks = "Greska";
    }
    while (pSelect->moveNext())
    {
        td::Variant val, val1, val2;
        val = Ime;
        val1 = Prezime;
        val2 = Indeks;
        _name.setValue(val);
        _surname.setValue(val1);
        _indeks.setValue(val2);
    }
}





void ViewUpload::populateDataForTable1()
{

    _pDS = dp::getMainDatabase()->createDataSet("SELECT P.Naziv_Predmeta, P.Sifra_Predmeta, A.Naziv_Aktivnosti, op.Datum_Predaje, op.Vrijeme_Predaje, P.ID_Predmeta FROM Predmet P, Aktivnosti A, Predaja pr, OpenPredaja op where P.ID_Predmeta = A.ID_Predmeta and (A.Tip_Aktivnosti = 2 or A.Tip_Aktivnosti = 5) and op.ID = pr.ID_OpenPredaja and pr.Predano = 0 and pr.ID_Aktivnosti = A.ID_Aktivnosti", dp::IDataSet::Execution::EX_MULT);
    dp::DSColumns cols(_pDS->allocBindColumns(6));
    cols << "Naziv_Predmeta" << td::string8 
        << "Sifra_Predmeta" << td::string8 
        << "Naziv_Aktivnosti" << td::string8 
        << "Datum_Predaje" << td::date 
        << "Vrijeme_Predaje" << td::time
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

 // treba vidjet koji tip podatka treba za datoteku

    _pDS2 = dp::getMainDatabase()->createDataSet("SELECT P.Naziv_Predmeta, P.Sifra_Predmeta, A.Naziv_Aktivnosti, op.Datum_Predaje, op.Vrijeme_Predaje, P.ID_Predmeta, pr.NazivFajla, pr.Datoteka FROM Predmet P, Aktivnosti A, Predaja pr, OpenPredaja op where P.ID_Predmeta = A.ID_Predmeta and (A.Tip_Aktivnosti = 2 or A.Tip_Aktivnosti = 5) and op.ID = pr.ID_OpenPredaja and pr.Predano = 1 and pr.ID_Aktivnosti = A.ID_Aktivnosti", dp::IDataSet::Execution::EX_MULT);
    dp::DSColumns cols(_pDS2->allocBindColumns(8));
    cols << "Naziv_Predmeta" << td::string8 << "Sifra_Predmeta" << td::string8 << "Naziv_Aktivnosti" << td::string8 << "Datum_Predaje" << td::date << "Vrijeme_Predaje" << td::time << "ID_Predmeta" << td::int4 << "NazivFajla" << td::string8 << "Datoteka" << td::string8; // treba vidjet koji tip podatka treba za datoteku

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
        int iRow = _table1.getFirstSelectedRow();
        auto& rowToBeDel = _table1.getCurrentRow();
        if (iRow < 0)
            return true;
        _table2.beginUpdate();
        auto& row = _table2.getEmptyRow();
        row = rowToBeDel;
        _table2.push_back();
        _table2.endUpdate();
        _table1.removeRow(iRow);
        return true;
    }

    if (pBtn == &_btnAddFile) {
        showOpenFileDialog();
        return true;
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

void ViewUpload::showOpenFileDialog()
{
    //create OpenFile dialog and open it
    auto pFD = new gui::OpenFileDialog(this, tr("OpenF"), { {tr("TxtDocs"),"*.txt"}, {tr("PDFDocs"),"*.pdf"}, {tr("JPGSlike"),"*.jpg"}, {tr("PNGSlike"),"*.png"} });
#ifdef USE_CALLBACKS
    pFD->openModal(&_callBackOpenFileDlg);
#else
    //pFD->openModalWithID(WndID::FileOpenDlg, this);
    pFD->openModal([this](gui::FileDialog* pFD)
        {
            auto status = pFD->getStatus();
            if (status == gui::FileDialog::Status::OK)
            {
                td::String strFileFullPath = pFD->getFileName();
                _attachedFile = pFD->getFileName();
                td::String strContent;

                if (fo::loadFileContent(strFileFullPath, strContent))
                {
                    //gui::TextEdit* pTE = (*this).getTextEdit();
                    //pTE->setText(strContent);

                    dp::IDatabase* pDB = dp::getMainDatabase();
                    //dp::getMainDatabase() if I were connected to DB before

                    dp::IStatementPtr pStatIns = pDB->createStatement("UPDATE Predaja SET Datoteka = ? WHERE ID_Predaje = ?");
                    dp::Params paramsInsert(pStatIns->allocParams());

                    fo::fs::path filePath(strFileFullPath.c_str());
                    //td::String strFileName = filePath.filename().string();//daj mi naziv fajla
                    td::String fileExtension = filePath.filename().extension().string(); //daj mi tip fajla

                    //tip BLOBa
                    td::BLOB::Type typeFile = td::BLOB::Type::TYPE_BINARY_UNKNOWN;
                    if (fileExtension.compareConstStr(".txt"))
                        typeFile = td::BLOB::Type::TYPE_TXT;
                    else if (fileExtension.compareConstStr(".pdf"))
                        typeFile = td::BLOB::Type::TYPE_PDF;
                    else if (fileExtension.compareConstStr(".jpg"))
                        typeFile = td::BLOB::Type::TYPE_JPG;
                    else if (fileExtension.compareConstStr(".png"))
                        typeFile = td::BLOB::Type::TYPE_PNG;


                    td::BLOB dataIn(td::BLOB::SRC_FILE, 16384U, typeFile);

                    int iRow = _table1.getFirstSelectedRow();
                    if (iRow < 0)
                        return true;

                    td::INT4 itemid = getIDfromTable(iRow);

                    static td::INT4 ID_predaje = itemid;
                    paramsInsert << dataIn << ID_predaje;
                    //Neophodno, sa ove lokacije (strFileFullPath) se uzima BLOB
                    if (!dataIn.setInFileName(strFileFullPath))
                    {
                        gui::Alert::show(tr("Error"), tr("Did you delete the selected file?"));
                        return true;
                    }

                    dp::Transaction transaction(pDB);

                    //bool delOK = pStatDel->execute();
                    bool insOK = pStatIns->execute();

                    bool commitOK = transaction.commit();

                    _titleFile.setTitle(filePath.filename());
                    _titleFile.setBold();
                }
            }
        });
#endif
}

td::INT4 ViewUpload::getIDfromTable(int rowID)
{
    dp::IDataSet* pDS = _table1.getDataSet();
    auto& row = pDS->getRow(rowID);
    return row[0].i4Val();
}
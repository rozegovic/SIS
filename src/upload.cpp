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
    , _btnUnSend(tr("UnSend"))
    , _btnAddFile(tr("SendFile"), tr("AddFileTT"))
    //, _btnWithdraw(tr("Withdraw"), tr("WithdrawTT"))
    , _gl(7, 6)
    , _db(dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3))
{

    //_hlBtnsDB.append(_btnDEnroll);
    _hlBtnsDB.appendSpacer();
    _hlBtnsDB.append(_btnReload);
    // _hlBtnsDB.appendSpace(20);
    // _hlBtnsDB.append(_btnWithdraw);
    //_hlBtnsDB.append(_btnSend);
    _hlBtnsDB.append(_btnUnSend);
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

    //_pDS = dp::getMainDatabase()->createDataSet("SELECT pr.ID_Predaje, P.Naziv_Predmeta, P.Sifra_Predmeta, A.Naziv_Aktivnosti, op.Datum_Predaje, op.Vrijeme_Predaje, P.ID_Predmeta, pr.NazivFajla FROM Predmet P, Aktivnosti A, Predaja pr, OpenPredaja op where P.ID_Predmeta = A.ID_Predmeta and (A.Tip_Aktivnosti = 2 or A.Tip_Aktivnosti = 5) and op.ID = pr.ID_OpenPredaja and pr.Predano = 0 and pr.ID_Aktivnosti = A.ID_Aktivnosti", dp::IDataSet::Execution::EX_MULT);
     
    
    //_pDS = dp::getMainDatabase()->createDataSet("SELECT pr.ID_Predaje, P.Naziv_Predmeta, P.Sifra_Predmeta, A.Naziv_Aktivnosti, op.Datum_Predaje, op.Vrijeme_Predaje, P.ID_Predmeta FROM Predmet P, Aktivnosti A, Predaja pr, OpenPredaja op, UpisPredmeta up where up.ID_Studenta = pr.ID_Studenta and pr.ID_Studenta = ? and P.ID_Predmeta = A.ID_Predmeta and up.ID_Predmeta = P.ID_Predmeta and (A.Tip_Aktivnosti = 2 or A.Tip_Aktivnosti = 5) and op.ID = pr.ID_OpenPredaja and pr.Predano = 0 and pr.ID_Aktivnosti = A.ID_Aktivnosti", dp::IDataSet::Execution::EX_MULT);
    // dodati ucitavanje id iz openpredaja na mjesto 0 u jednu i drugu tabelu
    _pDS = dp::getMainDatabase()->createDataSet("SELECT op.ID, P.Naziv_Predmeta as nameP, P.Sifra_Predmeta, A.Naziv_Aktivnosti, op.Datum_Predaje as dateR, op.Vrijeme_Predaje as timeR, P.ID_Predmeta FROM Predmet P, Aktivnosti A, OpenPredaja op, UpisPredmeta up where up.ID_Studenta = ? and P.ID_Predmeta = A.ID_Predmeta and up.ID_Predmeta = P.ID_Predmeta and (A.Tip_Aktivnosti = 2 or A.Tip_Aktivnosti = 5) and op.ID_Aktivnosti = A.ID_Aktivnosti and NOT EXISTS( SELECT 1 FROM Predaja WHERE Predaja.ID_OpenPredaja = op.ID); ", dp::IDataSet::Execution::EX_MULT);//Dodati NOT EXIST za provjeru da li postoji u Predaji vec neki red da se ne dodaje u prvu tabelu.
    dp::Params parDS(_pDS->allocParams());
   parDS << Globals::_currentUserID; // id;
    

    dp::DSColumns cols(_pDS->allocBindColumns(7));
    cols << "ID" << td::int4
        << "nameP" << td::string8
        << "Sifra_Predmeta" << td::string8
        << "Naziv_Aktivnosti" << td::string8
        << "dateR" << td::date  //krajnji rok
        << "timeR" << td::time //krajnji rok
        << "ID_Predmeta" << td::int4;
        //<< "Datoteka" << td::string8; // treba vidjet koji tip podatka treba za datoteku

    if (!_pDS->execute())
    {
        _pDS = nullptr;
        return;
    }
    //initTable1();
   _table1.init(_pDS,{1,3,4,5});
  /*  td::Date d;
    td::Time t;
    td::Date dnow(true);
    td::Time tnow(true);
    dp::IDataSet* pDS = _table1.getDataSet();
    auto nRows = pDS->getNumberOfRows();
    for (size_t i = 0; i < nRows; ++i) {
        auto& row = pDS->getRow(i);
        d = row[3];
        t = row[4];
        if (dnow < d)
            _table1.removeRow(i);
        else if (dnow == d && tnow < t)
            _table1.removeRow(i);
    }*/
}
void ViewUpload::populateDataForTable2() // treba modifikovati select da radi kad se preda na zadnji dan da se uporedi vrijeme 
{

 // treba vidjet koji tip podatka treba za datoteku

    _pDS2 = dp::getMainDatabase()->createDataSet("SELECT op.ID, pr.ID_Predaje, P.Naziv_Predmeta as nameP, P.Sifra_Predmeta, A.Naziv_Aktivnosti, pr.Datum_Predaje as dateP, pr.Vrijeme_Predaje as timeP, P.ID_Predmeta, pr.NazivFajla as DatName FROM Predmet P, Aktivnosti A, Predaja pr, OpenPredaja op, UpisPredmeta up where up.ID_Studenta = pr.ID_Studenta and pr.ID_Studenta = ? and P.ID_Predmeta = A.ID_Predmeta and up.ID_Predmeta = P.ID_Predmeta and op.ID_Aktivnosti = A.ID_Aktivnosti and op.ID = pr.ID_OpenPredaja", dp::IDataSet::Execution::EX_MULT);
    dp::Params parDS2(_pDS2->allocParams());
    parDS2 << Globals::_currentUserID; // id;
    
    dp::DSColumns cols(_pDS2->allocBindColumns(9));
    cols << "ID" << td::int4
        << "ID_Predaje" << td::int4
        << "nameP" << td::string8
        << "Sifra_Predmeta" << td::string8
        << "Naziv_Aktivnosti" << td::string8
        << "dateP" << td::date
        << "timeP" << td::time
        << "ID_Predmeta" << td::int4
        << "DatName" << td::string8;
        //<< "Datoteka" << td::string8; // treba vidjet koji tip podatka treba za datoteku

    if (!_pDS2->execute())
    {
       _pDS2 = nullptr;
        return;
   }
    //initTable2();

    _table2.init(_pDS2, {2,4,5,6,8});
}



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
    //if (pBtn == &_btnSend) {
    //    int iRow = _table1.getFirstSelectedRow();
    //    auto& rowToBeDel = _table1.getCurrentRow();
    //    if (iRow < 0)
    //        return true;
    //    _table2.beginUpdate();
    //    auto& row = _table2.getEmptyRow();
    //    // row[0]=finmaxid ??????
    //    row = rowToBeDel;
    //    _table2.push_back();
    //    _table2.endUpdate();
    //    _table1.removeRow(iRow);
    //    int id = row[0].i4Val();
    //    
    //    if (!PredajaPredano(id))
    //        return false;

    //    return true;
    //}
    if (pBtn == &_btnUnSend) {
        //int iRow = _table2.getFirstSelectedRow();
        //auto& rowToBeDel = _table2.getCurrentRow();
        //if (iRow < 0)
        //    return true;
        //_table1.beginUpdate();
        //auto& row = _table1.getEmptyRow();
        //row = rowToBeDel;
        //_table1.push_back();
        //_table1.endUpdate();
        //_table2.removeRow(iRow);
        //int id = row[0].i4Val();
        //
        //if (!PredajaNePredano(id))
        //    return false;

        UnSend();
        return true;
    }

    if (pBtn == &_btnAddFile) {
        showOpenFileDialog();
        _table1.reload();
        _table1.selectRow(0, true);
        _table2.reload();
        _table2.selectRow(0, true);
        return true;
    }

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

                    dp::IStatementPtr pStatIns = pDB->createStatement("insert into Predaja values(?,?,?,?,?,?,?)");
                    dp::Params paramsInsert(pStatIns->allocParams());

                    fo::fs::path filePath(strFileFullPath.c_str());
                    td::String strFileName = filePath.filename().string();//daj mi naziv fajla
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
                    dp::IDataSet* pDS = _table1.getDataSet();
                    auto& row = pDS->getRow(iRow);
                    td::INT4 idp = findMaxID();
                   // row[7] = strFileName;
                    td::INT4 op = row[0].i4Val();
                    td::Date d(true);
                    td::Time t(true);
                   // static td::INT4 ID_predaje = itemid;
                    //paramsInsert <<idp<<vrijedsnot row[0] << globals currentUserID  << dp::toNCh(strFileName, 100) << dataIn << date(now) << time (now);
                    paramsInsert << idp << op << Globals::_currentUserID << dp::toNCh(strFileName, 100) << dataIn << d << t;
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

                    _table1.reload();
                    _table1.selectRow(0, true);
                    _table2.reload();
                    _table2.selectRow(0, true);
                }
            }
        });
#endif
}

td::INT4 ViewUpload::getIDfromTable1(int rowID)
{
    dp::IDataSet* pDS = _table1.getDataSet();
    auto& row = pDS->getRow(rowID);
    return row[0].i4Val();
}

td::INT4 ViewUpload::getIDfromTable2(int rowID)
{
    dp::IDataSet* pDS = _table2.getDataSet();
    auto& row = pDS->getRow(rowID);
    return row[0].i4Val();
}

bool ViewUpload::PredajaPredano(int rowID) {
    dp::IStatementPtr pUpdateStat(dp::getMainDatabase()->createStatement("UPDATE Predaja SET Predano = 1 WHERE ID_Predaje = ?;"));
    dp::Params parDS(pUpdateStat->allocParams());
    parDS << rowID;
    //dp::Transaction tr(dp::getMainDatabase());
    if (!pUpdateStat->execute())
    {
        return false;
    }
    //if (!tr.commit())
        //tr.rollBack();
}
bool ViewUpload::PredajaNePredano(int rowID) {
    dp::IStatementPtr pUpdateStat(dp::getMainDatabase()->createStatement("UPDATE Predaja SET Predano = 0 WHERE ID_Predaje = ?;"));
    dp::Params parDS(pUpdateStat->allocParams());
    parDS << rowID;
    if (!pUpdateStat->execute())
    {
        return false;
    }
    return true;
}

td::INT4 ViewUpload::findMaxID()
{
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select ifnull(max(ID_Predaje), 0) as maxid from Predaja");
    dp::Columns pColumns = pSelect->allocBindColumns(1);
    td::INT4 maxID;
    pColumns << "maxid" << maxID;
    if (!pSelect->execute())
        return false;
    if (!pSelect->moveNext())
        return false;

    return ++maxID;
}

bool ViewUpload::UnSend() {
    dp::IStatementPtr pDelete = dp::getMainDatabase()->createStatement("DELETE FROM Predaja WHERE ID_OpenPredaja = ?;");
    dp::Params parDS2(pDelete->allocParams());
    int iRow = _table2.getFirstSelectedRow();
    auto& row = _pDS2->getRow(iRow);
    td::INT4 idpr = row[0].i4Val();
    parDS2 << idpr; // id;
    if (!pDelete->execute())
        return false;

    _table1.reload();
    _table1.selectRow(0, true);
    _table2.reload();
    _table2.selectRow(0, true);

    return true;
}
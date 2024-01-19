#include "ViewTasks.h"
#include "SendMessage.h"
#include <gui/FileDialog.h>
#include <gui/Alert.h>
#include <fo/FileOperations.h>
#include <td/BLOB.h>
#include <gui/Window.h>
#include "ViewIDs.h"

//#include "Globals.h"


ViewTasks::ViewTasks(td::INT4 SubjectID) :
    _db(dp::getMainDatabase()),
    _LblActName(tr("Activity")),
    _LblDateBegin(tr("ActivityDateP")),
    _LblTimeBegin(tr("ActivityTimeP")),
    _lblType(tr("ActivityNameForDateTime")),
    _type(td::int4),
    _lblCName(tr("Course:")),
    _btnAdd(tr("add"), tr("AddTT"))
    //, _btnUpdate(tr("Update"), tr("UpdateTT"))
    , _btnDelete(tr("Delete"), tr("DeleteTT"))
    , _btnDelete2(tr("Delete2"), tr("Delete2TT"))
    , _btnSave(tr("Save"), tr("SaveTT"))
    , _btnAddFile(tr("AddFile"), tr("AddFileTT"))
    , _hlBtnsDB(6)
    , _hl(3)
    , _gl(5, 4)
    , _SubjectID(SubjectID)
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
    _cName.setAsReadOnly();

    gui::GridComposer gc(_gl);


    /*gc.appendRow(_LblActName);
      gc.appendCol(_Activity);*/
    gc.appendRow(_lblCName);
    gc.appendCol(_cName);
    gc.appendCol(_lblType);
    gc.appendCol(_type);

    gc.appendRow(_LblDateBegin);
    gc.appendCol(_dateB);

    gc.appendCol(_LblTimeBegin);
    gc.appendCol(_timeB);

    _hl.append(_btnAddFile);
    _hl.appendSpacer();
    _hl.append(_btnDelete2);
    gc.appendRow(_hl, 0);

    gc.appendRow(_table, 0);
    gc.appendRow(_hlBtnsDB, 0);

    gui::View::setLayout(&_gl);
    populateData();
    loadComboBox("select ID_Aktivnosti as ID, Naziv_Aktivnosti as Naziv From Aktivnosti where Tip_Aktivnosti IN(2, 5) and ID_Predmeta=?", _type);

    /* if (_pDS->getNumberOfRows())
     {
         _table.selectRow(0, true);
     }*/
}
bool ViewTasks::loadComboBox(td::String select, gui::DBComboBox& combo)
{
    dp::IStatementPtr pSelect = _db->createStatement(select.c_str());
    dp::Params parDS(pSelect->allocParams());
    parDS << _SubjectID;
    dp::Columns pCols = pSelect->allocBindColumns(2);
    td::String kod;
    td::INT4 id;
    pCols << "ID" << id << "Naziv" << kod;
    if (!pSelect->execute())
        return false;

    while (pSelect->moveNext())
    {
        combo.addItem(kod, id);
    }
    combo.selectIndex(0);
    return true;
}
void ViewTasks::populateData()
{
    auto pDB = dp::getMainDatabase();
    _pDS = pDB->createDataSet("select a.ID, a.Datum_Predaje, a.Vrijeme_Predaje, a.ID_Aktivnosti, b.Naziv_Aktivnosti  from OpenPredaja a, Aktivnosti b where a.ID_Aktivnosti=b.ID_Aktivnosti and b.Tip_Aktivnosti IN(2, 5) and b.ID_Predmeta=?", dp::IDataSet::Execution::EX_MULT);

    dp::Params parDS(_pDS->allocParams());
    parDS << _SubjectID;
    td::String Predmet;

    dp::DSColumns cols(_pDS->allocBindColumns(5));
    cols << "ID" << td::int4 << "Datum_Predaje" << td::date << "Vrijeme_Predaje" << td::time << "ID_Aktivnosti" << td::int4 << "Naziv_Aktivnosti" << td::string8;

    if (!_pDS->execute())
    {
        _pDS = nullptr;
        return;
    }
    _table.init(_pDS, { 4,1,2 });
}
void ViewTasks::SetCurrentSubject() {
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
        _cName.setValue(val);

    }

}
void ViewTasks::SetActivityName(td::Variant& val, td::INT4 br) {
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT Naziv_Aktivnosti FROM Aktivnosti WHERE ID_Aktivnosti=?");
    dp::Params parDS(pSelect->allocParams());
    //d::INT4 IDPredmeta = Globals::_IDSubjectSelection;
    parDS << br;
    dp::Columns pCols = pSelect->allocBindColumns(1);
    td::String Aktivnost;
    pCols << "Naziv_Aktivnosti" << Aktivnost;
    if (!pSelect->execute()) {
        Aktivnost = "Haos";
    }
    if (!pSelect->moveNext())
        return;
    val = Aktivnost;
}

bool ViewTasks::onChangedSelection(gui::TableEdit* pTE) {
    if (pTE == &_table) {
        int iRow = _table.getFirstSelectedRow();
        if (iRow < 0) {
            return true;
        }
        td::Variant val;
        dp::IDataSet* pDS = _table.getDataSet();
        auto& row = pDS->getRow(iRow);

        /*_Aktivnost.setValue(val);*/

        val = row[1];
        _dateB.setValue(val);

        val = row[2];
        _timeB.setValue(val);

        val = row[3];
        _type.setValue(val);

        return true;
    }
    return false;
}

void ViewTasks::populateDSRow(dp::IDataSet::Row& row, td::INT4 i)
{
    td::Variant val;
    _dateB.getValue(val);
    row[1].setValue(val);

    _timeB.getValue(val);
    row[2].setValue(val);

    td::Variant x = i;
    row[0].setValue(x);

    /*val = _ActivityID;*/
    /*row[7].setValue(val);*/

    td::Variant var;
    _type.getValue(val);
    row[3].setValue(val);
    td::INT4 a = val.i4Val();
    SetActivityName(var, a);
    row[4].setValue(var);

}
td::INT4 ViewTasks::findMaxID()
{
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select ifnull(max(ID), 0) as maxid from OpenPredaja");
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


bool  ViewTasks::saveData()
{
    dp::Transaction tran(_db);
    if (!eraseTasks())
        return false;

    if (!insertTasks())
        return false;

    if (tran.commit())
    {
        _itemsToDelete.clear();
        _itemsToInsert.clear();
        _itemsToUpdate.clear();
    }
    return true;
}

//void ViewDateTimeActivity::saveData()
//{
//    dp::IStatementPtr pInsStat(dp::getMainDatabase()->createStatement("INSERT INTO  Rokovi ( ID_Rokovi, ID_Aktivnosti, Datum_Pocetka, Vrijeme_Pocetka, Datum_Kraja, Vrijeme_Kraja, Datum_Prijave, Vrijeme_Prijave, ID_Predmeta) VALUES(?,?,?,?,?,?,?)"));
//    dp::Params parDS(pInsStat->allocParams());
//    dp::Transaction tr(dp::getMainDatabase());
//
//    td::Date dateb;
//    td::Time tb;
//    td::Date datee;
//    td::Time te;
//    td::Date datef;
//    td::Time tf;
//   
//    td::Variant val;
//    parDS <<dateb << tb << datee << te << datef << tf;
//    dp::IStatementPtr pDel(dp::getMainDatabase()->createStatement("DELETE FROM Rokovi"));
//    if (!pDel->execute())
//        return;
//    size_t nRows = _pDS->getNumberOfRows();
//    for (size_t i = 0; i < nRows; ++i)
//    {
//        auto row = _pDS->getRow(i);
//        dateb = row[0];
//        tb = row[1];
//        datee = row[2];
//        te = row[3];
//        datef = row[4];
//        tf = row[5];
//       
//
//        if (!pInsStat->execute())
//            return;
//    }
//    tr.commit();
//    return;
//}

bool ViewTasks::canAdd()
{
    if (doesItDexist())
        return false;
    return true;
}
bool ViewTasks::deleteTasks() {
    dp::IStatementPtr pDeleteItem(_db->createStatement("DELETE from DokumentiOpenPredaja where ID_Dokumenta=?"));
    dp::Params pParams2(pDeleteItem->allocParams());
    int iRow = _table.getFirstSelectedRow();
    if (iRow < 0) {
        return false;
    }
    td::Variant val;
    dp::IDataSet* pDS = _table.getDataSet();
    auto& row = pDS->getRow(iRow);
    val = row[0];
    td::INT4 id = val.i4Val();
    pParams2 << id;

    if (!pDeleteItem->execute())
    {
        return false;
    }
    return true;
}
bool ViewTasks::eraseTasks()

{
    td::INT4 id;
    dp::IStatementPtr pDeleteItem(_db->createStatement("DELETE from OpenPredaja where ID=?"));
    dp::Params pParams2(pDeleteItem->allocParams());
    pParams2 << id;

    for (auto itd : _itemsToDelete)
    {
        ///* dp::IStatementPtr pDeleteItem(_db->createStatement("delete from Rokovi where ID_Roka=?"));
        // dp::Params pParams2(pDeleteItem->allocParams());
        // pParams2 << id;*/
        id = itd;
        if (!pDeleteItem->execute())
        {
            return false;
        }
    }
    return true;
}

bool ViewTasks::insertTasks()
{
    //dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT COALESCE(MAX(ID), 0) as id FROM OpenPredaja");
    //dp::Columns pCols = pSelect->allocBindColumns(1);
    //td::INT4 id;
    //pCols << "id" << id;
    //pSelect->execute();
    //pSelect->moveNext();

    dp::IStatementPtr pInsertCourseP(_db->createStatement("insert into OpenPredaja (ID, Datum_Predaje, Vrijeme_Predaje, ID_Aktivnosti) values (?,?,?,?)"));
    dp::Params pParams2(pInsertCourseP->allocParams());
    td::INT4 id_akt, id;
    td::Date datump;
    td::Time vrijemep;
    //  id++;
    pParams2 << id << datump << vrijemep << id_akt;

    dp::IDataSet* pDS = _table.getDataSet();
    auto rowCnt = pDS->getNumberOfRows();
    for (size_t iRow = 0; iRow < rowCnt; ++iRow)
    {

        auto& row = pDS->getRow(iRow);
        id = row[0].i4Val();
        if (std::find(_itemsToInsert.begin(), _itemsToInsert.end(), id) == _itemsToInsert.end())
            continue;

        td::Variant val;

        datump = row[1];


        vrijemep = row[2];

        val = row[3];

        id_akt = val.i4Val();

        if (!pInsertCourseP->execute())
        {
            return false;
        }
    }
    return true;
}



bool ViewTasks::onClick(gui::Button* pBtn)
{
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

        return true;
    }
    if (pBtn == &_btnDelete2)
    {
        deleteTasks();

        return true;
    }

    if (pBtn == &_btnAdd)
    {

        td::INT4 itemid = findMaxID();
        if (!canAdd()) {
            showAlert(tr("AlertCNA"), tr("Already exists!"));
            return true;
        }

        _table.beginUpdate();
        auto& row = _table.getEmptyRow();
        populateDSRow(row, itemid);
        _table.push_back();
        _table.endUpdate();

        _itemsToInsert.push_back(itemid);
        return true;
    }
    if (pBtn == &_btnAddFile) {
        showOpenFileDialog();
        return true;
    }
    if (pBtn == &_btnSave) {
        showYesNoQuestionAsync(QuestionIDDDAAAA::Saveee, this, tr("alert"), tr("saveSure"), tr("Yes"), tr("No"));

        return true;
    }

    return false;
}
bool ViewTasks::doesItDexist()
{
    size_t nRows = _pDS->getNumberOfRows();
    td::Variant val;
    _type.getValue(val);
    for (size_t i = 0; i < nRows; ++i)
    {
        auto row = _pDS->getRow(i);
        if (row[3] == val)
            return true;
    }
    return false;
}
td::INT4 ViewTasks::getIDfromTable(int rowID)
{
    dp::IDataSet* pDS = _table.getDataSet();
    auto& row = pDS->getRow(rowID);
    return row[0].i4Val();
}





void ViewTasks::showOpenFileDialog()
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
                td::String strContent;

                if (fo::loadFileContent(strFileFullPath, strContent))
                {
                    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT COALESCE(MAX(ID), 0) as id FROM DokumentiOpenPredaja");
                    dp::Columns pCols = pSelect->allocBindColumns(1);
                    td::INT4 id;
                    pCols << "id" << id;
                    pSelect->execute();
                    pSelect->moveNext();
                    dp::IDatabase* pDB = dp::getMainDatabase();
                    //dp::getMainDatabase() if I were connected to DB before

                    dp::IStatementPtr pStatIns = pDB->createStatement("insert into DokumentiOpenPredaja(ID, Naziv, Dokumenti, ID_Dokumenta) values(?, ?, ?, ?)");
                    dp::Params paramsInsert(pStatIns->allocParams());
                    id++;

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

                    int iRow = _table.getFirstSelectedRow();
                    if (iRow < 0) {
                        return;
                    }
                    td::Variant val;
                    dp::IDataSet* pDS = _table.getDataSet();
                    auto& row = pDS->getRow(iRow);
                    val = row[0];
                    td::INT4 ID = val.i4Val();
                    paramsInsert << id << dp::toNCh(strFileName, MESSAGE_HEADER_LEN) << dataIn << ID;
                    //Neophodno, sa ove lokacije (strFileFullPath) se uzima BLOB
                    if (!dataIn.setInFileName(strFileFullPath))
                    {
                        gui::Alert::show(tr("Error"), tr("Did you delete the selected file?"));
                        return;
                    }

                    dp::Transaction transaction(pDB);

                    //bool delOK = pStatDel->execute();
                    bool insOK = pStatIns->execute();

                    bool commitOK = transaction.commit();
                }
            }
        });
#endif
}

//gui::TextEdit* ViewTasks::getTextEdit()
//{
//    return ;
//}

//void ViewTasks::selectFiles()
//{
//    auto pFD = new gui::OpenFileDialog(this, tr("OpenF"), { {tr("TxtDocs"),"*.txt"}, {tr("PDFDocs"),"*.pdf"}, {tr("JPGSlike"),"*.jpg"}, {tr("PNGSlike"),"*.png"} });
//
//#ifdef USE_CALLBACKS
//    pFD->openModal(&_callBackOpenFileDlg);
//#else
//    pFD->openModal([this](gui::FileDialog* pFD) {
//        auto status = pFD->getStatus();
//        if (status == gui::FileDialog::Status::OK) {
//
//            _attachedFiles.push_back(pFD->getFileName());
//
//            fo::fs::path filePathNow(_attachedFiles.last().c_str());/*
//            td::String strFileName = filePathNow.filename().string();
//            _titleFile.setTitle(strFileName);
//            _titleFile.setBold();*/
//        }
//        });
//#endif
//}


//bool ViewTasks::sendDocs()
//{
//    if (!dp::getMainDatabase()->isConnected())
//    {
//        gui::Alert::show(tr("Error"), tr("Problem with database..."));
//        return false;
//    }
//
//    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT COALESCE(MAX(ID), 0) as id FROM DokumentiOpenPredaja");
//    dp::Columns pCols = pSelect->allocBindColumns(1);
//    td::INT4 id;
//    pCols << "id" << id;
//    pSelect->execute();
//    pSelect->moveNext();
//
//    dp::IStatementPtr pStatIns = dp::getMainDatabase()->createStatement("INSERT INTO DokumentiOpenPredaja (ID, Dokumenti, ID_Dokumenta) VALUES (?, ?, ?)");
//    dp::Params parDS(pStatIns->allocParams());
//
//    fo::fs::path filePathNow(_attachedFiles.last().c_str());
//    td::String strFileName = filePathNow.filename().string();
//    td::String fileExtension = filePathNow.filename().extension().string();
//
//    td::BLOB::Type typeFile = td::BLOB::Type::TYPE_BINARY_UNKNOWN;
//    if (fileExtension.compareConstStr(".txt"))
//        typeFile = td::BLOB::Type::TYPE_TXT;
//    else if (fileExtension.compareConstStr(".pdf"))
//        typeFile = td::BLOB::Type::TYPE_PDF;
//    else if (fileExtension.compareConstStr(".jpg"))
//        typeFile = td::BLOB::Type::TYPE_JPG;
//    else if (fileExtension.compareConstStr(".png"))
//        typeFile = td::BLOB::Type::TYPE_PNG;
//
//    td::BLOB dataIn(td::BLOB::SRC_FILE, 16384U, typeFile);
//    td::INT4 d = 1;
//    id++;
//    parDS << id << dataIn<<d;
//
//    if (!dataIn.setInFileName(filePathNow))
//    {
//        gui::Alert::show(tr("Error"), tr("Did you delete selected file?"));
//        return true;
//    }
//    dp::Transaction tr(_db);
//
//    if (!pStatIns->execute())
//        return false;
//    return true;
//}

bool ViewTasks::onAnswer(td::UINT4 questionID, gui::Alert::Answer answer)//??
{
    if ((QuestionIDDDAAAA)questionID == QuestionIDDDAAAA::Saveee)
    {
        if (answer == gui::Alert::Answer::Yes) {
            saveData();

            showAlert(tr("succes"), tr("succesEE"));
            if (answer == gui::Alert::Answer::Yes) {
                saveData();
                //unutar button save se detektuje promjena i salje se poruka od sistema za sve studente
//sad bi trebao da bude poseban button za poruke na koji ce se otvoriti novi prozorcic koji ce prikazati tabelu Messages gdje je id korisnika jednak user id poruke
                std::vector<td::INT4> userIDs;
                //svim studentima
                dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT ID FROM KORISNICI WHERE PozicijaID=5");
                dp::Columns pCols = pSelect->allocBindColumns(1);
                td::INT4 id;
                pCols << "ID" << id;
                if (!pSelect->execute())
                    return false;
                while (pSelect->moveNext())
                {
                    userIDs.push_back(id);
                }
                td::String naslov = "Imate novi zadatak!";
                td::String poruka = "Registrovana je promjena iz predmeta ";
                poruka += _cName.getText();
                poruka += ".";
                MsgSender msg;
                msg.sendSystemMsgtoUsers(naslov, poruka, userIDs);
            }
        }
        return true;
    }/*
    if ((QuestionIDA)questionID == QuestionIDA::OpenFile)
    {
        if (answer == gui::Alert::Answer::Yes)
            showOpenFileDialog();
        return true;
    }*/
    return false;
}


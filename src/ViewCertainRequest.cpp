#pragma once
#include "ViewCertainRequest.h"
#include "ViewIDs.h"
#include <gui/FileDialog.h>
#include <gui/Alert.h>
#include <fo/FileOperations.h>
#include <td/BLOB.h>



ViewCertainRequest::ViewCertainRequest(td::String ime, td::String prezime, td::String indeks, td::String tipKarte, td::String statusKarte, td::String sadrzajKarte, td::String naslovKarte)

    : _lblstudentsTicket(tr("Student's ticket"))
    , _namelbl(tr("nameUser"))
    , _surnamelbl(tr("surnameUser"))
    , _indekslbl(tr("indexUser"))
    , _titleOfTicketlbl(tr("titleOfTicket"))
    , _typeOfTicketlbl(tr("typeOfTicket"))
    , _statuslbl(tr("status"))
    , _answerlbl(tr("Answer:"))
    , _hlBtnsDB(3)
    , _btnSend(tr("send"), tr("sendTT"))
    //, _btnOpenAttachment(tr("Open Attachment"), tr("Open AttachmentTT"))
    , _btnSaveAttachment(tr("SaveAttachment"), tr("SaveAttachmentTT"))
    , _gl(7, 6)
    , _db(dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3))
    , indeks(indeks)
{

    _hlBtnsDB.append(_btnSend);
    _hlBtnsDB.appendSpacer();
    _hlBtnsDB.append(_btnSaveAttachment);
    //_hlBtnsDB.append(_btnOpenAttachment);

    _btnSend.setType(gui::Button::Type::Default);
    _btnSaveAttachment.setType(gui::Button::Type::Default);
    // _btnOpenAttachment.setType(gui::Button::Type::Default);

    gui::GridComposer gc(_gl);

    gc.appendRow(_lblstudentsTicket);
    gc.appendRow(_namelbl);
    gc.appendCol(_name);
    gc.appendCol(_surnamelbl);
    gc.appendCol(_surname);
    gc.appendCol(_indekslbl);
    gc.appendCol(_indeks);
    gc.appendRow(_titleOfTicketlbl);
    gc.appendCol(_titleOfTicket);
    gc.appendCol(_typeOfTicketlbl);
    gc.appendCol(_typeOfTicket);
    gc.appendCol(_statuslbl);
    gc.appendCol(_status);
    gc.appendRow(_bodyOfTicket, 0);
    gc.appendRow(_answerlbl);
    gc.appendRow(_answer, 0);
    gc.appendRow(_hlBtnsDB, 0);

    gui::View::setLayout(&_gl);

    _db = dp::getMainDatabase();

    _name.setValue(ime);
    _surname.setValue(prezime);
    _indeks.setValue(indeks);
    _titleOfTicket.setValue(naslovKarte);
    _typeOfTicket.setValue(tipKarte);
    _status.setValue(statusKarte);
    _bodyOfTicket.setText(sadrzajKarte);


    _name.setAsReadOnly();
    _surname.setAsReadOnly();
    _indeks.setAsReadOnly();
    _typeOfTicket.setAsReadOnly();
    _bodyOfTicket.setAsReadOnly();

    //populateData();
    td::String status = "Obradjen";
    _answerlbl.setBold();
    if (statusKarte == status)
    {
        _answerlbl.hide(true, true);
        _answer.hide(true, true);
        _btnSend.hide(true, true);
        _btnSaveAttachment.hide(true, true);
    }

}
//void ViewCertainRequest::populateData(){
//    td::String data;
//    td::String setstr = "Select Request from SAOStudentTicket where Indeks=";
//    setstr.append(indeks);
//    dp::IStatement* pSel = dp::getMainDatabase()->createStatement(setstr);
//    dp::Columns cols(pSel->allocBindColumns(1));
//    cols << "Request" << data;
//    if (!pSel->execute())
//    {
//        showAlert("Error populating data","");
//        return;
//    }
//    _bodyOfTicket.setText(data);
//}

//void ViewCertainRequest::initTableStudent()
//{
//    gui::Columns visCols(_tableStudent.allocBindColumns(4));
//    visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, tr("nameUser"), "", 305) << gui::Header(1, tr("surnameUser"), "", 305) << gui::Header(2, tr("indexUser"), "", 305) << gui::Header(3, tr("typeOfTicket"), "", 305);
//    _tableStudent.init(_pDS);
//    if (_pDS->getNumberOfRows()) {
//        _tableStudent.selectRow(0, true);
//    }
//}
//
//void ViewCertainRequest::populateDataForStudent() {   //iz nekog razloga ova funkcija ne postavi vrijednosti za _name,_surname itd
//    td::String setstr= "SELECT k.Ime as Name, k.Prezime as Surname, k.Indeks as Ind, p.Ticket_Tip as TicketType FROM Korisnici k JOIN SAOStudentTicket p ON k.Indeks = p.Indeks WHERE k.Indeks=";
//    setstr.append(indeks);
//    td::Variant ime, prezime, ind, tipKarte;
//    dp::IStatement* pSelect= dp::getMainDatabase()->createStatement(setstr);
//    dp::Columns cols(pSelect->allocBindColumns(4));
//    cols << "Name" << ime << "Surname" << prezime << "Ind" << ind << "TicketType" << tipKarte;
//    if (!pSelect->execute())
//    {
//        pSelect= nullptr;
//        showAlert("errorReading", "");
//        return;
//    }
//    _name.setValue(ime);
//    _surname.setValue(prezime);
//    _indeks.setValue(indeks);
//    _typeOfTicket.setValue(tipKarte);
//}


void ViewCertainRequest::showSaveFileDialog() {
    auto pFD = new gui::SelectFolderDialog(this, tr("SelectFolder"));
    pFD->openModal([this](gui::FileDialog* pFD)
        {
            auto status = pFD->getStatus();
            if (status == gui::FileDialog::Status::OK)
            {
                //folder gdje snimamo BLOB iz tabele u bazi
                td::String strFolderName = pFD->getFileName();
                fo::fs::path filePath(strFolderName.c_str());

                td::String setstr1 = "select Name_attachment from SAOStudentTicket where Indeks =";
                setstr1.append(indeks);
                td::String setstr2 = "select Attachment from SAOStudentTicket where Indeks =";
                setstr2.append(indeks);

                dp::IStatementPtr pSelFileName = dp::getMainDatabase()->createStatement(setstr1);
                dp::IStatementPtr pSelBlob = dp::getMainDatabase()->createStatement(setstr2);

                //zelimo ime fajla
                td::String fileName;
                dp::Columns Cols(pSelFileName->allocBindColumns(1));
                Cols << "Name_attachment" << fileName;


                if (!pSelFileName->execute())
                {
                    showAlert("Error selecting name of file", "");
                    return;
                }

                if (!pSelFileName->moveNext())
                {
                    showAlert("Error selecting name of file", "");
                    return;
                }

                filePath /= fileName.c_str();

                td::String fullFilePath = filePath.string();

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

                td::BLOB BLOBout(td::BLOB::SRC_FILE, 16384U, typeFile);
                //select sada i snimi u dokument
                dp::Columns Cols1(pSelBlob->allocBindColumns(1));
                Cols1 << "Attachment" << BLOBout;

                if (!BLOBout.setOutFileName(fullFilePath))
                {
                    showAlert("Folder is not selected or file already exists with this filepath", "");
                    //nije odabran folder?
                    //ili vec postoji fajl sa ovim fullFilePath?        
                    return;
                }
                if (!pSelBlob->execute())
                {
                    showAlert("Error selecting attachment", "");
                    return;
                }
                if (!pSelBlob->moveNext())
                {
                    showAlert("Error selecting attachment", "");
                    return;
                }
            }
            showAlert("Successfully saved", "");
        });
}

bool ViewCertainRequest::onClick(gui::Button* pBtn)
{
    if (pBtn == &_btnSend)
    {
        if (_answer.isEmpty()) {
            showAlert("Empty body!", "Do you want to enter your answer first?");
            return false;
        }

        td::String setstr = "Update SAOStudentTicket set Status_ID=2 where Indeks=";
        setstr.append(indeks);
        dp::IStatementPtr pUpdate(dp::getMainDatabase()->createStatement(setstr));
        if (!pUpdate->execute()) {
            showAlert("Error updating", "");
            return false;
        }
        _status.setValue("Obradjen");

        showAlert("Successfully sent", "");
        return true;

    }

    if (pBtn == &_btnSaveAttachment)
    {
        td::String setstr1 = "select Name_attachment from SAOStudentTicket where Indeks =";
        setstr1.append(indeks);
        dp::IStatementPtr pSelFileName = dp::getMainDatabase()->createStatement(setstr1);
        td::String fileName;
        dp::Columns Cols(pSelFileName->allocBindColumns(1));
        Cols << "Name_attachment" << fileName;


        if (!pSelFileName->execute())
        {
            showAlert("Error selecting name of file", "");
            return false;
        }

        if (!pSelFileName->moveNext())
        {
            showAlert("Error selecting name of file", "");
            return false;
        }

        if (fileName != td::String("")) showSaveFileDialog();
        else showAlert("Student has no attachment", "");
        return true;
    }

    return false;
}
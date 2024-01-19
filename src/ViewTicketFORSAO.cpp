#pragma once
#include "ViewTicketForSAO.h"
#include "ViewIDs.h"
#include "WindowCertainRequest.h"

ViewTicketForSAO::ViewTicketForSAO()
	: _onHoldTicketslbl(tr("On hold tickets:"))
	, _answeredTicketslbl(tr("Answered tickets:"))
	, _hlBtnsDB(2)
	, _btnOpen(tr("open"), tr("openTT"))
	, _gl(5, 2)
	, _db(dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3))
{
	_onHoldTicketslbl.setBold();
	_answeredTicketslbl.setBold();
	_hlBtnsDB.appendSpacer();
	_hlBtnsDB.append(_btnOpen);

	_btnOpen.setType(gui::Button::Type::Default);


	gui::GridComposer gc(_gl);

	gc.appendRow(_onHoldTicketslbl);
	gc.appendRow(_onHoldTickets, 0);
	gc.appendRow(_answeredTicketslbl);
	gc.appendRow(_answeredTickets, 0);
	gc.appendRow(_hlBtnsDB, 0);
	gui::View::setLayout(&_gl);
	_db = dp::getMainDatabase();

	populateAnsweredTickets();
	populateOnHoldTickets();
	
}

void ViewTicketForSAO::initOnHoldTable()
{
	gui::Columns visCols(_onHoldTickets.allocBindColumns(6));
	visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, tr("nameUser"), "", 305) << gui::Header(1, tr("surnameUser"), "", 305) << gui::Header(2, tr("indexUser"), "", 305) << gui::Header(3, tr("typeOfTicket"), "", 305) << gui::Header(4, tr("titleOfTicket"), "", 305) << gui::Header(6, tr("status"), "", 305);
	_onHoldTickets.init(_pDS_hold);
	if (_pDS_hold->getNumberOfRows()) {
		_onHoldTickets.selectRow(0, true);
	}

}

void ViewTicketForSAO::initAnsweredTable()
{
	gui::Columns visCols(_answeredTickets.allocBindColumns(6));
	visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, tr("nameUser"), "", 305) << gui::Header(1, tr("surnameUser"), "", 305) << gui::Header(2, tr("indexUser"), "", 305) << gui::Header(3, tr("typeOfTicket"), "", 305) << gui::Header(4, tr("titleOfTicket"), "", 305) << gui::Header(6, tr("status"), "", 305);
	_answeredTickets.init(_pDS_answered);
	if (_pDS_answered->getNumberOfRows()) {
		_answeredTickets.selectRow(0, true,false);
	}

}

void ViewTicketForSAO::populateOnHoldTickets()
{
	td::String setstr = "SELECT Korisnici.Ime as Name, Korisnici.Prezime as Surname,SAOStudentTicket.Indeks as StudentIndex, SAOStudentTicket.Ticket_Tip as TypeOfTicket, SAOStudentTicket.Req_Title as TitleofTicket, SAOStudentTicket.Status_ID as Status_ID, SAOTicket_Status.Status as Status, SAOStudentTicket.Request as Request From Korisnici, SAOStudentTicket, SAOTicket_Status where Korisnici.Indeks=SAOStudentTicket.Indeks AND Status_ID=1 AND SAOTicket_Status.ID=SAOStudentTicket.Status_ID";
	_pDS_hold = dp::getMainDatabase()->createDataSet(setstr, dp::IDataSet::Execution::EX_MULT);
	dp::DSColumns cols(_pDS_hold->allocBindColumns(8));
	cols << "Name" << td::string8 << "Surname" << td::string8 << "StudentIndex" << td::string8 << "TypeOfTicket" << td::string8 << "TitleOfTicket" << td::string8 <<"Status_ID"<<td::int4<< "Status" << td::string8 << "Request" << td::string8;
	if (!_pDS_hold->execute())
	{
		_pDS_hold = nullptr;
		showAlert("errorReadingOnHoldTable", "");
		return;
	}
	initOnHoldTable();
}

void ViewTicketForSAO::populateAnsweredTickets()
{
	td::String setstr = "SELECT Korisnici.Ime as Name, Korisnici.Prezime as Surname,SAOStudentTicket.Indeks as StudentIndex, SAOStudentTicket.Ticket_Tip as TypeOfTicket, SAOStudentTicket.Req_Title as TitleofTicket, SAOStudentTicket.Status_ID as Status_ID,SAOTicket_Status.Status as Status, SAOStudentTicket.Request as Request From Korisnici, SAOStudentTicket, SAOTicket_Status where Korisnici.Indeks=SAOStudentTicket.Indeks AND Status_ID=2 AND SAOTicket_Status.ID=SAOStudentTicket.Status_ID";
	_pDS_answered = dp::getMainDatabase()->createDataSet(setstr, dp::IDataSet::Execution::EX_MULT);
	dp::DSColumns cols(_pDS_answered->allocBindColumns(8));
	cols << "Name" << td::string8 << "Surname" << td::string8 << "StudentIndex" << td::string8 << "TypeOfTicket" << td::string8 << "TitleOfTicket" << td::string8 << "Status_ID" << td::int4 << "Status" << td::string8 << "Request" << td::string8;
	if (!_pDS_answered->execute())
	{
		_pDS_answered = nullptr;
		showAlert("errorReadingAnsweredTable", "");
		return;
	}
	initAnsweredTable();
}
//
//
//td::String ViewTicketForSAO::getIndex()
//{
//	int iRow = _onHoldTickets.getFirstSelectedRow();
//	/*if (iRow < 0) {
//		return;
//	}*/
//	dp::IDataSet* pDS = _onHoldTickets.getDataSet();
//	auto& row = pDS->getRow(iRow);
//
//	td::Variant val = row[2];
//	return val.strVal();
//
//}
//
//td::String ViewTicketForSAO::getName()
//{
//	int iRow = _onHoldTickets.getFirstSelectedRow();
//	/*if (iRow < 0) {
//		return;
//	}*/
//	dp::IDataSet* pDS = _onHoldTickets.getDataSet();
//	auto& row = pDS->getRow(iRow);
//
//	td::Variant val = row[0];
//	return val.strVal();
//
//}
//
//td::String ViewTicketForSAO::getSurname()
//{
//	int iRow = _onHoldTickets.getFirstSelectedRow();
//	/*if (iRow < 0) {
//		return;
//	}*/
//	dp::IDataSet* pDS = _onHoldTickets.getDataSet();
//	auto& row = pDS->getRow(iRow);
//
//	td::Variant val = row[1];
//	return val.strVal();
//
//}
//
//td::String ViewTicketForSAO::getTypeOfTicket()
//{
//	int iRow = _onHoldTickets.getFirstSelectedRow();
//	/*if (iRow < 0) {
//		return;
//	}*/
//	dp::IDataSet* pDS = _onHoldTickets.getDataSet();
//	auto& row = pDS->getRow(iRow);
//
//	td::Variant val = row[3];
//	return val.strVal();
//
//}

//td::String ViewTicketForSAO::getStatus(td::INT4& val)
//{
//	td::String setstr = "SELECT Status From SAOTicket_Status where ID=val";
//	dp::IStatement* _pSel = dp::getMainDatabase()->createStatement(setstr);
//	dp::Columns Cols(_pSel->allocBindColumns(1));
//	Cols << "Status" << status;
//	return status;
//}

//td::String ViewTicketForSAO::getRequest()
//{
//	int iRow = _onHoldTickets.getFirstSelectedRow();
//	/*if (iRow < 0) {
//		return;
//	}*/
//	dp::IDataSet* pDS = _onHoldTickets.getDataSet();
//	auto& row = pDS->getRow(iRow);
//
//	td::Variant val = row[6];
//	return val.strVal();
//}
//
//td::String ViewTicketForSAO::getTitle()
//{
//	int iRow = _onHoldTickets.getFirstSelectedRow();
//	/*if (iRow < 0) {
//		return;
//	}*/
//	dp::IDataSet* pDS = _onHoldTickets.getDataSet();
//	auto& row = pDS->getRow(iRow);
//
//	td::Variant val = row[4];
//	return val.strVal();
//}


bool ViewTicketForSAO::onChangedSelection(gui::TableEdit* pTE)
{
	if (pTE == &_onHoldTickets)
	{
		int iRow = _onHoldTickets.getFirstSelectedRow();
		if (iRow < 0) {
			return true;
		}
		td::Variant val;
		dp::IDataSet* pDS = _onHoldTickets.getDataSet();
		auto& row = pDS->getRow(iRow);

		val = row[0];
		ime = val.getConstStr();

		val = row[1];
		prezime = val.getConstStr();

		val = row[2];
		indeks = val.getConstStr();

		val = row[3];
		tipKarte = val.getConstStr();

		val = row[4];
		title = val.getConstStr();

		val = row[6];
		status = val.getConstStr();

		val = row[7];
		request = val.getConstStr();

	}

	if (pTE == &_answeredTickets)
	{
		int iRow = _answeredTickets.getFirstSelectedRow();
		if (iRow < 0) {
			return true;
		}
		td::Variant val;
		dp::IDataSet* pDS = _answeredTickets.getDataSet();
		auto& row = pDS->getRow(iRow);

		val = row[0];
		ime = val.getConstStr();

		val = row[1];
		prezime = val.getConstStr();

		val = row[2];
		indeks = val.getConstStr();

		val = row[3];
		tipKarte = val.getConstStr();

		val = row[4];
		title = val.getConstStr();

		val = row[6];
		status = val.getConstStr();

		val = row[7];
		request = val.getConstStr();

	}
}

bool ViewTicketForSAO::onClick(gui::Button* pBtn)
{
	if (pBtn = &_btnOpen)
	{
		/*td::String indeks = getIndex();
		td::String ime = getName();
		td::String prezime = getSurname();
		td::String tipKarte = getTypeOfTicket();
		td::String status = getStatus();
		td::String request = getRequest();
		td::String title = getTitle();*/
		gui::Window* pParentWnd = getParentWindow();
		auto pWnd = new WindowCertainRequest(pParentWnd,indeks,ime,prezime,tipKarte,status,request,title);
		pWnd->keepOnTopOfParent();
		pWnd->open();
		return true;
	}
}
#pragma once
#pragma once
#include "ViewTicket.h"
#include "ViewIDs.h"

ViewTicket::ViewTicket()
	: _namelbl(tr("nameUser"))
	, _surnamelbl(tr("surnameUser"))
	, _indexlbl(tr("indexUserL"))
	, _typelbl(tr("typeOfMessage"))
	, _type(td::int4)
	, _subjectlbl(tr("subject"))
	, _bodylbl(tr("body"))
	, _hlBtnsDB(2)
	, _btnSend(tr("Send"))
	, _gl(5, 6)
	, _db(dp::getMainDatabase())
{
	_hlBtnsDB.appendSpacer();
	_hlBtnsDB.append(_btnSend); 

	_btnSend.setType(gui::Button::Type::Default);

	gui::GridComposer gc(_gl);

	gc.appendRow(_namelbl);
	gc.appendCol(_name);
	gc.appendCol(_surnamelbl);
	gc.appendCol(_surname);
	gc.appendCol(_indexlbl);
	gc.appendCol(_index);

	gc.appendRow(_typelbl);
	gc.appendCol(_type);
	gc.appendCol(_subjectlbl);
	gc.appendCol(_subject);
	gc.appendRow(_bodylbl);
	gc.appendRow(_body,0);
	gc.appendRow(_hlBtnsDB, 0);
	gui::View::setLayout(&_gl);

	populateTypeCombo(_type);
	
	/*td::INT4 ticket = _type.getSelectedIndex();
	switch (ticket) {
	case 0: _bodylbl = tr("Molba");
	case 1: _bodylbl = tr("Pitanje");
	case 2: _bodylbl = tr("Žalba");
	}*/

}


void ViewTicket::populateTypeCombo(gui::DBComboBox& combo)
{
	dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT ID,Ticket FROM SAOTicket");
	dp::Columns pCols = pSelect->allocBindColumns(2);
	td::String name;
	td::INT4 id;
	pCols << "ID" << id << "Ticket" << name;
	pSelect->execute();

	while (pSelect->moveNext())
	{
		combo.addItem(name,id);
	}
}


bool ViewTicket::onClick(gui::Button* pBtn)
{
	if (pBtn == &_btnSend)
	{
		showYesNoQuestionAsync(QuestionID::Save, this, tr("alert"), tr("saveSureTicket"), tr("Yes"), tr("No"));
		return true;
	}

	return false;
}



bool ViewTicket::sendTicket() {

	dp::IStatementPtr pInsStat(_db->createStatement("INSERT INTO SAOStudentTicket (Indeks,ID_Ticket,Req_Title,Request) VALUES (?,?,?,?)"));
	dp::Params parDS(pInsStat->allocParams());
	td::String index, request,requestTitle;
	td::INT4 idticket;
	parDS << dp::toNCh(index, 30) << idticket <<dp::toNCh(requestTitle,100)<< dp::toNCh(request, 1000);


	td::Variant var;

	_index.getValue(var);
	index = var.getConstStr();
	_subject.getValue(var);
	requestTitle = var.getConstStr();
	request =_body.getText();
	idticket =_type.getSelectedIndex()+ 1;

	dp::Transaction tr(_db);

	if (!pInsStat->execute())
	{
		td::String error;

		pInsStat->getErrorStr(error);
		showAlert(error, "");

		return false;

	}
	tr.commit();

	return true;

}

bool ViewTicket::IsIndexInUsersTable() {

	dp::IDataSetPtr pompDS = dp::getMainDatabase()->createDataSet("SELECT Korisnici.Indeks as indeks FROM Korisnici WHERE Korisnici.ID!=0 AND Korisnici.ID!=-1;", dp::IDataSet::Execution::EX_MULT);
	dp::DSColumns cols(pompDS->allocBindColumns(1));
	cols << "indeks" << td::string8;
	if (!pompDS->execute())
	{
		showAlert(tr("errorReadingUsers"), "");
		pompDS = nullptr;
		return false;
	}

	bool returnVal=false;

	int n=pompDS->getNumberOfRows();

	td::String index = _index.getText();

	for (int i = 0;i < n;i++)
	{
		auto& row = pompDS->getRow(i);
		if (row[0] == index)
			returnVal = true;
	}

	return returnVal;

}

bool ViewTicket::onAnswer(td::UINT4 questionID, gui::Alert::Answer answer) {
	if ((QuestionID)questionID == QuestionID::Save)
	{
		if (!IsIndexInUsersTable())
		{
			showAlert(tr("errorNoIndexInUsers"),"");
			return false;

		}
		if (answer == gui::Alert::Answer::Yes) {
			sendTicket();
			showAlert(tr("succes"), tr("succesEE"));
		}
		return true;
	}
	return false;
}

bool ViewTicket::onChangedSelection(gui::DBComboBox* pCmb) {
	if (pCmb == &_type)
	{
		td::String ticket = pCmb->getSelectedText();
		_bodylbl.setTitle(ticket);
		return true;
	}
	return false;
}
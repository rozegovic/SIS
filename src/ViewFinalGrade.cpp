#pragma once
#include "ViewFinalGrade.h"
#include <td/Types.h>
#include "Reports.h"
#include "SendMessage.h"
#include "Globals.h"
#include "SendMessage.h"

ViewFinalGrade::ViewFinalGrade(td::INT4 SubjectID) : _db(dp::getMainDatabase())
, _lblname(tr("userName:"))
, _lblsname(tr("userLName:"))
, _lblindex(tr("lblindex"))
, _lblGrade(tr("grade:")) 
, _lblCName(tr("courseName:"))
, _btnDelete(tr("Delete"))
, _lblpoints(tr("points"))
, _btnAdd(tr("Add"))
, _hlBtns(6)
, _gl(6, 4) 
, _SubjectID(SubjectID)
,moguciBodovi(0.)
,_grade(td::int4)
{


	_hlBtns.appendSpacer();
	_hlBtns.append(_btnAdd);
	_hlBtns.append(_btnDelete);

	//  _btnUpdate.setType(gui::Button::Type::Default);
	gui::GridComposer gc(_gl);

	SetCurrentSubject();
	_cName.setAsReadOnly();

	gc.appendRow(_lblCName);
	gc.appendCol(_cName);

	gc.appendCol(_lblindex);
	gc.appendCol(_index);

	gc.appendRow(_lblname);
	gc.appendCol(_name);

	gc.appendCol(_lblsname);
	gc.appendCol(_sname);

	gc.appendRow(_lblpoints);
	gc.appendCol(_points);

	gc.appendCol(_lblGrade);
	gc.appendCol(_grade);


	gc.appendRow(_table, 0);

	gc.appendRow(_hlBtns, 0);
	gui::View::setLayout(&_gl);

	populateData();
	//onChangedSelection(&_table);
}

void ViewFinalGrade::populateData()
{

	_pDS = _db->createDataSet("SELECT DISTINCT Korisnici.ID AS ID, Korisnici.Ime AS Name, Korisnici.Prezime As Surname, Korisnici.Indeks AS Indeks "
		" FROM Korisnici, PolazniciAktivnosti WHERE PolazniciAktivnosti.ID_Korisnika = Korisnici.ID AND "
		" PolazniciAktivnosti.ID_Aktivnosti IN(SELECT Aktivnosti.ID_Aktivnosti FROM Aktivnosti WHERE Aktivnosti.ID_Predmeta = ?)");


	dp::Params parDS(_pDS->allocParams());

	parDS << _SubjectID;
	dp::DSColumns cols(_pDS->allocBindColumns(4));
	cols << "ID" << td::int4 << "Name" << td::string8 << "Surname" << td::string8 << "Indeks" << td::string8;


	if (!_pDS->execute())
	{
		showAlert("","populateData greska");
		_pDS = nullptr;
		return;
	}

	dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT Aktivnosti.Procenat AS p FROM Aktivnosti WHERE Aktivnosti.ID_Predmeta=?");
	dp::Params parDS1(pSelect->allocParams());
	parDS1 << _SubjectID;
	dp::Columns pCols = pSelect->allocBindColumns(1);
	td::INT4 bodoviAktivnost;
	pCols << "p" << bodoviAktivnost;
	if (!pSelect->execute()) {
		showAlert("Aktivnosti.Procenat pravigresku", "u populateData()");
	}

	while (pSelect->moveNext())
	{
		moguciBodovi += bodoviAktivnost;
	}


	initTable();

	//_table.init(_pDS);
}

bool ViewFinalGrade::onChangedSelection(gui::TableEdit* pTE)
{
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
		_name.setValue(val);

		val = row[2];
		_sname.setValue(val);

		val = row[3];
		_index.setValue(val);


		td::INT4 bodovi = 0;



		dp::IStatementPtr pSelect2 = dp::getMainDatabase()->createStatement("SELECT OcjeneIspita.Ocjena AS ocjena FROM OcjeneIspita,Aktivnosti WHERE"
			" Aktivnosti.ID_Aktivnosti=OcjeneIspita.ID_Aktivnosti AND Aktivnosti.ID_Predmeta=? AND OcjeneIspita.ID_Korisnika=?");

		dp::Params parDS2(pSelect2->allocParams());
		parDS2 << _SubjectID << row[0].i4Val();
		dp::Columns pCols2 = pSelect2->allocBindColumns(1);
		td::String ispiti;
		pCols2 << "ocjena" << ispiti;
		if (!pSelect2->execute()) {
			showAlert("", "u populateData()");
		}

		while (pSelect2->moveNext())
		{
			bodovi += ispiti.toINT4();
		}

		dp::IStatementPtr pSelect3 = dp::getMainDatabase()->createStatement("SELECT OcjeneLabZadace.Ocjena AS ocjena FROM OcjeneLabZadace,Aktivnosti"
			" WHERE Aktivnosti.ID_Aktivnosti=OcjeneLabZadace.ID_Aktivnosti AND Aktivnosti.ID_Predmeta=? AND OcjeneLabZadace.ID_Korisnika=?");

		dp::Params parDS3(pSelect3->allocParams());
		parDS3 << _SubjectID << row[0].i4Val();
		dp::Columns pCols3 = pSelect3->allocBindColumns(1);
		td::String dodaci;
		pCols3 << "ocjena" << dodaci;
		if (!pSelect3->execute()) {
			showAlert("", "u populateData()");
		}

		while (pSelect3->moveNext())
		{
			bodovi += dodaci.toINT4();
		}

		  td::Variant var(std::to_string(bodovi));

		_points.setValue(var);

		//_points.setText(std::to_string(bodovi));


		auto pompDS = _db->createDataSet("SELECT OcjeneFinal.Ocjena AS ocjena FROM OcjeneFinal WHERE OcjeneFinal.ID_Predmeta=? AND OcjeneFinal.ID_Korisnika=?");


		dp::Params parDS(pompDS->allocParams());

		parDS << _SubjectID << row[0].i4Val();
		dp::DSColumns cols(pompDS->allocBindColumns(1));
		cols << "ocjena"<<td::string8;


		if (!pompDS->execute())
		{
			showAlert("", "populateData greska");
			pompDS = nullptr;
			return false;
		}


		if (pompDS->getNumberOfRows() != 0)
		{
			auto& pomrow=pompDS->getRow(0);

			_grade.setValue(9);
		}
		else {

			float br1=bodovi;
			float br2 = moguciBodovi;


			int postotak = ( br1/br2) * 100.;

			if (postotak <55)
				_grade.setValue(5);
			else if (postotak >= 55 && postotak < 65)
				_grade.setValue(6);
			else if (postotak >= 65 && postotak < 75)
				_grade.setValue(7);
			else if (postotak >= 75 && postotak < 85)
				_grade.setValue(8);
			else if (postotak >= 85 && postotak < 95)
				_grade.setValue(9);
			else if (postotak >= 95)
				_grade.setValue(10);
		}

		  



		return true;
	}
	return false;
};




void ViewFinalGrade::SetCurrentSubject()
{
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




void ViewFinalGrade::initTable()
{
	gui::Columns visCols(_table.allocBindColumns(4));
	visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, tr("IDUser"), "", 305) << gui::Header(3, tr("nameUser"), "", 305) << gui::Header(1, tr("surnameUser"), "", 305) << gui::Header(2, tr("indexUser"), "", 305);
	_table.init(_pDS);
	if (_pDS->getNumberOfRows()) {
		_table.selectRow(0, true);
	}
}


bool ViewFinalGrade::onClick(gui::Button* pBtn) {

	if (pBtn == &_btnAdd)
	{
		auto& row = _table.getCurrentRow();

		dp::Transaction tr(_db);

		dp::IStatementPtr pDelStat(_db->createStatement("DELETE FROM OcjeneFinal WHERE OcjeneFinal.ID_Predmeta=? AND OcjeneFinal.ID_Korisnika=?"));
		dp::Params parDSdel(pDelStat->allocParams());
		td::INT4 predmet, id;
		parDSdel << predmet<<id;

		predmet = _SubjectID;
		id = row[0].i4Val();

		if (!pDelStat->execute())
			return false;



		dp::IStatementPtr pInsStat(_db->createStatement("INSERT INTO OcjeneFinal (ID_Korisnika,ID_Predmeta,Ocjena) VALUES(?,?,?) "));
		dp::Params parDS(pInsStat->allocParams());
		td::INT4 ocjena;
		parDS << id << predmet << ocjena;



		ocjena = _grade.getValue().i4Val();

		pInsStat->execute();


		tr.commit();


		td::String naslov= _grade.getValue().getConstStr();
		td::String poruka = "Dobili ste ";
		poruka.append(std::to_string(_grade.getValue().i4Val()));
		MsgSender msg;
		msg.sendSystemMsgtoUser(naslov, poruka, row[0].i4Val());



		return true;
	}


	return true;
}
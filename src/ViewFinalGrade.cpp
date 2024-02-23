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
, _lblindex(tr("indexUserL"))
, _lblGrade(tr("grade:")) 
, _lblCName(tr("courseName:"))
, _btnDelete(tr("Delete"))
, _lblpoints(tr("ActivityPoints"))
, _btnAdd(tr("Enroll"))
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

	td::String strSet("SELECT DISTINCT Korisnici.ID AS ID, Korisnici.Ime AS Name, Korisnici.Prezime As Surname, Korisnici.Indeks AS Indeks,Korisnici.Adresa as Ocjena "
		" FROM Korisnici, PolazniciAktivnosti WHERE PolazniciAktivnosti.ID_Korisnika = Korisnici.ID AND "
		" PolazniciAktivnosti.ID_Aktivnosti IN(SELECT Aktivnosti.ID_Aktivnosti FROM Aktivnosti WHERE Aktivnosti.ID_Predmeta = ");
	strSet.append(std::to_string(_SubjectID));
	strSet.append(");");


	_pDS = _db->createDataSet(strSet);


	dp::DSColumns cols(_pDS->allocBindColumns(5));
	cols << "ID" << td::int4 << "Name" << td::string8 << "Surname" << td::string8 << "Indeks" << td::string8<< "Ocjena" << td::string8;


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



	td::INT4 vel=_pDS->getNumberOfRows();

	for (int i = 0;i < vel;i++)
	{
		auto& row = _pDS->getRow(i);

		row[4] = "";

		_pDS->updateRow(i);
	}




	auto pompDS = _db->createDataSet("SELECT OcjeneFinal.ID_Korisnika AS UserID,OcjeneFinal.Ocjena AS ocjena FROM OcjeneFinal WHERE OcjeneFinal.ID_Predmeta=?");


	dp::Params parDS2(pompDS->allocParams());

	parDS2<< _SubjectID;
	dp::DSColumns cols2(pompDS->allocBindColumns(2));
	cols2 << "UserID"<< td::int4 << "ocjena" << td::string8;


	if (!pompDS->execute())
	{
		showAlert("", "populateData greska");
		pompDS = nullptr;
	}



	size_t vel1 = pompDS->getNumberOfRows();


	for (int i = 0;i < vel1;i++)
	{
		auto& row1 = pompDS->getRow(i);
		for (int j = 0;j < vel;j++)
		{
		  auto& row = _pDS->getRow(j);
			if (row[0] == row1[0])

				row[4] = row1[1];

			_pDS->updateRow(j);
		}
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
			_lblGrade.setTitle(tr("grade:"));

			auto& pomrow=pompDS->getRow(0);

			td::String var;

			var=pomrow[0].strVal();

			_grade.setValue(var.toInt());
			
		}
		else {

			_lblGrade.setTitle(tr("Preporucena ocjena*:"));

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
	gui::Columns visCols(_table.allocBindColumns(5));
	visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, tr("IDUser"), "", 150) << gui::Header(1, tr("nameUser"), "", 150) << gui::Header(2, tr("surnameUser"), "", 150) << gui::Header(3, tr("indexUser"), "", 150)<<gui::Header(4, tr("grade"), "", 150);
	_table.init(_pDS);
	if (_pDS->getNumberOfRows()) {
		_table.selectRow(0, true);
	}
}


bool ViewFinalGrade::onClick(gui::Button* pBtn) {

	if (pBtn == &_btnAdd)
	{
		td::Variant v;
		_grade.getValue(v);
		
		if (v.i4Val() < 0 || v.i4Val() > 10)
		{
			showAlert(tr("Greška"), tr("Neispravna ocjena"));
			return true;
		}

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

		td::Variant var;

		_cName.getValue(var);

		td::String naslov= var.getConstStr();
		td::String poruka = "Dobili ste ";
		_grade.getValue(var);
		poruka.append(std::to_string(var.i4Val()));
		MsgSender msg;
		msg.sendSystemMsgtoUser(naslov, poruka, row[0].i4Val());

	       
		reloadTable();

		

		return true;
	}

	if (pBtn == &_btnDelete)
	{


		auto& row = _table.getCurrentRow();

		dp::Transaction tr(_db);

		dp::IStatementPtr pDelStat(_db->createStatement("DELETE FROM OcjeneFinal WHERE OcjeneFinal.ID_Predmeta=? AND OcjeneFinal.ID_Korisnika=?"));
		dp::Params parDSdel(pDelStat->allocParams());
		td::INT4 predmet, id;
		parDSdel << predmet << id;

		predmet = _SubjectID;
		id = row[0].i4Val();

		if (!pDelStat->execute())
			return false;


		tr.commit();



		reloadTable();
	}

	return true;
}








void ViewFinalGrade::reloadTable() {

	
	td::String strSet("SELECT DISTINCT Korisnici.ID AS ID, Korisnici.Ime AS Name, Korisnici.Prezime As Surname, Korisnici.Indeks AS Indeks,Korisnici.Adresa as Ocjena "
		" FROM Korisnici, PolazniciAktivnosti WHERE PolazniciAktivnosti.ID_Korisnika = Korisnici.ID AND "
		" PolazniciAktivnosti.ID_Aktivnosti IN(SELECT Aktivnosti.ID_Aktivnosti FROM Aktivnosti WHERE Aktivnosti.ID_Predmeta = ");
	strSet.append(std::to_string(_SubjectID));
	strSet.append(");");


	auto pDStemp = _db->createDataSet(strSet);


	dp::DSColumns cols(pDStemp->allocBindColumns(5));
	cols << "ID" << td::int4 << "Name" << td::string8 << "Surname" << td::string8 << "Indeks" << td::string8 << "Ocjena" << td::string8;


	if (!pDStemp->execute())
	{
		showAlert("", "populateData greska");
		pDStemp = nullptr;
		return;
	}

	moguciBodovi = 0;

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



	td::INT4 vel = pDStemp->getNumberOfRows();

	for (int i = 0;i < vel;i++)
	{
		auto& row = pDStemp->getRow(i);

		row[4] = "";

		pDStemp->updateRow(i);
	}




	auto pompDS = _db->createDataSet("SELECT OcjeneFinal.ID_Korisnika AS UserID,OcjeneFinal.Ocjena AS ocjena FROM OcjeneFinal WHERE OcjeneFinal.ID_Predmeta=?");


	dp::Params parDS2(pompDS->allocParams());

	parDS2 << _SubjectID;
	dp::DSColumns cols2(pompDS->allocBindColumns(2));
	cols2 << "UserID" << td::int4 << "ocjena" << td::string8;


	if (!pompDS->execute())
	{
		showAlert("", "populateData greska");
		pompDS = nullptr;
	}


	size_t vel1 = pompDS->getNumberOfRows();


	for (int i = 0;i < vel1;i++)
	{
		auto& row1 = pompDS->getRow(i);
		for (int j = 0;j < vel;j++)
		{
			auto& row = pDStemp->getRow(j);
			if (row[0] == row1[0])

				row[4] = row1[1];

			pDStemp->updateRow(j);
		}
	}




	_table.clean();

	for (int i = 0;i < vel;i++)
	{
		_table.beginUpdate();
		auto& row = _table.getEmptyRow();
		auto& rowpushB=pDStemp->getRow(i);

		row = rowpushB;

		_table.push_back();

		_table.endUpdate();

	}

	if (vel > 0)
		_table.selectRow(0);

	onChangedSelection(&_table);


}
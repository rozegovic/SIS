#pragma once
#include "ViewGradeExams.h"
#include <td/Types.h>


ViewGradeExams::ViewGradeExams(td::INT4 SubjectID) : _db(dp::getMainDatabase())
, _lblName(tr("nameUser:"))
, _lblLName(tr("lastName:"))
, _lblIndex(tr("index:"))
, _lblGrade(tr("grade:")) // napomena za Eminu - poslije dodati send message
, _lblActivityName(tr("activityName:"))
, _lblCName(tr("courseName:"))
, _btnAdd(tr("add"))
, _btnSave(tr("save"))
, _btnDelete(tr("Delete"))
, _btnUpdate(tr("Update"))
, _hlBtns(5)
, _gl(6, 4) // pazi na brojeve----neka budu tri reda ovih labela (naziv aktivnosti i naziv predmeta, ime i prezime, indeks i ocjena)
, _SubjectID(SubjectID)
{

	_hlBtns.appendSpacer();
	_hlBtns.append(_btnDelete);
	// _hlBtnsDB.append(_btnUpdate); 
	_hlBtns.append(_btnSave);
	_hlBtns.append(_btnAdd);
	_hlBtns.append(_btnUpdate);
	_hlBtns.appendSpacer();


	//  _btnUpdate.setType(gui::Button::Type::Default);
	_btnSave.setType(gui::Button::Type::Default);
	_btnDelete.setType(gui::Button::Type::Destructive);
	_btnAdd.setType(gui::Button::Type::Constructive);
	gui::GridComposer gc(_gl);

	SetCurrentSubject();
	_cName.setAsReadOnly();  // postavlja se u funkciji setcurrentsubject

	gc.appendRow(_lblActivityName);
	gc.appendCol(_activityName);

	gc.appendCol(_lblCName);
	gc.appendCol(_cName);

	gc.appendRow(_lblName);
	gc.appendCol(_name);

	gc.appendCol(_lblLName);
	gc.appendCol(_lName);

	gc.appendRow(_lblIndex);
	gc.appendCol(_index);

	gc.appendCol(_lblGrade);
	gc.appendCol(_grade);


	gc.appendRow(_table, 0);

	gc.appendRow(_hlBtns, 0);
	gui::View::setLayout(&_gl);
	insertValues(_SubjectID);
	populateData();
}

void ViewGradeExams::populateData()
{
	//VAŽNO!!!!
	////// Ne radi SELECT problem je pronadjen al ne znam zasto je problem u dijelu WHERE tacnije a.ID_Korisnika = b.ID_Korisnika kada se izbaci ovaj uslov select radi////

	//auto pDB = dp::getMainDatabase();
	_pDS = _db->createDataSet("SELECT DISTINCT a.ID_Korisnika, a.ID_Aktivnosti, b.Naziv_Aktivnosti, c.Indeks, c.Ime, c.Prezime, d.Ocjena FROM PolazniciAktivnosti a, Aktivnosti b, Korisnici c, OcjeneIspita d WHERE a.ID_Aktivnosti = b.ID_Aktivnosti AND a.ID_Korisnika = c.ID AND b.ID_Predmeta = ? AND b.Tip_Aktivnosti = 1 ORDER BY b.Naziv_Aktivnosti DESC", dp::IDataSet::Execution::EX_MULT);

	dp::Params parDS(_pDS->allocParams());
	//td::INT4 IDPredmeta = Globals::_IDSubjectSelection;

	//u parDS ce se ucitavati Globals::CurrentActivity
	parDS << _SubjectID;
	dp::DSColumns cols(_pDS->allocBindColumns(7));
	cols << "ID_Korisnika" << td::int4 << "ID_Aktivnosti" << td::int4  << "Naziv_Aktivnosti" << td::string8 << "Indeks" << td::string8 << "Ime" << td::string8 << "Prezime" << td::string8 << "Ocjena" << td::string8;

	if (!_pDS->execute())
	{
		_pDS = nullptr;
		return;
	}
	_table.init(_pDS, { 2,3,4,5,6 });
}

bool ViewGradeExams::onChangedSelection(gui::TableEdit* pTE)
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

		val = row[3];
		_grade.setValue(val);

		val = row[4];
		_name.setValue(val);

		val = row[5];
		_lName.setValue(val);

		val = row[6];
		_index.setValue(val);

		val = row[7];
		_activityName.setValue(val);

		val = row[9];
		_cName.setValue(val);

		return true;
	}
	return false;
}

void ViewGradeExams::populateDSRow(dp::IDataSet::Row& row, td::INT4 id)
{
	td::Variant val;
	_grade.getValue(val);
	row[3].setValue(val);

	_name.getValue(val);
	row[4].setValue(val);

	_lName.getValue(val);
	row[5].setValue(val);

	_index.getValue(val);
	row[6].setValue(val);

	_activityName.getValue(val);
	row[7].setValue(val);

	_cName.getValue(val);
	row[8].setValue(val);

	td::Variant x = id;
	row[0].setValue(x);

	val = _UserID;
	row[1].setValue(val);

	val = _ActivityID;
	row[2].setValue(val);

	val = _SubjectID;
	row[9].setValue(val);
	
}

bool ViewGradeExams::canAdd()
{
	// nema provjera za sada
	return true;
}

bool ViewGradeExams::eraseExamGrade()
{
	return false;
}

bool ViewGradeExams::insertExamGrade()
{
	return false;
}

bool ViewGradeExams::updateExamGrade()
{
	return false;
}

bool ViewGradeExams::saveData()
{
	dp::Transaction tran(_db);
	if (!eraseExamGrade())
		return false;

	if (!insertExamGrade())
		return false;

	if (!updateExamGrade())
		return false;

	if (tran.commit())
	{
		_itemsToDelete.clear();
		_itemsToInsert.clear();
		_itemsToUpdate.clear();
	}
	return true;
}

bool ViewGradeExams::onClick(gui::Button* pBtn)
{
	if (pBtn == &_btnDelete)
	{
		int iRow = _table.getFirstSelectedRow();
		if (iRow < 0)
			return true;
		//-------------uzimamo id reda kojeg je potrebno obrisati
		td::INT4 itemid = getIDfromTable(iRow);

		_table.beginUpdate();
		_table.removeRow(iRow);
		_table.endUpdate();
		_itemsToDelete.push_back(itemid);

		//--------------ako brisemo predmet koji se nije sacuvan u bazi, ali se nalazi u vektorima obrisati ga iz njih
		_itemsToInsert.erase(std::remove(_itemsToInsert.begin(), _itemsToInsert.end(), itemid), _itemsToInsert.end());
		_itemsToUpdate.erase(std::remove(_itemsToUpdate.begin(), _itemsToUpdate.end(), itemid), _itemsToUpdate.end());

		return true;
	}
	if (pBtn == &_btnUpdate)
	{
		int iRow = _table.getFirstSelectedRow();
		if (iRow < 0)
			return true;
		td::INT4 itemid = getIDfromTable(iRow);

		_table.beginUpdate();
		auto& row = _table.getCurrentRow();
		populateDSRow(row, itemid);
		_table.updateRow(iRow);
		_table.endUpdate();

		if (std::find(_itemsToInsert.begin(), _itemsToInsert.end(), itemid) == _itemsToInsert.end())
			_itemsToUpdate.push_back(itemid);

		return true;
	}
	if (pBtn == &_btnAdd)
	{
		td::INT4 itemid = findMaxID();
		if (!canAdd())
			return true;
		_table.beginUpdate();
		auto& row = _table.getEmptyRow();
		populateDSRow(row, itemid);
		_table.push_back();
		_table.endUpdate();

		_itemsToUpdate.erase(std::remove(_itemsToUpdate.begin(), _itemsToUpdate.end(), itemid), _itemsToUpdate.end());
		_itemsToInsert.push_back(itemid);
		return true;
	}
	if (pBtn == &_btnSave) {
		saveData();
	}

	return false;
}

void ViewGradeExams::SetCurrentSubject()
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

td::INT4 ViewGradeExams::getIDfromTable(int rowID)
{
	dp::IDataSet* pDS = _table.getDataSet();
	auto& row = pDS->getRow(rowID);
	//---------------------pod pretpostavkom da je ucitano kao 0---provjeriti poslije
	return row[0].i4Val();
}

td::INT4 ViewGradeExams::findMaxID()
{
	dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select ifnull(max(ID), 0) as maxid from OcjeneIspita");
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

void ViewGradeExams::insertValues(td::INT4 subjectID)
{
	//ovo cu dodati ja - Emina
}


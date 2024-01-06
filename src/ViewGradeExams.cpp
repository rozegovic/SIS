#pragma once
#include "ViewGradeExams.h"
#include <td/Types.h>
#include "ViewIDs.h"

ViewGradeExams::ViewGradeExams(td::INT4 SubjectID): _db(dp::getMainDatabase())
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
, _hlBtns(11)
, _gl(6, 4) // pazi na brojeve----neka budu tri reda ovih labela (naziv aktivnosti i naziv predmeta, ime i prezime, indeks i ocjena)
, _SubjectID(SubjectID)
{
	SetCurrentSubject();
	_cName.setAsReadOnly();  // postavlja se u funkciji setcurrentsubject

}

void ViewGradeExams::populateData()
{
	// ucitavaju se ime, prezime, indeks, naziv aktivnosti i ocjena (ocjena ce se na pocetku ucitati kao nista tj. 0 jer jos nije unesena)
	//  za sve studente prijavljene na ZAVRSENE ISPITE (dodati provjeru da li je proslo vrijeme kraja ispita)
	//  ako ti trebaju jos neke kolone mozes dodati 
	// vjv ce trebati id, id_aktivnosti, id_korisnika iz tabele OcjeneIspita ali one se nece ispisivati
}

bool ViewGradeExams::onChangedSelection(gui::TableEdit* pTE)
{
	// nema nikakvih posebnih napomena
	return false;
}

void ViewGradeExams::populateDSRow(dp::IDataSet::Row& row, td::INT4 i)
{
	// nema posebnih napomena
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


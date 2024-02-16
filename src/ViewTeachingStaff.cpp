#pragma once
#include "ViewTeachingStaff.h"
#include "Globals.h"


bool ViewTeachingStaff::insertPositionInDSRow(dp::IDataSet::Row& row)
{
	td::INT4 idk;
	idk = row[0].i4Val();

	dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select a.Naziv from Pozicija a, Korisnici b where b.PozicijaID = a.ID and b.ID = ?");
	dp::Params pParams(pSelect->allocParams());
	pParams << idk;

	dp::Columns pColumns = pSelect->allocBindColumns(1);
	td::String s;
	pColumns << "Naziv" << s;
	if (!pSelect->execute())
		return false;
	if (!pSelect->moveNext())
		return false;

	row[5].setValue(s);
	return true;
}

ViewTeachingStaff::ViewTeachingStaff(td::INT4 SubjectID) :
	_db(dp::getMainDatabase())
	, _lblCode(tr("courseCode:"))
	, _lblCourseID(tr("courseID:"))
	, _courseID(td::int4)
	, _lblCourseName(tr("courseName:"))
	, _lblUserID(tr("userID:"))
	, _userID(td::int4, gui::LineEdit::Messages::Send)
	, _lblUserName(tr("userName:"))
	, _lblUserLN(tr("userLName:"))
	, _btnAdd(tr("add"))
	, _btnSave(tr("save"))
	, _btnReload(tr("Reload"))
	, _btnDelete(tr("Delete"))
	, _btnInsert(tr("Insert"))
	, _btnRemoveAll(tr("deleteAll"))
	, _hlBtns(11)
	, _gl(10, 6)
	, _SubjectID(SubjectID)
{
	SetCurrentSubject();
	_userName.setAsReadOnly();
	_userLName.setAsReadOnly();
	_courseName.setAsReadOnly();
	_courseID.setAsReadOnly();
	_code.setAsReadOnly();

	_hlBtns.appendSpace(3);
	_hlBtns.append(_btnSave, td::HAlignment::Left);
	_hlBtns.appendSpacer();
	_hlBtns.append(_btnReload);
	_hlBtns.append(_btnRemoveAll);
	_hlBtns.append(_btnDelete);
	_hlBtns.append(_btnInsert);
	_hlBtns.append(_btnAdd, td::HAlignment::Right);
	_btnSave.setType(gui::Button::Type::Default);
	_btnDelete.setType(gui::Button::Type::Destructive);
	_btnAdd.setType(gui::Button::Type::Constructive);

	gui::GridComposer gc(_gl);
	gc.appendRow(_lblCourseID, td::HAlignment::Right);
	gc.appendCol(_courseID);
	gc.appendCol(_lblCode, td::HAlignment::Right);
	gc.appendCol(_code);
	gc.appendCol(_lblCourseName, td::HAlignment::Right);
	gc.appendCol(_courseName);
	// dodati razmak radi ljepseg izgleda
	// ovako razmak ne radi??
	//gc.appendSpace(3,1);
	gc.appendRow(_lblUserID, td::HAlignment::Right);
	gc.appendCol(_userID);
	gc.appendCol(_lblUserName, td::HAlignment::Right);
	gc.appendCol(_userName);
	gc.appendCol(_lblUserLN, td::HAlignment::Right);
	gc.appendCol(_userLName);
	//gc.appendSpace(1, 1);
	gc.appendRow(_table, 0);
	gc.appendRow(_hlBtns, 0);
	gui::View::setLayout(&_gl);

	populateData();

}

ViewTeachingStaff::~ViewTeachingStaff()
{
}


void ViewTeachingStaff::initTable(int type)
{
	if (type == 0)
	{
		_table.init(_pDS, { 2,3, 4, 5 });
	}
	else
	{
		gui::Columns visCols(_table.allocBindColumns(4));
		visCols << gui::ThSep::DoNotShowThSep << gui::Header(2, tr("ime"), tr("imeTT"), 100, td::HAlignment::Left)
			<< gui::Header(3, tr("prezime"), tr("prezimeTT"), 100, td::HAlignment::Left)
			<< gui::Header(4, tr("imePredmet"), tr("imePredmetTT"), 200, td::HAlignment::Left)
			<< gui::Header(5, tr("pozicija"), tr("pozicijaTT"), 100, td::HAlignment::Left);

		_table.init(_pDS);
	}
}

void ViewTeachingStaff::populateData()
{
	_pDS = _db->createDataSet("select a.ID_Korisnika, a.ID_Predmeta, b.Ime,  b.Prezime, c.Naziv_Predmeta as 'Naziv predmeta', d.Naziv as Pozicija, c.Sifra_Predmeta, b.PozicijaID from PredmetStaff a, Korisnici b, Predmet c, Pozicija d where a.ID_Korisnika = b.ID and a.ID_Predmeta = c.ID_Predmeta and b.PozicijaID = d.ID and a.ID_Predmeta = ?", dp::IDataSet::Execution::EX_MULT);
	dp::Params params(_pDS->allocParams());
	params << _SubjectID;
	dp::DSColumns cols(_pDS->allocBindColumns(8));
	cols << "ID_Korisnika" << td::int4 << "ID_Predmeta" << td::int4 << "Ime" << td::string8 << "Prezime" << td::string8 << "Naziv predmeta" << td::string8 << "Pozicija" << td::string8 << "Sifra_Predmeta" << td::string8 << "PozicijaID" << td::int4;

	if (!_pDS->execute())
	{
		_pDS = nullptr;
		return;
	}
	initTable(1);
}

bool ViewTeachingStaff::loadComboBox(td::String select, gui::DBComboBox& combo)
{
	//nema potrebe
	return false;
}

bool ViewTeachingStaff::onFinishEdit(gui::LineEdit* pCtrl)
{
	if (pCtrl == &_userID) {
		td::Variant val;
		_userID.getValue(val);
		td::INT4 pom = val.i4Val();

		dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select Ime, Prezime from Korisnici where ID=?");
		dp::Params params(pSelect->allocParams());
		params << pom;

		dp::Columns pColumns = pSelect->allocBindColumns(2);
		td::String s1, s2, s3;
		pColumns << "Ime" << s1 << "Prezime" << s2;
		if (!pSelect->execute()) {
			_userName.toZero();
			_userLName.toZero();
			return false;
		}
		if (!pSelect->moveNext()) {
			_userName.toZero();
			_userLName.toZero();
			return false;
		}

		_userName.setValue(s1);
		_userLName.setValue(s2);
	}

	return false;
}

bool ViewTeachingStaff::onChangedSelection(gui::TableEdit* pTE)
{
	if (pTE == &_table)
	{
		int iRow = _table.getFirstSelectedRow();
		if (iRow < 0)
		{
			_code.toZero();
			_courseID.toZero();
			_userID.toZero();

			return true;
		}
		td::Variant val;
		dp::IDataSet* pDS = _table.getDataSet();
		auto& row = pDS->getRow(iRow);
		val = row[0];
		_userID.setValue(val);
		val = row[1];
		_courseID.setValue(val, false);

		_userName.setValue(row[2], false);
		val = row[3];
		_userLName.setValue(val, false);
		val = row[4];
		_courseName.setValue(val, false);
		val = row[6];
		_code.setValue(val, false);
		insertPositionInDSRow(row);

		return true;
	}
	return false;
}
void ViewTeachingStaff::populateDSRow(dp::IDataSet::Row& row)
{
	td::Variant val;
	_userID.getValue(val);
	row[0].setValue(val);
	_courseID.getValue(val);
	row[1].setValue(val);
	_userName.getValue(val);
	row[2].setValue(val);
	_userLName.getValue(val);
	row[3].setValue(val);
	_courseName.getValue(val);
	row[4].setValue(val);
	_code.getValue(val);
	row[6].setValue(val);

	insertPositionInDSRow(row);
}

bool ViewTeachingStaff::canAdd()
{
	//-------------------mozda provjera da li vec postoji ta osoba postoji na predmetu
	td::Variant id = _userID.getValue().i4Val();
	td::Variant cid = _courseID.getValue().i4Val();
	if (id <= 0 || cid <= 0) {
		showAlert(tr("alert"), tr("alertNEG"));
		return false;
	}

	dp::IDataSet* pDS = _table.getDataSet();
	for (size_t i = 0; i < pDS->getNumberOfRows(); ++i)
	{
		auto& row = pDS->getRow(i);
		if (row[0] == id && row[1] == cid)
		{
			showAlert(tr("alert"), tr("alertTSTF"));
			return false;
		}
	}

	//--------------DODATI U NAJNOVIJI FAJL
	td::Variant val;
	_userID.getValue(val);
	td::INT4 idk;
	val.getValue(idk);
	dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select a.Naziv, b.PozicijaID from Pozicija a, Korisnici b where b.PozicijaID = a.ID and b.ID = ?");
	dp::Params pParams(pSelect->allocParams());
	pParams << idk;

	dp::Columns pColumns = pSelect->allocBindColumns(2);
	td::String s; td::INT4 a;
	pColumns << "Naziv" << s << "PozicijaID" << a;
	if (!pSelect->execute())
		return false;
	if (!pSelect->moveNext())
		return false;
	if (a != 1 && a != 2 && a != 3)
		return false;


	return true;
}

std::pair<td::INT4, td::INT4> ViewTeachingStaff::getIDfromTable(int rowID)
{
	dp::IDataSet* pDS = _table.getDataSet();
	auto& row = pDS->getRow(rowID);
	td::INT4 x, y;
	x = row[0].i4Val();
	y = row[1].i4Val();
	std::pair<td::INT4, td::INT4> a;
	a.first = x;
	a.second = y;
	return a;
}

bool ViewTeachingStaff::existsInDb(td::INT4 id)
{
	//--------------------vjerovatno nepotrebno
	return false;
}

bool ViewTeachingStaff::canDelete(int iRow)
{
	//------------------zasto se ne bi mogao izbrisati?
	return false;
}

bool ViewTeachingStaff::eraseStaff()
{
	dp::IStatementPtr pDeleteNaming(_db->createStatement("delete from PredmetStaff where ID_Korisnika=? and ID_Predmeta=?"));
	dp::Params pParams1(pDeleteNaming->allocParams());
	td::INT4 id1, id2;  // id1 za korisnika, id2 za predmet
	pParams1 << id1 << id2;

	for (auto item : _itemsToDelete)
	{
		id1 = item.first;
		id2 = item.second;
		if (!pDeleteNaming->execute())
		{
			return false;
		}
	}

	return true;
}

bool ViewTeachingStaff::insertStaff()
{

	td::INT4 idUser, idCourse;
	dp::IStatementPtr pInsertCourseP(_db->createStatement("insert into PredmetStaff(ID_Korisnika, ID_Predmeta) VALUES (?,?)"));
	dp::Params pParams2(pInsertCourseP->allocParams());
	pParams2 << idUser << idCourse;

	dp::IDataSet* pDS = _table.getDataSet();
	auto rowCnt = pDS->getNumberOfRows();
	for (size_t iRow = 0; iRow < rowCnt; ++iRow)
	{
		auto& row = pDS->getRow(iRow);
		idUser = row[0].i4Val();
		idCourse = row[1].i4Val();
		std::pair<td::INT4, td::INT4> itemid;
		itemid.first = idUser;
		itemid.second = idCourse;
		if (std::find(_itemsToInsert.begin(), _itemsToInsert.end(), itemid) == _itemsToInsert.end())
			continue;

		if (!pInsertCourseP->execute())
		{
			return false;
		}
	}
	return true;
}



bool ViewTeachingStaff::saveData()
{
	dp::Transaction tran(_db);
	if (!eraseStaff())
		return false;

	if (!insertStaff())
		return false;

	if (tran.commit())
	{
		_itemsToDelete.clear();
		_itemsToInsert.clear();
	}
	return true;
}

bool ViewTeachingStaff::onClick(gui::Button* pBtn)
{
	if (pBtn == &_btnReload)
	{
		//--------------ne funkcionise, ne ucitava
		_table.reload();
		_table.selectRow(0, true);
		_itemsToDelete.clear();
		_itemsToInsert.clear();
		//_itemsToUpdate.clear();
		return true;
	}
	if (pBtn == &_btnDelete)
	{
		int iRow = _table.getFirstSelectedRow();
		if (iRow < 0)
			return true;
		//-------------uzimamo id par reda kojeg je potrebno obrisati
		auto itemid = getIDfromTable(iRow);

		_table.beginUpdate();
		_table.removeRow(iRow);
		_table.endUpdate();
		_itemsToDelete.push_back(itemid);

		_itemsToInsert.erase(std::remove(_itemsToInsert.begin(), _itemsToInsert.end(), itemid), _itemsToInsert.end());

		return true;
	}
	if (pBtn == &_btnInsert)
	{
		int iRow = _table.getFirstSelectedRow();
		if (iRow < 0)
			iRow = 0;
		if (!canAdd())
			return true;
		//-------------uzimamo id par reda kojeg je potrebno obrisati
		auto itemid = getIDfromTable(iRow);

		_table.beginUpdate();
		auto& row = _table.getEmptyRow();
		populateDSRow(row);
		_table.insertRow(iRow);
		_table.endUpdate();

		_itemsToInsert.push_back(itemid);
		return true;
	}
	if (pBtn == &_btnAdd)
	{
		std::pair<td::INT4, td::INT4> itemid;
		itemid.first = _userID.getValue().i4Val();
		itemid.second = _courseID.getValue().i4Val();

		if (!canAdd())
			return true;
		_table.beginUpdate();
		auto& row = _table.getEmptyRow();
		populateDSRow(row);
		_table.push_back();
		_table.endUpdate();

		_itemsToInsert.push_back(itemid);
		return true;
	}
	if (pBtn == &_btnSave)
	{
		//saveData();
		showYesNoQuestionAsync(QuestionID::Save, this, tr("alert"), tr("saveSure"), tr("Yes"), tr("No"));
		return true;
	}
	return false;
}

bool ViewTeachingStaff::onAnswer(td::UINT4 questionID, gui::Alert::Answer answer)
{
	if ((QuestionID)questionID == QuestionID::Save)
	{
		if (answer == gui::Alert::Answer::Yes) {
			saveData();
			showAlert(tr("succes"), tr("succesEE"));
		}
		return true;
	}
	return false;
}

void ViewTeachingStaff::SetCurrentSubject() {
	dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT Naziv_Predmeta, Sifra_Predmeta FROM Predmet WHERE ID_Predmeta = ?");
	dp::Params parDS(pSelect->allocParams());
	parDS << _SubjectID;
	dp::Columns pCols = pSelect->allocBindColumns(2);
	td::String Predmet, Sifra;
	pCols << "Naziv_Predmeta" << Predmet << "Sifra_Predmeta" << Sifra;
	if (!pSelect->execute()) {
		return;
	}
	while (pSelect->moveNext())
	{
		td::Variant val;
		val = Predmet;
		_courseName.setValue(val);
		val = Sifra;
		_code.setValue(val);
		_courseID.setValue(_SubjectID);
	}

}

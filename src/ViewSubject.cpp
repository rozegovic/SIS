#include "ViewSubject.h"
#include "Globals.h"
#include "ViewIDs.h"
#include <td/StringConverter.h>
ViewSubject::ViewSubject(td::INT4 SubjectID) :
	_lblName(tr("Name"))
	, _lblSurname(tr("Surname"))
	, _hlBtnsDB(4)
	, _btnPresent(tr("Present"))
	, _btnNotPresent(tr("NotPresent"))
	, _lblTime(tr("Time"))
	, _time(td::int4)
	, _lblDate(tr("Date"))
	, _date(td::int4)
	, _gl(4, 4)
	, _SubjectID(SubjectID)
{
	_name.setAsReadOnly();
	_surname.setAsReadOnly();

	_hlBtnsDB.appendSpacer();
	_hlBtnsDB.append(_btnNotPresent);
	_hlBtnsDB.append(_btnPresent);
	_hlBtnsDB.appendSpacer();

	_btnPresent.setType(gui::Button::Type::Constructive);
	_btnNotPresent.setType(gui::Button::Type::Destructive);

	gui::GridComposer gc(_gl);
	gc.appendRow(_lblDate);
	gc.appendCol(_date);
	gc.appendCol(_lblTime);
	gc.appendCol(_time);
	
	gc.appendRow(_lblName);
	gc.appendCol(_name);
	gc.appendCol(_lblSurname);
	gc.appendCol(_surname);

	gc.appendRow(_table, 0);
	gc.appendRow(_hlBtnsDB, 0);

	gui::View::setLayout(&_gl);
	populateDateCombo(_date);
	populateData();
	_table.init(_pDS, { 0,1 });
	if (_pDS->getNumberOfRows())
	{
		_table.selectRow(0, true);
	}
}

void ViewSubject::populateDateCombo(gui::DBComboBox& combo) 
{   
	std::vector<td::Date> vekt;
	dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT ID, Datum FROM Termini");
	dp::Columns pCols = pSelect->allocBindColumns(2);
	td::Date date;
	td::INT4 id;
	td::String pom;
	pCols << "ID" << id << "Datum" << date;
	
	
		pSelect->execute();

	s: while (pSelect->moveNext())
	{ pom = date.toString();
	
	for (auto a : vekt)
	{
		if (a == date)
		{
			goto s;
		}
	}
	vekt.push_back(date);
		combo.addItem(pom, id);
	}
	combo.selectIndex(0);
	if (vekt.size() == 0)
		return;
	populateTimeCombo(_time, vekt.at(0));
}
void ViewSubject::populateTimeCombo(gui::DBComboBox& combo, td::Date date)
{
	//td::INT4 indeks=_date.getSelectedIndex();
	dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT ID, Vrijeme FROM Termini where Termini.Datum=?");
	dp::Params pParams(pSelect->allocParams());
	pParams << date;
	dp::Columns pCols = pSelect->allocBindColumns(2);
	td::Time time;
	td::INT4 id;
	td::String pom;
	pCols << "ID" << id << "Vrijeme" << time;
	pSelect->execute();
	
	while (pSelect->moveNext())
	{
		pom = time.toString();
		combo.addItem(pom, id);
	}
	combo.selectIndex(0);
}

void ViewSubject::populateData()
{
	auto pDB = dp::getMainDatabase();
	_pDS = pDB->createDataSet("select Ime as Name, Prezime as Surname, Korisnici.ID as sID from Korisnici, Upis, Predmet where Korisnici.PozicijaID==5 and Upis.ID_Smjera==Predmet.ID_Smjera and Predmet.Semestar==Upis.Semestar and Korisnici.Indeks==Upis.Indeks and Predmet.ID_Predmeta=?", dp::IDataSet::Execution::EX_MULT);
	dp::Params pParams(_pDS->allocParams());
	pParams << _SubjectID;
	dp::DSColumns cols(_pDS->allocBindColumns(3));
	cols << "Name" << td::string8 << "Surname" << td::string8<<"sID" << td::int4;

	if (!_pDS->execute())
	{
		_pDS = nullptr;
		return;
	}
}
void ViewSubject::saveData()
{
	dp::IStatementPtr pInsStat1(dp::getMainDatabase()->createStatement("Select ID from Termini where Termin.Datum = ? and Termin:Time = ?"));
	td::Date date;
	td::String str = _date.getSelectedText();
	date.fromString(str);
	td::Time time;
	td::String str2 = _time.getSelectedText();
	time.fromString(str2);
	dp::Params parDS1(pInsStat1->allocParams());
	parDS1 << date << time;
	
	dp::Columns pCols = pInsStat1->allocBindColumns(1);
	td::INT4 ID;
	pCols << "ID" << ID;
	if (!pInsStat1->execute())
		return;


 
	dp::IStatementPtr pInsStat(dp::getMainDatabase()->createStatement("INSERT INTO Prisustvo (ID_termina,ID_studenta) VALUES(?,?)"));
	dp::Params parDS(pInsStat->allocParams());
	td::INT4 tID, pID;
	parDS <<tID << pID;
	dp::Transaction tr(dp::getMainDatabase());
	td::INT4 curRow = _pDS->getCurrentRowNo();

		auto row = _pDS->getRow(curRow);
		pID = row[2].i4Val();
		tID = ID;
		
		if (!pInsStat->execute())
			return;

	tr.commit();
	return;
}
bool ViewSubject::onChangedSelection(gui::TableEdit* pTE) {

	if (pTE == &_table) {
		int iRow = _table.getFirstSelectedRow();
		if (iRow < 0) {
			return true;
		}
		td::Variant val;
		dp::IDataSet* pDS = _table.getDataSet();
		auto& row = pDS->getRow(iRow);

		val = row[0];
		_name.setValue(val);

		val = row[1];
		_surname.setValue(val);

		return true;
	}
	return false;
}

bool ViewSubject::onClick(gui::Button* pBtn)
{
	if (pBtn == &_btnPresent)
	{
		td::INT4 curRow= _pDS->getCurrentRowNo();
		dp::IDataSet* pDS = _table.getDataSet();
		auto& row = pDS->getRow(curRow);
		td::INT4 curID = row[2].i4Val();
		if (doesIDexist(curID))
		{
			showAlert(tr("alert"), tr("alertPr"));
			if (curRow < _pDS->getNumberOfRows()-1)
				curRow++;

			_table.selectRow(curRow, true);
			return true;
		}
		saveData();
		if(curRow<_pDS->getNumberOfRows()-1)
		curRow++;
		
		_table.selectRow(curRow, true);
		return true;
	}

	if (pBtn == &_btnNotPresent)
	{
		td::INT4 curRow = _pDS->getCurrentRowNo();
		dp::IDataSet* pDS = _table.getDataSet();
		auto& row = pDS->getRow(curRow);
		td::INT4 curID = row[2].i4Val();
		if (doesIDexist(curID))
		{
			showAlert(tr("alert"), tr("alertPr"));
			if (curRow < _pDS->getNumberOfRows() - 1)
				curRow++;

			_table.selectRow(curRow, true);
			return true;
		}
	
		if (curRow < _pDS->getNumberOfRows()-1)
		curRow++;
		_table.selectRow(curRow, true);
		return true;
	}


	return false;
}

bool ViewSubject::doesIDexist(td::INT4 ID)
{
	auto pDB = dp::getMainDatabase();
	_pDSPos = pDB->createDataSet("SELECT ID_studenta  FROM Prisustvo", dp::IDataSet::Execution::EX_MULT);
	dp::DSColumns cols(_pDSPos->allocBindColumns(1));
	cols << "ID_studenta" << td::int4;

	if (!_pDSPos->execute())
	{
		_pDSPos = nullptr;
		return false;
	}
	size_t nRows = _pDSPos->getNumberOfRows();
	for (size_t i = 0; i < nRows; ++i)
	{
		auto row = _pDSPos->getRow(i);
	if (row[0] == ID )
			return true;
	}
	return false;
}

bool ViewSubject::onChangedSelection(gui::DBComboBox* pCB) {
	if (pCB == &_date)
	{
		_time.clean();
		td::String str =_date.getSelectedText();
		td::Date dt;
		dt.fromString(str);
		populateTimeCombo(_time,dt);

	}
	return false;

}
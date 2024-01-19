#include "ViewSubject.h"
#include "Globals.h"
#include "ViewIDs.h"
#include <td/StringConverter.h>
#include <gui/PasswordEdit.h>

ViewSubject::ViewSubject(td::INT4 SubjectID) :
	_lblName(tr("SubjName"))
	, _lblSurname(tr("SubjSurname"))
	, _lblDay(tr("SubjDay"))
	, _lblWeek(tr("SubjWeek"))
	, _hlBtnsDB(4)
	, _lblTablePresent(tr("SubjTablePresent"))
	, _btnPresent(tr("SubjPresent"))
	, _btnNotPresent(tr("SubjNotPresent"))
	, _lblTime(tr("SubjTime"))
	, _time(td::int4)
	, _gl(7, 4)
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
	gc.appendRow(_lblWeek);
	gc.appendCol(_weekCombo);
	
	gc.appendRow(_lblDay);
	gc.appendCol(_dayCombo);
	gc.appendCol(_lblTime);
	gc.appendCol(_time);
	
	gc.appendRow(_lblName);
	gc.appendCol(_name);
	gc.appendCol(_lblSurname);
	gc.appendCol(_surname);

	gc.appendRow(_table, 0);
	gc.appendRow(_lblTablePresent);
	gc.appendRow(_tablePresent, 0);
	gc.appendRow(_hlBtnsDB, 0);

	gui::View::setLayout(&_gl);
	
		
	_TerminID = getCurrentTerminID();
     populateData();
	_table.init(_pDS, { 0,1});
	if (_pDS->getNumberOfRows())
	{
		_table.selectRow(0, true);
	}
populateTablePresent();
	populateDayCombo(_dayCombo);
	populateWeekCombo(_weekCombo);
}
void ViewSubject::populateWeekCombo(gui::ComboBox& combo)
{
	combo.addItem(tr("Sedmica 1"));
	combo.addItem(tr("Sedmica 2"));
	combo.addItem(tr("Sedmica 3"));
	combo.addItem(tr("Sedmica 4"));
	combo.addItem(tr("Sedmica 5"));
	combo.addItem(tr("Sedmica 6"));
	combo.addItem(tr("Sedmica 7"));
	combo.addItem(tr("Sedmica 8"));
	combo.addItem(tr("Sedmica 9"));
	combo.addItem(tr("Sedmica 10"));
	combo.addItem(tr("Sedmica 11"));
	combo.addItem(tr("Sedmica 12"));
	combo.addItem(tr("Sedmica 13"));
	combo.addItem(tr("Sedmica 14"));
	combo.addItem(tr("Sedmica 15"));
	
	combo.selectIndex(getMaxWeek());

}
void ViewSubject::populateDayCombo(gui::ComboBox& combo)
{
	std::vector<td::String> vekt;
	dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT  Dan FROM Termini where Predmet_ID=?");
	dp::Params pParams(pSelect->allocParams());
	pParams << _SubjectID;
	dp::Columns pCols = pSelect->allocBindColumns(1);
	td::String day;
	pCols << "Dan" << day;

	pSelect->execute();

s: while (pSelect->moveNext())
{
	for (auto a : vekt)
	{
		if (a == day)
		{
			goto s;
		}
	}
	vekt.push_back(day);
	combo.addItem(day);
}

if (vekt.size() == 0)
return;
combo.selectIndex(0);
populateTimeCombo(_time, vekt.at(0));
}

td::INT4 ViewSubject::getMaxWeek()
{
	std::vector<td::INT4> vekt;
	td::INT4 IDterm = getCurrentTerminID();
	dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT Br_sedmice FROM Prisustvo where ID_termina=?");
	dp::Params pParams(pSelect->allocParams());
	pParams << IDterm;
	dp::Columns pCols = pSelect->allocBindColumns(1);
	td::INT4 week;

	pCols << "Br_sedmice" << week;
	pSelect->execute();

s: while (pSelect->moveNext())
{
	for (auto a : vekt)
	{
		if (a == week)
		{
			goto s;
		}
	}
	vekt.push_back(week);
}
td::INT4 max = 0;
for (int i = 0; i < vekt.size(); i++)
{
	if (vekt[i] > max)
		max = vekt[i];
}
if (max == 15)
     max--;
return max;
}

void ViewSubject::populateTimeCombo(gui::DBComboBox& combo, td::String day)
{
	if (_dayCombo.getSelectedIndex()<0)
	{
		return;
     }
	dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT ID, Vrijeme FROM Termini where Termini.Dan=?");
	dp::Params pParams(pSelect->allocParams());
	pParams << dp::toNCh(day,30);
	dp::Columns pCols = pSelect->allocBindColumns(2);
	td::Time time;
	time.formatFromString("TimeOwnShortHMM");
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
	_pDS = pDB->createDataSet("select Ime as Name, Prezime as Surname, Korisnici.ID as sID from Korisnici, UpisPredmeta where Korisnici.PozicijaID==5 and Korisnici.ID=UpisPredmeta.Id_Studenta and UpisPredmeta.ID_Predmeta=?", dp::IDataSet::Execution::EX_MULT);
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


void ViewSubject::populateTablePresent() 
{   

	auto pDB = dp::getMainDatabase();
	_pDS2 = pDB->createDataSet("select Ime as Name, Prezime as Surname, Br_sedmice as Week from Korisnici,Prisustvo where Korisnici.PozicijaID==5 and Korisnici.ID == Prisustvo.ID_studenta and Prisustvo.ID_termina=?");
	dp::Params pParams(_pDS2->allocParams());
	 _TerminID = getCurrentTerminID();
	pParams << _TerminID;
	dp::DSColumns cols(_pDS2->allocBindColumns(3));
	cols << "Name" << td::string8 << "Surname" << td::string8<<"Week" << td::int4;

	if (!_pDS2->execute())
	{
		_pDS2 = nullptr;
		return;
	}
	_tablePresent.init(_pDS2, {0,1});

	if (_pDS2->getNumberOfRows())
	{
		_tablePresent.selectRow(0, true);
	}
	
}

bool ViewSubject::saveData()
{   
	td::INT4 _TerminID = getCurrentTerminID();
 
	dp::IStatementPtr pInsStat(dp::getMainDatabase()->createStatement("INSERT INTO Prisustvo (ID_termina,ID_studenta,Br_sedmice) VALUES(?,?,?)"));
	dp::Params parDS(pInsStat->allocParams());
	td::INT4 tID, pID;
	td::INT4 week=getCurrentWeekNum();
	parDS <<tID << pID << week;
	tID = _TerminID;
	td::INT4 curRow = _pDS->getCurrentRowNo();
		auto row = _pDS->getRow(curRow);
		pID = row[2].i4Val();
		
		if (!pInsStat->execute())
			return false;

	return true; 

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
		
		if (_pDS->getNumberOfRows() == 0)
			return false;
		if (_TerminID < 1)
		{    
			showAlert(tr("alert"), tr("alertNoTerm"));
			return false;
		}
		td::INT4 curRow= _pDS->getCurrentRowNo();
		dp::IDataSet* pDS = _table.getDataSet();
		auto& row = pDS->getRow(curRow);
		td::INT4 curID = row[2].i4Val();
		_TerminID = getCurrentTerminID();
		if (doesIDexist(curID))
		{
			showAlert(tr("alert"), tr("alertPr"));
			if (curRow < _pDS->getNumberOfRows() - 1)
				curRow++;

			_table.selectRow(curRow, true);
			return true;
		}
		saveData();
		UpdatePresentDataSet();
		if(curRow<_pDS->getNumberOfRows()-1)
		curRow++;
		_table.selectRow(curRow, true);
		return true;
	}

	if (pBtn == &_btnNotPresent)
	{
		if (_pDS->getNumberOfRows() == 0)
			return false;
		if (_TerminID < 1)
		{   
			showAlert(tr("alert"), tr("alertCmb"));
			return false;
		}
		td::INT4 curRow = _pDS->getCurrentRowNo();
		dp::IDataSet* pDS = _table.getDataSet();
		auto& row = pDS->getRow(curRow);
		td::INT4 curID = row[2].i4Val();
		_TerminID = getCurrentTerminID();
		td::INT4 week=getCurrentWeekNum();
		if (doesIDexist(curID))
		{
			dp::Transaction tr(dp::getMainDatabase());
			dp::IStatementPtr pInsStat(dp::getMainDatabase()->createStatement("Delete from prisustvo where ID_termina=? and ID_studenta=? and Br_sedmice=? "));
			dp::Params parDS(pInsStat->allocParams());
			parDS<<_TerminID<<curID<<week;
			if (!pInsStat->execute())
				return false;
			while (pInsStat->moveNext());
			tr.commit();
		}
		UpdatePresentDataSet();
		if (curRow < _pDS->getNumberOfRows()-1)
		curRow++;
		_table.selectRow(curRow, true);
		return true;
	}


	return false;
}

bool ViewSubject::doesIDexist(td::INT4 ID)
{
	_TerminID = getCurrentTerminID();
	auto pDB = dp::getMainDatabase();
	_pDSPos = pDB->createDataSet("SELECT ID_studenta, ID_termina, Br_sedmice  FROM Prisustvo", dp::IDataSet::Execution::EX_MULT);
	dp::DSColumns cols(_pDSPos->allocBindColumns(3));
	cols << "ID_studenta" << td::int4<<"ID_termina" << td::int4<<"Br_sedmice"<<td::int4;

	if (!_pDSPos->execute())
	{
		_pDSPos = nullptr;
		return false;
	}
	td::INT4 week=getCurrentWeekNum();
	size_t nRows = _pDSPos->getNumberOfRows();
	for (size_t i = 0; i < nRows; ++i)
	{
		auto row = _pDSPos->getRow(i);
	if (row[0] == ID && row[1]==_TerminID && row[2] == week)
			return true;
	}
	return false;
}

bool ViewSubject::onChangedSelection(gui::ComboBox* pCB) {
	if (pCB == &_dayCombo)
	{ 
		_time.clean();
		td::String str =_dayCombo.getSelectedText();
		populateTimeCombo(_time,str);
		if (_pDS->getNumberOfRows() > 0)
			_table.selectRow(0, true);
		return true;
	}
	if (pCB == &_weekCombo)
	{
		UpdatePresentDataSet();
		if (_pDS->getNumberOfRows() > 0)
			_table.selectRow(0, true);
		return true;
	}
	return false;

}
bool ViewSubject::onChangedSelection(gui::DBComboBox* pCB) {
	
	if (pCB == &_time)
	{
		UpdatePresentDataSet();
		//_tablePresent.clean();
		if ( _pDS->getNumberOfRows()>0)
		_table.selectRow(0, true);
	return true;
	}
	return false;

}

td::INT4 ViewSubject::getCurrentTerminID()
{
	dp::Transaction tr(dp::getMainDatabase());
	dp::IStatementPtr pInsStat1(dp::getMainDatabase()->createStatement("Select ID from Termini where Termini.Dan = ? and Termini.Vrijeme = ?"));
	td::String day = _dayCombo.getSelectedText();
	td::Time time;
	td::String str2 = _time.getSelectedText();
	time.fromString(str2);
	dp::Params parDS1(pInsStat1->allocParams());
	parDS1 << dp::toNCh(day,30) << time;

	dp::Columns pCols = pInsStat1->allocBindColumns(1);
	td::INT4 ID_term;
	pCols << "ID" << ID_term;
	if (!pInsStat1->execute())
		return false;
	while (pInsStat1->moveNext());
	tr.commit();
	return ID_term;

}
td::INT4 ViewSubject::getCurrentWeekNum()
{
	td::String str = _weekCombo.getSelectedText();
	td::String pom = "Sedmica 1"; if (str == pom) { return 1; }
	pom = "Sedmica 2"; if (str == pom){return 2;}
	pom = "Sedmica 3"; if (str == pom) { return 3; }
	pom = "Sedmica 4"; if (str == pom) { return 4; }
	pom = "Sedmica 5"; if (str == pom) { return 5; }
	pom = "Sedmica 6"; if (str == pom) { return 6; }
	pom = "Sedmica 7"; if (str == pom) { return 7; }
	pom = "Sedmica 8"; if (str == pom) { return 8; }
	pom = "Sedmica 9"; if (str == pom) { return 9; }
	pom = "Sedmica 10"; if (str == pom) { return 10; }
	pom = "Sedmica 11"; if (str == pom) { return 11; }
	pom = "Sedmica 12"; if (str == pom) { return 12; }
	pom = "Sedmica 13"; if (str == pom) { return 13; }
	pom = "Sedmica 14"; if (str == pom) { return 14; }
	pom = "Sedmica 15"; if (str == pom) { return 15; }
	return -1;
}

void ViewSubject::UpdatePresentDataSet() {

	
	dp::IDataSetPtr pomDS = dp::getMainDatabase()->createDataSet("select Ime as Name, Prezime as Surname, Br_sedmice as Week from Korisnici,Prisustvo where Korisnici.PozicijaID==5 and Korisnici.ID == Prisustvo.ID_studenta and Prisustvo.ID_termina=? and Prisustvo.Br_sedmice=?", dp::IDataSet::Execution::EX_MULT);
	dp::Params pParams(pomDS->allocParams());
	_TerminID = getCurrentTerminID();
	td::INT4 week = getCurrentWeekNum();
	pParams << _TerminID << week;
	dp::DSColumns cols(pomDS->allocBindColumns(3));
	cols << "Name" << td::string8 << "Surname" << td::string8<<"Week"<<td::int4;
	if (!pomDS->execute())
	{
		pomDS = nullptr;
		return;
	}


	_tablePresent.clean();


	size_t nRows = pomDS->getNumberOfRows();
	for (size_t i = 0; i < nRows; i++) {
		_tablePresent.beginUpdate();
		auto& rowpom = pomDS->getRow(i);
		auto& row = _tablePresent.getEmptyRow();
		row = rowpom;

		_tablePresent.push_back();

		_tablePresent.endUpdate();
	}

}


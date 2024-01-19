#pragma once
#include <algorithm>
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/LineEdit.h>
#include <gui/TextEdit.h>
#include <gui/VerticalLayout.h>
#include <gui/HorizontalLayout.h>
#include <gui/GridLayout.h>
#include <gui/Button.h>
#include <gui/NumericEdit.h>
#include <gui/ComboBox.h>
#include <gui/DBComboBox.h>
#include <gui/CheckBox.h>
#include <gui/Slider.h>
#include <gui/ProgressIndicator.h>
#include <gui/GridComposer.h>
#include <gui/DateEdit.h>
#include <gui/TimeEdit.h>
#include <gui/ColorPicker.h>
#include <gui/TableEdit.h>
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>
#include <dp/IDataSetDelegate.h>
#include <fo/FileOperations.h>
#include <gui/ImageView.h>
#include <rnd/MinMax.h>
#include "ViewIDs.h"

class ViewCertainRequest : public gui::View {
private:

protected:

	gui::Label _lblstudentsTicket;
	gui::Label _namelbl;
	gui::LineEdit _name;
	gui::Label _surnamelbl;
	gui::LineEdit _surname;
	gui::Label _indekslbl;
	gui::LineEdit _indeks;
	gui::Label _titleOfTicketlbl;
	gui::LineEdit _titleOfTicket;
	gui::Label _typeOfTicketlbl;
	gui::LineEdit _typeOfTicket;
	gui::Label _statuslbl;
	gui::LineEdit _status;
	gui::Label _answerlbl;
	gui::TextEdit _bodyOfTicket;
	gui::TextEdit _answer;

	//Bottom
	gui::HorizontalLayout _hlBtnsDB;
	gui::Button _btnSend;
	//gui::Button _btnOpenAttachment;
	gui::Button _btnSaveAttachment;
	gui::GridLayout _gl;
	dp::IDatabase* _db;
	dp::IDataSetPtr _pDS;
	td::String indeks;
public:


	ViewCertainRequest(td::String ime, td::String prezime, td::String indeks, td::String tipKarte, td::String statusKarte, td::String sadrzajKarte, td::String naslovKarte);


protected:

	//void populateData();
	/*void initTableStudent();
	void populateDataForRequest();
	void initTableRequest();*/
	void showSaveFileDialog();
	//void saveFile(gui::FileDialog* pFD);
	bool onClick(gui::Button* pBtn);
};
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
#include "ViewUsers.h"
#include "Globals.h"


class ViewTicketForSAO : public gui::View
{
private:

protected:


	gui::Label _onHoldTicketslbl;
	gui::Label _answeredTicketslbl;
	gui::TableEdit _answeredTickets;
	gui::TableEdit _onHoldTickets;


	gui::HorizontalLayout _hlBtnsDB;
	gui::Button _btnOpen;

	gui::GridLayout _gl;
	dp::IDatabase* _db;
	dp::IDataSetPtr _pDS_hold;
	dp::IDataSetPtr _pDS_answered;
	td::String indeks;
	td::String ime;
	td::String prezime;
	td::String tipKarte;
	td::String status;
	td::String request;
	td::String title;


public:
	ViewTicketForSAO();

protected:

	void populateOnHoldTickets();
	void populateAnsweredTickets();
	void initOnHoldTable();
	void initAnsweredTable();
	bool onClick(gui::Button* pBtn);
	bool onChangedSelection(gui::TableEdit* pTE);
	/*td::String getIndex();
	td::String getName();
	td::String getSurname();
	td::String getTypeOfTicket();

	td::String getRequest();
	td::String getTitle();*/
	//td::String getStatus(td::INT4& val);

};
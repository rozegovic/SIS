#pragma once
#include "ViewExamAttendance.h"
#include "ReportData.h"
#include <rpt/IViewer.h>
#include <dp/IDataSetDelegate.h>
#include <gui/ContextMenu.h>
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>
#include <mu/IAppSettings.h>
#include <gui/Image.h>
#include <gui/Frame.h>


void examAttendance(const gui::Image* pImage, td::INT4 SubjectID)
{
	dp::IDatabase* pDB = dp::getMainDatabase();

	//#ifdef REPTEST
	DocumentData docData(SubjectID);
	CompanyData companyData;
	ObjectData objectData;

	rpt::ReportData repData(1, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());

	rpt::ReportData::iterator it(repData.begin());
	//#endif
		//Body 
	{
		dp::IDataSet* pDP(pDB->createDataSet("select a.ID_Korisnika, a.ID_Aktivnosti, b.Naziv_Aktivnosti, c.Indeks, c.Ime, c.Prezime from PolazniciAktivnosti a, Aktivnosti b, Korisnici c where a.ID_Aktivnosti = b.ID_Aktivnosti and a.ID_Korisnika = c.ID and b.ID_Predmeta = ? and b.Tip_Aktivnosti=1 Order by b.Naziv_Aktivnosti DESC"));

		//#ifdef REPTEST
		it << rpt::ContainerType::CNT_Body << pDP; //define field and its data
		//#endif
		dp::DSColumns cols(pDP->allocBindColumns(6));
		cols << "ID_Korisnika" << td::int4 << "ID_Aktivnosti" << td::int4 << "Naziv_Aktivnosti" << td::string8 << "Indeks" << td::string8 << "Ime" << td::string8 << "Prezime" << td::string8;
		dp::Params pars = pDP->allocParams();
		pars << SubjectID;


		if (!pDP->execute())//sto ovdje nece ne znam...
		{
			return;
		}


		size_t nRows = pDP->getNumberOfRows();
		if (nRows == 0)
		{
			//-----------------alert da nema ispita na predmetu ili nema prijavljenih studenata
			//showAlert(tr("AccessNotAllowed"), "");

			return;
		}
	}

	//InvoiceSimple
	td::String configName("ExamAttendanceRep");

	rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
	if (pRepView)
	{
		pRepView->show(gui::tr("ExamAttendanceRep"), pImage);

	}

}
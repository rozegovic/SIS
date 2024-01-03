#include "ViewExamAttendance.h"
#include "ReportData.h"
#include <rpt/IViewer.h>
#include <dp/IDataSetDelegate.h>
#include <gui/ContextMenu.h>

//-----------------------promijeniti funkcije---------------testni koncept

void populateCustomerData(dp::IDatabase* pDB, rpt::ReportData::iterator& it, td::INT4 customerID)
{
	//	dp::IDataSet* pDP(pDB->createDataSet(dp::IDataSet::Size::SingleRow, "RepCustomer"));
		//
	dp::IDataSet* pDP(dp::createConnectionlessDataSet(dp::IDataSet::Size::SingleRow));

	dp::DSColumns cols(pDP->allocBindColumns(9));
	cols << "CustomerID" << td::int4 << "GID" << td::string8 << "TaxID" << td::string8 << "Name" << td::string8 << "Subname" << td::string8 << "Street" << td::string8 << "Post" << td::string8
		<< "Town" << td::string8 << "Country" << td::string8;

	if (!pDP->execute())
	{
		assert(false);
		return;
	}

	it << rpt::ContainerType::CNT_DocHeader << rpt::ContainerType::CNT_Group << "Customer" << pDP;

	dp::IDataSet::Row row(pDP->getRow(0));
	row[0] = customerID; //CustomerID
	row[1] = "4062592827172"; //GID (13 digits)
	row[2] = "206154431123"; //TaxID (12 digits)	
	row[3] = "Siemens AG"; //Name
	row[4] = "Smart Grid Division"; //Subname
	row[5] = "Humboldtstr. 59"; //Street
	row[6] = "90459"; //Post
	row[7] = "Nurnberg"; //Town
	row[8] = "Germany"; //Country

	//    pDP->push_back();
}

void populateObjectData(dp::IDatabase* pDB, rpt::ReportData::iterator& it)
{
	//	dp::IDataSet* pDP(pDB->createDataSet(dp::IDataSet::Size::SingleRow, "RepInvoiceData"));
	dp::IDataSet* pDP(dp::createConnectionlessDataSet(dp::IDataSet::Size::SingleRow));

	dp::DSColumns cols(pDP->allocBindColumns(5));
	cols << "PaymentType" << td::string8 << "ProInvoiceID" << td::string8 << "ProInvoiceDate" << td::date << "AgreementID" << td::string8 << "AgreementDate" << td::date;

	if (!pDP->execute())
	{
		assert(false);
		return;
	}

	it << rpt::ContainerType::CNT_DocHeader << rpt::ContainerType::CNT_Group << "InvoiceData" << pDP;

	dp::IDataSet::Row row(pDP->getRow(0));
	td::Date proInvoiceDate(2015, 7, 22);
	td::Date agreementDate(2015, 8, 22);
	row[0] = "Bank transfer"; //PaymentType
	row[1] = "M1P102/15"; //ProInvoiceID
	row[2] = proInvoiceDate; //ProInvoiceDate
	row[3] = "M1A409/15"; //AgreementID
	row[4] = agreementDate; //AgreementDate	

	//    pDP->push_back();
}

void populateFromToCardData(dp::IDatabase* pDB, rpt::ReportData::iterator& it, td::INT4 fromID, td::INT4 toID)
{
	//		dp::IDataSet* pDP(pDB->createSingleRowDataSet("/")); //sta ovdje??
	dp::IDataSet* pDP = dp::createConnectionlessDataSet(dp::IDataSet::Size::SingleRow);

	dp::DSColumns cols(pDP->allocBindColumns(2));
	cols << "FromID" << td::int4 << "ToID" << td::int4;

	if (!pDP->execute())
	{
		assert(false);
		return;
	}

	it << rpt::ContainerType::CNT_DocHeader << pDP;

	dp::IDataSet::Row row(pDP->getRow(0));
	row[0] = fromID; //FromID
	row[1] = toID; //ToID

	//    pDP->push_back();

}

//#endif

void examAttendance(const gui::Image* pImage, td::INT4 SubjectID)
{
	dp::IDatabase* pDB = dp::getMainDatabase();

	//#ifdef REPTEST
	DocumentData docData;
	CompanyData companyData;
	ObjectData objectData;

	rpt::ReportData repData(3, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());

	rpt::ReportData::iterator it(repData.begin());
	////it.setLayoutNo(0); no need to call this one

	//Customer DataGroup
	populateCustomerData(pDB, it, 100);

	++it;
	//Object DataGroup
	populateObjectData(pDB, it);

	++it;
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
			//showAlert(tr("alert"), tr("Nisu pronadeni podaci o ispitu ili prijavljenim studentima."));

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
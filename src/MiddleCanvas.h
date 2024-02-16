#pragma once
#include <gui/View.h>
#include <gui/Image.h>
#include <gui/ImageView.h>
#include <gui/SplitterLayout.h>
#include <td/Variant.h>
#include<td/String.h>
#include <algorithm>
#include <gui/DrawableButton.h>


class MiddleCanvas : public gui::Canvas
{
private:
protected:
    gui::Image _etf;
    gui::Shape _shapeCircle1;
    gui::Circle c;
    gui::Rect r1;
    gui::Rect r2;
    gui::Rect r3;
    gui::Shape _roundedRect1;
    gui::Shape _roundedRect2;
    gui::Shape _roundedRect3;
    gui::Shape _shapeCircle;
    gui::Circle cc;
    int i = 0;
    td::INT4 brojPoruke=-1;


public:
    MiddleCanvas()
        : _etf(":ETF")
        , gui::Canvas({ gui::InputDevice::Event::PrimaryClicks })
    {


    }

    void onDraw(const gui::Rect& rect) override {
        const bool check = false;
        // pogled za profesora i asistenta ------ grupa 3

           if (Globals::_currentUserID == 1 || Globals::_currentUserID == 3) {

               gui::Size sz;
               getSize(sz);


               gui::Point cp1(sz.width / 2, sz.height / 2);
               td::INT4 x1 = cp1.x;
               td::INT4 y1 = cp1.y;

               gui::Rect imgRect(x1 - 15 - x1 / 4, y1 - 15 - y1 / 4, x1 + 15 + x1 / 4, y1 + 15 + y1 / 4);
               _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center); //etf logo


               gui::Point cp(sz.width / 8, sz.height / 8);
               td::INT4 x = cp.x;
               td::INT4 y = cp.y;

               gui::Rect r3(x, 1, 8 * x, y - 1);
               _roundedRect3.createRoundedRect(r3, 20, 1, td::LinePattern::Solid);
               _roundedRect3.drawFill(td::ColorID::SkyBlue); //bar za prikazivanje osobe i sl 

               gui::Rect r2(x, y, 8 * x, 7 * y - 1);
               _roundedRect1.createRoundedRect(r2, 20, 1, td::LinePattern::Solid);
               _roundedRect1.drawFill(td::ColorID::Transparent); //prostor u kojem se nalaze poruke  

               gui::Rect r1(x, 7 * y, 8 * x, 8 * y);
               _roundedRect2.createRoundedRect(r1, 20, 1, td::LinePattern::Solid);
               _roundedRect2.drawFill(td::ColorID::SkyBlue); //prostor u kojem se kucaju poruke

               gui::Circle cc(8 * x - x / 4, 7 * y + y / 2, 20);
               _shapeCircle.createCircle(cc, 15, td::LinePattern::Solid);
               _shapeCircle.drawFill(td::ColorID::MediumBlue); //dugme za slanje poruke 

               //slobodno mijenjati boje po zelji, ja sam odabrao random boje
            
           }

        // pogled za SAO ------ grupa 1
        else if (Globals::isSAO && brojPoruke > -1) {
            
            gui::Size sz;
            getSize(sz);
            gui::Rect rectt(0, 0,sz.width-100,sz.height-50);//

            rectt.translate(50, 25);

            gui::Shape::drawRect(rectt, td::ColorID::Silver, td::ColorID::MidnightBlue, 2, td::LinePattern::Solid);
        
            DrawChatInfoSAO();
            }
            

        
        // pogled za studenta ------ grupa 2 ------ ako ne koristite mozete samo ostaviti da bude nacrtan etf znak
        else if (Globals::_currentUserID == 5) {
            gui::Size sz;
            getSize(sz);
            gui::Point cp(sz.width / 2, sz.height / 2);
            td::INT4 x = cp.x;
            td::INT4 y = cp.y;

            gui::Rect imgRect(x - 15 - x / 4, y - 15 - y / 4, x + 15 + x / 4, y + 15 + y / 4);
            _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center);
        }
        // kada nema ulogovane osobe
       /* else if(Globals::isLogged==check){

        }*/
        //samo srednji canvas crta etf znak
        else {
            gui::Size sz;
            getSize(sz);
            gui::Point cp(sz.width / 2, sz.height / 2);
            td::INT4 x = cp.x;
            td::INT4 y = cp.y;

            gui::Rect imgRect(x - 15 - x / 4, y - 15 - y / 4, x + 15 + x / 4, y + 15 + y / 4);
            _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center);
        }

    };




    void SetMessageNumSAO(td::INT4 br) {
        brojPoruke = br;
        reset();
    };

    void DrawChatInfoSAO() {

        gui::Size sz;
        getSize(sz);

        dp::IDataSet* pDS = dp::getMainDatabase()->createDataSet("SELECT Korisnici.Ime as Name, Korisnici.Prezime as Surname,SAOStudentTicket.Indeks as StudentIndex,"
            " SAOStudentTicket.Ticket_Tip as TypeOfTicket, SAOStudentTicket.Req_Title as TitleofTicket, SAOStudentTicket.Status_ID as Status_ID,SAOTicket_Status.Status as Status,"
            " SAOStudentTicket.Request as Request From Korisnici, SAOStudentTicket, SAOTicket_Status where Korisnici.Indeks=SAOStudentTicket.Indeks AND SAOTicket_Status.ID=SAOStudentTicket.Status_ID");

        dp::DSColumns cols(pDS->allocBindColumns(8));
        cols << "Name" << td::string8 << "Surname" << td::string8 << "StudentIndex" << td::string8 << "TypeOfTicket" << td::string8 << "TitleOfTicket" << td::string8 << "Status_ID" << td::int4 << "Status" << td::string8 << "Request" << td::string8;
        if (!pDS->execute())
        {
            pDS = nullptr;
            showAlert("errorReadingTable", "");
            return;
        }

        //showAlert("", std::to_string(brojPoruke));


        auto row=pDS->getRow(brojPoruke);
 
        td::String ime = row[0].getConstStr();
        ime.append(" ");
        ime.append(row[1].getConstStr());

        td::String req = row[7].getConstStr();
   
        if (req.length() > 100) {

            int brred = req.length() / 100;
            auto it = req.begin();
            td::String pom = "";
        
            for (int i = 0;i < brred;i++)
            {
                    pom .append(req.subStr(i * 100,(i*100)+100));
                pom.append("\n");
            }

            req = pom;

        }

        gui::DrawableString Ime = ime;
        gui::DrawableString Tip = row[3].getConstStr();
        gui::DrawableString Title= row[4].getConstStr();
        gui::DrawableString Status=row[6].getConstStr();
        gui::DrawableString Request = req;


        Ime.draw({ 75,50 }, gui::Font::ID::SystemBold, td::ColorID::Black);//
        Tip.draw({ sz.width-150,50 }, gui::Font::ID::SystemBold, td::ColorID::Black);//
        Title.draw({ 75,75}, gui::Font::ID::SystemBold, td::ColorID::Black);//
        Status.draw({ sz.width-150,75 }, gui::Font::ID::SystemBold, td::ColorID::Black);//
        Request.draw({100,125}, gui::Font::ID::SystemLarger, td::ColorID::Black);//


    };






    void reset() {
        reDraw();
    };

    void Reset(td::INT4 userID) {
        // id korisnika sa kojim je chat aktivan
        reDraw();
    };

    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override {

        reDraw();
        
    }
    bool getModelSize(gui::Size& modelSize) const override
    {
        //dodati da se dinamicki pomjera
        gui::Size sz;
        getSize(sz);
        
        modelSize.width = sz.width; //
        modelSize.height = sz.height; // mozda potrebna popravka
        return true;
    }
};


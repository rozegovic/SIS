#pragma once
#include <gui/View.h>
#include <gui/Image.h>
#include <gui/ImageView.h>
#include <gui/SplitterLayout.h>
#include <td/Variant.h>
#include<td/String.h>
#include <algorithm>
#include <gui/DrawableButton.h>
#include <gui/ViewScroller.h>
#include <td/Types.h>
#include "SendMessage.h"
#include "GlobalsCanvas.h"
#include "ViewCertainRequest.h"
#include "WindowCertainRequest.h"
#include "ViewTicketForSAO.h"
#include "ViewIDs.h"



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
    gui::Rect chat1;
    gui::Shape _chat1;
    gui::Rect chat2;
    gui::Shape _chat2;
    int i = 0;
    td::INT4 brojPoruke = -1;
    td::INT4 _chatUserID;
    td::String _name;
    td::INT4 _h;
    td::INT4 x;
    td::INT4 y;
    std::string str;
    gui::DrawableString dr;
    dp::IDatabase* _db;
    MsgSender msg;
    gui::Image img;
    td::INT4 red=1;
    td::INT4 skrolV = 0;
    td::String indeks;
    td::String Ime;
    td::String prezime;
    td::String tipKarte;
    td::String request;
    td::String title;
    td::String status;
    gui::Rect rectBottomRight;
    gui::Point mousePosition;

public:
    MiddleCanvas()
        : _etf(":ETF")
        , gui::Canvas({ gui::InputDevice::Event::PrimaryClicks,  gui::InputDevice::Event::Keyboard ,gui::InputDevice::Event::CursorMove})
        , _chatUserID(-2)
        , _h(800)
        , _db(dp::getMainDatabase())
        , img(":circ")
        , mousePosition(0, 0)
    {


    }

    void onDraw(const gui::Rect& rect) override {
        const bool check = false;
        // pogled za profesora i asistenta ------ grupa 3

        if ((Globals::_currentUserRole == 1 || Globals::_currentUserRole == 3) && _chatUserID!=-2) {

            gui::Size sz;
            getSize(sz);


            gui::Point cp1(sz.width / 2, sz.height / 2);
            td::INT4 x1 = cp1.x;
            td::INT4 y1 = cp1.y;

            gui::Rect imgRect(x1 - 15 - x1 / 4, y1 - 15 - y1 / 4, x1 + 15 + x1 / 4, y1 + 15 + y1 / 4);
            _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center); //etf logo


            gui::Point cp(sz.width / 8, sz.height / 8);
            x = cp.x;
            y = cp.y;



            gui::Rect r3(0, 1, 8 * x, y - 1);
            _roundedRect3.createRoundedRect(r3, 20, 1, td::LinePattern::Solid);
            _roundedRect3.drawFill(td::ColorID::SkyBlue); //bar za prikazivanje osobe i sl 

            if (Globals::_currentUserID == _chatUserID) {
                gui::DrawableString nesto = "Biljeske";
                nesto.draw(r3, gui::Font::ID::SystemLargestBold, td::ColorID::Black, td::TextAlignment::Center, td::VAlignment::Center);
            }
            else {
                gui::DrawableString nesto = td::String(_name);
                nesto.draw(r3, gui::Font::ID::SystemLargestBold, td::ColorID::Black, td::TextAlignment::Center, td::VAlignment::Center);
            }

            
            gui::Rect r1(0, 7 * y, 8 * x, 8 * y);
            
           /* if (str.size() == 52 * red && red < 6) { 
                str = str + '\n'; 
                red++; 
                gui::Rect r1(x, (7 - red) * y, 8 * x, 8 * y); 
            }*/
            _roundedRect2.createRoundedRect(r1, 20, 1, td::LinePattern::Solid);
            _roundedRect2.drawFill(td::ColorID::SkyBlue); //prostor u kojem se kucaju poruke

            gui::Rect r2(10, 7 * y, 8 * x - (16 + x / 6)*2, 8 * y);
            _roundedRect1.createRoundedRect(r2, 20, 1, td::LinePattern::Solid);
            _roundedRect1.drawFill(td::ColorID::Transparent); //prostor u kojem se nalaze poruke  
            dr = str;
            dr.draw(r2, gui::Font::ID::SystemNormal, td::ColorID::Black, td::TextAlignment::Left, td::VAlignment::Center, td::TextEllipsize::End); //no
            if (str.size() == 0)
            {
                gui::DrawableString mes = "Message...";
                mes.draw(r2, gui::Font::ID::SystemItalic, td::ColorID::Black, td::TextAlignment::Left, td::VAlignment::Center, td::TextEllipsize::End);
            }

            gui::Circle cc(8 * x - x / 4-7, 7 * y + y / 2, 9+x/6);
           // _shapeCircle.createCircle(cc, 15, td::LinePattern::Solid);
           // _shapeCircle.drawFill(td::ColorID::MediumBlue); //dugme za slanje poruke 
            img.draw(cc);

            //slobodno mijenjati boje po zelji, ja sam odabrao random boje 
            showChat();
        }

        // pogled za SAO ------ grupa 1
        else if (Globals::isSAO && brojPoruke > -1) {
            DrawChatInfoSAO();
        }



        // pogled za studenta ------ grupa 2 ------ ako ne koristite mozete samo ostaviti da bude nacrtan etf znak
        else if (Globals::_currentUserRole == 5) { //treba 5 
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

            gui::Rect imgRect(x - 30 - x / 4, y - 30 - y / 4, x + 30 + x / 4, y + 30 + y / 4);
            _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center);
        }

    };




    void SetMessageNumSAO(td::INT4 br) {
        brojPoruke = br;
        reset();
    };

    void DrawChatInfoSAO() {

      enableResizeEvent(true);

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


      auto row = pDS->getRow(brojPoruke);

      td::String ime = row[0].getConstStr();
      ime.append(" ");
      ime.append(row[1].getConstStr());

      td::String req = row[7].getConstStr();



      td::INT4 maxLength = (int(sz.width - 200) / 8);


      if (req.length() > maxLength) {

          td::String pom = "";
          auto itA = req.begin();
          auto itB(itA);
          bool SpaceExists = false;
          auto pomit = itA;

          if (req.length() > 0)
              pom.append(req.subStr(0, 0));


          while (itA != req.end())
          {
              pomit = itA;
              //GET MAX RANGE FOR CHARS IN A ROW
              while (itB != req.end() && itB <= itA + maxLength)
                  itB++;
              //DOES SPACE EXIST IN CURRENT ROW?    
              while (pomit != itB)
              {
                  if (*pomit == ' ')
                  {
                      SpaceExists = true;  break;
                  }
                  pomit++;
              }
              //FIND SUITABLE PLACE TO BREAK REQUEST APART
              while (itB != req.end() && SpaceExists && *(itB) != ' ')
                  itB--;

              if (itB > ++itA)
                  pom.append(req.subStr(itA - req.begin(), itB - req.begin()));

              pom.append("\n");

              //showAlert(std::to_string(itA-req.begin()), std::to_string(itB-req.begin()));

              while (itA < itB)
                  itA++;


          }

          req = pom;

      }

      //TREBA MI ZA GLOBALNE VARIJABLE RADI PROSLIJEDJIVANJA

      //**************//
      Ime = row[0].getConstStr();
      prezime = row[1].getConstStr();
      indeks = row[2].getConstStr();
      tipKarte = row[3].getConstStr();
      request = row[7].getConstStr();
      title = row[4].getConstStr();
      status = row[6].getConstStr();
      //****************//




     // showAlert("",std::to_string(sz.width));


      gui::DrawableString Ime = ime;
      gui::DrawableString Tip = row[3].getConstStr();
      gui::DrawableString Title = row[4].getConstStr();
      gui::DrawableString Status = row[6].getConstStr();
      gui::DrawableString Request = req;

      gui::Size szpom;
      Request.measure(gui::Font::ID::SystemLarger, szpom);

      skrolV = szpom.height + 200;

      gui::Rect rectt(0, 0, sz.width - 100, skrolV);//

      if (skrolV < sz.height)
          rectt.setHeight(sz.height - 50);


      rectt.translate(50, 25);

      // gui::Shape::drawRect(rectt, td::ColorID::Silver, td::ColorID::MidnightBlue, 2, td::LinePattern::Solid);

      gui::Shape visibleRect;

      visibleRect.createRoundedRect(rectt, 10);
      visibleRect.drawFillAndWire(td::ColorID::Silver, td::ColorID::MidnightBlue);


      Ime.draw({ 75,50 }, gui::Font::ID::SystemBold, td::ColorID::Black);//
      Tip.draw({ sz.width - 150,50 }, gui::Font::ID::SystemBold, td::ColorID::Black);//
      Title.draw({ 75,75 }, gui::Font::ID::SystemBold, td::ColorID::Black);//
      Status.draw({ sz.width - 150,75 }, gui::Font::ID::SystemBold, td::ColorID::Black);//
      if (!req.isNull())
          Request.draw({ 100,125 }, gui::Font::ID::SystemLarger, td::ColorID::Black);//

      szpom.height = skrolV;

      this->getScroller()->setContentSize(szpom);

      int bottomRightX = sz.width - 100; // X koordinata
      int bottomRightY = sz.height - 70; // Y koordinata
      int rectWidth = 80;
      int rectHeight = 30;

      szpom.width = rectWidth;
      szpom.height = rectHeight;


      if (skrolV > sz.height)
          bottomRightY = skrolV - 20;

      gui::Point origin(bottomRightX - rectWidth, bottomRightY - rectHeight);


      // Nacrtaj pravokutnik
      gui::DrawableString answer = tr("Odgovori");
      rectBottomRight.setOriginAndSize(origin, szpom);

      // gui::Shape::drawRect(rectBottomRight, td::ColorID::MidnightBlue, td::ColorID::MidnightBlue, 2, td::LinePattern::Solid);

      gui::Shape buttonSAO;
      buttonSAO.createRoundedRect(rectBottomRight, 10);

      gui::Point  p(bottomRightX - 73, bottomRightY - 27);

      int x = mousePosition.x;
      int y = mousePosition.y;

      if (x > rectBottomRight.left && x < rectBottomRight.right && y < rectBottomRight.bottom && y > rectBottomRight.top)
      {
          buttonSAO.drawFillAndWire(td::ColorID::AquaMarine, td::ColorID::MidnightBlue);
            answer.draw(p, gui::Font::ID::SystemBold, td::ColorID::Blue);
      }
      else
      {      
          buttonSAO.drawFillAndWire(td::ColorID::Blue, td::ColorID::MidnightBlue);
           answer.draw(p, gui::Font::ID::SystemBold, td::ColorID::White);
       }


  };

    void reset() {
        _chatUserID = -2;
        reDraw();
    }

    void Reset(td::INT4 userID, td::String s) {
        // id korisnika sa kojim je chat aktivan
        red = 1;
        _name = s;
        _chatUserID = userID;
        reDraw();
    }

    //bool insertMessage() {
    //    td::Date d(true);
    //   // td:time t(true);
    //    dp::IDatabase* _db;
    //    dp::IStatementPtr pInsStat(_db->createStatement("INSERT INTO Messages (AuthorID, Subject, Poruke, Datum, Vrijeme) VALUES (?, 'chat',?, ?, ?)"));
    //    dp::Params parDS(pInsStat->allocParams());
    //    parDS << Globals::_currentUserID << ovde treba uneseni string  <<d<<t;
    //    dp::Transaction tr(_db);
    //    tr.commit();
    //    
    //}


    bool insertMessage() {
        return msg.sendMsgtoUser("chat", str, _chatUserID);
    }


    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override {
        if (Globals::_currentUserRole == 1 || Globals::_currentUserRole == 3) {

            double tempk = 0;
            double x1 = 8 * x - x / 4;
            double y1 = 7 * y + y / 2;
            double yid = inputDevice.getModelPoint().y;
            double xid = inputDevice.getModelPoint().x;
            double xd = pow(xid - x1, 2);
            double yd = pow(yid - y1, 2);
            double d = sqrt(xd + yd);





            // if (tempk < int(inputDevice.getFramePoint().y) && inputDevice.getFramePoint().y < (tempk + _visinaChata)) {
            if (d <= 9 + x / 6) {
                insertMessage();
                str = "";
                //  showChat();
                red = 1;
                reDraw();
                // showAlert("OK","PRITISNUTO DUGME");
            }



            reDraw();

        }
        
      if (Globals::isSAO && inputDevice.getModelPoint().x>rectBottomRight.left && inputDevice.getModelPoint().x < rectBottomRight.right && 
      inputDevice.getModelPoint().y<rectBottomRight.bottom && inputDevice.getModelPoint().y > rectBottomRight.top) {
      gui::Window* pParentWnd = getParentWindow();
      auto pWnd = new WindowCertainRequest(pParentWnd, indeks, Ime, prezime, tipKarte, status, request, title);
      pWnd->keepOnTopOfParent();
      pWnd->open();
      reDraw();
  }
    }

    bool getModelSize(gui::Size& modelSize) const override
    {
        //dodati da se dinamicki pomjera
        gui::Size sz;
        getSize(sz);

        modelSize.width = sz.width; //
        modelSize.height = _h;
        return true;
    }

    void setModelHeight(gui::Size& modelSize, td::INT4 h) {
        modelSize.height = h;
    }

    td::INT4 getChatUserID() {

    }



    void showChat() {

        //  _h = 800;


        gui::Size sz;
        getSize(sz);

        gui::Point cp(sz.width / 8, sz.height / 8);

        td::INT4 x = cp.x;
        td::INT4 y = cp.y;
        ///* if (logyy) {*/
        //     y = cp.y;
        ///*     logyy = false;
        // }*/
        td::INT4 dy = y;




        dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT Messages.Poruke, MsgReceivers.UserID FROM Messages JOIN MsgReceivers ON Messages.ID = MsgReceivers.MsgID WHERE (MsgReceivers.UserID = ? AND Messages.AuthorID=?) OR (MsgReceivers.UserID = ? AND Messages.AuthorID=?) ORDER BY MsgReceivers.MsgID ASC;");
        dp::Columns pCols = pSelect->allocBindColumns(2);
        dp::Params parDS(pSelect->allocParams());
        parDS << _chatUserID << Globals::_currentUserID << Globals::_currentUserID << _chatUserID;

        td::String s;
        td::INT4 user_id;
        pCols << "Poruke" << s << "UserID" << user_id;

        if (!pSelect->execute()) {
            s = "Greska";
            user_id = -1;

        }
        while (pSelect->moveNext()) {

            std::string s1 = s.c_str();
            td::INT4 id = user_id;
            int br = s1.length() / 31 + 1;

            /*  if (s1.length() > 30) {
                  std::string s2="";
                  for (int i = 0; i < s1.length() / 30; i++) {
                      int k = 0;
                      if (k + 30 < s1.length()) {
                          s2 += s1.substr(k, k + 29) + "\n";
                          k += 30;
                      }
                      else s2 += s1.substr(k, s1.length());
                  }

                  s1 = s2;
              }*/
            if (id == _chatUserID && id == Globals::_currentUserID) {
                gui::Rect chat2(8 * x - 250, dy + 10, 8 * x - 10, dy + 10 + 25 * br);
                _chat2.createRoundedRect(chat2, 5, 1, td::LinePattern::Solid);
                _chat2.drawFill(td::ColorID::WhiteSmoke); //user
                gui::DrawableString nesto2 = s;
                nesto2.draw(chat2, gui::Font::ID::SystemNormal, td::ColorID::Black);
                dy += 25 * br + 15;
            }
            if (id == _chatUserID && id != Globals::_currentUserID) {

                // gui::Rect chat2(6 * x + 10, dy*y + 10, 8 * x - 2, (dy+1)* y - 2);
                gui::Rect chat2(8 * x - 250, dy + 10, 8 * x - 10, dy + 10 + 25 * br);
                _chat2.createRoundedRect(chat2, 5, 1, td::LinePattern::Solid);
                _chat2.drawFill(td::ColorID::WhiteSmoke); //user
                gui::DrawableString nesto2 = s;
                nesto2.draw(chat2, gui::Font::ID::SystemNormal, td::ColorID::Black);
                dy += 25 * br + 15;

            }

            if (id == Globals::_currentUserID && id != _chatUserID) {
                //  gui::Rect chat1(0 + 10, dy*y + 10, 2 * x - 2, (dy+1)* y - 2);
                gui::Rect chat1(10, dy + 10, 250, dy + 10 + 25 * br);
                _chat1.createRoundedRect(chat1, 5, 1, td::LinePattern::Solid);
                _chat1.drawFill(td::ColorID::Gainsboro); //onaj sa kim se dopisujemo 
                gui::DrawableString nesto = s;
                nesto.draw(chat1, gui::Font::ID::SystemNormal, td::ColorID::Black);
                dy += 25 * br + 15;

            }

            if (dy > _h) {
                _h += dy - _h + 20;


                /*gui::Size ss;
                ss.width = sz.width;
                ss.height = _h;
                setSize(ss);*/

                //sada bi trebalo pozvati funkciju set model height i postaviti novu visinu na_h
            }

        }
    }


    bool onKeyPressed(const gui::Key& key) override
    {
        if (key.getVirtual() == gui::Key::Virtual::Delete)
        {

            if (!str.empty()) {
                str.erase(str.size() - 1);
                reDraw();
                return true;
            }
        }
        else if (key.getVirtual() == gui::Key::Virtual::BackSpace)
        {
            if (!str.empty()) {
                str.erase(str.size() - 1);
                reDraw();
                return true;
            }
        }
        else if (key.getVirtual() == gui::Key::Virtual::NumEnter)
       // else if (key.getCode() == 13)
        {
            //potrebna implementacija -- koje dugme ulazi u ovaj if
            if (!str.empty()) {
                str.erase(str.size() - 1);
                reDraw();
                return true;
            }
        }
        else if (key.getChar() >= 32 && key.getChar() <= 127) {
            for (char i = 32; i <= 127; i++) {
                if (key.getChar() == i)
                {
                    str = str + i;
                    reDraw();
                    return true;
                }
            }
        }
        // ako se promijeni na td::String pogledati da li rade afrikati
        // u tom slucaju dodati njihovu implementaciju rucno - za svaki afrikat



        return false;

    }
    
        void onCursorMoved(const gui::InputDevice& inputDevice)override {
        
        
        if (Globals::isSAO) {
            gui::Size sz;
            getSize(sz);

            int x = inputDevice.getModelPoint().x;
            int y = inputDevice.getModelPoint().y;

            mousePosition.x = x;
            mousePosition.y = y;

            if (x > rectBottomRight.left && x < rectBottomRight.right && y < rectBottomRight.bottom && y > rectBottomRight.top)
                setCursor(gui::Cursor::Type::Finger);
            else
                setCursor(gui::Cursor::Type::Default);

            reDraw();
        }


    }

};
#pragma once
#include <gui/View.h>
#include <gui/Image.h>
#include <gui/ImageView.h>
#include <gui/ViewScroller.h>
#include <gui/SplitterLayout.h>
#include "GlobalsCanvas.h"


//-------------------Klasa za obavjestenja (sve grupe rade zajedno)
//-------------------dodati provjeru ko je prijavljen u onDraw i sa tim dalje prikazati obavjestenja
//-------------------izbrisati sliku etf (tu samo da se nesto prikazuje)!!!
class Notifications1 : public gui::Canvas
{
private:
protected:
    gui::Image _etf;
    td::UINT2 priorKlikx=0;
    td::INT4 dragBoxNum=-1;

    td::INT4 boxTranslate = 0;
    td::INT4 ForDeletion = -1;
    td::INT4 MessageToExpand = -1;
    td::INT4 AddToScroller = 0;

    friend class Notifications;
public:
    Notifications1()
        : Canvas({ gui::InputDevice::Event::CursorDrag, gui::InputDevice::Event::PrimaryClicks})
        , _etf(":ETF")
    {

    }

    void onDraw(const gui::Rect& rect) override {

        //showAlert("", std::to_string(dragBoxNum));

        if (Globals::_currentUserID == -1)
            return;

        gui::Size sz;
        getSize(sz);

        //problem je sto se ne mogu skrolat poruke vec treba promijeniti size prozora da se sve vidi --> RJESENO
        //dodati provjeru da se ne ispisuje obavijest za ispit ako je rok za prijavu istog istekao


        dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT a.Subject, a.Poruke "
            "  FROM Messages a "
            "  JOIN MsgReceivers mr ON mr.MsgID = a.ID "
            "   WHERE mr.UserID = ? ");

        dp::Params parDS(pSelect->allocParams());
        parDS << Globals::_currentUserID;

        dp::Columns pCols = pSelect->allocBindColumns(2);
        td::String Subject, Poruke;
        pCols << "Subject" << Subject << "Poruke" << Poruke;
        if (!pSelect->execute()) {
            Subject = "Greska";
            Poruke = "Greska";
        }
        gui::Rect imgRect(0, 0, sz.width, 60);
        gui::Point pt(10, 0);
        gui::Point pt2(10, 30);


        gui::Rect rectt(0, 0, sz.width, 60);//

        gui::Shape rrect;//

        GlobalsCanvas::brObavijesti = 0;


        AddToScroller = 0;


        while (pSelect->moveNext())
        {

            if (GlobalsCanvas::brObavijesti == MessageToExpand)
            {

                if (GlobalsCanvas::brObavijesti == dragBoxNum)
                {
                    rectt.translate(boxTranslate, 0);
                    pt.translate(boxTranslate, 0);
                    pt2.translate(boxTranslate, 0);
                }



                td::INT4 maxLength = (int(sz.width - 32) / 8);


                if (Poruke.length() > maxLength) {

                    td::String pom = "";
                    auto itA = Poruke.begin();
                    auto itB(itA);
                    bool SpaceExists = false;
                    auto pomit = itA;

                    if (Poruke.length() > 0)
                        pom.append(Poruke.subStr(0, 0));


                    while (itA != Poruke.end())
                    {
                        pomit = itA;
                        //GET MAX RANGE FOR CHARS IN A ROW
                        while (itB != Poruke.end() && itB <= itA + maxLength)
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
                        while (itB != Poruke.end() && SpaceExists && *(itB) != ' ')
                            itB--;

                        if (itB > ++itA)
                            pom.append(Poruke.subStr(itA - Poruke.begin(), itB - Poruke.begin()));

                        pom.append("\n");

                        //showAlert(std::to_string(itA-req.begin()), std::to_string(itB-req.begin()));

                        while (itA < itB)
                            itA++;


                    }

                    Poruke = pom;

                }

                gui::DrawableString tekst = Subject;
                gui::DrawableString tekst2 = Poruke;

                gui::Size szpom;
                tekst2.measure(gui::Font::ID::SystemNormal, szpom);

                AddToScroller = szpom.height;

                rectt.setHeight(szpom.height + 40);

                rrect.createRoundedRect(rectt, 20);//
                rrect.drawFill(td::ColorID::Silver);//

                /*  tekst.draw(pt, gui::Font::ID::SystemLargerBold, td::ColorID::Navy);
                  tekst2.draw(pt2, gui::Font::ID::SystemNormal, td::ColorID::Navy);*/
                tekst.draw(pt, gui::Font::ID::SystemLargerBold, td::ColorID::Black);//
                tekst2.draw(pt2, gui::Font::ID::SystemNormal, td::ColorID::Black);//
                pt.translate(0,szpom.height + 50);
                pt2.translate(0, szpom.height + 50);
                //imgRect.translate(0, 70);
                rectt.setHeight(60);
                rectt.translate(0,szpom.height+50);//

                if (GlobalsCanvas::brObavijesti == dragBoxNum)
                {
                    rectt.translate(-boxTranslate, 0);
                    pt.translate(-boxTranslate, 0);
                    pt2.translate(-boxTranslate, 0);
                }


            }

            else {

                if (GlobalsCanvas::brObavijesti == dragBoxNum)
                {
                    rectt.translate(boxTranslate, 0);
                    pt.translate(boxTranslate, 0);
                    pt2.translate(boxTranslate, 0);
                }

                //gui::Shape::drawRect(imgRect, td::ColorID::LightGray, td::ColorID::MidnightBlue, 2, td::LinePattern::Solid);

                rrect.createRoundedRect(rectt, 20);//
                rrect.drawFill(td::ColorID::Silver);//

                gui::DrawableString tekst = Subject;
                gui::DrawableString tekst2 = Poruke;
                /*  tekst.draw(pt, gui::Font::ID::SystemLargerBold, td::ColorID::Navy);
                  tekst2.draw(pt2, gui::Font::ID::SystemNormal, td::ColorID::Navy);*/
                tekst.draw(pt, gui::Font::ID::SystemLargerBold, td::ColorID::Black);//
                tekst2.draw(pt2, gui::Font::ID::SystemNormal, td::ColorID::Black);//
                pt.translate(0, 70);
                pt2.translate(0, 70);
                //imgRect.translate(0, 70);
                rectt.translate(0, 70);//

                if (GlobalsCanvas::brObavijesti == dragBoxNum)
                {
                    rectt.translate(-boxTranslate, 0);
                    pt.translate(-boxTranslate, 0);
                    pt2.translate(-boxTranslate, 0);
                }
            }
                GlobalsCanvas::brObavijesti++;
            
        }

        if (GlobalsCanvas::brObavijesti == 0)
        {

            gui::DrawableString str = tr("noNotifs");//

            rrect.createRoundedRect(rectt, 20);//
            rrect.drawFill(td::ColorID::Silver);//

            pt.translate(sz.width / 2 - 75, 20);
            
            str.draw(pt, gui::Font::ID::SystemLargerBold, td::ColorID::Black);//

        }

        gui::Size szpom;
        getSize(szpom);
        szpom.height = (GlobalsCanvas::brObavijesti * 70) + (AddToScroller + 50);

        getScroller()->setContentSize(szpom);

    };
    void reset() {

        td::String str("SELECT a.Subject AS A, a.Poruke AS B "
            "  FROM Messages a "
            "  JOIN MsgReceivers mr ON mr.MsgID = a.ID "
            "   WHERE mr.UserID =  ");
        str.append(std::to_string(Globals::_currentUserID));

        dp::IDataSetPtr pDS = dp::getMainDatabase()->createDataSet(str);


        dp::DSColumns cols(pDS->allocBindColumns(2));
        cols << "A" << td::string8 << "B" << td::string8;

        if (!pDS->execute())
        {
            pDS = nullptr;
            return;
        }

        GlobalsCanvas::brObavijesti = pDS->getNumberOfRows();

        reDraw();

        // this->getScroller()->setContentSize();
    };

    bool getModelSize(gui::Size& modelSize) const override
    {
        modelSize.width = 230;
        modelSize.height = (GlobalsCanvas::brObavijesti * 70)+(AddToScroller);
        // showAlert("", std::to_string(BrObavijesti));
        return true;
    };
    void measure(gui::CellInfo& ci) override
    {
        gui::Size sz;
        getSize(sz);
        ci.minHor = 230;
    }
    void reMeasure(gui::CellInfo& ci) override
    {
        gui::Size sz;
        getSize(sz);
    }

    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override {
        
        gui::Point klik= inputDevice.getModelPoint();

         if (klik.x < 0 || klik.y < 0)
            return;

        td::INT4 razlika = 0;

        if(MessageToExpand!=-1)
        td::INT4 razlika = ((MessageToExpand - 1) * 70) + AddToScroller + 60 - klik.y;

        
        if (razlika<0)
        {
             dragBoxNum = (- razlika / 70) + MessageToExpand;
             return;
        }
        else if (razlika >0 && razlika<AddToScroller+60)
        {
            dragBoxNum = MessageToExpand;
            return;
        }


        dragBoxNum = klik.y / 70;

        //showAlert("klikk", std::to_string(dragBoxNum));

    }


    void onCursorDragged(const gui::InputDevice& inputDevice) override {

       

        td::INT4 klikx = inputDevice.getModelPoint().x;
        
        if (klikx > 0 && priorKlikx != 0)
        {
            boxTranslate = (klikx-priorKlikx);

            setCursor(gui::Cursor::Type::Finger);
        }


        if (priorKlikx == 0) {
            priorKlikx = klikx;
        }
        reDraw();
    }

    void onPrimaryButtonReleased(const gui::InputDevice& inputDevice) override {

        gui::Size sz;

        getSize(sz);

        setCursor(gui::Cursor::Type::Default);

        if (priorKlikx == 0 && MessageToExpand == -1 && boxTranslate==0) {


            MessageToExpand = dragBoxNum;
            reDraw();
            return;
        }

        if (priorKlikx == 0)
        {
            MessageToExpand = -1;
            reDraw();
            return;
        }

        if (boxTranslate > sz.width / 2)
            ForDeletion = dragBoxNum;

        priorKlikx = 0;
        boxTranslate = 0;
        dragBoxNum = -1;


        if (ForDeletion > -1)
        {

            dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT a.ID "
                "  FROM Messages a "
                "  JOIN MsgReceivers mr ON mr.MsgID = a.ID "
                "   WHERE mr.UserID = ? ");

            dp::Params parDS(pSelect->allocParams());
            parDS << Globals::_currentUserID;

            dp::Columns pCols = pSelect->allocBindColumns(1);
            td::INT4 id;
            pCols << "ID" << id;
            if (!pSelect->execute()) {
                id= -1;
            }

            int i = 0;

            while (pSelect->moveNext())
            {

                if (ForDeletion == i)
                {
                    td::String setstr = "DELETE FROM Messages WHERE ID=";
                    setstr.append(std::to_string(id));

                    dp::IStatementPtr pDelStat = dp::getMainDatabase()->createStatement(setstr);

                    pDelStat->execute();

                    td::String setstr1 = "DELETE FROM MsgReceivers WHERE MsgID=";
                    setstr1.append(std::to_string(id));
                    setstr1.append(" AND UserID = ");
                    setstr1.append(std::to_string(Globals::_currentUserID));

                    dp::IStatementPtr pDelStat1 = dp::getMainDatabase()->createStatement(setstr1);

                    pDelStat1->execute();
                }
                i++;
            }
        }
        MessageToExpand = -1;
        ForDeletion = -1;

        reDraw();
    }



};




class Notifications : public gui::ViewScroller
{
private:
protected:
    Notifications1 _canvas;
protected:

public:
    Notifications()
        : gui::ViewScroller(gui::ViewScroller::Type::NoScroll, gui::ViewScroller::Type::ScrollerAlwaysVisible)
    {
        setContentView(&_canvas);
    }

    Notifications1& getView()
    {
        return _canvas;


    }

    void reset() {
        _canvas.reset();
        gui::Size sz;
        sz.width = 230;
        sz.height = GlobalsCanvas::brObavijesti * 70;
        this->setContentSize(sz);
    }
};

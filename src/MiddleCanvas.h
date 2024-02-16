#pragma once
#include <gui/View.h>
#include <gui/Image.h>
#include <gui/ImageView.h>
#include <gui/SplitterLayout.h>

class MiddleCanvas : public gui::Canvas
{
private:
protected:
    gui::Image _etf;
    gui::Shape _shapeCircle1;
    gui::Circle c;
    int i = 0;

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

            //--------test da radi funkcija onPrimaryButtonPressed-------
               gui::Circle c(120+10*i, 120, 30);
               float lw = 10;
                if(i==0) {
                    //--prvi poziv onDraw----
                    i++;
                    gui::Size sz;
                    getSize(sz);
                    gui::Point cp(sz.width / 2, sz.height / 2);
                    td::INT4 x = cp.x;
                    td::INT4 y = cp.y;

                    gui::Rect imgRect(x - 15 - x / 4, y - 15 - y / 4, x + 15 + x / 4, y + 15 + y / 4);
                    _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center);
                }
                else if (i % 2 != 0) {
                    c.translate(20, 0);
                    _shapeCircle1.createCircle(c, lw, td::LinePattern::Solid);
                    _shapeCircle1.drawFill(td::ColorID::DarkMagenta);
                    i++;
                }
                else if (i % 2 == 0) {
                    c.translate(20, 50);
                    _shapeCircle1.createCircle(c, lw, td::LinePattern::Solid);
                    _shapeCircle1.drawFill(td::ColorID::DarkOliveGreen);
                    i++;
                }
             //-----------------------------------------------------------------  
            }
           
        // pogled za SAO ------ grupa 1
        else if (Globals::_currentUserID == 4) {
            gui::Size sz;
            getSize(sz);
            gui::Point cp(sz.width / 2, sz.height / 2);
            td::INT4 x = cp.x;
            td::INT4 y = cp.y;

            gui::Rect imgRect(x - 15 - x / 4, y - 15 - y / 4, x + 15 + x / 4, y + 15 + y / 4);
            _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center);
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

    void reset() {
        reDraw();
    };

    void Reset(td::INT4 userID) {
        reDraw();
    };

    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override {

        reDraw();
        
    }
    bool getModelSize(gui::Size& modelSize) const override
    {
        modelSize.width = 1300;
        modelSize.height = 550;
        return true;
    }
};
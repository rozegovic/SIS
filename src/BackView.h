#pragma once
#include <gui/View.h>
#include <gui/Image.h>
#include <gui/ImageView.h>
#include <gui/SplitterLayout.h>
#include "MiddleCanvas.h"
#include "FarLeftCanvas.h"
#include "Globals.h"
#include <gui/ViewScroller.h>


class MiddleScroll : public gui::ViewScroller
{
private:
public:
    MiddleCanvas _middleCanvas;
protected:

public:
    MiddleScroll()
        : gui::ViewScroller(gui::ViewScroller::Type::ScrollAndAutoHide, gui::ViewScroller::Type::ScrollAndAutoHide)
    {
        setContentView(&_middleCanvas);
    }

    MiddleCanvas& getView()
    {
        return _middleCanvas;
    }
};

class FarLeftScroll : public gui::ViewScroller
{
private:
protected:
    FarLeftCanvas _leftCanvas;

protected:

public:
    FarLeftScroll(MiddleScroll* canvas)
        : gui::ViewScroller(gui::ViewScroller::Type::ScrollAndAutoHide, gui::ViewScroller::Type::ScrollAndAutoHide)
        , _leftCanvas(&canvas->_middleCanvas)
    {
        setContentView(&_leftCanvas);
    }
    FarLeftCanvas& getView()
    {
        return _leftCanvas;
    }
};


class BackView : public gui::View
{
private:
protected:
    FarLeftScroll _leftCanvas;
    MiddleScroll _middleCanvas;   //  vjerovatno ce leftCanvas morati primati pokazivac na srednji
    gui::SplitterLayout _splitter;

public:
    BackView()
        : _splitter(gui::SplitterLayout::Orientation::Horizontal, gui::SplitterLayout::AuxiliaryCell::First)
        , _leftCanvas(&_middleCanvas)
    {

        _splitter.setContent(_leftCanvas, _middleCanvas);
        setLayout(&_splitter);
    }

    void reset() {
        _middleCanvas.getView().reset();
        _leftCanvas.getView().reset();
    }
    /*void measure(CellInfo& ci) override
    {
        gui::Size imgSize;
        _image.getSize(imgSize);
        ci.minHor = imgSize.width;
        ci.minVer = imgSize.height;
        ci.nResHor = 0;
        ci.nResVer = 0;
    }
    void reMeasure(CellInfo& ci) override
    {
        gui::Size imgSize;
        _image.getSize(imgSize);
        ci.minHor = imgSize.width;
        ci.minVer = imgSize.height;
        ci.nResHor = 0;
        ci.nResVer = 0;
    }*/
};

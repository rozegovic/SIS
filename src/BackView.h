#pragma once
#include <gui/View.h>
#include <gui/Image.h>
#include <gui/ImageView.h>
#include <gui/SplitterLayout.h>
#include "MiddleCanvas.h"
#include "FarLeftCanvas.h"
#include "Globals.h"

class BackView : public gui::View
{
private:
protected:
    FarLeftCanvas _leftCanvas;
    MiddleCanvas _middleCanvas;   //  vjerovatno ce leftCanvas morati primati pokazivac na srednji
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
        _middleCanvas.reset();
        
        _leftCanvas.reset();
        
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

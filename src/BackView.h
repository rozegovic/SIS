#pragma once
#include <gui/View.h>
#include <gui/Image.h>
#include <gui/ImageView.h>
#include <gui/SplitterLayout.h>
#include "ChatsForTStaff.h"
#include "ConvoForTStaff.h"
#include "ChatsForSAO.h"
#include "ConvoForSAO.h"
#include "CanvasETF.h"
#include "CanvasSubject.h"
#include "Globals.h"

//----------------------Svaka grupa treba zakometarisati sve osim onog uslova koji njima treba
//----------------------sve znaci sve osim linije _splitter(...)
//----------------------trenutno radi za grupu 3


class BackView : public gui::View
{
private:
protected:
    ChatsTS _chatsTS;
    ConvoTS _convoTS;
    ChatsSAO _chatsSAO;
    ConvoSAO _convoSAO;
    ETFBackground _etf;
    SubjectShow _subject;
    gui::SplitterLayout _splitter;

public:
    BackView()
    : _splitter(gui::SplitterLayout::Orientation::Horizontal, gui::SplitterLayout::AuxiliaryCell::First) //potrebna nam dva ova???
    {
        //grupa 3 je prvi if, drugi je grupa 1, treci je grupa 2
      /*  if(Globals::_currentUserID ==1 || Globals::_currentUserID ==3) 
        {*/
            _splitter.setContent(_chatsTS, _convoTS);
        ///*}

        //else if (Globals::_currentUserID == 4)
        //{
          //  _splitter.setContent(_chatsSAO, _convoSAO);
        //}

        //else if (Globals::_currentUserID == 5)
        //{
          //  _splitter.setContent(_subject, _etf);
        //}*/

        setLayout(&_splitter);
    }


};

//
// wx1px.cpp - 1px wxWidgets version
//
// github:
//     https://github.com/yoggy/1px
//
// license:
//     Copyright (c) 2015 yoggy <yoggy0@gmail.com>
//     Released under the MIT license
//     http://opensource.org/licenses/mit-license.php
//
// usage:
//     $ sudo apt-get install libwxgtk2.8-dev
//     $ git clone https://github.com/yoggy/1px.git
//     $ cd 1px/wxwidgets/2.8/
//     $ make
//     $ ./wx1px
//
#include "wx1px.h"

#define TIMER_ID 12345

BEGIN_EVENT_TABLE(wx1pxDrawPane, wxPanel)
EVT_PAINT(wx1pxDrawPane::paintEvent)
EVT_KEY_UP(wx1pxDrawPane::keyReleased)
EVT_TIMER(TIMER_ID, wx1pxDrawPane::onTimer)
END_EVENT_TABLE()

wx1pxDrawPane::wx1pxDrawPane(wxFrame* parent) : wxPanel(parent), frame_count(0), timer(this, TIMER_ID)
{
	SetBackgroundColour(wxColour(*wxBLACK));
	timer.Start(250);
}

void wx1pxDrawPane::paintEvent(wxPaintEvent &evt)
{
    wxPaintDC dc(this);

	switch(frame_count % 4) {
	case 0:
    	dc.SetPen(wxPen(wxColor(255, 0, 0), 1));
		break;
	case 1:
    	dc.SetPen(wxPen(wxColor(0, 255, 0), 1));
		break;
	case 2:
    	dc.SetPen(wxPen(wxColor(0, 0, 255), 1));
		break;
	case 3:
    	dc.SetPen(wxPen(wxColor(0, 0, 0), 1));
		break;
	}
    dc.DrawPoint(100, 100);

	frame_count ++;
}

void wx1pxDrawPane::onTimer(wxTimerEvent &event)
{
	Refresh();
}

void wx1pxDrawPane::keyReleased(wxKeyEvent& evt)
{
	if (evt.GetKeyCode() == 27) {
		exit(0);
	}
}

IMPLEMENT_APP(wx1pxApp)

bool wx1pxApp::OnInit()
{
	wxFrame *frame = new wxFrame((wxFrame*)NULL, -1, _T("1px - wxWidgets version"));
	frame->SetSize(200, 200);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	wx1pxDrawPane* drawPane = new wx1pxDrawPane((wxFrame*)frame);
	drawPane->SetSize(0, 0, 200, 200);
	sizer->Add(drawPane, 1, wxALIGN_LEFT | wxALIGN_TOP);

	frame->SetSizer(sizer);
    frame->SetAutoLayout(true);

	frame->Show(true);
	SetTopWindow(frame);

	return true;
}


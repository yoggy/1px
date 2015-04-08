//
// wx1px.h - 1px wxWidgets version
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
#pragma once

#include <wx/wx.h>

class wx1pxDrawPane : public wxPanel
{
public:
	wx1pxDrawPane(wxFrame* parent);
	
	void paintEvent(wxPaintEvent & evt);
	void onTimer(wxTimerEvent &event);
	void keyReleased(wxKeyEvent& evt);

	DECLARE_EVENT_TABLE()

protected:
	int frame_count;
	wxTimer timer;
};

class wx1pxApp : public wxApp
{
public:
	virtual bool OnInit();
};

DECLARE_APP(wx1pxApp)


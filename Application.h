#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "MainFrame.h"


class Application : public wxApp
{
public:
	enum
	{
		MAIN_FRAME

	};

	Application();
	virtual bool OnInit() override;
	wxString mApplicationPath;
	MainFrame* mFrame;

};

DECLARE_APP(Application)

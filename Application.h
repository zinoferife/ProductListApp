#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx.h>
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

};

DECLARE_APP(Application)

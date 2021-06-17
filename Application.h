#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx.h>
#endif

class Application : public wxApp
{
public:
	Application();
	virtual bool OnInit() override;
};


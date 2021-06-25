#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include <wx/html/htmlwin.h>

class ProductDisplay : public wxHtmlWindow  
{
public:
	ProductDisplay(wxWindow* parent, wxWindowID id,
		const wxPoint& position,
		const wxSize& size);

};


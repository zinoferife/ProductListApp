#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP
#include <wx/textcompleter.h>
#include <unordered_map>
#include <string.h>

class TextAutoComplete : public wxTextCompleterSimple
{
public:
	TextAutoComplete();
	virtual void GetCompletions( const wxString& prefix, wxArrayString& completions) override;

private:
	wxArrayString mComplete;

};


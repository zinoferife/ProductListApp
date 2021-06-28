#include "TextAutoComplete.h"

TextAutoComplete::TextAutoComplete()
{
	mComplete.push_back(wxT("Three times a day"));
	mComplete.push_back(wxT("Two times a day"));
	mComplete.push_back(wxT("Four times a day"));
	mComplete.push_back(wxT("As directed"));
	mComplete.push_back(wxT("Tablets"));
	mComplete.push_back(wxT("Capsules"));
	mComplete.push_back(wxT("Liquid"));
	mComplete.push_back(wxT("Emulsions"));
	mComplete.push_back(wxT("Antibacteria"));
	mComplete.push_back(wxT("Antibiotic"));
	mComplete.push_back(wxT("High blood pressure"));
	mComplete.push_back(wxT("pain"));
	mComplete.push_back(wxT("Headache"));
	mComplete.push_back(wxT("Heart attack"));

}

void TextAutoComplete::GetCompletions(const wxString& prefix, wxArrayString& completions)
{
	completions = mComplete;
}

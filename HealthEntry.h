///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/menu.h>
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/bmpbuttn.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/listbox.h>
#include <wx/dialog.h>
#include <wx/stattext.h>
#include <list>
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class HealthTag
///////////////////////////////////////////////////////////////////////////////
class HealthTag : public wxDialog
{
	enum
	{
		ID_ADD_BUTTON,
		ID_CLEAR_BUTTON,
		ID_TEXT_ENTRY,
		ID_TAG_LIST,
		ID_CONTEXT_REMOVE

	};

	private:
		wxBoxSizer* bSizer2;
		void OnAddButton(wxCommandEvent& event);
		void OnOkButton(wxCommandEvent& event);
		void OnTagList(wxCommandEvent& event);
		void OnClearButton(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		void OnContextMenu(wxContextMenuEvent& event);
	protected:
		wxTextCtrl* mTagEntryText;
		wxBitmapButton* mAddButton;
		wxBitmapButton* mClearButton;
		wxListBox* mHealthTagList;


		virtual bool TransferDataFromWindow() override;
		virtual bool TransferDataToWindow() override;
		std::list<std::string>& mData;

		DECLARE_CLASS(HealthTag)
		DECLARE_EVENT_TABLE()
	public:

		HealthTag(std::list<std::string>& list,  wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Health conditions"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 408,299 ), long style = wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE| wxSYSTEM_MENU );
		~HealthTag();

};


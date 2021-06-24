///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <wx/bmpbuttn.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <vector>


///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class ProductEntry
///////////////////////////////////////////////////////////////////////////////
class ProductEntry : public wxPanel
{
	private:

	public:
		wxStaticText* m_staticText13;
		wxTextCtrl* ProductName;
		wxStaticText* m_staticText14;
		wxTextCtrl* ProductActIng;
		wxStaticText* m_staticText15;
		wxTextCtrl* ProductDescription;
		wxStaticText* m_staticText16;
		wxTextCtrl* ProductDirectionForUse;
		wxStaticText* m_staticText17;
		wxComboBox* ProductCategory;
		wxBitmapButton* AddNewCategory;
		wxButton* ProductOk;
		wxButton* PoductCancel;

		enum
		{
			ID_OK,
			ID_CANCEL
		};


		ProductEntry( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 528,283 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~ProductEntry();

		bool GetData(std::string& name,
			std::string& ActiveIng,
			std::string& ProductDescription,
			std::string& ProductCategory,
			std::string& DirectionForUse);
		void Clear();
		void AddCategory(const std::string category);
		wxArrayString mComboSeletion;
};


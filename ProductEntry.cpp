///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ProductEntry.h"

///////////////////////////////////////////////////////////////////////////

ProductEntry::ProductEntry( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	//this->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	this->SetToolTip( wxT("Enter a product") );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText13 = new wxStaticText( this, wxID_ANY, wxT("Product name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	fgSizer1->Add( m_staticText13, 0, wxALL, 5 );

	ProductName = new wxTextCtrl( this, wxID_ANY, wxT(" "), wxDefaultPosition, wxDefaultSize, 0 );
	ProductName->SetMinSize( wxSize( 180,-1 ) );

	fgSizer1->Add( ProductName, 1, wxALL, 5 );


	fgSizer1->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText14 = new wxStaticText( this, wxID_ANY, wxT("Product Active Ingredent"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	fgSizer1->Add( m_staticText14, 0, wxALL, 5 );

	ProductActIng = new wxTextCtrl( this, wxID_ANY, wxT("Artermether 80mg and lumenfantrin"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	ProductActIng->SetMinSize( wxSize( 180,-1 ) );

	fgSizer1->Add( ProductActIng, 0, wxALL, 5 );


	fgSizer1->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText15 = new wxStaticText( this, wxID_ANY, wxT("Product description"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	fgSizer1->Add( m_staticText15, 0, wxALL, 5 );

	ProductDescription = new wxTextCtrl( this, wxID_ANY, wxT("Anti malaria drug used for treament of things"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	ProductDescription->SetMinSize( wxSize( 180,-1 ) );

	fgSizer1->Add( ProductDescription, 0, wxALL, 5 );


	fgSizer1->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText16 = new wxStaticText( this, wxID_ANY, wxT("Direction for use"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	fgSizer1->Add( m_staticText16, 0, wxALL, 5 );

	ProductDirectionForUse = new wxTextCtrl( this, wxID_ANY, wxT("Take 2 tablets 3 times a day"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	ProductDirectionForUse->SetMinSize( wxSize( 180,-1 ) );

	fgSizer1->Add( ProductDirectionForUse, 0, wxALL, 5 );


	fgSizer1->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText17 = new wxStaticText( this, wxID_ANY, wxT("Product Category"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	fgSizer1->Add( m_staticText17, 0, wxALL, 5 );

	ProductCategory = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	ProductCategory->SetMinSize( wxSize( 180,-1 ) );

	fgSizer1->Add( ProductCategory, 0, wxALL, 5 );

	AddNewCategory = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxBORDER_NONE );

	AddNewCategory->SetBitmap( wxArtProvider::GetBitmap( "search"));
	AddNewCategory->SetBitmapDisabled( wxArtProvider::GetBitmap("search"));
	AddNewCategory->SetBitmapPressed( wxArtProvider::GetBitmap( "search"));
	AddNewCategory->SetBitmapFocus( wxArtProvider::GetBitmap("search"));
	AddNewCategory->SetBitmapCurrent( wxArtProvider::GetBitmap( "search"));
	AddNewCategory->SetToolTip( wxT("Add new category\n") );

	fgSizer1->Add( AddNewCategory, 0, wxALL, 5 );


	bSizer1->Add( fgSizer1, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );


	bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );

	ProductOk = new wxButton( this, ID_OK, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( ProductOk, 0, wxALL, 5 );

	PoductCancel = new wxButton( this, ID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( PoductCancel, 0, wxALL, 5 );


	bSizer1->Add( bSizer5, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
}

ProductEntry::~ProductEntry()
{
}

bool ProductEntry::GetData(std::string& name, std::string& ActiveIng, std::string& ProductDescription, std::string& ProductCategory, std::string& DirectionForUse)
{
	//validate the date


	name = this->ProductName->GetValue().ToStdString();
	ActiveIng = this->ProductActIng->GetValue().ToStdString();
	ProductDescription = this->ProductDescription->GetValue().ToStdString();
	ProductCategory = this->ProductCategory->GetValue().ToStdString();
	DirectionForUse = this->ProductDirectionForUse->GetValue().ToStdString();
	return true;


}

void ProductEntry::Clear()
{
	ProductName->Clear();
	ProductActIng->Clear();
	ProductDescription->Clear();
	ProductCategory->Clear();
	ProductDirectionForUse->Clear();
	ProductCategory->Append(mComboSeletion);
}

void ProductEntry::AddCategory(const std::string category)
{
	if (ProductCategory->FindString(category) == wxNOT_FOUND)
	{
		mComboSeletion.push_back(category);
	}
}

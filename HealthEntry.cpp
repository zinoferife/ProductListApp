///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "HealthEntry.h"

///////////////////////////////////////////////////////////////////////////
IMPLEMENT_ABSTRACT_CLASS(HealthTag,wxDialog)
BEGIN_EVENT_TABLE(HealthTag, wxDialog)
	EVT_BUTTON(ID_ADD_BUTTON,HealthTag::OnAddButton)
	EVT_BUTTON(ID_CLEAR_BUTTON,HealthTag::OnClearButton)
	EVT_BUTTON(wxID_OK,HealthTag::OnOkButton)
	EVT_TEXT_ENTER(ID_TEXT_ENTRY, HealthTag::OnAddButton)
	EVT_CLOSE(HealthTag::OnClose)
	EVT_MENU(HealthTag::ID_CONTEXT_REMOVE, HealthTag::OnTagList)
	EVT_CONTEXT_MENU(HealthTag::OnContextMenu)
END_EVENT_TABLE()


void HealthTag::OnAddButton(wxCommandEvent& event)
{
	wxString value = mTagEntryText->GetValue();
	if (!value.IsEmpty())
	{
		mHealthTagList->AppendAndEnsureVisible(mTagEntryText->GetValue());
		mTagEntryText->Clear();
	}
}

void HealthTag::OnOkButton(wxCommandEvent& event)
{
	if (Validate() && TransferDataFromWindow())
	{
		if (IsModal()) EndModal(wxID_OK);
		else
		{
			SetReturnCode(wxID_CANCEL);
			this->Show(false);
		}
	}
}

void HealthTag::OnTagList(wxCommandEvent& event)
{
	int sel = mHealthTagList->GetSelection();
	if(sel != wxNOT_FOUND) mHealthTagList->Delete(sel);
}

void HealthTag::OnClearButton(wxCommandEvent& event)
{
	mHealthTagList->Clear();
}


//TODO: the dialog is not sending the return code to the dialog
void HealthTag::OnClose(wxCloseEvent& event)
{
	if (IsModal()) EndModal(wxID_CANCEL);
	else
	{
		SetReturnCode(wxID_CANCEL);
		this->Show(false);
	}
	event.Skip();

}

void HealthTag::OnContextMenu(wxContextMenuEvent& event)
{
	int sel = mHealthTagList->GetSelection();
	if (sel != wxNOT_FOUND)
	{
		wxPoint pos = mHealthTagList->ScreenToClient(event.GetPosition());
		int hitPos = mHealthTagList->HitTest(pos);
		if (sel == hitPos)
		{
			wxMenu* menu = new wxMenu;
			menu->Append(ID_CONTEXT_REMOVE, "Remove");
			PopupMenu(menu);
		}
	}
}

bool HealthTag::TransferDataFromWindow()
{
	mData.clear();
	for (int i = 0; i < mHealthTagList->GetCount(); i++)
	{
		mData.push_back(mHealthTagList->GetString(i).ToStdString());
	}
	return true;
}

bool HealthTag::TransferDataToWindow()
{
	for (auto& i : mData)
	{
		mHealthTagList->AppendString(i);
	}
	return true;
}

HealthTag::HealthTag( std::list<std::string>& list, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style ), mData(list)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	bSizer2 = new wxBoxSizer( wxHORIZONTAL );

	bSizer1->Add(new wxStaticText(this, wxID_ANY, wxT("Please enter health conditions associated with this product:")), 0, wxALIGN_LEFT | wxALL, 5);

	mTagEntryText = new wxTextCtrl( this, ID_TEXT_ENTRY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	mTagEntryText->SetToolTip( wxT("Enter health conditions that this product acts on") );

	bSizer2->Add( mTagEntryText, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	mAddButton = new wxBitmapButton( this, ID_ADD_BUTTON, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );
	mClearButton = new wxBitmapButton( this, ID_CLEAR_BUTTON, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	mAddButton->SetBitmap( wxArtProvider::GetBitmap("reply"));
	mClearButton->SetBitmap( wxArtProvider::GetBitmap("delete"));
	bSizer2->Add( mAddButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	bSizer2->Add( mClearButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	bSizer1->Add( bSizer2, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	mHealthTagList = new wxListBox( this, ID_TAG_LIST, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	bSizer3->Add( mHealthTagList, 1, wxALL|wxEXPAND , 5 );


	bSizer1->Add( bSizer3, 1, wxEXPAND, 5 );

	wxBoxSizer* bsizer4 = new wxBoxSizer(wxHORIZONTAL);
	wxButton* ok = new wxButton(this, wxID_OK);

	bsizer4->AddStretchSpacer();
	bsizer4->Add(ok, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	bSizer1->Add(bsizer4, 0, wxEXPAND | wxALL, 5);

	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );
}

HealthTag::~HealthTag()
{
}

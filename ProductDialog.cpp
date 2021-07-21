#include "ProductDialog.h"
BEGIN_EVENT_TABLE(ProductDialog, wxDialog)
EVT_BUTTON(wxID_RESET, ProductDialog::OnReset)
EVT_BUTTON(wxID_OK, ProductDialog::OnOk)
EVT_BUTTON(wxID_CANCEL, ProductDialog::OnCancel)
EVT_BUTTON(ID_TAG_BUTTON, ProductDialog::OnHealthTag)
EVT_UPDATE_UI(wxID_ANY, ProductDialog::OnUpdateUI)
END_EVENT_TABLE()
IMPLEMENT_ABSTRACT_CLASS(ProductDialog, wxDialog)

ProductDialog::ProductDialog()
:mItemDataRef(nullptr){
	Init();
}

ProductDialog::ProductDialog(ProductItem* item, wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& position, const wxSize& size, long style)
: mItemDataRef(item){
	//Init();
	Create(parent, id, caption, position, size, style);

}

void ProductDialog::Init()
{
	mItemDataRef->ProductName().clear();
	mItemDataRef->CategoryName().clear();
	mItemDataRef->ProductActiveIng().clear();
	mItemDataRef->ProductDesc().clear();
	mItemDataRef->DirForUse().clear();
	mItemDataRef->ProductClass().clear();
	mItemDataRef->UnitPrice() = 0.00;
	mItemDataRef->StockCount() = 0;
	mTaglist.clear();

}

bool ProductDialog::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& position, const wxSize& size, long style)
{
	//ws_EX_BLOCK_EVENT removed, dont know what it was for
	SetExtraStyle(wxDIALOG_EX_CONTEXTHELP);
	if (!wxDialog::Create(parent, id, caption, position, size, style))
	{
		return false;
	}
	CreateControls();
	SetHelpDialog();

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Center();
	return true;
}

bool ProductDialog::TransferDataFromWindow()
{
	mItemDataRef->ProductName() = mProductNameCtrl->GetValue().ToStdString();
	mItemDataRef->CategoryName() = mProductCategoryCtrl->GetValue().ToStdString();
	mItemDataRef->ProductActiveIng() = mProductActiveIngredentCtrl->GetValue().ToStdString();
	mItemDataRef->ProductDesc() = mProductDescCtrl->GetValue().ToStdString();
	mItemDataRef->DirForUse() = mProductDirForUseCtrl->GetValue().ToStdString();
	mItemDataRef->ProductClass() = choices[mProductClassCtrl->GetSelection()].ToStdString();
	double price;
	mProductUnitPriceCtrl->GetValue().ToCDouble((double*)&price);
	mItemDataRef->UnitPrice() = price;
	mProductStockCtrl->GetValue().ToLong((long*)&(mItemDataRef->StockCount()));
	mItemDataRef->PackageSize() = mProductPackageSizeCtrl->GetValue();
	return true;
}

bool ProductDialog::TransferDataToWindow()
{
	mProductNameCtrl->SetValue(mItemDataRef->ProductName());
	mProductActiveIngredentCtrl->SetValue(mItemDataRef->ProductActiveIng());
	mProductDescCtrl->SetValue(mItemDataRef->ProductDesc());
	mProductDirForUseCtrl->SetValue(mItemDataRef->DirForUse());

	//might cause a problem due to non numeric characters 
	mProductStockCtrl->SetValue(std::to_string(mItemDataRef->StockCount()));
	mProductUnitPriceCtrl->SetValue(std::to_string(mItemDataRef->UnitPrice()));
	mProductPackageSizeCtrl->SetValue(mItemDataRef->GetPackageSize());
	mTaglist = mItemDataRef->GetHealthTag();
	return true;
}

void ProductDialog::CreateControls()

{
	//dialog controls
	choices.push_back("OTC");
	choices.push_back("P");
	choices.push_back("POM");

	mProductClassCtrl = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices);
	mProductClassCtrl->SetSelection(0);
	mProductCategoryCtrl = new wxComboBox(this, wxID_ANY);
	mProductNameCtrl = new wxTextCtrl(this, wxID_ANY);
	mProductActiveIngredentCtrl = new wxTextCtrl(this, wxID_ANY);
	mProductNameCtrl->AutoComplete(new TextAutoComplete);
	wxCollapsiblePane* pane = new wxCollapsiblePane(this, wxID_ANY, "Add details");
	wxWindow* wPane = pane->GetPane();
	mProductDescCtrl = new wxTextCtrl(wPane, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	mProductDirForUseCtrl = new wxTextCtrl(wPane, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	mProductDirForUseCtrl->AutoComplete(new TextAutoComplete);
	mProductUnitPriceCtrl = new wxTextCtrl(this, wxID_ANY);
	mProductStockCtrl = new wxTextCtrl(this, wxID_ANY);
	mProductPackageSizeCtrl = new wxSpinCtrl(this, wxID_ANY);

	wxStaticText* descp[10] = {
		new wxStaticText(this, wxID_ANY, wxT("Please enter a product to the system")),
		new wxStaticText(this, wxID_ANY, wxT("Product category:")),
		new wxStaticText(this, wxID_ANY, wxT("Product name:")),
		new wxStaticText(this, wxID_ANY, wxT("Product active ingredent:")),
		new wxStaticText(wPane, wxID_ANY, wxT("Product description:")),
		new wxStaticText(wPane, wxID_ANY, wxT("Product direction for use:")),
		new wxStaticText(this, wxID_ANY, wxT("Product unit price:")),
		new wxStaticText(this, wxID_ANY, wxT("Product package size:")),
		new wxStaticText(this, wxID_ANY, wxT("Product stock count:")),
		new wxStaticText(this, wxID_ANY, wxT("Product class:"))
	};

	//create buttons
	wxBitmapButton* reset = new wxBitmapButton(this, wxID_RESET, wxArtProvider::GetBitmap("minimize"));
	wxBitmapButton* tag = new wxBitmapButton(this, ID_TAG_BUTTON, wxArtProvider::GetBitmap("reply"));
	wxBitmapButton* ok = new wxBitmapButton(this, wxID_OK, wxArtProvider::GetBitmap("check"));
	wxBitmapButton* cancel = new wxBitmapButton(this, wxID_CANCEL, wxArtProvider::GetBitmap("delete"));
	
	reset->SetToolTip("Reset entry");
	tag->SetToolTip("Add health conditions to entry");
	ok->SetToolTip("ok entry");
	cancel->SetToolTip("cancel entry");

	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* boxSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* okCancleSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* classSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* collSizer = new wxBoxSizer(wxVERTICAL);

	//create a collapable pane

	//do the layout
	SetSizer(topSizer);
	topSizer->Add(boxSizer, 0, wxGROW | wxALL, 5);

	boxSizer->Add(descp[0], 0, wxALIGN_LEFT | wxALL, 5);

	classSizer->Add(descp[9], 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	classSizer->Add(mProductClassCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	boxSizer->Add(classSizer, wxALIGN_LEFT | wxALL, 5);

	boxSizer->Add(descp[1], 0, wxALIGN_LEFT | wxALL, 5);
	boxSizer->Add(mProductCategoryCtrl, 0, wxGROW | wxALL, 5);

	boxSizer->Add(descp[2], 0, wxALIGN_LEFT | wxALL, 5);
	boxSizer->Add(mProductNameCtrl, 0, wxGROW | wxALL, 5);

	boxSizer->Add(descp[3], 0, wxALIGN_LEFT | wxALL, 5);
	boxSizer->Add(mProductActiveIngredentCtrl, 0, wxGROW | wxALL, 5);

	collSizer->Add(descp[4], 0, wxALIGN_LEFT | wxALL, 5);
	collSizer->Add(mProductDescCtrl, 0, wxGROW | wxALL, 5);

	collSizer->Add(descp[5], 0, wxALIGN_LEFT | wxALL, 5);
	collSizer->Add(mProductDirForUseCtrl, 0, wxGROW | wxALL, 5);

	wPane->SetSizer(collSizer);
	collSizer->SetSizeHints(wPane);

	boxSizer->Add(pane, wxSizerFlags().Expand().Border(wxALL, 5));

	boxSizer->Add(descp[6], 0, wxALIGN_LEFT | wxALL, 5);
	boxSizer->Add(mProductUnitPriceCtrl, 0, wxGROW | wxALL, 5);

	boxSizer->Add(descp[8], 0, wxALIGN_LEFT | wxALL, 5);
	boxSizer->Add(mProductStockCtrl, 0, wxGROW | wxALL, 5);
	
	boxSizer->Add(descp[7], 0, wxALIGN_LEFT | wxALL, 5);
	boxSizer->Add(mProductPackageSizeCtrl, 0, wxGROW | wxALL, 5);

	wxStaticLine* line = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
	boxSizer->Add(line, 0, wxGROW | wxALL, 5);

	okCancleSizer->Add(180,16, 1, wxEXPAND, 5);
	okCancleSizer->Add(reset, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	okCancleSizer->Add(tag, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	okCancleSizer->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	okCancleSizer->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	boxSizer->Add(okCancleSizer, 0, wxEXPAND | wxALL, 5);


	//set validators
	//have to figure how to do this validator lool 
	wxTextValidator textValidator(wxFILTER_EMPTY);
	mProductNameCtrl->SetValidator(textValidator);

}

void ProductDialog::SetHelpDialog()
{
}

void ProductDialog::OnOk(wxCommandEvent& event)
{
	if (Validate() && TransferDataFromWindow())
	{
		if (IsModal()) EndModal(wxID_OK);
		else
		{
			SetReturnCode(wxID_OK);
			this->Show(false);
		}
	}
}

void ProductDialog::OnReset(wxCommandEvent& event)
{
	Init();
	TransferDataToWindow();
}

void ProductDialog::OnCancel(wxCommandEvent& event)
{
	if (wxMessageBox(wxT("Are you sure you want to cancel Product entry"), wxT("Product entry"), wxYES | wxNO) == wxYES)
	{
		if (IsModal()) EndModal(wxID_CANCEL);
		else
		{
			SetReturnCode(wxID_CANCEL);
			this->Show(false);
		}
	}
}

void ProductDialog::OnUpdateUI(wxUpdateUIEvent& event)
{

}

void ProductDialog::OnHealthTag(wxCommandEvent& event)
{
	HealthTag dlg(mTaglist, this, wxID_ANY);
	if (dlg.ShowModal() == wxID_OK)
	{
		mItemDataRef->HealthTag() = mTaglist;
	}
	else
	{
		wxMessageBox("Cancelled");
	}

}

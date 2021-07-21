#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP
#include <wx/statline.h>
#include <wx/collpane.h>
#include <wx/spinctrl.h>

#include <string>
#include "ProductItem.h"
#include "HealthEntry.h"
#include "TextAutoComplete.h"

class ProductDialog : public wxDialog
{
public:
	enum
	{
		ID_TAG_BUTTON
	};


	ProductDialog();
	ProductDialog(ProductItem* item, wxWindow* parent, wxWindowID id = wxID_ANY,
		const wxString& caption = wxString("Product item entry"),
		const wxPoint& position = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU);

	void Init();

	bool Create(wxWindow* parent, wxWindowID id = wxID_ANY,
		const wxString& caption = wxString("Product item entry"),
		const wxPoint& position = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU);

	virtual bool TransferDataFromWindow() override;
	virtual bool TransferDataToWindow() override;

	inline wxComboBox* GetCategoryControl() { return mProductCategoryCtrl; }
	inline wxChoice* GetProductClassControl() { return mProductClassCtrl; }
private:
	//dialog data
	ProductItem* mItemDataRef;
	std::list<std::string> mTaglist;

	//dialog controls
	wxComboBox* mProductCategoryCtrl;
	wxChoice* mProductClassCtrl;
	wxTextCtrl* mProductNameCtrl;
	wxTextCtrl* mProductActiveIngredentCtrl;
	wxTextCtrl* mProductDescCtrl;
	wxTextCtrl* mProductDirForUseCtrl;
	wxTextCtrl* mProductUnitPriceCtrl;
	wxTextCtrl* mProductStockCtrl;
	wxSpinCtrl* mProductPackageSizeCtrl;
	wxArrayString choices;

private:
	void CreateControls();
	void SetHelpDialog();

	//event handlers
	void OnOk(wxCommandEvent& event);
	void OnReset(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);
	void OnUpdateUI(wxUpdateUIEvent& event);
	void OnHealthTag(wxCommandEvent& event);

	DECLARE_CLASS(ProductDialog)
	DECLARE_EVENT_TABLE()

};


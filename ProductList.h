#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // !WX_PRECOMP

#include <wx/artprov.h>

#include <wx/dataview.h>
#include <string>
#include <vector>

#include <unordered_map>
#include "ProductItem.h"


class ProductList : public wxPanel
{
public:
	enum
	{
		ID_PRODUCT_VIEW
	};

	ProductList(wxWindow* parent, wxWindowID id, const wxPoint& position, const wxSize& size);

	bool HasCategory(const std::string& category);

	bool AddItem(const std::string& category, const ProductItem& item);
	bool AddItem(const ProductItem& item);

	ProductItem& GetItem(const std::string& ProductName, const std::string& Category);


	void LoadListDatabase();
	void SaveDatabase();
	void FindDatabase();
	void ParseJsonFile();
	void AppendProduct(const std::vector<std::string>& productDesc);
private:
	std::string mDatabaseFilename;
	std::unordered_map<std::string, std::vector<ProductItem> > mItemStore;

	DECLARE_EVENT_TABLE()
};


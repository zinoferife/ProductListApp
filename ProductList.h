#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // !WX_PRECOMP

#include <wx/artprov.h>
#include <wx/dataview.h>
#include <wx/aui/auibook.h>
#include <wx/dir.h>


#include <string>
#include <set>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <mutex>
#include <lock>

#include <unordered_map>
#include "ProductItem.h"


class ProductList : public wxPanel
{
public:
	enum
	{
		ID_PRODUCT_VIEW
	};
	
	//error codes
	enum
	{
		NO_PL_ERROR,
		PRODUCT_EXIST,
		NO_CATEGORY,
		DATABASE_ERROR,
		NO_DATABASE_PATH,
		UNUSUAL_ERROR_IN_LOAD,
		FILE_CORRUPTED,
		FILE_DOES_NOT_EXIST,
		DATABASE_SAVE_FAIL
	};


	ProductList(wxWindow* parent, wxWindowID id, const wxPoint& position, const wxSize& size);

	bool HasCategory(const std::string& category);

	bool AddItem(const std::string& category, const ProductItem& item);
	bool AddItem(const ProductItem& item);

	ProductItem& GetItem(const std::string& ProductName, const std::string& Category);

	
	void LoadListDatabase();

	//spins off a thread
	void SaveDatabase();


	const std::string& FindDatabase();
	void ParseJsonFile();
	void SaveJsonFile();
	void AppendProduct(const std::vector<std::string>& productDesc);

public:
	//GUI
	void CreateListView();

public:
	//operations on store
	void CreateCategory(const std::string& mCatrgory);
	
private:
	//product list event handlers
	void OnCategoryChange(const std::string& Category);
	void OnCategoryRemoved(const std::string& Category);
	void OnCategoryNameChange(const std::string& oldName, const std::string& newName);
	void OnProductAdded(ProductItem& item);
	void OnProductRemoved(ProductItem& item);
	void OnProductEdited(ProductItem& oldItem, ProductItem& newItem);
	void OnDataLoaded();

private:
	std::unordered_map<std::string, std::set<ProductItem> > mItemStore;

private:
	bool doload(std::fstream& file);
	bool doSave(std::fstream& file);
	void WriteErrorCode(int errorCode);
	//GUI
	wxDataViewListCtrl* mDataListViewControl;
	ProductItem mEmptyProduct;
	std::uint32_t mPLErrorCode;
	std::string mDatabasePath;


private:
	//thread semantics
	std::mutex mPLMutex;

	DECLARE_EVENT_TABLE()
};


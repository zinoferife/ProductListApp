#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // !WX_PRECOMP

#include <wx/artprov.h>
#include <wx/dataview.h>
#include <wx/aui/auibook.h>
#include <wx/dir.h>
#include <json/json.h>
#include <OpenXLSL/OpenXLSX.hpp>
#include <wx/progdlg.h>



#include <string>
#include <set>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <mutex>
#include <memory>
#include <regex>
#include <unordered_map>
#include "ProductItem.h"
#include "ProductDialog.h"



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
		DATABASE_SAVE_FAIL,
		ID_CONTEXT_REMOVE,
		ID_CONTEXT_EDIT,
		ID_CONTEXT_MOVE,
		ID_CONTEXT_DISPLAY
	};
	typedef std::unordered_map < std::string, std::set<ProductItem> > StoreType;
	typedef StoreType::iterator StoreIterator;
	typedef StoreType::const_iterator StoreConstIterator;

	ProductList(wxWindow* parent, wxWindowID id, const wxPoint& position, const wxSize& size);

	bool HasCategory(const std::string& category, StoreIterator* Founditer);
	inline bool IsEmpty() const { return mItemStore.empty(); }
	bool AddItem(const std::string& category, const ProductItem& item);
	bool AddItem(const ProductItem& item);
	bool RemoveItem(const std::string& category, const ProductItem& item);
	bool RemoveItem(const ProductItem& item);


	const ProductItem& GetItem(const std::string& ProductName, const std::string& Category) const;
	const ProductItem& GetFromDataView(const wxDataViewItem& item);

	void LoadListDatabase();
	void SaveDatabase();
	const std::string& FindDatabase();
	void ParseJsonFile();
	void SaveJsonFile();
	void SaveExcelFile();
	void SaveExcelLeanFile();
	//get code
	inline std::uint32_t GetErrorCode() { return mPLErrorCode; }

public:
	//GUI
	void PlugViewEventHandlers();
	void CreateListView();
	std::shared_ptr<wxDataViewListCtrl> GetListControl();
	void AppendToViewList(const ProductItem& item);
	void AppendMultiItemToViewList(const std::list<const ProductItem*>& items);
	void RemoveFromViewList(ProductItem& item);
	void InsertInListView(ProductItem& item);
	void ResetViewList();
	void RefreshViewList();
	void ShowAll();

public:
	//operations on store
	bool CreateCategory(const std::string& mCatrgory);
	inline std::string& GetCurrentCategory() { return mCurrentCategory; }
	void GetCategoryList(std::list<std::string>& categories);
	void SelectProduct(const ProductItem& item);
	bool MoveProduct(const std::string& from, const std::string& to, const ProductItem& product);

private:
	void OnListItemSelectionChanged(wxDataViewEvent& event);
	void OnListItemActivated(wxDataViewEvent& event);

public:
	//product list event handlers
	void OnCategoryChange(const std::string& Category);
	void OnCategoryRemoved(const std::string& Category);
	void OnCategoryNameChange(const std::string& oldName, const std::string& newName);
	void OnProductAdded(ProductItem& item);
	void OnProductRemoved(ProductItem& item);
	std::list<const ProductItem*> SearchForProduct(const std::string& matchName);
	void OnDataLoaded();
	void OnSaved();



	void OnProductStartEditing(wxDataViewEvent& event);
	void OnProductEndEditing(wxDataViewEvent& event);
	void OnProductEditStarted(wxDataViewEvent& event);
	void OnProductSelectionChanged(wxDataViewEvent& event);
	void OnProductActivated(wxDataViewEvent& event);
	void OnProductEdited(ProductItem& oldItem, ProductItem& newItem);


	//context menu
	void OnContextMenu(wxDataViewEvent& event);
	void OnContextRemove(wxCommandEvent& event);
	void OnContextEdit(wxCommandEvent& event);
	void OnContextMove(wxCommandEvent& event);
	void OnContextDisplay(wxCommandEvent& event);

	//test command

	//get stats
	std::size_t GetTotalProducts() const;

private:
	friend class ProductStat;
	StoreType mItemStore;
	std::string mCurrentCategory;
private:
	bool doload(std::fstream& file);
	bool doSave(std::fstream& file);
	bool doLoadEmptyCategoryNames();
	bool updateViewList(StoreIterator iterator);
	void WriteErrorCode(int errorCode);
	//GUI
	std::shared_ptr<wxDataViewListCtrl> mDataListViewControl;
	ProductItem mEmptyProduct;
	std::uint32_t mPLErrorCode;
	std::string mDatabasePath;

	std::string& FormatProductName(std::string& value);


	//editing system
	ProductItem* mProductCurrentEditing;

	//Search
	std::regex MakeRegexString(const std::string& searchString);
	void UpdateStatView(const std::string& category, int func);

private:
	//thread semantics
	std::mutex mPLMutex;
	std::mutex mPLStoreMutex;

	DECLARE_EVENT_TABLE()
};


#pragma once
#include <wx/wxprec.h>
#ifndef  WX_PRECOMP
#include <wx/wx.h>
#endif // ! WX_PRECOMP
#include <wx/aui/framemanager.h>
#include <wx/aui/dockart.h>
#include <wx/aui/auibar.h>
#include <wx/listctrl.h>
#include <wx/aboutdlg.h>
#include <wx/html/htmlwin.h>
#include <wx/choicdlg.h>

#include <fstream>
#include <sstream>
#include <memory>



#include "ProductList.h"
#include "ArtProvider.h"
#include "PLConfig.h"
#include "ProductEntry.h"
#include "ProductStat.h"

class MainFrame : public wxFrame
{
public:
	//window IDs;
	enum {
		//Menu
		ID_NEW_CATEGORY = wxID_HIGHEST + 1,
		ID_NEW_PRODUCT,
		ID_CATGORY_WINDOW,
		ID_PRODUCT_STAT_WINDOW,
		ID_PRODUCT_ENTRY,
		ID_TOOL_BACK,
		ID_TOOL_FRONT,
		ID_TOOL_ADD_PRODUCT,
		ID_TOOL_REMOVE_PRODUCT,
		ID_TOOL_ADD_CATEGORY,
		ID_TOOL_REMOVE_CATEGORY,
		ID_TOOL_SEARCH_PRODUCT,
		ID_TOOL_SAVE_DATABASE,
		ID_TOOL_USER,
		ID_PRODUCT_LIST,
		ID_CATEGORY_LIST,
		ID_TOOL_DOWNLOAD_DATA,
		ID_PRODUCT_DISPLAY,
		ID_PRODUCT_DISPLAY_WIN,
		ID_CATEGORY_CONTEXT_REMOVE,
		ID_CATEGORY_CONTEXT_RENAME
	};

	MainFrame(wxWindow* parent, wxWindowID id, const wxPoint& position, const wxSize& size);
	~MainFrame();
	void Load();
	bool isCreated;

	//hack so that Product list can forward
	void OnProductDisplay(wxCommandEvent& event);
private:
	//creation functions
	bool InitCreation();
	void CreateToolBar();
	void CreateMenuBar();
	void CreateStatusBar();

	//control functions
	void CreateProductList();
	void CreateCategoryList();
	void SaveAppConfig();
	void CreateDefaultArtSettings();
	void CreateProductDisplay();
	void CreateProductStats();

private:
	//event handlers
	//auto save if not saved
	void OnIdle(wxIdleEvent& event);
	void OnClose(wxCloseEvent& event);

	void OnSave(wxCommandEvent& event);
	void OnLoad(wxCommandEvent& event);
	void OnNew(wxCommandEvent& event);
	void OnNewCategory(wxCommandEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnNewProduct(wxCommandEvent& event);
	void OnNext(wxCommandEvent& event);
	void OnBack(wxCommandEvent& event);
	void OnAddProduct(wxCommandEvent& event);
	void OnRemoveProduct(wxCommandEvent& event);
	void OnProductRemoved(wxCommandEvent& evnt);
	void OnAddCategory(wxCommandEvent& event);
	void OnRemoveCategory(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnCategoryListSelection(wxCommandEvent& event);
	void OnCategoryWindow(wxCommandEvent& event);
	void OnCategoryListContext(wxContextMenuEvent& event);
	void OnCategoryRename(wxCommandEvent& event);
	void OnDownloadData(wxCommandEvent& event);
	void OnSearchForProduct(wxCommandEvent& event); 
	void OnProductStatWindow(wxCommandEvent& event);
	//erase
	void OnEraseBackground(wxEraseEvent& event);


private:
	std::unique_ptr<wxAuiManager> mFrameManager;
	std::unique_ptr<ProductList> mProductList;
	std::unique_ptr<wxListBox> mCategoryList;
	std::unique_ptr<PLConfig> mPLConfig;
	std::unique_ptr<wxHtmlWindow> mProductDisplay;
	std::unique_ptr<ProductStat> mProductStat;
	friend class ProductList;

	void PlugCategoryListEvents();
	wxPoint GetStartPosition();
	wxString ProductDisplayText(const ProductItem& item);
	DECLARE_EVENT_TABLE()

};


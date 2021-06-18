#pragma once
#include <wx/wxprec.h>
#ifndef  WX_PRECOMP
#include <wx/wx.h>
#endif // ! WX_PRECOMP
#include <wx/aui/framemanager.h>
#include <fstream>
#include <sstream>

#include "ProductList.h"
#include "ArtProvider.h"

class MainFrame : public wxFrame
{
public:
	//window IDs;
	enum {
		//Menu
		ID_NEW_CATEGORY


	};


	
	MainFrame(wxWindow* parent, wxWindowID id, const wxPoint& position, const wxSize& size);
	~MainFrame();
	bool isCreated;
private:
	//creation functions
	bool InitCreation();
	void CreateToolBar();
	void CreateMenuBar();
	void CreateStatusBar();

	//control functions
	void CreateProductList();


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


	//erase
	void OnEraseBackground(wxEraseEvent& event);


private:
	wxAuiManager* mFrameManager;
	wxAuiDockArt* mDorkart;
	ProductList* mProductList;

	DECLARE_EVENT_TABLE()

};


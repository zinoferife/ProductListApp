#include "MainFrame.h"
#include "Application.h"
BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_CLOSE(MainFrame::OnClose)
	EVT_ERASE_BACKGROUND(MainFrame::OnEraseBackground)
	EVT_SIZE(MainFrame::OnSize)
	EVT_MENU(wxID_NEW, MainFrame::OnNew)
	EVT_MENU(wxID_OPEN, MainFrame::OnLoad)
	EVT_MENU(wxID_SAVE, MainFrame::OnSave)
	EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
	EVT_MENU(ID_NEW_CATEGORY, MainFrame::OnAddCategory)
	EVT_TOOL(ID_TOOL_BACK, MainFrame::OnBack)
	EVT_TOOL(ID_TOOL_FRONT, MainFrame::OnNext)
	EVT_TOOL(ID_TOOL_ADD_PRODUCT, MainFrame::OnAddProduct)
	EVT_TOOL(ID_TOOL_ADD_CATEGORY, MainFrame::OnAddCategory)
	EVT_TOOL(ID_TOOL_REMOVE_PRODUCT, MainFrame::OnRemoveProduct)
	EVT_TOOL(ID_TOOL_REMOVE_CATEGORY, MainFrame::OnRemoveCategory)
	EVT_BUTTON(ProductEntry::ID_OK, MainFrame::OnProductAdded)
	EVT_BUTTON(ProductEntry::ID_CANCEL, MainFrame::OnProductEntryCancelled)
	EVT_LISTBOX(ID_CATEGORY_LIST, MainFrame::OnCategoryListSelection)
END_EVENT_TABLE()



MainFrame::MainFrame(wxWindow* parent, wxWindowID id, const wxPoint& position, const wxSize& size)
:wxFrame(parent,id , wxT("Dglopa product list"), position,size){
	//do initalize of system
	mFrameManager.reset(new wxAuiManager(this));
	mPLConfig.reset(new PLConfig(".data\\config.txt"));
	wxArtProvider::Push(new ArtProvider);
	if (!InitCreation())
	{
		wxMessageBox("Cannot create application, critical error", "ERROR MESSAGE", wxICON_ERROR | wxOK);
		isCreated = false;
	}

	isCreated = true;
	SetStatusText("Ready");
}

MainFrame::~MainFrame()
{
	if (mFrameManager)
	{
		mFrameManager->UnInit();
	}
}

bool MainFrame::InitCreation()
{
	CreateStatusBar();
	CreateMenuBar();
	CreateToolBar();
	CreateProductList();
	CreateCategoryList();
	CreateProductEntry();
	CreateDefaultArtSettings();
	mFrameManager->Update();
	return true;
}

void MainFrame::CreateToolBar()
{
	wxAuiToolBar* toolbar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_HORZ_TEXT | wxAUI_TB_OVERFLOW );
	toolbar->SetToolBitmapSize(wxSize(16, 16));
	toolbar->AddTool(ID_TOOL_BACK, wxT("Back"), wxArtProvider::GetBitmap("back"));
	toolbar->AddTool(ID_TOOL_FRONT, wxT("Next"), wxArtProvider::GetBitmap("next"));
	toolbar->AddTool(ID_TOOL_ADD_PRODUCT, wxT("Add product"), wxArtProvider::GetBitmap("add"));
	toolbar->AddTool(ID_TOOL_REMOVE_PRODUCT, wxT("Remove product"), wxArtProvider::GetBitmap("delete"));
	toolbar->AddTool(ID_TOOL_ADD_CATEGORY, wxT("Add Category"), wxArtProvider::GetBitmap("file"));
	toolbar->AddTool(ID_TOOL_REMOVE_CATEGORY, wxT("Remove Category"), wxArtProvider::GetBitmap("remove"));
	toolbar->AddTool(ID_TOOL_USER, wxT("User"), wxArtProvider::GetBitmap("user"));
	toolbar->AddStretchSpacer();
	toolbar->Realize();
	mFrameManager->AddPane(toolbar, wxAuiPaneInfo().Name(wxT("Tool")).Caption(wxT("Tool bar"))
		.ToolbarPane().Top().Resizable().Row(1).LeftDockable(false).RightDockable(false).Floatable(false).BottomDockable(false));
}

void MainFrame::CreateMenuBar()
{
	wxMenuBar* menubar = new wxMenuBar;
	wxMenu* file = new wxMenu;
	file->Append(wxID_NEW, "New\tCtrl-N");
	file->Append(wxID_SAVE, "Save\tCtrl-S");
	file->Append(wxID_OPEN, "Open\tCtrl-O");

	wxMenu* products = new wxMenu;
	products->Append(ID_NEW_CATEGORY, "New categorty\tCtrl-T");

	wxMenu* Help = new wxMenu;
	Help->Append(wxID_ABOUT);


	menubar->Append(file, wxT("&File"));
	menubar->Append(products, wxT("&Products"));
	menubar->Append(Help, wxT("Help"));
	SetMenuBar(menubar);
}

void MainFrame::CreateStatusBar()
{
	wxFrame::CreateStatusBar();
}

void MainFrame::CreateProductList()
{
	mProductList.reset(new ProductList(this, ID_PRODUCT_LIST, wxDefaultPosition, wxDefaultSize));
	mFrameManager->AddPane(mProductList.get(), wxAuiPaneInfo().Name("Product list").Caption("ProductList").CenterPane().Show());
}

void MainFrame::CreateCategoryList()
{
	mCategoryList.reset(new wxListBox(this, ID_CATEGORY_LIST));
	mFrameManager->AddPane(mCategoryList.get(), wxAuiPaneInfo().Name("Category list").Caption(wxT("Categories")).MinSize(wxSize(180, 180)).Left().Layer(0));
}

void MainFrame::CreateProductEntry()
{
	mFrameManager->AddPane(new ProductEntry(this, wxID_ANY),
		wxAuiPaneInfo().Name("Product Entry").Caption("Product Entry").Dockable(false).Float().Hide());
}

void MainFrame::SavePerspective()
{
	std::string perspective = mFrameManager->SavePerspective().ToStdString();
	(*mPLConfig)["APP_PERSPECTIVE"] = perspective;
}

void MainFrame::CreateDefaultArtSettings()
{
	wxAuiDockArt* art = mFrameManager->GetArtProvider();
	art->SetMetric(wxAUI_DOCKART_CAPTION_SIZE, 24);
	art->SetMetric(wxAUI_DOCKART_GRADIENT_TYPE, wxAUI_GRADIENT_HORIZONTAL);
	mFrameManager->SetFlags(mFrameManager->GetFlags() | wxAUI_MGR_LIVE_RESIZE | wxAUI_MGR_ALLOW_ACTIVE_PANE | wxAUI_MGR_VENETIAN_BLINDS_HINT);
}

void MainFrame::Load()
{
	mPLConfig->LoadConfigFile();
	mProductList->LoadListDatabase();

	if (mProductList->GetErrorCode() == ProductList::NO_PL_ERROR)
	{
		//inform the category list that a load has happend
		std::list<std::string> categoryList;
		mProductList->GetCategoryList(categoryList);

	}
}

void MainFrame::OnIdle(wxIdleEvent& event)
{
}

void MainFrame::OnClose(wxCloseEvent& event)
{
	//check for clean up and save 
	//SavePerspective();
	//mPLConfig->SaveConFigFile();


	event.Skip();
}

void MainFrame::OnSave(wxCommandEvent& event)
{
	//write the category and product count to the config
	mProductList->SaveDatabase();
	wxMessageBox("Saving", "Save", wxOK);
}

void MainFrame::OnLoad(wxCommandEvent& event)
{
	wxMessageBox("loading", "load", wxOK);
}

void MainFrame::OnNew(wxCommandEvent& event)
{
	
}

void MainFrame::OnNewCategory(wxCommandEvent& event)
{
	OnAddCategory(event);
}

void MainFrame::OnSize(wxSizeEvent& event)
{
	event.Skip();
}

void MainFrame::OnNewProduct(wxCommandEvent& event)
{
	OnAddProduct(event);
}

void MainFrame::OnNext(wxCommandEvent& event)
{
}

void MainFrame::OnBack(wxCommandEvent& event)
{
}

void MainFrame::OnAddProduct(wxCommandEvent& event)
{
	wxAuiPaneInfo& info = mFrameManager->GetPane(wxT("Product Entry")).Float().Show();
	if (info.floating_pos == wxDefaultPosition) info.FloatingPosition(GetStartPosition());
	mFrameManager->Update();

}

void MainFrame::OnProductAdded(wxCommandEvent& evnt)
{

	wxAuiPaneInfo& info = mFrameManager->GetPane(wxT("Product Entry")).Float().Hide();
	mFrameManager->Update();
	ProductEntry* entry = wxDynamicCast(info.window, ProductEntry);
	
	if (entry != NULL)
	{
		//get entry data
	    //update the productlist database and view
		ProductItem item;
		entry->GetData(item.ProductName(), item.ProductActiveIng(), item.ProductDesc(), item.CategoryName(), item.DirForUse());
		std::uint64_t id = ProductItem::IdGen::GetID();
		item.Id() = id;
		if (mProductList->AddItem(item.CategoryName(), item))
		{
			entry->AddCategory(item.CategoryName());
			entry->Clear();
		}
		else
		{
			wxMessageBox("Failed", "Successful", wxOK, this);
		}

		//update the category
		int index = mCategoryList->FindString(item.CategoryName());
		if (index == wxNOT_FOUND)
		{
			mCategoryList->AppendString(item.CategoryName());
		}
		else
		{
			mCategoryList->SetSelection(index);
		}
	}
	


}

void MainFrame::OnRemoveProduct(wxCommandEvent& event)
{
}

void MainFrame::OnProductRemoved(wxCommandEvent& evnt)
{
}

void MainFrame::OnAddCategory(wxCommandEvent& event)
{
	wxTextEntryDialog dlg(this, "Enter new category", wxT("New category"));
	if (dlg.ShowModal() == wxID_OK)
	{
		const std::string value = dlg.GetValue().ToStdString();
		if (!value.empty())
		{
			if (mProductList->CreateCategory(value))
			{
				mCategoryList->AppendString(value);
				ProductEntry* entry = wxDynamicCast(mFrameManager->GetPane("Product Entry").window,ProductEntry);
				if (entry)
				{
					entry->AddCategory(value);
					entry->Clear();
				}
			}
			else
			{
				int i = mCategoryList->FindString(value);
				if ( i != wxNOT_FOUND)
				{
					mCategoryList->SetSelection(i);
				}
			}
		}
	}

}

void MainFrame::OnRemoveCategory(wxCommandEvent& event)
{
}

void MainFrame::OnProductEntryCancelled(wxCommandEvent& event)
{
	wxAuiPaneInfo& info = mFrameManager->GetPane(wxT("Product Entry")).Float().Hide();
	mFrameManager->Update();
	ProductEntry* entry = wxDynamicCast(info.window, ProductEntry);
	entry->Clear();
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
	wxAboutDialogInfo info;
	info.SetName(wxT("Dglopa pharamcy management system"));
	info.SetVersion(wxT("0.0.0 pre beta"));
	info.SetDescription(wxT("Pharmacy mamagement system aid in the managment of pharmaceutical products"));
	info.SetCopyright(wxT("(C) 2021 Afrobug Software"));

	wxAboutBox(info);
}

void MainFrame::OnCategoryListSelection(wxCommandEvent& event)
{
	int i = event.GetSelection();
	std::string category = mCategoryList->GetString(i).ToStdString();
	if (mProductList->GetCurrentCategory() != category)
	{
		mProductList->OnCategoryChange(category);
	}
}

void MainFrame::OnEraseBackground(wxEraseEvent& event)
{
	event.Skip();
}

wxPoint MainFrame::GetStartPosition()
{
	static int x = 0;
	x += 20;
	wxPoint pt = ClientToScreen(wxPoint(0, 0));
	return wxPoint(pt.x + x, pt.y + x);
}

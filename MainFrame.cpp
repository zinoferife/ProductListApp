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
	EVT_MENU(MainFrame::ID_NEW_CATEGORY, MainFrame::OnAddCategory)
	EVT_MENU(MainFrame::ID_CATGORY_WINDOW, MainFrame::OnCategoryWindow)
	EVT_MENU(MainFrame::ID_NEW_PRODUCT, MainFrame::OnAddProduct)
	EVT_MENU(MainFrame::ID_PRODUCT_DISPLAY, MainFrame::OnProductDisplay)
	EVT_MENU(MainFrame::ID_CATEGORY_CONTEXT_REMOVE, MainFrame::OnRemoveCategory)
	EVT_MENU(MainFrame::ID_CATEGORY_CONTEXT_RENAME, MainFrame::OnCategoryRename)
	EVT_TOOL(MainFrame::ID_TOOL_BACK, MainFrame::OnBack)
	EVT_TOOL(MainFrame::ID_TOOL_FRONT, MainFrame::OnNext)
	EVT_TOOL(MainFrame::ID_TOOL_ADD_PRODUCT, MainFrame::OnAddProduct)
	EVT_TOOL(MainFrame::ID_TOOL_ADD_CATEGORY, MainFrame::OnAddCategory)
	EVT_TOOL(MainFrame::ID_TOOL_REMOVE_PRODUCT, MainFrame::OnRemoveProduct)
	EVT_TOOL(MainFrame::ID_TOOL_REMOVE_CATEGORY, MainFrame::OnRemoveCategory)
	EVT_TOOL(MainFrame::ID_TOOL_SEARCH_PRODUCT, MainFrame::OnSearchForProduct)
	EVT_TOOL(MainFrame::ID_TOOL_DOWNLOAD_DATA, MainFrame::OnDownloadData)
	EVT_LISTBOX_DCLICK(MainFrame::ID_CATEGORY_LIST, MainFrame::OnCategoryListSelection)
END_EVENT_TABLE()



MainFrame::MainFrame(wxWindow* parent, wxWindowID id, const wxPoint& position, const wxSize& size)
:wxFrame(parent,id , wxT("Dglopa pharmacy stock manager"), position,size){
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

	//setApp icon: Temp 
	wxIcon icon;
	icon.CopyFromBitmap(wxArtProvider::GetBitmap("appIcon"));
	SetIcon(icon);


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
	PlugCategoryListEvents();
	CreateProductDisplay();
	CreateDefaultArtSettings();
	mFrameManager->Update();
	return true;
}

void MainFrame::CreateToolBar()
{
	wxAuiToolBar* toolbar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_HORZ_TEXT | wxAUI_TB_OVERFLOW );
	toolbar->SetToolBitmapSize(wxSize(16, 16));
	toolbar->AddTool(ID_TOOL_BACK, wxEmptyString, wxArtProvider::GetBitmap("back"));
	toolbar->AddTool(ID_TOOL_FRONT, wxEmptyString, wxArtProvider::GetBitmap("next"));
	toolbar->AddSeparator();
	toolbar->AddTool(ID_TOOL_ADD_PRODUCT, wxT("Add product"), wxArtProvider::GetBitmap("add"));
	toolbar->AddTool(ID_TOOL_REMOVE_PRODUCT, wxT("Remove product"), wxArtProvider::GetBitmap("delete"));
	toolbar->AddTool(ID_TOOL_ADD_CATEGORY, wxT("Add category"), wxArtProvider::GetBitmap("file"));
	toolbar->AddTool(ID_TOOL_REMOVE_CATEGORY, wxT("Remove category"), wxArtProvider::GetBitmap("remove"));
	toolbar->AddTool(ID_TOOL_SEARCH_PRODUCT, wxT("Search product"), wxArtProvider::GetBitmap("search"));
	toolbar->AddStretchSpacer();
	toolbar->AddTool(ID_TOOL_DOWNLOAD_DATA, wxT("Download data"), wxArtProvider::GetBitmap("download"));
	toolbar->AddTool(ID_TOOL_USER, wxT("User"), wxArtProvider::GetBitmap("user"));
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
	products->Append(ID_NEW_PRODUCT, "New product\tCtrl-D");

	wxMenu* Help = new wxMenu;
	Help->Append(wxID_ABOUT);


	wxMenu* window = new wxMenu;
	window->Append(ID_PRODUCT_DISPLAY, "product display \tCtrl-G");
	window->Append(ID_CATGORY_WINDOW, "product categories");

	menubar->Append(file, wxT("&File"));
	menubar->Append(products, wxT("&Products"));
	menubar->Append(Help, wxT("Help"));
	menubar->Append(window, wxT("Window"));
	SetMenuBar(menubar);
}

void MainFrame::CreateStatusBar()
{
	wxFrame::CreateStatusBar();
}

void MainFrame::CreateProductList()
{
	mProductList.reset(new ProductList(this, ID_PRODUCT_LIST, wxDefaultPosition, wxDefaultSize));
	mFrameManager->AddPane(mProductList.get(), wxAuiPaneInfo().Name("Product list").Caption("Product list").CenterPane().Show());
}

void MainFrame::CreateCategoryList()
{
	mCategoryList.reset(new wxListBox(this, ID_CATEGORY_LIST));
	mFrameManager->AddPane(mCategoryList.get(), wxAuiPaneInfo().Name("Category list").Caption(wxT("Product Categories")).MinSize(wxSize(180, 180)).Floatable(true).Left().Layer(0));
}

void MainFrame::SaveAppConfig()
{
	std::string perspective = mFrameManager->SavePerspective().ToStdString();
	mPLConfig->InsertConfig("APP_PERSPECTIVE", perspective);
}

void MainFrame::CreateDefaultArtSettings()
{
	wxAuiDockArt* art = mFrameManager->GetArtProvider();
	art->SetMetric(wxAUI_DOCKART_CAPTION_SIZE, 24);
	art->SetMetric(wxAUI_DOCKART_GRADIENT_TYPE, wxAUI_GRADIENT_HORIZONTAL);
	mFrameManager->SetFlags(mFrameManager->GetFlags() | wxAUI_MGR_ALLOW_ACTIVE_PANE | wxAUI_MGR_VENETIAN_BLINDS_HINT);
}

void MainFrame::CreateProductDisplay()
{
	mProductDisplay.reset(new wxHtmlWindow(this, ID_PRODUCT_DISPLAY_WIN));
	mFrameManager->AddPane(mProductDisplay.get(),wxAuiPaneInfo().Name("Product display").Caption("Product display").Left().BestSize(wxSize(300,-1)).Hide());
}

void MainFrame::Load()
{
	mPLConfig->LoadConfigFile();
	mProductList->LoadListDatabase();

	//load the previous perspective
	if (!(*mPLConfig)["APP_PERSPECTIVE"].empty())
	{
		mFrameManager->LoadPerspective((*mPLConfig)["APP_PERSPECTIVE"]);
	}


	if (mProductList->GetErrorCode() == ProductList::NO_PL_ERROR)
	{
		//inform the category list that a load has happend
		mCategoryList->AppendAndEnsureVisible(std::string("All categories"));
		std::list<std::string> categoryList;
		mProductList->GetCategoryList(categoryList);
		for (auto& i : categoryList)
		{
			mCategoryList->AppendAndEnsureVisible(i);
		}
		//one after all categories
		mCategoryList->SetSelection(1);
	}
}

void MainFrame::OnIdle(wxIdleEvent& event)
{
}

void MainFrame::OnClose(wxCloseEvent& event)
{
	if(wxMessageBox("Are you sure you want to quit, do not forget to save", "Quit?", wxYES | wxNO | wxICON_INFORMATION) == wxNO)
	{
		return;
	}
	//check for clean up and save 
	SaveAppConfig();
	mPLConfig->SaveConFigFile();
	event.Skip();
}

void MainFrame::OnSave(wxCommandEvent& event)
{
	//write the category and product count to the config
	mProductList->SaveDatabase();
	wxMessageBox("Saved", "Save", wxOK);
}

void MainFrame::OnLoad(wxCommandEvent& event)
{
	
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
	ProductItem item;
	ProductDialog dlg(&item, this);
	std::list<std::string> mCategories;

	//load the drop down, 
	mProductList->GetCategoryList(mCategories);
	for (auto& i : mCategories)
	{
		dlg.GetCategoryControl()->AppendString(i);
	}
	auto i = dlg.GetCategoryControl()->FindString(mProductList->GetCurrentCategory());
	dlg.GetCategoryControl()->SetSelection(i);
	if (dlg.ShowModal() == wxID_OK)
	{
		std::uint64_t id = ProductItem::IdGen::GetID();
		item.Id() = id;
		if (mProductList->AddItem(item.CategoryName(), item))
		{
			//if we have a display product pane load the page with the item that was just added
		}
		else
		{
			wxMessageBox("Failed", "Addition status", wxOK, this);
		}

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
	if (wxMessageBox(wxT("Are you sure you want to remove product?"), wxT("Remove product"), wxYES_NO | wxICON_INFORMATION) == wxYES)
	{
		auto category = mProductList->GetCurrentCategory();
		if (!category.empty())
		{
			auto listView = mProductList->GetListControl();
			auto model = listView->GetModel();
			auto selItem = listView->GetCurrentItem();
			if (selItem.IsOk())
			{
				wxVariant itemDataName, itemDataCategory;
				model->GetValue(itemDataName, selItem, 0);
				model->GetValue(itemDataCategory, selItem, 2);
				//remove from the store and the view
				mProductList->RemoveItem(mProductList->GetItem(itemDataName.GetString().ToStdString(), itemDataCategory.GetString().ToStdString()));

				//lol 
				listView->DeleteItem(listView->ItemToRow(selItem));
			}
			else
			{
				wxMessageBox("No product selected", "Remove product");
			}
		}
	}
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
	int selection = mCategoryList->GetSelection();
	if (selection != wxNOT_FOUND)
	{
		auto category = mCategoryList->GetString(selection);
		if (category != "All categories")
		{
			wxString message = wxString("Do you want to remove category \" ") + category + " \" and all it\'s products";
			if (wxMessageBox(message, "Remove category", wxYES | wxNO | wxICON_INFORMATION) == wxYES)
			{
				mProductList->OnCategoryRemoved(category.ToStdString());
				mCategoryList->Delete(selection);
			}
		}
		else
		{
			wxMessageBox("Trying to delete all categories", "Remove category", wxOK | wxICON_ERROR);
		}
	}
	else
	{
		wxMessageBox("No category selected", "Remove category");
	}
}


void MainFrame::OnAbout(wxCommandEvent& event)
{
	wxAboutDialogInfo info;
	info.SetName(wxT("Dglopa pharamcy stock manager"));
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

void MainFrame::OnCategoryWindow(wxCommandEvent& event)
{
	if (!mFrameManager->GetPane("Category list").IsShown())
	{
		mFrameManager->GetPane(wxT("Category list")).Show();
		mFrameManager->Update();
	}
}

void MainFrame::OnCategoryListContext(wxContextMenuEvent& event)
{
	int sel = event.GetSelection();
	if (sel != wxNOT_FOUND)
	{
		wxMenu* menu = new wxMenu;
		menu->Append(ID_CATEGORY_CONTEXT_RENAME, "Rename");
		menu->Append(ID_CATEGORY_CONTEXT_REMOVE, "Remove");
		PopupMenu(menu);
	}
}

void MainFrame::OnCategoryRename(wxCommandEvent& event)
{
	int sel = mCategoryList->GetSelection();
	if (sel != wxNOT_FOUND)
	{
		wxTextEntryDialog dlg(this, "Please enter new name:", wxT("Category"));
		if (dlg.ShowModal() == wxID_OK)
		{
			std::string value = dlg.GetValue().ToStdString();
			if (!value.empty())
			{
				std::string oldvalue = mCategoryList->GetString(sel).ToStdString();
				mProductList->OnCategoryNameChange(oldvalue, value);
				mCategoryList->SetString(sel, value);
			}
		}

	}
}

void MainFrame::OnDownloadData(wxCommandEvent& event)
{
	mProductList->SaveJsonFile();
}

void MainFrame::OnSearchForProduct(wxCommandEvent& event)
{
	wxTextEntryDialog searchDlg(this, wxT("Search for product"), wxT("Search product"));
	if (searchDlg.ShowModal() == wxID_OK)
	{
		std::string value = searchDlg.GetValue().ToStdString();
		//quadratic search, need to find a better search lol but it is what it is  
		std::list<const ProductItem*> mItems = mProductList->SearchForProduct(value);
		if (mItems.empty())
		{
			if (wxMessageBox("No product found, try again?", "Search product", wxYES_NO | wxICON_INFORMATION) == wxYES)
			{
				OnSearchForProduct(event);
			}
		}
		else
		{
			wxArrayString choices;
			for (auto& i : mItems)
			{
				choices.push_back(i->GetProductName());
			}
			wxSingleChoiceDialog sDialog(this, wxT("Please select a product"), wxT("Product select"), choices);
			if (sDialog.ShowModal() == wxID_OK)
			{
				std::string selectedValue = sDialog.GetStringSelection().ToStdString();
				auto iter = std::find_if(mItems.begin(), mItems.end(), [&](const ProductItem*& item) {
					return (item->GetProductName() == selectedValue);
					
				});
				mCategoryList->Select(mCategoryList->FindString((*iter)->GetCategoryName()));
				mProductList->SelectProduct(*(*iter));
			}
		}
	}


}

void MainFrame::OnProductDisplay(wxCommandEvent& event)
{
	//get the item from store
	auto item = mProductList->GetListControl()->GetCurrentItem();
	if (item.IsOk())
	{
		const ProductItem& product = mProductList->GetFromDataView(item);
		mProductDisplay->SetPage(ProductDisplayText(product));
		mProductDisplay->Update();

		//display the product
		mFrameManager->GetPane("Product display").Show();
		mFrameManager->Update();
	}
	else
	{
		wxMessageBox("No product selected", "Product display");
		return;
	}


}

void MainFrame::OnEraseBackground(wxEraseEvent& event)
{
	event.Skip();
}

void MainFrame::PlugCategoryListEvents()
{
	mCategoryList->Connect(wxEVT_CONTEXT_MENU, wxContextMenuEventHandler(MainFrame::OnCategoryListContext));
}

wxPoint MainFrame::GetStartPosition()
{
	static int x = 0;
	x += 20;
	wxPoint pt = ClientToScreen(wxPoint(0, 0));
	return wxPoint(pt.x + x, pt.y + x);
}

wxString MainFrame::ProductDisplayText(const ProductItem& item)
{
	std::stringstream htmlData;
	htmlData << "<html><body>";
	htmlData << "<h3>" << item.GetProductName() << "</h3>";
	htmlData << "<br><b>Active ingredent: " << item.GetProductActIng() << "</b><br>";
	htmlData << "<br><b> Class: " << item.GetProductClass() << " Category: " << item.GetCategoryName() << "</b><br>";
	htmlData << "<br/><b>Product description</b><br>";
	htmlData << "<p>" << item.GetProdcutDesc() << "</p>";
	htmlData << "<p></p>";
	htmlData << "<br/><b>Direction for use</b><br>";
	htmlData << "<p>" << item.GetDirForUse() << "</p>";
	htmlData << "<p></p>";
	htmlData << "<p><b> Health conditions</b><p>";
	htmlData << "<ul>";
	for (auto& i : item.GetHealthTag())
	{
		htmlData << "<li>" << i << "</li>";
	}
	htmlData << "</ui>";
	htmlData << "</body></html>";

	return wxString(htmlData.str());
}

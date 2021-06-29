#include "ProductList.h"
#include "Application.h"

#define ALL_CATEGORIES "All categories"

BEGIN_EVENT_TABLE(ProductList, wxPanel)
EVT_DATAVIEW_SELECTION_CHANGED(ProductList::ID_PRODUCT_VIEW, ProductList::OnProductSelectionChanged)
EVT_DATAVIEW_ITEM_ACTIVATED(ProductList::ID_PRODUCT_VIEW, ProductList::OnProductActivated)
EVT_DATAVIEW_ITEM_EDITING_DONE(ProductList::ID_PRODUCT_VIEW, ProductList::OnProductEndEditing)
EVT_DATAVIEW_ITEM_CONTEXT_MENU(ProductList::ID_PRODUCT_VIEW, ProductList::OnContextMenu)
EVT_MENU(ProductList::ID_CONTEXT_REMOVE, ProductList::OnContextRemove)
EVT_MENU(ProductList::ID_CONTEXT_EDIT, ProductList::OnContextEdit)
EVT_MENU(ProductList::ID_CONTEXT_DISPLAY, ProductList::OnContextDisplay)
END_EVENT_TABLE()

ProductList::ProductList(wxWindow* parent, wxWindowID id, const wxPoint& position, const wxSize& size)
: wxPanel(parent, id, position,size), mProductCurrentEditing(NULL){
	mDatabasePath = wxGetApp().mApplicationPath + "\\.data";
	mPLErrorCode = NO_PL_ERROR;
	CreateListView();
	PlugViewEventHandlers();
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(mDataListViewControl.get(), 1, wxEXPAND);
	SetSizer(sizer);
	GetSizer()->SetSizeHints(this);
}

//checks if category exists 
bool ProductList::HasCategory(const std::string& category,  StoreIterator* FoundIter)
{
	auto found = mItemStore.find(category);
	if (FoundIter)
	{
		(*FoundIter) = found;
	}
	return (found != mItemStore.end());
}

//creates a new category if the category is not in the store
bool ProductList::AddItem(const std::string& category, const ProductItem& item)
{
	if (item.IsEmpty()) return false;
	StoreIterator iter;
	if (HasCategory(category, &iter))
	{
		ProductItem& citem = const_cast<ProductItem&>(item);
		FormatProductName(citem.ProductName());
		auto itemIter = iter->second.insert(item);
		if (itemIter.second)
		{
			if (mCurrentCategory != category)
			{
				OnCategoryChange(category);
			}
			else
			{
				mDataListViewControl->Freeze();
				AppendToViewList(item);
				mDataListViewControl->Thaw();
				mDataListViewControl->Update();
			}
		}
		return (itemIter.second);
	}
	else
	{
		//category does not exist create a new category
		std::set<ProductItem> items;
		auto iterI = items.insert(item);
		auto iterC = mItemStore.insert(std::make_pair(category, items));
		if (!iterI.second)
		{
			//fatal error, something wrong that we deffo do not know about, throw exception, crash the system, go home and stop programming
		}
		OnCategoryChange(category);
		return (iterI.second && iterC.second);
	}

}
//TODO: find duplicates in diffenent category sets

//fails if category does not exist, uses the category name in the item object
bool ProductList::AddItem(const ProductItem& item)
{
	if (item.IsEmpty()) return false;

	const std::string& category = item.GetCategoryName();
	StoreIterator iter;
	if (!HasCategory(category, &iter))
	{
		return false;
	}
	auto& itemsVec = iter->second;
	ProductItem& citem = const_cast<ProductItem&>(item);
	FormatProductName(citem.ProductName());
	auto iterI = itemsVec.insert(item);
	if (!iterI.second)
	{
		mPLErrorCode = PRODUCT_EXIST;
		return false;
	}
	//update the view, if current category
	if(mCurrentCategory == category) updateViewList(iter);
	return true;
}

bool ProductList::RemoveItem(const std::string& category, const ProductItem& item)
{
	

	return true;
}

bool ProductList::RemoveItem(const ProductItem& item)
{
	auto found = mItemStore.find(item.GetCategoryName());
	if (found != mItemStore.end())
	{
		//should handle 
		auto i = found->second.find(item);
		if (i == found->second.end()) return false;
		found->second.erase(i);
		return true;
	}
	else
	{
		return false;
	}
}

const ProductItem& ProductList::GetItem(const std::string& ProductName, const std::string& Category) const
{
	auto iter = mItemStore.find(Category);
	if (iter != mItemStore.end())
	{
		if (!iter->second.empty())
		{
			auto& itemVec = iter->second;

			// search
			//DEBUG
			ProductItem searchItem;
			searchItem.ProductName() = ProductName;
			auto iter = itemVec.find(searchItem);
			if (iter != itemVec.end())
			{
				return *iter;
			}
			else
			{
				wxString message = wxString("The product is not in: ") + Category;
				wxMessageBox(message, "USER ERROR", wxOK | wxICON_ERROR);
				return mEmptyProduct;
			}

		}
		else
		{
			wxString message = wxString("The category is empty: ") + Category;
			wxMessageBox(message, "USER ERROR", wxOK | wxICON_ERROR);
			return mEmptyProduct;
		}
	}
	else
	{
		//log trying to access a category that does not exist
		wxMessageBox(wxT("Trying to access a product category that does not exist"), wxT("USER ERROR"), wxOK | wxICON_ERROR);
		return mEmptyProduct;
	}

}

const ProductItem& ProductList::GetFromDataView(wxDataViewItem& item)
{
	
	auto model = mDataListViewControl->GetModel();

	wxVariant itemDataName, itemDataCategory;
	model->GetValue(itemDataName, item, 0);
	model->GetValue(itemDataCategory, item, 2);

	//search store and get the item
	return GetItem(itemDataName.GetString().ToStdString(), itemDataCategory.GetString().ToStdString());


}

void ProductList::LoadListDatabase()
{
	wxBusyCursor cursor;
	if (wxDir::Exists(mDatabasePath))
	{
		//director exist, 
		//load the data base from the file
		std::string npath = mDatabasePath;
		npath += "\\productlist.dat";
		std::fstream database(npath, std::ios::in | std::ios::binary);
		if (database.is_open() && database.good())
		{
			//do load
			if (!doload(database))
			{
				//load did not work
				if (mPLErrorCode == FILE_CORRUPTED)
				{
					wxMessageBox("Failed to load database, file corrupted", "DATABASE ERROR",wxOK | wxICON_ERROR);
					return;
				}
				wxMessageBox("Failed to load from database", "DATABASE ERROR", wxOK | wxICON_ERROR);
				WriteErrorCode(DATABASE_ERROR);
			}
			else
			{
				//database is loaded well call back and return
				OnDataLoaded();
				return;
			}
		}
		else
		{
			//if file does not open, assume file does not exist
			wxMessageBox("Failed to load database, \"productlist.dat\" does not exist in the folder", "DATABASE ERROR", wxOK | wxICON_INFORMATION);
			WriteErrorCode(FILE_DOES_NOT_EXIST);
			return;
		}
	}
	else
	{
		//database folder does not exsist, try another folder
		wxMessageBox("Cannot load database from folder, try loading from a different folder", "SYSTEM ERROR", wxOK | wxICON_ERROR);
		std::string file = FindDatabase();
		if (file.empty())
		{
			//
			WriteErrorCode(NO_DATABASE_PATH);
			return;
		}
		std::string save = mDatabasePath;
		mDatabasePath = file;
		LoadListDatabase();
		mDatabasePath = save;

		if (mPLErrorCode == NO_DATABASE_PATH)
		{
			wxMessageBox("No database loaded in to the system", "DATABASE ERROR", wxOK | wxICON_ERROR);
			return;
		}
		else if (mPLErrorCode == DATABASE_ERROR)
		{
			//cannot load from the file that we found, check error code 
			return;
		}


	}

	//load successful or not in terms of recursion, check mPLErrorCode for operation status
	return;
}


//save need to consider the fact that is running on a different thread
//file would always open, file is created in the folder if not found 
void ProductList::SaveDatabase()
{
	std::unique_lock<std::mutex> lock(mPLStoreMutex);
	if (mItemStore.empty())
	{
		//nothing to save
		return;
	}
	lock.unlock();

    if (wxDir::Exists(mDatabasePath))
	{
		std::string nPath = mDatabasePath + "\\productlist.dat";
		std::fstream database(nPath, std::ios::out | std::ios::binary);
		if (database.is_open())
		{
			//database file is in folder, save
			if (!doSave(database))
			{
				WriteErrorCode(DATABASE_SAVE_FAIL);
				return;
			}
			else
			{
				//tell the main thread that save is complete
				OnSaved();
				database.flush();
				return;
			}
		}
	}
	//all good
	return;
}

const std::string& ProductList::FindDatabase()
{
	//ask the User to specify a database file 
	wxDirDialog dlg(NULL, "Choose a database directory to search", mDatabasePath, wxDD_DEFAULT_STYLE |
		wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK)
	{
		return (dlg.GetPath().ToStdString());
	}
	else
	{
		return std::string();
	}
}

void ProductList::ParseJsonFile()
{
}

void ProductList::SaveJsonFile()
{
}

//runs on creation of the product list
void ProductList::CreateListView()
{
	if (mDataListViewControl == nullptr)
	{
		//create the list view control
		mDataListViewControl.reset(new wxDataViewListCtrl(this, ID_PRODUCT_VIEW, wxDefaultPosition, wxDefaultSize));
		//create column
		mDataListViewControl->AppendTextColumn("Product name", wxDATAVIEW_CELL_INERT, 180);
		mDataListViewControl->AppendBitmapColumn(wxT("AVL"),1);
		mDataListViewControl->AppendTextColumn("Category name", wxDATAVIEW_CELL_INERT, 180);
		mDataListViewControl->AppendTextColumn("Class", wxDATAVIEW_CELL_INERT, 100);
		mDataListViewControl->AppendTextColumn("Active ingredent", wxDATAVIEW_CELL_INERT, 180);
		mDataListViewControl->AppendTextColumn("Stock count", wxDATAVIEW_CELL_INERT, 100);
		mDataListViewControl->AppendTextColumn("Unit price (N)", wxDATAVIEW_CELL_INERT, 100);
		mDataListViewControl->Update();

	}
	if (mItemStore.empty())
	{
		//no data in the data base, empty list
		return;
	}

	//created, the list 
	//update list
	OnCategoryChange(mItemStore.begin()->first);
}

std::shared_ptr<wxDataViewListCtrl> ProductList::GetListControl()
{
	return mDataListViewControl;
}

void ProductList::AppendToViewList(const ProductItem& item)
{
	wxVector<wxVariant> mdata;
	mdata.push_back(wxVariant(item.GetProductName()));
	
	if (item.GetStockCount() != 0)
		mdata.push_back(wxVariant(wxArtProvider::GetBitmap("check")));
	else
		mdata.push_back(wxVariant(wxArtProvider::GetBitmap("delete")));

	mdata.push_back(wxVariant(item.GetCategoryName()));
	mdata.push_back(wxVariant(item.GetProductClass()));
	mdata.push_back(wxVariant(item.GetProductActIng()));
	mdata.push_back(wxVariant(std::to_string(item.GetStockCount())));

	wxString price;
	price.sprintf("%.2f", item.GetUnitPrice());
	mdata.push_back(wxVariant(price));

	mDataListViewControl->AppendItem(mdata);

}

void ProductList::RemoveFromViewList(ProductItem& item)
{
	if (!item.IsEmpty())
	{
		int nrow = mDataListViewControl->GetSelectedRow();
		nrow++;
		if (nrow != wxNOT_FOUND)
		{
			mDataListViewControl->DeleteItem(nrow);
		}
		else
		{
			wxMessageBox("No item selected", "Remove item");
		}
	}
}

void ProductList::InsertInListView(ProductItem& item)
{
	wxVector<wxVariant> mdata;
	mdata.push_back(wxVariant(item.GetProductName()));

	if (item.GetStockCount() != 0)
		mdata.push_back(wxVariant(wxArtProvider::GetBitmap("check")));
	else
		mdata.push_back(wxVariant(wxArtProvider::GetBitmap("delete")));

	mdata.push_back(wxVariant(item.GetCategoryName()));
	mdata.push_back(wxVariant(item.GetProductClass()));
	mdata.push_back(wxVariant(item.GetProductActIng()));
	mdata.push_back(wxVariant(std::to_string(item.GetStockCount())));

	wxString price;
	price.sprintf("%.2f", item.GetUnitPrice());
	mdata.push_back(wxVariant(price));

	int sel = mDataListViewControl->GetSelectedRow();
	mDataListViewControl->InsertItem(sel,mdata);

}

void ProductList::ShowAll()
{
	ResetViewList();
	for (auto& i : mItemStore)
	{
		for (auto& s : i.second)
		{
			AppendToViewList(s);
		}
	}
	mCurrentCategory = ALL_CATEGORIES;
}

bool ProductList::CreateCategory(const std::string& mCatrgory)
{
	auto iter = mItemStore.insert(std::make_pair(mCatrgory, std::set<ProductItem>()));
	if (!iter.second)
	{
		wxMessageBox("Category already exist", "Category", wxOK| wxICON_INFORMATION);
		return false;
	}
	return true;
}

void ProductList::GetCategoryList(std::list<std::string>& categories)
{
	for (auto items : mItemStore)
	{
		categories.push_back(items.first);
	}

}

void ProductList::OnListItemSelectionChanged(wxDataViewEvent& event)
{
}

void ProductList::OnListItemActivated(wxDataViewEvent& event)
{
}

void ProductList::OnCategoryChange(const std::string& Category)
{
	if (Category == ALL_CATEGORIES)
	{
		ShowAll();
		return;
	}
	StoreIterator catIter;
	if (mCurrentCategory != Category && HasCategory(Category, &catIter))
	{
		mDataListViewControl->Freeze();
		mDataListViewControl->DeleteAllItems(); 
		mCurrentCategory = Category;
		updateViewList(catIter);
		mDataListViewControl->Thaw();
		mDataListViewControl->Update();
	}
}

//User is sure that they want the category deleted with all its data
//Precondition: The store is not empty
//Postcondition: The store may be empty
void ProductList::OnCategoryRemoved(const std::string& Category)
{
	StoreIterator catIter;
	if (HasCategory(Category, &catIter))
	{
		if (mCurrentCategory == Category)
		{
			//change to the next category, if last wrap around and if empty return
			if (mItemStore.size() == 1)
			{
				//category is only category, empty the view
				ResetViewList();
				mItemStore.erase(catIter);
				return;
			}
			else
			{
				if((++catIter) == mItemStore.end())
				{
					OnCategoryChange(mItemStore.begin()->first);
				}
				else
				{
					OnCategoryChange(catIter->first);
				}
			}
		}
		else
		{
			mItemStore.erase(catIter);
			if (mCurrentCategory == "All categories")
			{
				//refresh view if on all categories
				ShowAll();
			}
		}
	}
}

//TODO: 
void ProductList::OnCategoryNameChange(const std::string& oldName, const std::string& newName)
{
	//get the set attached to the category
	//insert the pair with the new name 
	//remove old category
	auto iter = mItemStore.find(oldName);
	auto& set = iter->second;
	std::set<ProductItem> newSet;
	
	//c++ magic
	std::insert_iterator<std::set<ProductItem> > Insert(newSet, newSet.begin());
	std::transform(set.begin(), set.end(), Insert, [&](const ProductItem& item) {
		ProductItem& itemNC = const_cast<ProductItem&>(item);
		itemNC.CategoryName() = newName;
		return itemNC;
		});
	auto insetIter = mItemStore.insert(std::make_pair(newName, newSet));
	if (insetIter.second)
	{
		mItemStore.erase(oldName);
		if(oldName == mCurrentCategory)
		{
			ResetViewList();
			updateViewList(insetIter.first);
			mCurrentCategory = newName;
		}
	}
	else
	{
		wxMessageBox("Failed to change name, category may already exist", "Category");
	}
}

void ProductList::OnProductAdded(ProductItem& item)
{
	AddItem(item);
}

void ProductList::OnProductRemoved(ProductItem& item)
{
	RemoveItem(item);
}

void ProductList::OnProductEdited(ProductItem& oldItem, ProductItem& newItem)
{
	//workng in the current directory
	wxBusyCursor cur;
	StoreIterator itemSet;
	if (mCurrentCategory != ALL_CATEGORIES)
	{
		itemSet = mItemStore.find(mCurrentCategory);
	}
	else
	{
		itemSet = mItemStore.find(oldItem.GetCategoryName());
	}

	//remove old item
	itemSet->second.erase(oldItem);
	FormatProductName(newItem.ProductName());
	auto iterInsert = itemSet->second.insert(newItem);
	if (!iterInsert.second)
	{
		wxMessageBox("Edit failed", "Edit product");
	}

	//update the view
	mDataListViewControl->Freeze();
	InsertInListView(newItem);
	RemoveFromViewList(oldItem);
	mDataListViewControl->Thaw();
	mDataListViewControl->Refresh();
	mDataListViewControl->Select(mDataListViewControl->GetCurrentItem());
}

void ProductList::OnContextMenu(wxDataViewEvent& event)
{
	int rowSelected = GetListControl()->GetSelectedRow();
	if (rowSelected != wxNOT_FOUND)
	{
		wxMenu* menu = new wxMenu;
		menu->Append(ID_CONTEXT_REMOVE, wxT("Remove product"));
		menu->Append(ID_CONTEXT_EDIT, wxT("Edit product"));
		menu->Append(ID_CONTEXT_DISPLAY, wxT("Display product"));

		PopupMenu(menu);
	}
}

void ProductList::OnContextRemove(wxCommandEvent& event)
{
	auto category = GetCurrentCategory();
	if (!category.empty())
	{
		auto listView = GetListControl();
		auto model = listView->GetModel();
		auto selItem = listView->GetCurrentItem();
		if (selItem.IsOk())
		{
			wxVariant itemDataName, itemDataCategory;
			model->GetValue(itemDataName, selItem, 0);
			model->GetValue(itemDataCategory, selItem, 2);
			//remove from the store and the view
			RemoveItem(GetItem(itemDataName.GetString().ToStdString(), itemDataCategory.GetString().ToStdString()));

			//lol 
			listView->DeleteItem(listView->ItemToRow(selItem));
		}
		else
		{
			wxMessageBox("No product selected", "Remove product");
		}
	}
}

void ProductList::OnContextEdit(wxCommandEvent& event)
{
	auto model = mDataListViewControl->GetModel();
	auto selItem = mDataListViewControl->GetCurrentItem();
	if (selItem.IsOk())
	{
		wxVariant itemDataName, itemDataCategory;
		model->GetValue(itemDataName, selItem, 0);
		model->GetValue(itemDataCategory, selItem, 2);

		ProductItem& oldItem = const_cast<ProductItem&>(GetItem(itemDataName.GetString().ToStdString(), itemDataCategory.GetString().ToStdString()));
		ProductItem item = oldItem;
		if (!item.IsEmpty())
		{
			ProductDialog dlg(&item, this);
			dlg.GetCategoryControl()->SetValue(item.GetCategoryName());
			dlg.GetCategoryControl()->Enable(false);
			if (dlg.ShowModal() == wxID_OK)
			{
				OnProductEdited(oldItem, item);
			}
		}
		else
		{
			wxMessageBox("Invalid item to edit", "Edit item");
		}


	}
}

void ProductList::OnContextDisplay(wxCommandEvent& event)
{
	//ugly hack loool 
	MainFrame* frame = wxGetApp().mFrame;
	if (frame)
	{
		frame->OnProductDisplay(event);
	}
}


void ProductList::OnDataLoaded()
{
	//supposed to be called by load database when the file is loaded
	if (mItemStore.empty())
	{
		//fatal error
		wxMessageBox("Fatal error, store empty after load", "End all things", wxOK | wxICON_ERROR);
		return;
	}
	if (mCurrentCategory.empty())
	{
		OnCategoryChange(mItemStore.begin()->first);
	}
}

void ProductList::OnSaved()
{
	//called when the files are successfully saved

}

void ProductList::OnProductStartEditing(wxDataViewEvent& event)
{
	
}

//in place editing
void ProductList::OnProductEndEditing(wxDataViewEvent& event)
{

}

void ProductList::OnProductEditStarted(wxDataViewEvent& event)
{
	
}

void ProductList::OnProductSelectionChanged(wxDataViewEvent& event)
{
	MainFrame* frame = wxGetApp().mFrame;
	if (frame)
	{
		wxAuiPaneInfo info = frame->mFrameManager->GetPane("Product display");
		if (info.IsShown())
		{
			auto item = mDataListViewControl->GetCurrentItem();
			if (item.IsOk())
			{
				const ProductItem& product = GetFromDataView(item);
				((wxHtmlWindow*)info.window)->SetPage(frame->ProductDisplayText(product));
				((wxHtmlWindow*)info.window)->Update();

			}
		}
	}
}

void ProductList::OnProductActivated(wxDataViewEvent& event)
{
	
}

//file is chunk based, PLI1|PLI2 ... |PLIn
//therefore can use an iterator and treat the the file as a flat array
//load each item data into the store in turn
//throw unusual error 
bool ProductList::doload(std::fstream& file)
{
	ProductItem::AllocateReadBuffer();
	std::istream_iterator<ProductItem> iter(file);
	std::istream_iterator<ProductItem> eos;
	while (iter != eos)
	{
		const ProductItem item = *iter++;
		auto setIter = mItemStore.find(item.GetCategoryName());
		//if not inside store
		if (setIter == mItemStore.end())
		{
			std::set<ProductItem> items;
			auto iterS = mItemStore.insert(std::make_pair(item.GetCategoryName(), items));
			if (!iterS.second)
			{
				//should also not occur because set is new, and category name does not exist
				WriteErrorCode(UNUSUAL_ERROR_IN_LOAD);
				ProductItem::DeallocateReadBuffer();
				return false;
			}
			setIter = iterS.first;
		}
		auto& set = setIter->second;
		auto IterI = set.insert(item);
		if (!IterI.second)
		{
			//error corrupted file, file contains 2 exact products in the data base, 
			WriteErrorCode(DATABASE_ERROR);
			ProductItem::DeallocateReadBuffer();
			return false;
		}
	}
	bool ret = doLoadEmptyCategoryNames();
	ProductItem::DeallocateReadBuffer();
	return (ret);
}

bool ProductList::doSave(std::fstream& file)
{
	std::fstream mEmptyCatFile(mDatabasePath + "\\emptycategory.txt", std::ios::out);
	std::ostream_iterator<ProductItem> oIter(file);
	std::unique_lock<std::mutex> lock(mPLStoreMutex);
	for (auto& storeIter : mItemStore)
	{
		for (auto& item : storeIter.second)
		{
			*oIter = item;
			if (file.bad())
			{
				//stream corrupted
				return false;
			}
		}
		if (storeIter.second.empty())
		{
			mEmptyCatFile << storeIter.first <<"\n";
		}
	}

	//data saved, hopefully well
	return true;

}

bool ProductList::doLoadEmptyCategoryNames()
{
	std::fstream mEmptyCatFile(mDatabasePath + "\\emptycategory.txt", std::ios::in);
	if (mEmptyCatFile.is_open())
	{
		while (!mEmptyCatFile.eof() && mEmptyCatFile.good())
		{
			std::string category;
			std::getline(mEmptyCatFile, category);
			if (category.empty()) continue;
			else
			{
				//insert a new set into the category
				std::set<ProductItem> set;
				auto iterI = mItemStore.insert(std::make_pair(category, set));
				if (!iterI.second)
				{
					//skip, assume already in the store
					continue;
				}
			}
		}
	}
	else
	{
		wxMessageBox("Could not load empty categories from file", "Load category");
		return false;
	}

	return true;
}

bool ProductList::updateViewList(StoreIterator iterator)
{
	if (iterator == mItemStore.end())
	{
		wxMessageBox("The selected category does not exist", "USER ERROR", wxOK | wxICON_ERROR);
		return false;
	}

	//we are ready to load data
	auto& set = iterator->second;
	for (auto& item : set)
	{
		AppendToViewList(item);
	}

	return true;
}

void ProductList::ResetViewList()
{
	mDataListViewControl->Freeze();
	mDataListViewControl->DeleteAllItems();
	mDataListViewControl->Thaw();
	mDataListViewControl->Update();
}

void ProductList::WriteErrorCode(int errorCode)
{
	std::unique_lock<std::mutex> lock(mPLMutex);
	mPLErrorCode = errorCode;
}

std::string& ProductList::FormatProductName(std::string& value)
{
	std::transform(value.begin(), value.end(), value.begin(), [&](char& r) {
		if (isalpha(r)) return std::tolower(r);
		else
		{
			return (int)r;
		}
		});
	value[0] = std::toupper(value[0]);
	return value;
}

void ProductList::PlugViewEventHandlers()
{

}

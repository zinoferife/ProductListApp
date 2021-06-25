#include "ProductList.h"
#include "Application.h"
BEGIN_EVENT_TABLE(ProductList, wxPanel)
	EVT_DATAVIEW_SELECTION_CHANGED(ID_PRODUCT_VIEW, ProductList::OnProductSelectionChanged)
	EVT_DATAVIEW_ITEM_ACTIVATED(ID_PRODUCT_VIEW,ProductList::OnProductActivated)
	EVT_DATAVIEW_ITEM_EDITING_DONE(ID_PRODUCT_VIEW, ProductList::OnProductEndEditing)
	EVT_CONTEXT_MENU(ProductList::OnContextMenu)
	EVT_MENU(ProductList::ID_CONTEXT_ADD, ProductList::OnContextAdd)
	EVT_MENU(ProductList::ID_CONTEXT_REMOVE, ProductList::OnContextRemove)
	EVT_MENU(ProductList::ID_CONTEXT_EDIT, ProductList::OnContextEdit)
END_EVENT_TABLE()



ProductList::ProductList(wxWindow* parent, wxWindowID id, const wxPoint& position, const wxSize& size)
: wxPanel(parent, id, position,size), mProductCurrentEditing(NULL){
	mDatabasePath = wxGetApp().mApplicationPath + "\\.data";
	mPLErrorCode = NO_PL_ERROR;
	CreateListView();

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

void ProductList::LoadListDatabase()
{
	wxBusyCursor cursor;
	if (wxDir::Exists(mDatabasePath))
	{
		//director exist, 
		//load the data base from the file
		std::string npath = mDatabasePath;
		npath += "\\data.dat";
		std::fstream database(npath, std::ios::in | std::ios::binary);
		if (database.is_open())
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
			wxMessageBox("Failed to load database, \"data.dat\" does not exist in the folder", "DATABASE ERROR", wxOK | wxICON_INFORMATION);
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
		std::string nPath = mDatabasePath + "\\data.dat";
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
				return;
			}
		}
		database.flush();
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
		mDataListViewControl->AppendTextColumn("Product name", wxDATAVIEW_CELL_EDITABLE, 180);
		mDataListViewControl->AppendBitmapColumn(wxT("AVL"),1);
		mDataListViewControl->AppendTextColumn("Category name", wxDATAVIEW_CELL_INERT, 180);
		mDataListViewControl->AppendTextColumn("Class", wxDATAVIEW_CELL_INERT, 100);
		mDataListViewControl->AppendTextColumn("Active ingredent", wxDATAVIEW_CELL_EDITABLE, 180);
		mDataListViewControl->AppendTextColumn("Stock count", wxDATAVIEW_CELL_EDITABLE, 100);
		mDataListViewControl->AppendTextColumn("Unit price (N)", wxDATAVIEW_CELL_EDITABLE, 100);
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
	mCurrentCategory = "All categories";
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
	if (Category == "All categories")
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

void ProductList::OnCategoryNameChange(const std::string& oldName, const std::string& newName)
{
	//create a new category
	//get old category set, 
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
	RemoveItem(oldItem);
	AddItem(newItem);
}

void ProductList::OnContextMenu(wxContextMenuEvent& event)
{
	int rowSelected = GetListControl()->GetSelectedRow();
	if (rowSelected != wxNOT_FOUND)
	{
		wxMenu* menu = new wxMenu;
		menu->Append(ID_CONTEXT_ADD, wxT("Add product"));
		menu->Append(ID_CONTEXT_REMOVE, wxT("Remove product"));
		menu->Append(ID_CONTEXT_EDIT, wxT("Edit product"));

		PopupMenu(menu);
	}
}

void ProductList::OnContextAdd(wxCommandEvent& event)
{
	wxMessageBox("Context add");
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
}

void ProductList::OnDataLoaded()
{
	//supposed to be called by load database when the file is loaded
	if (mItemStore.empty())
	{
		//fatal error
		wxMessageBox("Fatal error", "End all things", wxOK | wxICON_ERROR);
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
	wxMessageBox("Start editing", "Product Selection", wxOK, this);
}

//in place editing
void ProductList::OnProductEndEditing(wxDataViewEvent& event)
{
	if (event.IsEditCancelled())
	{
		wxMessageBox("Messge here");
	}

	auto model = event.GetModel();
	auto col = event.GetDataViewColumn();
	auto item = event.GetItem();

	if (col && model && item.IsOk() && mProductCurrentEditing)
	{
		
		

		wxDataViewRenderer* renderer = col->GetRenderer();
		renderer->FinishEditing();
	}
}

void ProductList::OnProductEditStarted(wxDataViewEvent& event)
{
	wxMessageBox("editing started", "Product Selection", wxOK, this);
}

void ProductList::OnProductSelectionChanged(wxDataViewEvent& event)
{

}

void ProductList::OnProductActivated(wxDataViewEvent& event)
{
	auto model = event.GetModel();
	auto col = event.GetDataViewColumn();
	auto item = event.GetItem();

	if (col && model && item.IsOk())
	{
		wxVariant itemDataName, itemDataCategory;
		model->GetValue(itemDataName, item, 0);
		model->GetValue(itemDataCategory, item, 2);
		
		//get the current editing item from the store
		ProductItem test;
		test.ProductName() = itemDataName.GetString().ToStdString();
		const ProductItem* itemTest = &GetItem(itemDataName.GetString().ToStdString(), itemDataCategory.GetString().ToStdString());//&(*mItemStore.find(itemDataCategory.GetString().ToStdString())->second.find(test));
		mProductCurrentEditing = const_cast<ProductItem*>(itemTest);


		wxDataViewRenderer* renderer = col->GetRenderer();
		wxRect itemRect = mDataListViewControl->GetItemRect(item);
		renderer->StartEditing(item, itemRect);
		
	}
}

//file is chunk based, PLI1|PLI2 ... |PLIn
//therefore can use an iterator and treat the the file as a flat array
//load each item data into the store in turn
//throw unusual error 
bool ProductList::doload(std::fstream& file)
{
	std::istream_iterator<ProductItem> iter(file);
	std::istream_iterator<ProductItem> eos;
	while (iter != eos)
	{
		const ProductItem item = *iter;
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
			return false;
		}
		iter++;
	}

	return true;
}

bool ProductList::doSave(std::fstream& file)
{
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
	}

	//data saved, hopefully well
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

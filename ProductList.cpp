#include "ProductList.h"
#include "Application.h"
BEGIN_EVENT_TABLE(ProductList, wxPanel)
END_EVENT_TABLE()


ProductList::ProductList(wxWindow* parent, wxWindowID id, const wxPoint& position, const wxSize& size)
:wxPanel(parent, id, position, size){

	wxStaticBitmap* control = new wxStaticBitmap(this, wxID_ANY, wxArtProvider::GetBitmap("add"));
	//set the default database folder
	mDatabasePath = wxGetApp().mApplicationPath + "\\.data";
	mPLErrorCode = NO_PL_ERROR;

}


//checks if category exists 
bool ProductList::HasCategory(const std::string& category)
{
	auto found = mItemStore.find(category);
	return (found != mItemStore.end());
}

//creates a new category if the category is not in the store
bool ProductList::AddItem(const std::string& category, const ProductItem& item)
{
	if (item.IsEmpty()) return false;

	auto iter = mItemStore.find(category);
	if (iter != mItemStore.end())
	{
		auto itemIter = iter->second.insert(item);
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
			wxMessageBox(wxT("Product Already exist"), wxT("USER ERROR"), wxOK | wxICON_INFORMATION);
		}
		return (iterI.second && iterC.second);
	}

}

//fails if category does not exist, uses the category name in the item object
bool ProductList::AddItem(const ProductItem& item)
{
	if (item.IsEmpty()) return false;

	const std::string& category = item.GetCategoryName();
	auto iter = mItemStore.find(category);
	if (iter == mItemStore.end()) return false;

	auto& itemsVec = iter->second;
	auto iterI = itemsVec.insert(item);
	if (!iterI.second)
	{
		mPLErrorCode = PRODUCT_EXIST;
		return false;
	}
	return true;
}

ProductItem& ProductList::GetItem(const std::string& ProductName, const std::string& Category)
{
	auto iter = mItemStore.find(Category);
	if (iter != mItemStore.end())
	{
		if (!iter->second.empty())
		{
			auto& itemVec = iter->second;

			//linear search
			auto itemIter = std::find_if(itemVec.begin(), itemVec.end(), [&](ProductItem& item) {
				 return (item.GetProductName == ProductName);
				});


		}
		else
		{
			wxString message = wxString("Product is not in the category: ") + Category;
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
				//load ddi not work
				if (mPLErrorCode == FILE_CORRUPTED)
				{
					wxMessageBox("Failed to load database, file corrupted", "DATABASE ERROR",wxOK | wxICON_ERROR);
					return;
				}
				wxMessageBox("Failed to load from database", "DATABASE ERROR", wxOK | wxICON_ERROR);
				WriteErrorCode(DATABASE_ERROR);
			}
		}
		else
		{
			//if file does not open, assume file does not exist
			wxMessageBox("Failed to load database, \"data.dat\" does not exist in the folder", "DATABASE ERROR", wxOK | wxICON_ERROR);
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
}

void ProductList::ParseJsonFile()
{
}

void ProductList::SaveJsonFile()
{
}

void ProductList::AppendProduct(const std::vector<std::string>& productDesc)
{
}

void ProductList::CreateListView()
{
}

void ProductList::CreateCategory(const std::string& mCatrgory)
{
}

void ProductList::OnCategoryChange(const std::string& Category)
{
}

void ProductList::OnCategoryRemoved(const std::string& Category)
{
}

void ProductList::OnCategoryNameChange(const std::string& oldName, const std::string& newName)
{
}

void ProductList::OnProductAdded(ProductItem& item)
{
}

void ProductList::OnProductRemoved(ProductItem& item)
{
}

void ProductList::OnProductEdited(ProductItem& oldItem, ProductItem& newItem)
{
}

void ProductList::OnDataLoaded()
{
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
		ProductItem item = *iter;
		auto setIter = mItemStore.find(item.GetCategoryName());
		//if not inside store
		if (setIter == mItemStore.end())
		{
			std::set<ProductItem> items;
			auto iterI = items.insert(item);
			if (!iterI.second)
			{
				//should not occur because set is new
				WriteErrorCode(UNUSUAL_ERROR_IN_LOAD);
				return false;
			}
			auto iterS = mItemStore.insert(std::make_pair(item.GetCategoryName(), items));
			if (!iterS.second)
			{
				//should also not occur because set is new, and category name does not exist
				WriteErrorCode(UNUSUAL_ERROR_IN_LOAD);
				return false;
			}
		}
		auto& set = setIter->second;
		auto IterI = set.insert(item);
		if (!IterI.second)
		{
			//error corrupted file, file contains 2 exact products in the data base, 
			WriteErrorCode(DATABASE_ERROR);
			return false;
		}
	}

	return true;
}

bool ProductList::doSave(std::fstream& file)
{
	return false;
}

void ProductList::WriteErrorCode(int errorCode)
{
	std::unique_lock<std::mutex> lock(mPLMutex);
	mPLErrorCode = errorCode;
}

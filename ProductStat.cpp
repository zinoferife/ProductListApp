#include "ProductStat.h"

ProductStat::ProductStat(wxWindow* parent, wxWindowID id, const wxPoint& position, const wxSize& size)
: wxPropertyGrid(parent,id, position, size, wxPG_AUTO_SORT
	| wxPG_SPLITTER_AUTO_CENTER), mTotal(0), mCreated(false){
	SetCaptionBackgroundColour(wxColour(37, 150, 190));
	SetCaptionTextColour(wxColor(255, 255, 255));
	SetBackgroundColour(wxColor(255, 255, 255));
}

void ProductStat::CreateStatsFromStore(ProductList& store)
{
	if (!store.IsEmpty())
	{
		ProductList::StoreIterator iter = store.mItemStore.begin();
		AppendCategory("Product statistics by catergory");
		while (iter != store.mItemStore.end())
		{
			const std::string& catName = iter->first;
			auto& set = iter->second;
			AppendIntProperty(catName, set.size());
			mTotal += set.size();
			iter++;
		}
		AppendCategory("Total");
		AppendIntProperty("Total product in store", mTotal);
		mCreated = true;
	}
}

void ProductStat::UpdateProductStatsFromStore(ProductList& store)
{
	mTotal = 0;
	if (!store.IsEmpty() && mCreated)
	{
		ProductList::StoreIterator iter = store.mItemStore.begin();
		while (iter != store.mItemStore.end())
		{
			SetIntProperty(iter->first, iter->second.size());
			mTotal += iter->second.size();
			iter++;
		}
		SetIntProperty("Total product in store", mTotal);
	}
}

void ProductStat::UpdateCategoryCount(const std::string& category, ProductList& store, int function)
{
	if (!store.IsEmpty() && mCreated)
	{
		ProductList::StoreIterator iter = store.mItemStore.find(category);
		if (function == ADD)
		{
			if (iter != store.mItemStore.end())
			{
				SetIntProperty(category, iter->second.size());
				mTotal++;
				SetIntProperty("Total product in store", mTotal);
			}
		}
		else if (function == REMOVE)
		{
			if (iter != store.mItemStore.end())
			{
				SetIntProperty(category, iter->second.size() - 1);
				mTotal--;
				SetIntProperty("Total product in store", mTotal);
			}
		}
	}
}

void ProductStat::AppendCategory(const std::string& categoryName)
{
	Append(new wxPropertyCategory(categoryName));
}

void ProductStat::AppendStringProperty(const std::string& stringName, const std::string& value)
{
	Append(new wxStringProperty(stringName, wxPG_LABEL, value));
}

void ProductStat::AppendIntProperty(const std::string& stringName, int value)
{
	Append(new wxIntProperty(stringName, wxPG_LABEL, value));
}

void ProductStat::AppendFloatProperty(const std::string& stringName, float value)
{
	Append(new wxFloatProperty(stringName, wxPG_LABEL, value));
}

void ProductStat::InsertIntProperty(const std::string& stringName, int value, ProductList& store)
{
	ProductList::StoreIterator atBack = --store.mItemStore.end();
	while (atBack->first == stringName)
	{
		//find first thats not in the store
		atBack--;
	}


	if(!atBack->first.empty())
	{
		wxPGProperty* props = GetProperty(atBack->first);
		if (props)
		{
			Insert(props, new wxIntProperty(stringName, wxPG_LABEL, value));
			Sort();
		}
	}
}

void ProductStat::RemoveIntProperty(const std::string& stringName, ProductList& store)
{
	//Reflect a deleted category
	//
}

void ProductStat::SetStringProperty(const std::string& label, const std::string& value)
{
	auto property = GetPropertyByName(label);
	if (property)
	{
		property->SetValueFromString(value);
	}
}

void ProductStat::SetFloatProperty(const std::string& label, float value)
{
	auto property = GetPropertyByName(label);
	if (property)
	{
		property->SetValue(wxVariant(value));
	}
}

void ProductStat::SetIntProperty(const std::string& label, int value)
{
	auto property = GetPropertyByName(label);
	if (property)
	{
		property->SetValue(wxVariant(value));
	}
}

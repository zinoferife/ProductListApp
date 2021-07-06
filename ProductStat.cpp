#include "ProductStat.h"

ProductStat::ProductStat(wxWindow* parent, wxWindowID id, const wxPoint& position, const wxSize& size)
: wxPropertyGrid(parent,id, position, size, wxPG_AUTO_SORT
	| wxPG_SPLITTER_AUTO_CENTER){
	SetCaptionBackgroundColour(wxColour(37, 150, 190));
	SetCaptionTextColour(wxColor(255, 255, 255));
	SetBackgroundColour(wxColor(255, 255, 255));
}

void ProductStat::CreateStatsFromStore(ProductList& store)
{
	if (!store.IsEmpty())
	{
		std::size_t total = 0;
		ProductList::StoreIterator iter = store.mItemStore.begin();
		AppendCategory("Product statistics by catergory");
		while (iter != store.mItemStore.end())
		{
			const std::string& catName = iter->first;
			auto& set = iter->second;
			AppendIntProperty(catName, set.size());
			total += set.size();
			iter++;
		}
		AppendCategory("Total");
		AppendIntProperty("Total product in store", total);
	}
}

void ProductStat::UpdateProductStatsFromStore(ProductList& store)
{
	if (!store.IsEmpty())
	{
		std::size_t total = 0;
		ProductList::StoreIterator iter = store.mItemStore.begin();
		while (iter != store.mItemStore.end())
		{
			SetIntProperty(iter->first, iter->second.size());
			total += iter->second.size();
			iter++;
		}
		SetIntProperty("Total product in store", total);
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

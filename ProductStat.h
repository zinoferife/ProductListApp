#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // !WX_PRECOMP
#include <wx/propgrid/propgrid.h>
#include "ProductList.h"

//collects and displays stats about 
class ProductStat : public wxPropertyGrid
{
public:
	enum
	{
		ADD,
		REMOVE
	};
	ProductStat(wxWindow* parent, wxWindowID id, const wxPoint& position = wxDefaultPosition,
		const wxSize& size = wxDefaultSize);
	void CreateStatsFromStore(ProductList& store);
	void UpdateProductStatsFromStore(ProductList& store);
	void UpdateCategoryCount(const std::string& category, ProductList& store, int function);


	void AppendCategory(const std::string& categoryName);
	void AppendStringProperty(const std::string& stringName, const std::string& value);
	void AppendIntProperty(const std::string& stringName, int value);
	void AppendFloatProperty(const std::string& stringName, float value);

	void InsertIntProperty(const std::string& stringName, int value, ProductList& store);
	void RemoveIntProperty(const std::string& stringName, ProductList& store);
	void SetStringProperty(const std::string& label, const std::string& value );
	void SetFloatProperty(const std::string& label, float value);
	void SetIntProperty(const std::string& label, int value);
private:
	std::size_t mTotal;
	bool mCreated;

};


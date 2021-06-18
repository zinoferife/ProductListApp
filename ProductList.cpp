#include "ProductList.h"
BEGIN_EVENT_TABLE(ProductList, wxPanel)
END_EVENT_TABLE()


ProductList::ProductList(wxWindow* parent, wxWindowID id, const wxPoint& position, const wxSize& size)
:wxPanel(parent, id, position, size){

	wxStaticBitmap* control = new wxStaticBitmap(this, wxID_ANY, wxArtProvider::GetBitmap("add"));
}

bool ProductList::HasCategory(const std::string& category)
{
	return false;
}

void ProductList::LoadListDatabase()
{
}

void ProductList::SaveDatabase()
{
}

void ProductList::FindDatabase()
{
}

void ProductList::ParseJsonFile()
{
}

void ProductList::AppendProduct(const std::vector<std::string>& productDesc)
{
}

#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP
#include <wx/artprov.h>
#include <unordered_map>

#define ART_PRODUCT "PRODUCTLIST"
 
class ArtProvider: public wxArtProvider
{
public:
	ArtProvider();
protected:
	void CreateImageStore();
	virtual wxBitmap CreateBitmap(const wxArtID& id, const wxArtClient& client, const wxSize& size) override;
private:
	std::unordered_map<std::string, wxBitmap> mImageStore;
	wxString mPath;
};

